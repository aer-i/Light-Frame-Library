#pragma once

class VulkanContext
{
public:
	VulkanContext();
	~VulkanContext();

	vk::Instance instance;
	vk::DebugUtilsMessengerEXT debugMessenger;
	vk::SurfaceKHR surface;
	vk::PhysicalDevice gpu;
	vk::Device device;
	vk::Queue graphicsQueue, presentQueue;
	uint32_t graphicsFamily, presentFamily;
};

