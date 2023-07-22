#pragma once
#include <string>

namespace lf2d
{
	int getWindowWidth();
	int getWindowHeight();

	struct Color
	{
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			: r{r}, g{g}, b{b}, a{a} {}

		constexpr Color()
			: r{ 0 }, g{ 0 }, b{ 0 }, a{ 0 } {}

		uint8_t r, g, b, a;
	};
	
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

		void clearColor(Color color);

		void createWindow(int width, int height, std::string const& title, bool resizable);
		void closeWindow();
		bool windowShouldClose();
	};
}