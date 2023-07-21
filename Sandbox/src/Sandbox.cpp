#include <lf2d.hpp>

auto main(int argc, char* argv[]) -> int
{
	lf2d::Renderer renderer;
	renderer.createWindow(1280, 720, "x", 1);

	while (!renderer.windowShouldClose())
	{
		renderer.beginRendering();

		renderer.endRendering();
	}
}