#include "pch.hpp"
#include "lf2d.hpp"
#include "window/Window.hpp"
#include "renderer/VulkanRenderer.hpp"
#include "engine/InputController.hpp"

static lfWindow s_window;
static lfRenderer s_renderer;
static Mesh s_mesh;
static bool s_shouldClose = false;

namespace lf2d
{
	float getDeltaTime()
	{
		return s_renderer.getDeltaTime();
	}

	double getTime()
	{
		return glfwGetTime();
	}

	bool isKeyPressed(int key)
	{
		return InputController::IsKeyPressed(key);
	}

	bool isKeyDown(int key)
	{
		return InputController::IsKeyDown(key);
	}

	bool isKeyReleased(int key)
	{
		return InputController::IsKeyReleased(key);
	}

	bool isKeyUp(int key)
	{
		return !InputController::IsKeyDown(key);
	}

	bool isButtonPressed(int button)
	{
		return InputController::IsButtonPressed(button);
	}

	bool isButtonDown(int button)
	{
		return InputController::IsButtonDown(button);
	}

	bool isButtonReleased(int button)
	{
		return InputController::IsButtonReleased(button);
	}

	bool isButtonUp(int button)
	{
		return !InputController::IsButtonDown(button);
	}

	glm::vec2 getCursorPos()
	{
		return { InputController::GetCursorPosX(), InputController::GetCursorPosY() };
	}

	double getCursorPosX()
	{
		return InputController::GetCursorPosX();
	}

	double getCursorPosY()
	{
		return InputController::GetCursorPosY();
	}

	double getMouseWheelOffset()
	{
		return InputController::GetMouseWheelOffset();
	}

	void window::create(int width, int height, std::string const& title, bool resizable, bool enableValidationLayers)
	{
		if (!s_window.isCreated())
		{
			s_window.create(width, height, title, resizable);
			s_renderer.create(enableValidationLayers);
		}
	}

	void window::waitEvents()
	{
		s_window.waitEvents();
	}

	bool window::shouldClose()
	{
		return s_shouldClose;
	}

	void window::close()
	{
		s_window.close();
	}

	std::string const& window::getTitle()
	{
		return lfWindow::GetTitle();
	}

	void window::setTitle(std::string_view title)
	{
		if (s_window.isCreated())
			lfWindow::SetTitle(title);
		else
			printf("[error] You can't set window title when it's not created\n");
	}

	glm::vec2 window::size()
	{
		return { lfWindow::GetWidth(), lfWindow::GetHeight() };
	}

	int window::width()
	{
		return lfWindow::GetWidth();
	}

	int window::height()
	{
		return lfWindow::GetHeight();
	}

	const char* window::getMonitorName()
	{
		return lfWindow::GetMonitorName();
	}

	glm::vec4 window::getMonitorWorkarea()
	{
		return lfWindow::GetMonitorWorkarea();
	}

	glm::vec2 window::getMonitorPhysicalSize()
	{
		return lfWindow::GetMonitorPhysicalSize();
	}

	glm::vec2 window::getMonitorPos()
	{
		return lfWindow::GetMonitorPos();
	}

	void renderer::beginRendering(Camera& camera)
	{
		if (!s_window.isCreated())
		{
			printf("[error] You can't call 'beginRendering' function when window is not created\n");
			return;
		}

		s_window.pollEvents();
		s_shouldClose = s_window.shouldClose();
		s_renderer.beginFrame(&camera);
		s_mesh.setCamera(&camera);
	}

	void renderer::endRendering()
	{
		if (!s_window.isCreated())
		{
			printf("[error] You can't call 'endRendering' function when window is not created\n");
			return;
		}

		s_renderer.endFrame(s_mesh);

		if (s_shouldClose)
			s_renderer.waitIdle();
	}

	void renderer::renderRect(const Rect& rect, Color color)
	{
		s_mesh.addRect(rect, color);
	}

	void renderer::renderRectGradientV(const Rect& rect, Color color1, Color color2)
	{
		s_mesh.addRectGradient(rect, color1, color1, color2, color2);
	}

	void renderer::renderRectGradientH(const Rect& rect, Color color1, Color color2)
	{
		s_mesh.addRectGradient(rect, color1, color2, color1, color2);
	}

	void renderer::renderRectGradient(const Rect& rect, Color color1, Color color2, Color color3, Color color4)
	{
		s_mesh.addRectGradient(rect, color1, color4, color2, color3);
	}

	void renderer::clearColor(Color color)
	{
		s_renderer.clearColor(color);
	}

	void renderer::setVsync(bool enabled)
	{
		s_renderer.setVsync(enabled);
	}
}