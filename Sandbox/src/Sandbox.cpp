#include <lf2d.hpp>
#include <cmath>

auto main(int argc, char* const argv[]) -> int
{
	// Create lf2d renderer object;
	// Basically main lf2d class that takes responsibility for window and renderer
	lf2d::Renderer renderer;

	// Main camera
	lf2d::Camera camera;

#ifndef NDEBUG
	// You can enable validation layers if you want
	// Vulkan SDK required
	constexpr bool enableValidationLayers = true;
#else
	// Disable validation layers in release mode for better performance
	constexpr bool enableValidationLayers = false;
#endif
	constexpr bool resizable = true;
	constexpr bool vsync = false;

	// Enabling v-sync for lower power usage and no visible screen tearing
	renderer.setVsync(vsync);
	// Initializing lf2d (GLFW, Vulkan, e.t.c)
	renderer.createWindow(1280, 720, "Light Frame - Example App", resizable, enableValidationLayers);

	// You can clear color just once or every frame (or don't (black is default color))
	renderer.clearColor(Color_Silver);

	// true is returned when window is closed
	while (!renderer.windowShouldClose()) // Main loop. Executing every frame
	{
		camera.position = lf2d::vec2{ 0.f, 0.f };
		// Setting this camera offset causes objects at position {0, 0} to be rendered in the center of the screen instead of in the top left corner
		camera.offset = { lf2d::getWindowWidth() / 2.f, lf2d::getWindowHeight() / 2.f };

		renderer.beginRendering();
		{
			// Add quad to render queue 
			renderer.renderRect(0 /*pos X in px*/, 0 /*pos Y in px*/, 100 /*width in px*/, 100 /*height in px*/, Color_Maroon);
			renderer.renderRect(-100 /*pos X in px*/, -100 /*pos Y in px*/, 100 /*width in px*/, 100 /*height in px*/, {std::sin(lf2d::getTime())});
		}
		renderer.endRendering(camera);
	}
	// Exit main loop

	// Need to call this at the end, otherwise Vulkan objects won't be destroyed
	// TODO: Delete that function :3
	renderer.closeWindow();
}