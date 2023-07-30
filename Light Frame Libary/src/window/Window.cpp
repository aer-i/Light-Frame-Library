#include "pch.hpp"
#include "Window.hpp"

GLFWwindow* lfWindow::s_glfwWindow = nullptr;
int			lfWindow::s_width = 0;
int			lfWindow::s_height = 0;
std::string lfWindow::s_title;

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

	w = std::max(100, w);
	h = std::max(100, h);

	s_glfwWindow = glfwCreateWindow(w, h, n.c_str(), nullptr, nullptr);

	InputController::SetCallback(s_glfwWindow);

	lfWindow::s_width = w;
	lfWindow::s_height = h;
	lfWindow::s_title = n;

	if (s_glfwWindow)
	{
		spdlog::info("Created window with {} px width and {} px height", w, h);
	}
	else
	{
		spdlog::critical("Failed to create window!");
	}
}

vk::SurfaceKHR lfWindow::CreateSurface(vk::Instance instance)
{
	VkSurfaceKHR cSurface;
	if (glfwCreateWindowSurface(instance, s_glfwWindow, nullptr, &cSurface) != VK_SUCCESS)
	{
		spdlog::critical("Failed to create window surface!");
		return nullptr;
	}

	return vk::SurfaceKHR(cSurface);
}