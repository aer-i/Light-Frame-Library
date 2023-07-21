#include "pch.hpp"
#include "lf2d.hpp"

static int windowWidth = 0;
static int windowHeight = 0;

static GLFWwindow* window = nullptr;

namespace lf2d
{
	int getWindowWidth()
	{
		return windowWidth;
	}

	int getWindowHeight()
	{
		return windowHeight;
	}

	void Renderer::createWindow(int width, int height, const std::string& title, bool resizable)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

		width = width <= 0 ? 100 : width;
		height = height <= 0 ? 100 : height;

		window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	}

	bool Renderer::windowShouldClose()
	{
		if (window != nullptr)
			return glfwWindowShouldClose(window);
		else
			return true;
	}
}
