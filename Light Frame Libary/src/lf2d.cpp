#include "pch.hpp"
#include "lf2d.hpp"
#include "window/Window.hpp"
#include "renderer/VulkanRenderer.hpp"

static lfWindow window;
static lfRenderer renderer;
static Mesh mesh;
static bool shouldClose = false;

namespace lf2d
{
	int getWindowWidth()
	{
		return lfWindow::GetWidth();
	}
	int getWindowHeight()
	{
		return lfWindow::GetHeight();
	}

	float getDeltaTime()
	{
		return renderer.getDeltaTime();
	}

	double getTime()
	{
		return glfwGetTime();
	}

	void Renderer::beginRendering(Camera& camera)
	{
		window.pollEvents();
		shouldClose = window.shouldClose();
		renderer.beginFrame(&camera);
		mesh.setCamera(&camera);
	}

	void Renderer::endRendering()
	{
		renderer.endFrame(mesh);

		if (shouldClose)
		{
			renderer.waitIdle();
		}
	}

	void Renderer::renderRect(const Rect& rect, Color color)
	{
		mesh.addRect(rect, color);
	}

	void Renderer::renderRectGradientV(const Rect& rect, Color color1, Color color2)
	{
		mesh.addRectGradient(rect, color1, color2, color1, color2);
	}

	void Renderer::renderRectGradientH(const Rect& rect, Color color1, Color color2)
	{
		mesh.addRectGradient(rect, color1, color1, color2, color2);
	}

	void Renderer::renderRectGradient(const Rect& rect, Color color1, Color color2, Color color3, Color color4)
	{
		mesh.addRectGradient(rect, color1, color2, color3, color4);
	}

	void Renderer::clearColor(Color color)
	{
		renderer.clearColor(color);
	}

	void Renderer::setVsync(bool enabled)
	{
		renderer.setVsync(enabled);
	}

	void Renderer::createWindow(int width, int height, std::string const& title, bool resizable, bool enableValidationLayers)
	{
		static std::once_flag flag;
		std::call_once(flag, [&]
		{
			window.create(width, height, title, resizable);
			renderer.create(enableValidationLayers);
		});
	}

	bool Renderer::windowShouldClose()
	{
		return shouldClose;
	}
}