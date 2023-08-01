#pragma once

class InputController
{
public:
	static inline void SetCallback(GLFWwindow* window) { Get().setCallback(window); }

	static bool IsKeyPressed(int key);
	static bool IsKeyDown(int key);
	static bool IsKeyReleased(int key);

	static bool IsButtonPressed(int button);
	static bool IsButtonDown(int button);
	static bool IsButtonReleased(int button);

	static inline double GetCursorPosX() { return s_mouseX; }
	static inline double GetCursorPosY() { return s_mouseY; }

	static void Update();

private:
	static InputController& Get()
	{
		static InputController controller;
		return controller;
	}

	void setCallback(GLFWwindow* window);

	static bool s_keys[GLFW_KEY_LAST];
	static bool s_keysPressed[GLFW_KEY_LAST];
	static bool s_keysReleased[GLFW_KEY_LAST];

	static bool s_buttons[GLFW_MOUSE_BUTTON_LAST];
	static bool s_buttonsPressed[GLFW_MOUSE_BUTTON_LAST];
	static bool s_buttonsReleased[GLFW_MOUSE_BUTTON_LAST];

	static double s_mouseX, s_mouseY;

private:
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
};

