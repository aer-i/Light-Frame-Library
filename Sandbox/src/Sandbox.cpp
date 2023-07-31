#include <lf2d.hpp>

auto main(int argc, char* const argv[]) -> int
{
	// Create lf2d renderer object;
	// Basically main lf2d class that takes responsibility for window and renderer
	lf2d::Renderer renderer;

	// Main camera
	lf2d::Camera camera;
	camera.position = lf2d::vec2{ 0.f, 0.f };
	camera.rotation = 0.f;
	camera.zoom = 1.0f;

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
	renderer.clearColor(Color_Blue);

	// true is returned when window is closed
	while (!renderer.windowShouldClose()) // Main loop. Executing every frame
	{
		// Setting this camera offset causes objects at position {0, 0} to be rendered in the center of the screen instead of in the top left corner
		camera.offset = { lf2d::getWindowWidth() / 2.f, lf2d::getWindowHeight() / 2.f };

		if (lf2d::isKeyDown(lf2d::Key::E))
		{
			camera.zoom += 1.f * lf2d::getDeltaTime();
		}
		if (lf2d::isKeyDown(lf2d::Key::Q))
		{
			camera.zoom -= 1.f * lf2d::getDeltaTime();
		}

		if (lf2d::isKeyDown(lf2d::Key::D))
			camera.position.x += 300.f * lf2d::getDeltaTime();

		if (lf2d::isKeyDown(lf2d::Key::A))
			camera.position.x -= 300.f * lf2d::getDeltaTime();

		if (lf2d::isKeyDown(lf2d::Key::W))
			camera.position.y -= 300.f * lf2d::getDeltaTime();

		if (lf2d::isKeyDown(lf2d::Key::S))
			camera.position.y += 300.f * lf2d::getDeltaTime();

		renderer.beginRendering(camera);
		{
			// Add quad to render queue
			renderer.renderRectGradientV({ -1920, -1080, 1920 * 2, 1080 * 2 }, Color_Olive, Color_Navy);
			renderer.renderRect({ 0 /*pos X in px*/, 0 /*pos Y in px*/, 100 /*width in px*/, 100 /*height in px*/ }, Color_Maroon);
			renderer.renderRectGradientV({ -100, -100, 100, 100 }, Color_Black, Color_White);
			// TODO: Fix colors for gradient
			renderer.renderRectGradient({ -100, 0, 100, 100 }, Color_Blue, Color_Aqua, Color_Magenta, Color_Red);
			renderer.renderRectGradientH({ 0, -100, 100, 100 }, Color_Olive, Color_Yellow);

		}
		renderer.endRendering();
	}
	// Exit main loop

}