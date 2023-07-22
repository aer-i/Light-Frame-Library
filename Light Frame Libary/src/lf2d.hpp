#pragma once
#include <string>

#if defined(LF_ENABLE_VALIDATION)
static inline bool validationLayersEnabled = true;
#else
static inline bool validationLayersEnabled = false;
#endif

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