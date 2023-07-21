#include "pch.hpp"
#include "lf2d.hpp"
#include "Window.hpp"

static lfWindow window;

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
		glfwInit();
	}

	Renderer::~Renderer()
	{
		glfwTerminate();
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
	}

	bool Renderer::windowShouldClose()
	{
		return window.shouldClose();
	}
}