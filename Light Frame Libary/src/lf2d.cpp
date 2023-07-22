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
		return {};
	}
	int getWindowHeight()
	{
		return {};
	}

	Renderer::Renderer()
	{
		
	}

	Renderer::~Renderer()
	{
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
		window.create(width, height, title, resizable);
		renderer.create(window);
	}

	bool Renderer::windowShouldClose()
	{
		return window.shouldClose();
	}
}