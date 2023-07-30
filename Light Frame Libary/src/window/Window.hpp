#pragma once
#include "engine/InputController.hpp"

class lfWindow
{
public:
	lfWindow();
	~lfWindow();
	void create(int, int, std::string const&, bool);
	static vk::SurfaceKHR CreateSurface(vk::Instance instance);

	inline void pollEvents() { glfwPollEvents(); InputController::UpdateKeys(); }
	inline bool shouldClose() { return glfwWindowShouldClose(s_glfwWindow); }
	static inline int GetWidth() { return s_width; }
	static inline int GetHeight() { return s_height; }
	static inline void SetWidth(int w) { s_width = w; }
	static inline void SetHeight(int h) { s_height = h; }
	static inline const std::string& GetTitle() { return s_title; }

private:
	static GLFWwindow* s_glfwWindow;
	static std::string s_title;
	static int s_width, s_height;
};