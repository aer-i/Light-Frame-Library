#pragma once
#include <string>

namespace lf2d
{
	int getWindowWidth();
	int getWindowHeight();
	
	class Renderer
	{
	public:
		Renderer() = default;
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

		void beginRendering();
		void endRendering();

		void createWindow(int width, int height, std::string const& title, bool resizable);
		bool windowShouldClose();
	};
}