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
		printf("[critical] Failed to initialize GLFW\n");
	}
	else
	{
		printf("[info] Successfully initialized GLFW\n");
	}
}

lfWindow::~lfWindow()
{
	glfwTerminate();
	printf("[info] GLFW terminated\n");
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
		printf("[info] Created window with %d px width and %d px height\n", w, h);
	}
	else
	{
		printf("[critical] Failed to create window\n");
	}
}

vk::SurfaceKHR lfWindow::CreateSurface(vk::Instance instance)
{
	VkSurfaceKHR cSurface;
	if (glfwCreateWindowSurface(instance, s_glfwWindow, nullptr, &cSurface) != VK_SUCCESS)
	{
		printf("[critical] Failed to create window surface\n");
		return nullptr;
	}

	return vk::SurfaceKHR(cSurface);
}