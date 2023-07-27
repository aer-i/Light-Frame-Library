#include <lf2d.hpp>

auto main(int argc, char* const argv[]) -> int
{
	// Create lf2d renderer object;
	// Basically main lf2d class that takes responsibility for window and renderer
	lf2d::Renderer renderer;

#ifndef NDEBUG
	constexpr bool enableValidationLayers = true;
#else
	constexpr bool enableValidationLayers = false;
#endif
	constexpr bool resizable = true;
	constexpr bool vsync = true;

	// Enabling v-sync for lower power usage and no visible screen tearing
	renderer.setVsync(vsync);
	// Initializing lf2d (GLFW, Vulkan, e.t.c)
	renderer.createWindow(1280, 720, "Light Frame - Example App", resizable, enableValidationLayers);

	// You can clear color just once or every frame (or don't (black is default color))
	renderer.clearColor(Color_Teal);

	// true is returned when window is closed
	while (!renderer.windowShouldClose()) // Main loop. Executing every frame
	{
		renderer.beginRendering();
		{
			
		}
		renderer.endRendering();
	}
	// Exit main loop

	// Need to call this at the end, otherwise Vulkan objects won't be destroyed
	// TODO: Delete that function :3
	renderer.closeWindow();
}