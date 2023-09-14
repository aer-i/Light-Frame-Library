#include <lf2d.hpp>
#include <GLFW/glfw3.h>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif
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

	// Initialize GLFW
	glfwInit();
	// Creating GLFW window
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Light Frame sample", nullptr, nullptr);

	// Initializing lf2d renderer
	lf2d::Renderer renderer{ glfwGetWin32Window(window), enableValidationLayers };

	// Enabling v-sync for lower power usage and no visible screen tearing
	renderer.setVsync(false);
	// You can clear color just once or every frame (or don't (black is default color))
	renderer.clearColor(lf2d::Color::Blue());

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

	// Same with textures
	lf2d::Font fonts[] = {
#ifdef _WIN32
		lf2d::Font{"C:/Windows/Fonts/Arial.ttf"},
		lf2d::Font{"C:/Windows/Fonts/Comic.ttf"}
#endif
	};

	// true is returned when window is closed
	while (!glfwWindowShouldClose(window)) // Main loop. Executing every frame
	{
		glfwPollEvents();

		int width, height;
		glfwGetWindowSize(window, &width, &height);
		renderer.updateViewport(width, height);

		//camera.zoom = std::max(0.25f, std::min(3.f, camera.zoom + 0.05f * (float)lf2d::getMouseWheelOffset()));
		
		if (GetAsyncKeyState('D'))
			camera.position.x += 300.f;// * lf2d::getDeltaTime();

		if (GetAsyncKeyState('A'))
			camera.position.x -= 300.f;// *lf2d::getDeltaTime();

		if (GetAsyncKeyState('W'))
			camera.position.y -= 300.f;// *lf2d::getDeltaTime();

		if (GetAsyncKeyState('S'))
			camera.position.y += 300.f;// *lf2d::getDeltaTime();

		renderer.begin(camera);
		{
			renderer.rect(camera.getViewRect(), texture3);
			
			renderer.rect({ 0 /*pos X in px*/, 0 /*pos Y in px*/, 100 /*width in px*/, 100 /*height in px*/ }, texture1);
			
			static constexpr lf2d::Rect rect = lf2d::Rect(-100, -100, 100, 100);
			renderer.rectGradientV(rect, lf2d::Color::Black(), lf2d::Color::White());
			
			renderer.rectGradient({ -100, 0, 100, 100 }, texture2, { 255, 0, 0, 255 }, { 255, 255, 255, 255 }, {0, 0, 255, 255}, {0, 255, 0, 255});
			
			renderer.rectGradientH(lf2d::Rect{ 0, -100, 100, 100 }, lf2d::Color::Gold(), lf2d::Color::Transparent());
			
			static std::vector<lf2d::Rect> cursorPosRects;
			
			/*if (lf2d::isButtonPressed(lf2d::Button::Left))
			{
				static auto lastCursorPos = lf2d::getCursorPos();
				auto cursorPos = camera.fromScreenToWorldPos(lf2d::getCursorPos());
			
				float distance = glm::distance(lastCursorPos, cursorPos);
			
				for (int i = 0; i <= distance; i += 20)
				{
					cursorPosRects.emplace_back(glm::lerp(lastCursorPos - 5.f, cursorPos - 5.f, i / distance), 10, 10);
				}
			
				lastCursorPos = cursorPos;
			}*/
			
			for (const auto& cursorRect : cursorPosRects)
			{
				renderer.rectGradientV(cursorRect, {255, 0, 255, 25}, { 255, 0, 255, 25 });
			}

			char text[16];
			sprintf_s(text, "FPS:");
			renderer.text(fonts[0], text, {100, 100}, 0.4f, lf2d::Color::Red());
			renderer.worldText(fonts[1], "Text 123~!@#$%^&*()_+?", {-120, 250}, 1.f);
		}
		renderer.end();
	}
	// Exit main loop

	// Terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
}
