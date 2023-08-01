#include "pch.hpp"
#include "InputController.hpp"
#include "window/Window.hpp"

bool InputController::s_keys[GLFW_KEY_LAST] = { false };
bool InputController::s_keysPressed[GLFW_KEY_LAST] = { false };
bool InputController::s_keysReleased[GLFW_KEY_LAST] = { false };

bool InputController::s_buttons[GLFW_MOUSE_BUTTON_LAST] = { false };
bool InputController::s_buttonsPressed[GLFW_MOUSE_BUTTON_LAST] = { false };
bool InputController::s_buttonsReleased[GLFW_MOUSE_BUTTON_LAST] = { false };

double InputController::s_mouseX = 0.0, InputController::s_mouseY = 0.0;

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

bool InputController::IsButtonPressed(int button)
{
	if (button < 0 || button >= GLFW_MOUSE_BUTTON_LAST)
		return false;

	return s_buttonsPressed[button];
}

bool InputController::IsButtonDown(int button)
{
	if (button < 0 || button >= GLFW_MOUSE_BUTTON_LAST)
		return false;

	return s_buttons[button];
}

bool InputController::IsButtonReleased(int button)
{
	if (button < 0 || button >= GLFW_MOUSE_BUTTON_LAST)
		return false;

	return s_buttonsReleased[button];
}

void InputController::Update()
{
	for (int i = 0; i < GLFW_KEY_LAST; i++)
	{
		s_keysPressed[i] = false;
		s_keysReleased[i] = false;
	}

	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
	{
		s_buttonsPressed[i] = false;
		s_buttonsReleased[i] = false;
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
	glfwSetMouseButtonCallback(window, mouseCallback);
	glfwSetCursorPosCallback(window, cursorCallback);
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

void InputController::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		s_buttons[button] = true;
		s_buttonsPressed[button] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		s_buttons[button] = false;
		s_buttonsReleased[button] = true;
	}
}

void InputController::cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	s_mouseX = xpos;
	s_mouseY = ypos;
}
