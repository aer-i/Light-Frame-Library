#include "pch.hpp"
#include "lf2d.hpp"
#include "window/Window.hpp"
#include "renderer/vulkan/VulkanRenderer.hpp"

static lfWindow window;
static lfRenderer renderer;

namespace lf2d
{
	int getWindowWidth()
	{
		return window.GetWidth();
	}
	int getWindowHeight()
	{
		return window.GetHeight();
	}

	void Renderer::beginRendering()
	{
		window.pollEvents();
		
	}

	void Renderer::endRendering()
	{

	}

	void Renderer::createWindow(int width, int height, std::string const& title, bool resizable)
	{
		static std::once_flag flag;
		std::call_once(flag, [&]
		{
			window.create(width, height, title, resizable);
			renderer.create();
		});
	}

	bool Renderer::windowShouldClose()
	{
		return window.shouldClose();
	}
}