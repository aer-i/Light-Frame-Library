#pragma once
#include "engine/InputController.hpp"

class lfWindow
{
public:
	lfWindow();
	~lfWindow();
	void create(int, int, std::string const&, bool);
	static vk::SurfaceKHR CreateSurface(vk::Instance instance);

	inline void pollEvents() { InputController::Update(); glfwPollEvents(); }
	inline void waitEvents() { InputController::Update(); glfwWaitEvents(); }
	inline void close() { glfwSetWindowShouldClose(s_glfwWindow, 1); }
	inline bool shouldClose() { return glfwWindowShouldClose(s_glfwWindow); }
	inline bool isCreated() { return m_isCreated; }
	static inline int GetWidth() { return s_width; }
	static inline int GetHeight() { return s_height; }
	static inline void SetWidth(int w) { s_width = w; }
	static inline void SetHeight(int h) { s_height = h; }
	static inline const std::string& GetTitle() { return s_title; }
	static inline void SetTitle(std::string_view title) { glfwSetWindowTitle(s_glfwWindow, title.data()); }
	static inline const char* GetMonitorName() { return glfwGetMonitorName(s_glfwMonitor); }
	static inline glm::vec4 GetMonitorWorkarea() { int x, y, w, h; glfwGetMonitorWorkarea(s_glfwMonitor, &x, &y, &w, &h); return { x,y,w,h }; }
	static inline glm::vec2 GetMonitorPhysicalSize() { int w, h; glfwGetMonitorPhysicalSize(s_glfwMonitor, &w, &h); return { w,h }; }
	static inline glm::vec2 GetMonitorPos() { int x, y; glfwGetMonitorPos(s_glfwMonitor, &x, &y); return { x,y }; }

private:
	static GLFWwindow* s_glfwWindow;
	static GLFWmonitor* s_glfwMonitor;
	static std::string s_title;
	static int s_width, s_height;
	bool m_isCreated = false;
};