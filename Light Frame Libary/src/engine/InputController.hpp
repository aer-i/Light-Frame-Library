#pragma once

class InputController
{
public:
	static inline void SetCallback(GLFWwindow* window) { Get().setCallback(window); }

	static bool IsKeyPressed(int key);
	static bool IsKeyDown(int key);
	static bool IsKeyReleased(int key);

	static void UpdateKeys();

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

private:
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

