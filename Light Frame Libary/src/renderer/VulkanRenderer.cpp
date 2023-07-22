#include "pch.hpp"
#include "VulkanRenderer.hpp"
#include "window/Window.hpp"

lfRenderer::~lfRenderer()
{
	this->teardown();
}

void lfRenderer::create()
{
	vc::Create();
	m_swapchain.create();

	m_frames.resize(m_swapchain.images.size());

	for (auto& frame : m_frames)
	{
		frame.create();
	}
}

void lfRenderer::beginFrame()
{
	vc::Get().device.waitForFences(1, &m_frames[m_imageIndex].fence, true, UINT64_MAX);
	vc::Get().device.resetFences(1, &m_frames[m_imageIndex].fence);

	if (m_swapchain.acquireNextImage(m_frames[m_frameIndex].imageAvailable, &m_imageIndex) == vk::Result::eErrorOutOfDateKHR)
	{
		this->recreateSwapchain();
	}

	m_currentFrame = &m_frames[m_frameIndex];

	m_currentFrame->commandBuffer.begin(vk::CommandBufferBeginInfo{});

	vk::ImageMemoryBarrier imageMemoryBarrier {
		.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite,
		.oldLayout = vk::ImageLayout::eUndefined,
		.newLayout = vk::ImageLayout::eColorAttachmentOptimal,
		.image = m_swapchain.images[m_imageIndex],
		.subresourceRange = {
				.aspectMask = vk::ImageAspectFlagBits::eColor,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
		}
	};

	m_currentFrame->commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eColorAttachmentOutput, {}, nullptr, nullptr, imageMemoryBarrier);

	vk::RenderingAttachmentInfo colorAttachment {
		.imageView = m_swapchain.imageViews[m_imageIndex],
		.imageLayout = vk::ImageLayout::eAttachmentOptimal,
		.loadOp = vk::AttachmentLoadOp::eClear,
		.storeOp = vk::AttachmentStoreOp::eStore,
		.clearValue = { std::array{1.f, 1.f, 1.f, 1.f} }
	};

	m_currentFrame->commandBuffer.beginRendering({
		.renderArea = {{0, 0}, m_swapchain.extent},
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachment
	});

}

void lfRenderer::endFrame()
{
	m_currentFrame->commandBuffer.endRendering();

	vk::ImageMemoryBarrier imageMemoryBarrier {
		.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite,
		.oldLayout = vk::ImageLayout::eColorAttachmentOptimal,
		.newLayout = vk::ImageLayout::ePresentSrcKHR,
		.image = m_swapchain.images[m_imageIndex],
		.subresourceRange = {
				.aspectMask = vk::ImageAspectFlagBits::eColor,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
		}
	};

	m_currentFrame->commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eBottomOfPipe, {}, nullptr, nullptr, imageMemoryBarrier);

	m_currentFrame->commandBuffer.end();

	constexpr vk::PipelineStageFlags pipelineStage[]{ vk::PipelineStageFlagBits::eColorAttachmentOutput };

	vk::SubmitInfo submitInfo {
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &m_frames[m_frameIndex].imageAvailable,
			.pWaitDstStageMask = pipelineStage,
			.commandBufferCount = 1,
			.pCommandBuffers = &m_frames[m_frameIndex].commandBuffer,
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = &m_frames[m_frameIndex].renderFinished
	};

	vc::Get().graphicsQueue.submit(1, &submitInfo, m_frames[m_frameIndex].fence);

	vk::PresentInfoKHR presentInfo {
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &m_frames[m_frameIndex].renderFinished,
		.swapchainCount = 1,
		.pSwapchains = m_swapchain,
		.pImageIndices = &m_imageIndex
	};

	vc::Get().presentQueue.presentKHR(presentInfo);

	//vc::Get().device.waitIdle();

	m_frameIndex = (m_frameIndex + 1) % static_cast<uint32_t>(m_swapchain.images.size());
}

void lfRenderer::recreateSwapchain()
{
	int w = lfWindow::GetWidth();
	int h = lfWindow::GetHeight();

	while (w == 0 || h == 0)
	{
		w = lfWindow::GetWidth();
		h = lfWindow::GetHeight();
		glfwWaitEvents();
	}

	vc::Get().device.waitIdle();
	m_swapchain.recreate();
}

void lfRenderer::teardown()
{
	for (auto& frame : m_frames)
		frame.teardown();
	m_swapchain.teardown();
	vc::Teardown();
}
