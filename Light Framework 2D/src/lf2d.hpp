#pragma once
#include <string>

namespace lf2d
{
	int getWindowWidth();
	int getWindowHeight();
	
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void beginRendering();
		void endRendering();

		void createWindow(int width, int height, const std::string& title, bool resizable);
		bool windowShouldClose();

	private:


	};
}