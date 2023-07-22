#pragma once

class VulkanContext
{
public:
	VulkanContext();
	~VulkanContext();

	void create();

	vk::Instance instance;
	vk::DebugUtilsMessengerEXT debugMessenger;
	vk::SurfaceKHR surface;
	vk::PhysicalDevice gpu;
	vk::Device device;
	vk::Queue graphicsQueue, presentQueue;
	uint32_t graphicsFamily, presentFamily;

	struct
	{
		vk::CommandPool cmdPool;
		vk::CommandBuffer cmdBuffer;
		vk::Fence fence;
	} uploadContext;

	vk::PhysicalDeviceFeatures enabledDeviceFeatures;

private:
	void teardown();
};

