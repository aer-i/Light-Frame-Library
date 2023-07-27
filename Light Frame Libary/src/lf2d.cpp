#include "pch.hpp"
#include "lf2d.hpp"
#include "window/Window.hpp"
#include "renderer/VulkanRenderer.hpp"

static lfWindow window;
static lfRenderer renderer;

namespace lf2d
{
	int getWindowWidth()
	{
		return lfWindow::GetWidth();
	}
	int getWindowHeight()
	{
		return lfWindow::GetHeight();
	}

	void Renderer::beginRendering()
	{
		window.pollEvents();
		renderer.beginFrame();
	}

	void Renderer::endRendering()
	{
		renderer.endFrame();
	}

	void Renderer::clearColor(Color color)
	{
		renderer.clearColor(color);
	}

	void Renderer::setVsync(bool enabled)
	{
		renderer.setVsync(enabled);
	}

	void Renderer::createWindow(int width, int height, std::string const& title, bool resizable, bool enableValidationLayers)
	{
		static std::once_flag flag;
		std::call_once(flag, [&]
		{
			window.create(width, height, title, resizable);
			renderer.create(enableValidationLayers);
		});
	}

	void Renderer::closeWindow()
	{
		renderer.waitIdle();
	}

	bool Renderer::windowShouldClose()
	{
		return window.shouldClose();
	}
}