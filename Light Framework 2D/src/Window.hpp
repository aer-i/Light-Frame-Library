#pragma once

class lfWindow
{
public:
	lfWindow() = default;
	~lfWindow() = default;
	void create(int, int, std::string const&, bool);

	inline void pollEvents() { glfwPollEvents(); }
	inline bool shouldClose() { return glfwWindowShouldClose(m_glfwWindow); }

private:
	GLFWwindow* m_glfwWindow = nullptr;
};