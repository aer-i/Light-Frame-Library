#include "pch.hpp"
#include "InputController.hpp"
#include "window/Window.hpp"

bool InputController::s_keys[GLFW_KEY_LAST] = { false };
bool InputController::s_keysPressed[GLFW_KEY_LAST] = { false };
bool InputController::s_keysReleased[GLFW_KEY_LAST] = { false };

bool InputController::IsKeyPressed(int key)
{
	if (key < 0 || key >= GLFW_KEY_LAST)
		return false;

	return s_keysPressed[key];
}

bool InputController::IsKeyDown(int key)
{
	if (key < 0 || key >= GLFW_KEY_LAST)
		return false;

	return s_keys[key];
}

bool InputController::IsKeyReleased(int key)
{
	if (key < 0 || key >= GLFW_KEY_LAST)
		return false;

	return s_keysReleased[key];
}

void InputController::UpdateKeys()
{
	for (int i = 0; i < GLFW_KEY_LAST; i++)
	{
		s_keysPressed[i] = false;
		s_keysReleased[i] = false;
	}
}

void InputController::setCallback(GLFWwindow* window)
{
	if (!window)
	{
		spdlog::error("GLFW window not created");
		return;
	}

	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	glfwSetKeyCallback(window, keyCallback);
}

void InputController::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	lfWindow::SetWidth(width);
	lfWindow::SetHeight(height);
}

void InputController::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UNKNOWN) return;

	if (action == GLFW_PRESS)
	{
		s_keys[key] = true;
		s_keysPressed[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		s_keys[key] = false;
		s_keysReleased[key] = true;
	}
}
