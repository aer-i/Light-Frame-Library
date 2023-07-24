#include "pch.hpp"
#include "VulkanContext.hpp"
#include "window/Window.hpp"
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
			.subresourceRange = { {aspectFlags}, 0, 1, 0, viewType == vk::ImageViewType::eCube ? 6u : 1u }
	});
}

void VulkanContext::create()
{
#pragma region Dispatcher
	{
		vi::dispatcherInit();
	}
#pragma endregion

#pragma region Instance
	{
		instance = vi::createInstance(lfWindow::GetTitle());

		vi::dispatcherLoadInstance(instance);
	}
#pragma endregion

#pragma region Debug Messenger
	{
		debugMessenger = vi::createDebugUtilsMessenger(instance);
	}
#pragma endregion

#pragma region Window Surface
	{
		surface = lfWindow::CreateSurface(instance);
	}
#pragma endregion

#pragma region Physical Device
	{
		gpu = vi::selectPhysicalDeviceByScore(instance);
	}
#pragma endregion

#pragma region Queue Families
	{
		auto[graphics, present] = vi::selectQueueFamilies(gpu, surface);
		graphicsFamily = graphics;
		presentFamily = present;
	}
#pragma endregion

#pragma region Logical Device
	{
		auto [tempDevice, tempDeviceFeatures] = vi::createLogicalDevice(gpu, graphicsFamily, presentFamily);
		device = tempDevice;
		enabledDeviceFeatures = tempDeviceFeatures;

		vi::dispatcherLoadDevice(device);
		
		graphicsQueue = device.getQueue2({ .queueFamilyIndex = graphicsFamily, .queueIndex = 0 });
		presentQueue = device.getQueue2({ .queueFamilyIndex = presentFamily, .queueIndex = 0 });
	}
#pragma endregion

#pragma region Upload Context
	{
		uploadContext.cmdPool = device.createCommandPool({.queueFamilyIndex = graphicsFamily});
		uploadContext.cmdBuffer = device.allocateCommandBuffers({ .commandPool = uploadContext.cmdPool, .level = {}, .commandBufferCount = 1}).front();
		uploadContext.fence = device.createFence({});
	}
#pragma endregion

#pragma region Allocator
	{
		vk::DynamicLoader dl;

		VmaVulkanFunctions const functions {
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
		spdlog::info("Created vulkan context");
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

	spdlog::info("Destroyed vulkan context");
}
