#include <lf2d.hpp>

auto main(int argc, char* const argv[]) -> int
{
	// Create lf2d renderer object;
	lf2d::Renderer renderer;

	// Initializing lf2d (GLFW, Vulkan, e.t.c)
	renderer.createWindow(1280, 720, "Light Frame - Example App", true);

	// You can clear color just once or every frame (or don't (black is default color))
	renderer.clearColor(Color_Gray);

	while (!renderer.windowShouldClose())
	{
		renderer.beginRendering();

		renderer.endRendering();
	}

	// Need to call this at the end, otherwise Vulkan objects won't be destroyed
	// TODO: Delete that function
	renderer.closeWindow();
}