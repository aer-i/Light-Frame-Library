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
		mesh.addRect({(float)x, (float)y, (float)width, (float)height}, color);
	}

	void Renderer::renderRectV(vec2 position, vec2 size, Color color)
	{
		mesh.addRect({position.x, position.y, size.x, size.y}, color);
	}

	void Renderer::renderRectR(const Rect& rect, Color color)
	{
		mesh.addRect(rect, color);
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