#include "pch.hpp"
#include "VulkanSwapchain.hpp"
#include "VulkanContext.hpp"
#include "window/Window.hpp"

void VulkanSwapchain::create()
{
	this->createHandle();
}

vk::Result VulkanSwapchain::acquireNextImage(vk::Semaphore semaphore, uint32_t* imageIndex) const
{
	return vc::Get().device.acquireNextImageKHR(m_swapchain, UINT64_MAX, semaphore, nullptr, imageIndex);
}

void VulkanSwapchain::createHandle()
{
	const auto capabilities  = vc::Get().gpu.getSurfaceCapabilitiesKHR(vc::Get().surface);
	const auto surfaceFormat = this->setFormat(vc::Get().gpu.getSurfaceFormatsKHR(vc::Get().surface));
	const auto presentMode	 = this->setPresentMode(vc::Get().gpu.getSurfacePresentModesKHR(vc::Get().surface));
	const auto extent		 = this->setExtent(capabilities);

	this->extent = extent;
	this->format = surfaceFormat.format;

	const std::array families = { vc::Get().graphicsFamily, vc::Get().presentFamily };

	if (m_swapchain)
	{
		m_oldSwapchain = m_swapchain;
	}

	try
	{
		m_swapchain = vc::Get().device.createSwapchainKHR({
			.surface = vc::Get().surface,
			.minImageCount = std::max(2u, capabilities.minImageCount),
			.imageFormat = surfaceFormat.format,
			.imageColorSpace = surfaceFormat.colorSpace,
			.imageExtent = extent,
			.imageArrayLayers = 1,
			.imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
			.imageSharingMode = vc::Get().graphicsFamily == vc::Get().presentFamily ? vk::SharingMode::eExclusive : vk::SharingMode::eConcurrent,
			.queueFamilyIndexCount = vc::Get().graphicsFamily == vc::Get().presentFamily ? 0u : 2u,
			.pQueueFamilyIndices = vc::Get().graphicsFamily == vc::Get().presentFamily ? nullptr : families.data(),
			.preTransform = capabilities.currentTransform,
			.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
			.presentMode = presentMode,
			.clipped = VK_TRUE,
			.oldSwapchain = m_oldSwapchain
		});

		spdlog::info("Created vulkan swapchain. Extent: {} px width, {} px height", extent.width, extent.height);
	}
	catch (const vk::SystemError& e)
	{
		spdlog::critical(e.what());
	}

	vc::Get().device.destroy(m_oldSwapchain);

	images.clear();
	images = vc::Get().device.getSwapchainImagesKHR(m_swapchain);

	imageViews.clear();
	imageViews.reserve(images.size());

	for (const auto& image : images)
	{
		imageViews.emplace_back(vc::CreateImageView(
			image,
			vk::ImageViewType::e2D,
			format,
			vk::ImageAspectFlagBits::eColor)
		);
	}
}

void VulkanSwapchain::teardown()
{
	for (auto& imageView : imageViews)
	{
		vc::Get().device.destroy(imageView);
	}
	if (m_swapchain)
	{
		vc::Get().device.destroy(m_swapchain);
		spdlog::info("Vulkan swapchain destroyed");
	}
}

void VulkanSwapchain::recreate()
{

}

vk::SurfaceFormatKHR VulkanSwapchain::setFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
{
	if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined)
	{
		return { vk::Format::eR8G8B8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear };
	}
	for (const auto& f : availableFormats)
	{
		if (f.format == vk::Format::eR8G8B8A8Unorm && f.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
			return f;
	}
	return availableFormats[0];
}

vk::PresentModeKHR VulkanSwapchain::setPresentMode(const std::vector<vk::PresentModeKHR>& availableModes)
{
	for (const auto& mode : availableModes)
	{
		if (mode == vk::PresentModeKHR::eMailbox)
		{
			return mode;
		}
	}

	for (const auto& mode : availableModes)
	{
		if (mode == vk::PresentModeKHR::eImmediate)
		{
			return mode;
		}
	}

	return vk::PresentModeKHR::eFifo;
}

vk::Extent2D VulkanSwapchain::setExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.height != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}

	vk::Extent2D tempExtent {.width = (uint32_t)lfWindow::GetWidth(), .height = (uint32_t)lfWindow::GetHeight()};

	tempExtent.width = std::clamp(tempExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
	tempExtent.height = std::clamp(tempExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

	return tempExtent;
}