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

#define Color_Black		{   0,   0,   0, 255 }
#define Color_White		{ 255, 255, 255, 255 }
#define Color_Red		{ 255,   0,   0, 255 }
#define Color_Lime		{   0, 255,   0, 255 }
#define Color_Blue		{   0,   0, 255, 255 }
#define Color_Yellow	{ 255, 255,   0, 255 }
#define Color_Aqua		{   0, 255, 255, 255 }
#define Color_Magenta	{ 255,   0, 255, 255 }
#define Color_Silver	{ 192, 192, 192, 255 }
#define Color_Gray		{ 128, 128, 128, 255 }
#define Color_Maroon	{ 128,   0,   0, 255 }
#define Color_Olive		{ 128, 128,   0, 255 }
#define Color_Green		{   0, 128,   0, 255 }
#define Color_Purple	{ 128,   0, 128, 255 }
#define Color_Teal		{   0, 128, 128, 255 }
#define Color_Navy		{   0,   0, 128, 255 }

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
		void setVsync(bool enabled);

		void createWindow(int width, int height, std::string const& title, bool resizable, bool enableValidationLayers = false);
		void closeWindow();
		bool windowShouldClose();
	};
}