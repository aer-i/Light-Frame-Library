#include "pch.hpp"
#include "VulkanContext.hpp"
#include "vk/Instance.hpp"
#include "vk/Device.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

const vk::ImageView VulkanContext::CreateImageView(vk::Image image, vk::ImageViewType viewType, vk::Format format, vk::ImageAspectFlags aspectFlags)
{
	return Get().device.createImageView({
		.image = image,
		.viewType = viewType,
		.format = format,
		.components = { vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity },
		.subresourceRange = { {aspectFlags}, 0, 1, 0, 1 }
	});
}

const void VulkanContext::ImmediateSubmit(std::function<void(vk::CommandBuffer commandBuffer)>&& function)
{
	Get().uploadContext.cmdBuffer.begin({ .flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit });

	function(Get().uploadContext.cmdBuffer);

	Get().uploadContext.cmdBuffer.end();

	Get().graphicsQueue.submit(
		vk::SubmitInfo {
			.commandBufferCount = 1,
			.pCommandBuffers = &Get().uploadContext.cmdBuffer
		},
		Get().uploadContext.fence
	);

	Get().device.waitForFences(Get().uploadContext.fence, true, UINT64_MAX);
	Get().device.resetFences(Get().uploadContext.fence);

	Get().device.resetCommandPool(Get().uploadContext.cmdPool);
}

const void VulkanContext::TransitionImageLayout(vk::Image image, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
{
	vk::ImageMemoryBarrier2 barrier {
		.oldLayout = oldLayout,
		.newLayout = newLayout,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = image,
		.subresourceRange = { {vk::ImageAspectFlagBits::eColor}, 0, 1, 0, 1 }
	};

	if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
	{
		barrier.srcAccessMask = {};
		barrier.dstAccessMask = vk::AccessFlagBits2::eTransferWrite;

		barrier.srcStageMask = vk::PipelineStageFlagBits2::eTopOfPipe;
		barrier.dstStageMask = vk::PipelineStageFlagBits2::eTransfer;
	}
	else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
	{
		barrier.srcAccessMask = vk::AccessFlagBits2::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits2::eShaderRead;

		barrier.srcStageMask = vk::PipelineStageFlagBits2::eTransfer;
		barrier.dstStageMask = vk::PipelineStageFlagBits2::eFragmentShader;
	}

	ImmediateSubmit([&](vk::CommandBuffer commandBuffer)
	{
		commandBuffer.pipelineBarrier2({
			.imageMemoryBarrierCount = 1,
			.pImageMemoryBarriers = &barrier
		});
	});
}

const void VulkanContext::CopyBufferToImage(vk::Buffer buffer, vk::Image image, uint32_t width, uint32_t height)
{
	ImmediateSubmit([&](vk::CommandBuffer commandBuffer)
	{
		commandBuffer.copyBufferToImage(
			buffer,
			image,
			vk::ImageLayout::eTransferDstOptimal,
			vk::BufferImageCopy {
				.imageSubresource = { {vk::ImageAspectFlagBits::eColor}, 0, 0, 1 },
				.imageOffset = { 0, 0, 0 },
				.imageExtent = { width, height, 1 }
			}
		);
	});
}

#ifdef _WIN32
void VulkanContext::create(HWND window, bool enableVL)
#endif
{
#pragma region Dispatcher
	{
		vi::dispatcherInit();
	}
#pragma endregion

#pragma region Instance
	{
		instance = vi::createInstance(enableVL);

		vi::dispatcherLoadInstance(instance);
	}
#pragma endregion

#pragma region Debug Messenger
	{
		if (enableVL)
			debugMessenger = vi::createDebugUtilsMessenger(instance);
	}
#pragma endregion

#pragma region Window Surface
	{
#ifdef _WIN32
		surface = instance.createWin32SurfaceKHR(vk::Win32SurfaceCreateInfoKHR{
			.hinstance = GetModuleHandle(nullptr),
			.hwnd = window
			});

#elif __linux__
		surface = instance.createXcbSurfaceKHR(vk::XcbSurfaceCreateInfoKHR{
			.connection = nullptr,
			.window = nullptr
			});
#endif
	}
#pragma endregion

#pragma region Physical Device
	{
		gpu = vi::selectPhysicalDeviceByScore(instance);
	}
#pragma endregion

#pragma region Queue Families
	{
		auto [graphics, present] = vi::selectQueueFamilies(gpu, surface);
		graphicsFamily = graphics;
		presentFamily = present;
	}
#pragma endregion

#pragma region Logical Device
	{
		auto [tempDevice, tempDeviceFeatures] = vi::createLogicalDevice(gpu, graphicsFamily, presentFamily, enableVL);
		device = tempDevice;
		enabledDeviceFeatures = tempDeviceFeatures;

		vi::dispatcherLoadDevice(device);

		graphicsQueue = device.getQueue2({ .queueFamilyIndex = graphicsFamily, .queueIndex = 0 });
		presentQueue = device.getQueue2({ .queueFamilyIndex = presentFamily, .queueIndex = 0 });
	}
#pragma endregion

#pragma region Upload Context
	{
		uploadContext.cmdPool = device.createCommandPool({ .queueFamilyIndex = graphicsFamily });
		uploadContext.cmdBuffer = device.allocateCommandBuffers({ .commandPool = uploadContext.cmdPool, .level = {}, .commandBufferCount = 1 }).front();
		uploadContext.fence = device.createFence({});
	}
#pragma endregion

#pragma region Allocator
	{
		vk::DynamicLoader dl;

		VmaVulkanFunctions const functions{
			.vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr"),
			.vkGetDeviceProcAddr = dl.getProcAddress<PFN_vkGetDeviceProcAddr>("vkGetDeviceProcAddr")
		};

		VmaAllocatorCreateInfo const allocatorCI{
			.physicalDevice = gpu,
			.device = device,
			.pVulkanFunctions = &functions,
			.instance = instance,
			.vulkanApiVersion = VK_API_VERSION_1_3
		};

		vmaCreateAllocator(&allocatorCI, &allocator);
	}
#pragma endregion

#pragma region Log
	{
		printf("[info] Created vulkan context\n");
	}
#pragma endregion
}

void VulkanContext::teardown()
{
	if (allocator)				vmaDestroyAllocator(allocator);
	if (uploadContext.fence)	device.destroy(uploadContext.fence);
	if (uploadContext.cmdPool)	device.destroy(uploadContext.cmdPool);
	if (device)					device.destroy();
	if (surface)				instance.destroy(surface);
	if (debugMessenger)			instance.destroy(debugMessenger);
	if (instance)				instance.destroy();

	printf("[info] Destroyed vulkan context\n");
}
