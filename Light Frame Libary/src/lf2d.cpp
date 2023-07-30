#include "pch.hpp"
#include "lf2d.hpp"
#include "window/Window.hpp"
#include "renderer/VulkanRenderer.hpp"

static lfWindow window;
static lfRenderer renderer;
static Mesh mesh;

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

	void Renderer::beginRendering()
	{
		window.pollEvents();
		renderer.beginFrame();
	}

	void Renderer::endRendering(Camera const& camera)
	{
		renderer.endFrame(mesh, camera);
	}

	void Renderer::renderRect(int x, int y, int width, int height, Color color)
	{
		mesh.addRect({static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height)}, color);
	}

	void Renderer::renderRectV(vec2 position, vec2 size, Color color)
	{
		mesh.addRect({position.x, position.y, size.x, size.y}, color);
	}

	void Renderer::renderRectR(const Rect& rect, Color color)
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

	void Renderer::renderRectGradientF(const Rect& rect, Color color1, Color color2, Color color3, Color color4)
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

	void Renderer::closeWindow()
	{
		renderer.waitIdle();
	}

	bool Renderer::windowShouldClose()
	{
		return window.shouldClose();
	}
}