#include "pch.hpp"
#include "Window.hpp"

void lfWindow::create(int w, int h, std::string const& n, bool resizable)
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

	m_glfwWindow = glfwCreateWindow(w, h, n.c_str(), nullptr, nullptr);

}
