#pragma once

class VulkanSwapchain
{
public:
	VulkanSwapchain() = default;

	void create();
	void teardown();
	void recreate();

	vk::Result acquireNextImage(vk::Semaphore semaphore, uint32_t* imageIndex) const;

	std::vector<vk::Image> images;
	std::vector<vk::ImageView> imageViews;
	vk::Extent2D extent;
	vk::Format format;
	
private:
	vk::SwapchainKHR m_swapchain;
	vk::SwapchainKHR m_oldSwapchain;
	
	void createHandle();

	static vk::SurfaceFormatKHR setFormat(const std::vector<vk::SurfaceFormatKHR>&);
	static vk::PresentModeKHR setPresentMode(const std::vector<vk::PresentModeKHR>&);
	static vk::Extent2D setExtent(const vk::SurfaceCapabilitiesKHR&);
};

