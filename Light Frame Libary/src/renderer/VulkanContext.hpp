#pragma once

class VulkanContext
{
public:
	VulkanContext() = default;

	static const vk::ImageView CreateImageView(vk::Image image, vk::ImageViewType viewType, vk::Format format, vk::ImageAspectFlags aspectFlags);

	static VulkanContext& Get() { static VulkanContext vc; return vc; }
	static void Create() { Get().create(); }
	static void Teardown() { Get().teardown(); }

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
	void create();
	void teardown();
};

using vc = VulkanContext;