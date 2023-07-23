#include <lf2d.hpp>

auto main(int argc, char* const argv[]) -> int
{
	lf2d::Renderer renderer;

	renderer.createWindow(1280, 720, "Light Frame - Example App", true);
	renderer.clearColor({ 50, 200, 255, 255 });

	while (!renderer.windowShouldClose())
	{
		renderer.beginRendering();

		renderer.endRendering();
	}

	renderer.closeWindow();
}