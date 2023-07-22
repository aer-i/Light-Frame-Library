#pragma once

class lfWindow
{
public:
	lfWindow();
	~lfWindow();
	void create(int, int, std::string const&, bool);
	vk::SurfaceKHR createSurface(vk::Instance instance);

	inline void pollEvents() { glfwPollEvents(); }
	inline bool shouldClose() { return glfwWindowShouldClose(m_glfwWindow); }
	inline const std::string& getTitle() const { return m_title; }

private:
	GLFWwindow* m_glfwWindow = nullptr;
	std::string m_title;
};