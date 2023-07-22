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
			.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
			.pEngineName = "Light Frame",
			.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
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

#ifndef NDEBUG
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			spdlog::warn("Vulkan validation layer:\n{}\n", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			spdlog::error("Vulkan validation layer:\n{}\n", pCallbackData->pMessage);
			break;
		default:
			spdlog::info("Vulkan validation layer:\n{}\n", pCallbackData->pMessage);
			break;
		}

		return VK_FALSE;
	}
#endif

	inline vk::DebugUtilsMessengerEXT createDebugUtilsMessenger(vk::Instance instance)
	{
#ifndef NDEBUG
		spdlog::warn("Validation layers are enabled!");

		const vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCI {
			.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
			.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
			.pfnUserCallback = debugCallback
		};

		try
		{
			return instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCI);
		}
		catch (const vk::SystemError& e)
		{
			spdlog::critical(e.what());
		}
#endif
		return nullptr;
	}
}

