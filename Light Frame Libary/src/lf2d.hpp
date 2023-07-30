#pragma once
#include <string>

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

namespace lf2d
{
	int getWindowWidth();
	int getWindowHeight();
	float getDeltaTime();

	struct vec2
	{
		float x, y;
	};

	struct vec3
	{
		float x, y, z;
	};

	struct vec4
	{
		float x, y, z, w;
	};

	struct Rect
	{
		float x, y, width, height;
	};

	struct Color
	{
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			: r{r}, g{g}, b{b}, a{a} {}

		constexpr Color()
			: r{ 0 }, g{ 0 }, b{ 0 }, a{ 0 } {}

		vec3 normalizedVec3() { return { r / 255.f, g / 255.f, b / 255.f }; }

		uint8_t r, g, b, a;
	};

	struct Camera
	{
		vec2 position{}, offset{};
		float zoom{}, rotation{};
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
		void endRendering(Camera const& camera);

		void renderRect(int x, int y, int width, int height, Color color);
		void renderRectV(vec2 position, vec2 size, Color color);
		void renderRectR(const Rect& rect, Color color);

		void clearColor(Color color);
		void setVsync(bool enabled);

		void createWindow(int width, int height, std::string const& title, bool resizable, bool enableValidationLayers = false);
		void closeWindow();
		bool windowShouldClose();
	};
}