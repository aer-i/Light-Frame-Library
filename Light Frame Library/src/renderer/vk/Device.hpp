#pragma once
#include "renderer/VulkanContext.hpp"

namespace vi
{
	inline vk::PhysicalDevice selectPhysicalDeviceByScore(vk::Instance instance)
	{
		std::multimap<uint32_t, vk::PhysicalDevice> gpus;

		for (const auto& gpu : instance.enumeratePhysicalDevices())
		{
			uint32_t score = 0;

			if (!gpu.getFeatures().geometryShader)
			{
				gpus.insert(std::make_pair(score, gpu));
				continue;
			}

			if (gpu.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
			{
				score += 1000;
			}

			score += gpu.getProperties().limits.maxImageDimension2D;

			gpus.insert(std::make_pair(score, gpu));
		}

		vc::Get().deviceProperties.pNext = &vc::Get().descriptorIndexingProperties;
		gpus.rbegin()->second.getProperties2(&vc::Get().deviceProperties);

		printf("[info] Selected physical device: %s with a score of %d\n", vc::Get().deviceProperties.properties.deviceName.data(), gpus.rbegin()->first);
		printf("[info] Physical device type: %s\n", vk::to_string(vc::Get().deviceProperties.properties.deviceType).c_str());
		return gpus.rbegin()->second;
	}

	inline std::tuple<uint32_t, uint32_t> selectQueueFamilies(vk::PhysicalDevice gpu, vk::SurfaceKHR surface)
	{
		uint32_t graphics = UINT32_MAX, present = UINT32_MAX;

		uint32_t i = 0;
		for (const auto& family : gpu.getQueueFamilyProperties())
		{
			if (family.queueFlags & vk::QueueFlagBits::eTransfer)
			{
				printf("[info] Transfer queue family available at index: %d\n", i);
			}
			if (family.queueFlags & vk::QueueFlagBits::eCompute)
			{
				printf("[info] Compute queue family available at index: %d\n", i);
			}
			if (family.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				graphics = i;
			}
			if (gpu.getSurfaceSupportKHR(i, surface))
			{
				present = i;
			}
			if (UINT32_MAX != graphics && UINT32_MAX != present)
			{
				printf("[info] Graphics queue family index: %d, Present queue family index: %d\n", graphics, present);
				if (graphics == present)
					printf("[info] Exclusive sharing mode available\n");
				else
					printf("[info] Concurrent sharing mode available\n");

				return std::make_tuple(graphics, present);
			}
			++i;
		}

		return std::make_tuple(0u, 0u);
	}

	inline std::pair<vk::Device, vk::PhysicalDeviceFeatures> createLogicalDevice(vk::PhysicalDevice gpu, uint32_t graphicsFamily, uint32_t presentFamily, bool enableValidation)
	{
		std::vector uniqueIndices =
			graphicsFamily == presentFamily ?
			std::vector{ graphicsFamily } : std::vector{ graphicsFamily, presentFamily };

		std::vector<vk::DeviceQueueCreateInfo> queueInfo;
		queueInfo.reserve(uniqueIndices.size());

		constexpr float priority = 0.f;

		for (const auto& i : uniqueIndices)
		{
			queueInfo.emplace_back(vk::DeviceQueueCreateInfo{
			.queueFamilyIndex = i,
			.queueCount = 1,
			.pQueuePriorities = &priority});
		}

		std::vector<const char*> layers;
		std::vector<const char*> deviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		if (enableValidation)
			layers.emplace_back("VK_LAYER_KHRONOS_validation");

		vk::PhysicalDeviceFeatures enabledFeatures;
		vk::PhysicalDeviceFeatures deviceFeatures = gpu.getFeatures();
		
		enabledFeatures.shaderSampledImageArrayDynamicIndexing = true;

		constexpr vk::PhysicalDeviceShaderDrawParametersFeatures shaderDrawFeatures {
			.shaderDrawParameters = true
		};

		vk::PhysicalDeviceVulkan12Features const vulkan12features{
			.pNext = (void*)&shaderDrawFeatures,
			.shaderSampledImageArrayNonUniformIndexing = true,
			.descriptorBindingSampledImageUpdateAfterBind = true,
			.descriptorBindingUpdateUnusedWhilePending = true,
			.descriptorBindingPartiallyBound = true,
			.descriptorBindingVariableDescriptorCount = true,
			.runtimeDescriptorArray = true,
		};

		vk::PhysicalDeviceVulkan13Features const vulkan13features {
			.pNext = (void*)&vulkan12features,
			.synchronization2 = true,
			.dynamicRendering = true
		};		

		try
		{
			return std::make_pair(gpu.createDevice({
				.pNext = &vulkan13features,
				.queueCreateInfoCount = static_cast<uint32_t>(queueInfo.size()),
				.pQueueCreateInfos = queueInfo.data(),
				.enabledLayerCount = static_cast<uint32_t>(layers.size()),
				.ppEnabledLayerNames = layers.data(),
				.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
				.ppEnabledExtensionNames = deviceExtensions.data(),
				.pEnabledFeatures = &enabledFeatures}),
			
				enabledFeatures
			);
			
		}
		catch (const vk::SystemError& e)
		{
			printf("[critical] %s\n", e.what());
		}

		return {};
	}
}
