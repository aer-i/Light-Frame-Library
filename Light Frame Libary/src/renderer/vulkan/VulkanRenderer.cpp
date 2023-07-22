#include "pch.hpp"
#include "VulkanRenderer.hpp"
#include "window/Window.hpp"

lfRenderer::~lfRenderer()
{
	m_swapchain.teardown();
	vc::Teardown();
}

void lfRenderer::beginFrame()
{
	/*if (m_swapchain.acquireNextImage(nullptr, nullptr) == vk::Result::eErrorOutOfDateKHR)
	{
		this->recreateSwapchain();
	}*/
}

void lfRenderer::endFrame()
{

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
