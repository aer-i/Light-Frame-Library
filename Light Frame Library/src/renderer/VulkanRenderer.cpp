#include "pch.hpp"
#include "VulkanRenderer.hpp"
#include "window/Window.hpp"
#include "lf2d.hpp"

struct CameraPushConstant
{
	glm::mat4 projView;
};

lfRenderer::~lfRenderer()
{
	this->teardown();
}

void lfRenderer::clearColor(lf2d::Color color)
{
	m_color[0] = static_cast<float>(color.r) / 255.f;
	m_color[1] = static_cast<float>(color.g) / 255.f;
	m_color[2] = static_cast<float>(color.b) / 255.f;
	m_color[3] = static_cast<float>(color.a) / 255.f;
}

void lfRenderer::setVsync(bool enabled)
{
	if (m_vsync != enabled)
	{
		m_vsync = enabled;

		if (vc::Get().device)
		{
			recreateSwapchain();
		}
	}
}

void lfRenderer::loadTexture(std::string_view filepath, bool pixelated)
{
	m_texturePool.loadTexture(filepath, pixelated);
}

void lfRenderer::loadTexture(void* buffer, vk::DeviceSize bufferSize, uint32_t width, uint32_t height)
{
	m_texturePool.loadTexture(buffer, bufferSize, width, height);
}

void lfRenderer::create(bool enableVL)
{
	vc::Create(enableVL);
	m_swapchain.create(m_vsync);

	m_frames.resize(m_swapchain.images.size());

	for (auto& frame : m_frames)
	{
		frame.create();
	}

	m_texturePool.create();

	m_defaultPipelineLayout = PipelineLayout::Builder()
		.addPushConstantRange({ .stageFlags = vk::ShaderStageFlagBits::eVertex, .size = sizeof(CameraPushConstant) })
		.addDescriptorSetLayout(m_texturePool.descriptorSetLayout)
		.build();

	m_defaultPipeline.construct(m_swapchain, m_defaultPipelineLayout);
	m_textPipeline.construct(m_swapchain, m_defaultPipelineLayout, Pipeline::Type::eText);
}

static VulkanFrame* frame = nullptr;

void lfRenderer::beginFrame(lf2d::Camera* camera)
{
	m_currentCamera = camera;
	auto width = lfWindow::GetWidth();
	auto height = lfWindow::GetHeight();

	frame = &m_frames[m_currentFrame];

	static double lastTime = glfwGetTime();

	m_deltaTime = static_cast<float>(glfwGetTime() - lastTime);
	lastTime = glfwGetTime();

	if (vc::Get().device.getFenceStatus(frame->fence) != vk::Result::eSuccess)
	{
		vc::Get().device.waitForFences(1, &frame->fence, true, UINT64_MAX);
	}

	if (m_swapchain.acquireNextImage(frame->imageAvailable, &m_imageIndex) == vk::Result::eErrorOutOfDateKHR)
	{
		this->recreateSwapchain();
	}

	frame->commandBuffer.begin({.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
	
	vk::ImageMemoryBarrier2 const imageMemoryBarrier {
		.srcStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
		.srcAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite,
		.dstStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
		.oldLayout = vk::ImageLayout::eUndefined,
		.newLayout = vk::ImageLayout::eAttachmentOptimal,
		.image = m_swapchain.images[m_imageIndex],
		.subresourceRange = {
			.aspectMask = vk::ImageAspectFlagBits::eColor,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1
		}
	};

	frame->commandBuffer.pipelineBarrier2({ .imageMemoryBarrierCount = 1, .pImageMemoryBarriers = &imageMemoryBarrier });

	vk::RenderingAttachmentInfo colorAttachment {
		.imageView = m_swapchain.imageViews[m_imageIndex],
		.imageLayout = vk::ImageLayout::eAttachmentOptimal,
		.loadOp = vk::AttachmentLoadOp::eClear,
		.storeOp = vk::AttachmentStoreOp::eStore,
		.clearValue = { vk::ClearColorValue{m_color} }
	};

	frame->commandBuffer.beginRendering({
		.renderArea = {{0, 0}, m_swapchain.extent},
		.layerCount = 1,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachment
	});

	frame->commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_defaultPipeline);

	vk::Viewport const viewport{0, 0, static_cast<float>(m_swapchain.extent.width), static_cast<float>(m_swapchain.extent.height), 0.f, 1.f};
	vk::Rect2D const scissor{.offset = { 0, 0 }, .extent = m_swapchain.extent};

	frame->commandBuffer.setViewport(0, viewport);
	frame->commandBuffer.setScissor(0, scissor);

	frame->commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_defaultPipelineLayout, 0, 1, &m_texturePool.descriptorSet, 0, nullptr);
}

void lfRenderer::endFrame(Mesh& mesh)
{
	float	  const halfZoom   = 0.5f / m_currentCamera->zoom;
	glm::mat4 const projection = glm::ortho(-halfZoom, halfZoom, -halfZoom, halfZoom, -1.f, 1.f);

	CameraPushConstant const cameraConstant {
		.projView = projection * glm::inverse(
			glm::translate(
				glm::mat4(1.f),
				{ m_currentCamera->getPosWithOffset() / lf2d::window::size(), 0.f }
			)
		)
	};
		
	frame->commandBuffer.pushConstants(m_defaultPipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(CameraPushConstant), &cameraConstant);

	mesh.render(frame);

	frame->commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_textPipeline);

	mesh.renderText(frame);

	vk::ImageMemoryBarrier2 const imageMemoryBarrier {
		.srcStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
		.srcAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite,
		.oldLayout = vk::ImageLayout::eAttachmentOptimal,
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

	frame->commandBuffer.endRendering();
	frame->commandBuffer.pipelineBarrier2({.imageMemoryBarrierCount = 1, .pImageMemoryBarriers = &imageMemoryBarrier });
	frame->commandBuffer.end();

	vc::Get().device.resetFences(1, &frame->fence);

	constexpr vk::PipelineStageFlags pipelineStage[]{ vk::PipelineStageFlagBits::eColorAttachmentOutput };
	
	vc::Get().graphicsQueue.submit(
		vk::SubmitInfo{
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &frame->imageAvailable,
			.pWaitDstStageMask = pipelineStage,
			.commandBufferCount = 1,
			.pCommandBuffers = &frame->commandBuffer,
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = &frame->renderFinished
		},
		frame->fence
	);

	auto const result = vc::Get().presentQueue.presentKHR(
		vk::PresentInfoKHR {
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &frame->renderFinished,
			.swapchainCount = 1,
			.pSwapchains = m_swapchain,
			.pImageIndices = &m_imageIndex
	});

	if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR)
	{
		recreateSwapchain();
	}

	m_currentFrame = (m_currentFrame + 1) % 2;
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
	m_swapchain.recreate(m_vsync);
}

void lfRenderer::teardown()
{
	m_texturePool.teardown();
	m_textPipeline.teardown();
	m_defaultPipeline.teardown();
	m_defaultPipelineLayout.teardown();

	for (auto& frame : m_frames)
		frame.teardown();

	m_swapchain.teardown();
	vc::Teardown();
}
