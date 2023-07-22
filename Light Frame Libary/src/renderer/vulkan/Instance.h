#pragma once

namespace vi
{
	inline void dispatcherInit()
	{
		vk::DynamicLoader dl;
		PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
	}

	inline void dispatcherLoadInstance(vk::Instance& instance)
	{
		VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);
	}

	inline void dispatcherLoadDevice(vk::Device& device)
	{
		VULKAN_HPP_DEFAULT_DISPATCHER.init(device);
	}

	inline vk::Instance createInstance(std::string const& appName)
	{

		uint32_t extensionCount;
		const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);
		
		std::vector<const char*> extensionNames(extensions, extensions + extensionCount);
		std::vector<const char*> layerNames;

#ifndef NDEBUG
		extensionNames.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		layerNames.emplace_back("VK_LAYER_KHRONOS_validation");
#endif
		
		const vk::ApplicationInfo appInfo {
			.pApplicationName = appName.c_str(),
			.applicationVersion = 0,
			.pEngineName = "Light Frame",
			.engineVersion = 0,
			.apiVersion = VK_API_VERSION_1_3
		};

		try
		{
			return vk::createInstance({
				.pApplicationInfo = &appInfo,
				.enabledLayerCount = static_cast<uint32_t>(layerNames.size()),
				.ppEnabledLayerNames = layerNames.data(),
				.enabledExtensionCount = static_cast<uint32_t>(extensionNames.size()),
				.ppEnabledExtensionNames = extensionNames.data() });
			
		}
		catch (const vk::SystemError& e)
		{
			spdlog::critical(e.what());
		}

		return nullptr;
	}
}

