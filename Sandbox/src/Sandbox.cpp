#include <lf2d.hpp>
#include <glm/gtx/compatibility.hpp>
#include <vector>

auto main([[maybe_unused]]int argc, [[maybe_unused]]char* const argv[]) -> int
{
#ifndef NDEBUG
	// You can enable validation layers if you want
	// Vulkan SDK required
	constexpr bool enableValidationLayers = true;
#else
	// Disable validation layers in release mode for better performance
	constexpr bool enableValidationLayers = false;
#endif

	lf2d::Font font("fonts/arial.ttf");

	// Initializing lf2d (GLFW, Vulkan, e.t.c)
	lf2d::window::create(1280, 720, "Light Frame - Example App", true, enableValidationLayers);

	// Enabling v-sync for lower power usage and no visible screen tearing
	lf2d::renderer::setVsync(false);
	// You can clear color just once or every frame (or don't (black is default color))
	lf2d::renderer::clearColor(lf2d::Color::Blue());

	// Main camera
	lf2d::Camera camera;
	// Camera position is always in the center of the screen
	camera.position = glm::vec2{ 0.f, 0.f };
	// For example, if you want camera position to be in top left corner set offset to window::size() / 2.0f
	// Normally it should be the other way around, but I'll change it
	camera.offset = glm::vec2{ 0.f };
	// Default zoom should be equal to 1.
	camera.zoom = 1.0f;

	// Always load textures after window creation
	lf2d::Texture texture1("textures/hit.png", true);
	lf2d::Texture texture2("textures/pexels.jpg", false);
	lf2d::Texture texture3("textures/nx.png", true);

	// true is returned when window is closed
	while (!lf2d::window::shouldClose()) // Main loop. Executing every frame
	{
		camera.zoom = std::max(0.25f, std::min(3.f, camera.zoom + 0.05f * (float)lf2d::getMouseWheelOffset()));
		
		if (lf2d::isKeyDown(lf2d::Key::D))
			camera.position.x += 300.f * lf2d::getDeltaTime();

		if (lf2d::isKeyDown(lf2d::Key::A))
			camera.position.x -= 300.f * lf2d::getDeltaTime();

		if (lf2d::isKeyDown(lf2d::Key::W))
			camera.position.y -= 300.f * lf2d::getDeltaTime();

		if (lf2d::isKeyDown(lf2d::Key::S))
			camera.position.y += 300.f * lf2d::getDeltaTime();

		lf2d::renderer::beginRendering(camera);
		{
			lf2d::renderer::renderRect(camera.getViewRect(), texture3);

			// Add quads to render queue
			lf2d::renderer::renderRect({ 0 /*pos X in px*/, 0 /*pos Y in px*/, 100 /*width in px*/, 100 /*height in px*/ }, texture1);

			static constexpr lf2d::Rect rect = lf2d::Rect(-100, -100, 100, 100);
			lf2d::renderer::renderRectGradientV(rect, lf2d::Color::Black(), lf2d::Color::White());

			lf2d::renderer::renderRectGradient({ -100, 0, 100, 100 }, texture2, { 255, 0, 0, 255 }, { 255, 255, 255, 255 }, {0, 0, 255, 255}, {0, 255, 0, 255});

			lf2d::renderer::renderRectGradientH(lf2d::Rect{ 0, -100, 100, 100 }, lf2d::Color::Gold(), lf2d::Color::Transparent());

			static std::vector<lf2d::Rect> cursorPosRects;

			if (lf2d::isButtonPressed(lf2d::Button::Left))
			{
				static auto lastCursorPos = lf2d::getCursorPos();
				auto cursorPos = camera.fromScreenToWorldPos(lf2d::getCursorPos());

				float distance = glm::distance(lastCursorPos, cursorPos);

				for (int i = 0; i <= distance; i += 20)
				{
					cursorPosRects.emplace_back(glm::lerp(lastCursorPos - 5.f, cursorPos - 5.f, i / distance), 10, 10);
				}

				lastCursorPos = cursorPos;
			}

			for (const auto& cursorRect : cursorPosRects)
			{
				lf2d::renderer::renderRectGradientV(cursorRect, {255, 0, 255, 25}, { 255, 0, 255, 25 });
			}
		}
		lf2d::renderer::endRendering();
	}
	// Exit main loop
}