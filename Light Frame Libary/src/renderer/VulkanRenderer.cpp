#include "pch.hpp"
#include "VulkanRenderer.hpp"
#include "window/Window.hpp"
#include "lf2d.hpp"
#include "Mesh.hpp"

struct CameraPushConstant
{
	glm::mat4 MPV;
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

	const std::vector<Vertex> vertices = {
		{{0.0f, -0.5f}},
		{{0.5f, 0.5f}},
		{{-0.5f, 0.5f}},
	};

	m_vertexBuffer.create(sizeof(vertices[0]) * vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

	memcpy(m_vertexBuffer.mapped, vertices.data(), m_vertexBuffer.size);

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
	
	vk::ImageMemoryBarrier2 const imageMemoryBarrier {
		.srcStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
			.srcAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite,
			.dstStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
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

	// TODO: Add memory barrier

	m_currentCmd->pipelineBarrier2({ .imageMemoryBarrierCount = 1, .pImageMemoryBarriers = &imageMemoryBarrier });

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

	vk::Viewport const viewport{0, 0, static_cast<float>(m_swapchain.extent.width), static_cast<float>(m_swapchain.extent.height), 0.f, 1.f};
	vk::Rect2D const scissor{.offset = { 0, 0 }, .extent = m_swapchain.extent};

	m_currentCmd->setViewport(0, viewport);
	m_currentCmd->setScissor(0, scissor);

	static std::vector<Vertex> vertices = {
		{{0.0f, -0.1f}},
		{{0.1f, 0.1f}},
		{{-0.1f, 0.1f}}
	};

	if (GetAsyncKeyState('A'))
	{
		for (auto& vertex : vertices)
		{
			vertex.position.x -= 0.001f;
		}
	}
	if (GetAsyncKeyState('D'))
	{
		for (auto& vertex : vertices)
		{
			vertex.position.x += 0.001f;
		}
	}
	if (GetAsyncKeyState('W'))
	{
		for (auto& vertex : vertices)
		{
			vertex.position.y -= 0.001f;
		}
	}
	if (GetAsyncKeyState('S'))
	{
		for (auto& vertex : vertices)
		{
			vertex.position.y += 0.001f;
		}
	}

	if (GetAsyncKeyState('R'))
	{
		vertices.push_back({ {0.0f, -0.1f} });
		vertices.push_back({ {0.1f, 0.1f} });
		vertices.push_back({ {-0.1f, 0.1f} });

		vertices.push_back({ {0.0f, -0.1f + 3} });
		vertices.push_back({ {0.1f, 0.1f + 3} });
		vertices.push_back({ {-0.1f, 0.1f + 3} });

		vertices.push_back({ {0.0f, -0.1f - 3} });
		vertices.push_back({ {0.1f, 0.1f - 3} });
		vertices.push_back({ {-0.1f, 0.1f - 3} });
	}

	if (vertices.size() > 0)
	{
		if (vertices.size() > m_vertexBuffer.count || vertices.size() < m_vertexBuffer.count / 2)
		{
			m_vertexBuffer.create(sizeof(vertices[0]) * vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
			m_vertexBuffer.count = static_cast<uint32_t>(vertices.size());
		}

		memcpy(m_vertexBuffer.mapped, vertices.data(), m_vertexBuffer.size);

		constexpr vk::DeviceSize offsets[]{ 0 };
		m_currentCmd->bindVertexBuffers(0, 1, m_vertexBuffer, offsets);
		m_currentCmd->draw(m_vertexBuffer.count, 1, 0, 0);
	}

}

void lfRenderer::endFrame()
{
	vk::ImageMemoryBarrier2 const imageMemoryBarrier {
		.srcStageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput,
		.srcAccessMask = vk::AccessFlagBits2::eColorAttachmentWrite,
		.dstStageMask = vk::PipelineStageFlagBits2::eBottomOfPipe,
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
	m_currentCmd->pipelineBarrier2({ .imageMemoryBarrierCount = 1, .pImageMemoryBarriers = &imageMemoryBarrier });
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

	auto const result = vc::Get().presentQueue.presentKHR(
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
	m_swapchain.recreate(vsync);
}

void lfRenderer::teardown()
{
	m_defaultPipeline.teardown();
	m_defaultPipelineLayout.teardown();

	m_vertexBuffer.free();

	for (auto& frame : m_frames)
		frame.teardown();
	m_swapchain.teardown();
	vc::Teardown();
}
