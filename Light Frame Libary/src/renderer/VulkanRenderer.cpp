#include "pch.hpp"
#include "VulkanRenderer.hpp"
#include "window/Window.hpp"
#include "lf2d.hpp"
#include "Mesh.hpp"

struct CameraPushConstant
{
	glm::mat4 projView;
};

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

void lfRenderer::setVsync(bool enabled)
{
	vsync = enabled;

	if (vc::Get().device)
	{
		recreateSwapchain();
	}
}

void lfRenderer::create(bool enableVL)
{
	vc::Create(enableVL);
	m_swapchain.create(vsync);

	m_frames.resize(m_swapchain.images.size());

	for (auto& frame : m_frames)
	{
		frame.create();
	}

	m_defaultPipelineLayout = PipelineLayout::Builder()
		.addPushConstantRange({.stageFlags = vk::ShaderStageFlagBits::eVertex, .size = sizeof(CameraPushConstant)})
		.build();

	m_defaultPipeline.construct(m_swapchain, m_defaultPipelineLayout);
}

static VulkanFrame* frame = nullptr;

void lfRenderer::beginFrame()
{
	frame = &m_frames[m_frameIndex];

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

	static std::vector<Vertex> vertices = {
		{{0.0f, -0.1f}},
		{{0.1f, 0.1f}},
		{{-0.1f, 0.1f}}
	};

	if (!vertices.empty())
	{
		if (vertices.size() > frame->vertexBuffer.count || vertices.size() < frame->vertexBuffer.count / 2 )
		{
			frame->vertexBuffer.create(sizeof(vertices[0]) * vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
			frame->vertexBuffer.count = static_cast<uint32_t>(vertices.size());
		}

		memcpy(frame->vertexBuffer.mapped, vertices.data(), frame->vertexBuffer.size);


		static glm::vec3 position{};

		if (GetAsyncKeyState('W'))
		{
			position.y += 0.001f;
		}
		if (GetAsyncKeyState('S'))
		{
			position.y -= 0.001f;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::rotate(glm::mat4(1.f), glm::radians(0.f), glm::vec3(0, 0, 1));

		glm::mat4 view = glm::inverse(transform);
		glm::mat4 projection = glm::ortho(0, 1, 0, 1, -1, 1);

		CameraPushConstant cameraConstant{};
		cameraConstant.projView = projection * view;

		frame->commandBuffer.pushConstants(m_defaultPipelineLayout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(CameraPushConstant), &cameraConstant);

		constexpr vk::DeviceSize offsets[]{ 0 };
		frame->commandBuffer.bindVertexBuffers(0, 1, frame->vertexBuffer, offsets);
		frame->commandBuffer.draw(frame->vertexBuffer.count, 1, 0, 0);
	}
}

void lfRenderer::endFrame()
{
	vk::ImageMemoryBarrier2 const imageMemoryBarrier {
		.srcStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
		.srcAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite,
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
	m_swapchain.recreate(vsync);
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
