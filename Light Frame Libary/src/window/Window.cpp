#include "pch.hpp"
#include "Window.hpp"

lfWindow::lfWindow()
{
	if (glfwInit() == GLFW_FALSE)
	{
		spdlog::critical("Failed to initialize GLFW");
	}
	else
	{
		spdlog::info("Successfully initialized GLFW");
	}
}

lfWindow::~lfWindow()
{
	glfwTerminate();
	spdlog::info("GLFW terminated");
}

void lfWindow::create(int w, int h, std::string const& n, bool resizable)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

	m_glfwWindow = glfwCreateWindow(w, h, n.c_str(), nullptr, nullptr);
	spdlog::info("Created window with {} px width and {} px height", w, h);
}

vk::SurfaceKHR lfWindow::createSurface(vk::Instance instance)
{
	VkSurfaceKHR cSurface;
	if (glfwCreateWindowSurface(instance, m_glfwWindow, nullptr, &cSurface) != VK_SUCCESS)
	{
		spdlog::critical("Failed to create window surface!");
		return nullptr;
	}

	return vk::SurfaceKHR(cSurface);
}
