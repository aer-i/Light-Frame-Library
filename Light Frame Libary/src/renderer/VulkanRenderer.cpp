#include "pch.hpp"
#include "VulkanRenderer.hpp"
#include "window/Window.hpp"
#include "lf2d.hpp"

lfRenderer::~lfRenderer()
{
	this->teardown();
}

void lfRenderer::clearColor(lf2d::Color const& color)
{
	m_color[0] = static_cast<float>(color.r) / 255.f;
	m_color[1] = static_cast<float>(color.g) / 255.f;
	m_color[2] = static_cast<float>(color.b) / 255.f;
	m_color[3] = static_cast<float>(color.a) / 255.f;
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

	m_defaultPipelineLayout = PipelineLayout::Builder()
		.build();

	m_defaultPipeline.construct(m_swapchain, m_defaultPipelineLayout);
}

void lfRenderer::beginFrame()
{
	if (vc::Get().device.getFenceStatus(m_frames[m_frameIndex].fence) != vk::Result::eSuccess)
	{
		vc::Get().device.waitForFences(1, &m_frames[m_frameIndex].fence, true, UINT64_MAX);
	}

	if (m_swapchain.acquireNextImage(m_frames[m_frameIndex].imageAvailable, &m_imageIndex) == vk::Result::eErrorOutOfDateKHR)
	{
		this->recreateSwapchain();
	}

	m_currentCmd = &m_frames[m_frameIndex].commandBuffer;

	m_currentCmd->begin({.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit});

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

	m_currentCmd->pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eColorAttachmentOutput, {}, nullptr, nullptr, imageMemoryBarrier);

	vk::RenderingAttachmentInfo colorAttachment {
		.imageView = m_swapchain.imageViews[m_imageIndex],
		.imageLayout = vk::ImageLayout::eAttachmentOptimal,
		.loadOp = vk::AttachmentLoadOp::eClear,
		.storeOp = vk::AttachmentStoreOp::eStore,
		.clearValue = { vk::ClearColorValue{m_color} }
	};

	m_currentCmd->beginRendering({
		.renderArea = {{0, 0}, m_swapchain.extent},
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachment
	});

	m_currentCmd->bindPipeline(vk::PipelineBindPoint::eGraphics, m_defaultPipeline);

	vk::Viewport viewport{0, 0, (float)m_swapchain.extent.width, (float)m_swapchain.extent.height, 0.f, 1.f};
	vk::Rect2D scissor{.offset = { 0, 0 }, .extent = m_swapchain.extent};

	m_currentCmd->setViewport(0, viewport);
	m_currentCmd->setScissor(0, scissor);

	m_currentCmd->draw(3, 1, 0, 0); // Magick numbberz

}

void lfRenderer::endFrame()
{
	vk::ImageMemoryBarrier const imageMemoryBarrier {
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

	m_currentCmd->endRendering();
	m_currentCmd->pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eBottomOfPipe, {}, nullptr, nullptr, imageMemoryBarrier);
	m_currentCmd->end();

	vc::Get().device.resetFences(1, &m_frames[m_frameIndex].fence);

	constexpr vk::PipelineStageFlags pipelineStage[]{ vk::PipelineStageFlagBits::eColorAttachmentOutput };
	
	vc::Get().graphicsQueue.submit(
		vk::SubmitInfo{
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &m_frames[m_frameIndex].imageAvailable,
			.pWaitDstStageMask = pipelineStage,
			.commandBufferCount = 1,
			.pCommandBuffers = &m_frames[m_frameIndex].commandBuffer,
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = &m_frames[m_frameIndex].renderFinished
		},
		m_frames[m_frameIndex].fence
	);

	auto result = vc::Get().presentQueue.presentKHR(
		vk::PresentInfoKHR {
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &m_frames[m_frameIndex].renderFinished,
			.swapchainCount = 1,
			.pSwapchains = m_swapchain,
			.pImageIndices = &m_imageIndex
	});

	if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
	{
		recreateSwapchain();
	}

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
	m_defaultPipeline.teardown();
	m_defaultPipelineLayout.teardown();

	for (auto& frame : m_frames)
		frame.teardown();
	m_swapchain.teardown();
	vc::Teardown();
}
