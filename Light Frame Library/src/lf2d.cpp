#include "pch.hpp"
#include "lf2d.hpp"
#include "window/Window.hpp"
#include "renderer/VulkanRenderer.hpp"
#include "engine/InputController.hpp"
#include "renderer/Text.hpp"

static lfWindow s_window;
static lfRenderer s_renderer;
static Mesh s_mesh;
static Text s_text;
static lf2d::Camera* s_cameraPtr;
static bool s_shouldClose = false;

namespace lf2d
{
	int32_t Texture::s_currentTextureIndex = 0;

	Texture::Texture(std::string_view filepath, bool pixelated)
		: m_index{ s_currentTextureIndex++ }
	{
		s_renderer.loadTexture(filepath, pixelated);
	}

	Texture::Texture(void* buffer, size_t bufferSize, uint32_t width, uint32_t height)
		: m_index{ s_currentTextureIndex++ }
	{
		s_renderer.loadTexture(buffer, bufferSize, width, height);
	}

	Texture::Texture(const Texture& other)
		: m_index{ other.m_index }
	{}

	void Texture::free()
	{
		s_renderer.unloadTexture(m_index);
	}

	Font::Font(std::string_view filepath, uint32_t textureResolution)
	{
		auto face = s_text.load(filepath, textureResolution);

		for (uint8_t c = 32; c < 128; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				continue;
			}

			Texture texture(face->glyph->bitmap.buffer, face->glyph->bitmap.width * face->glyph->bitmap.rows, face->glyph->bitmap.width, face->glyph->bitmap.rows);

			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<uint32_t>(face->glyph->advance.x)
			};

			m_characters.insert(std::pair<char, Character>(c, character));
		}

		FT_Done_Face(face);
	}

	void Font::free()
	{
		for (const auto& c : m_characters)
		{
			s_renderer.unloadTexture(c.second.texture.getIndex());
		}
	}

	float getDeltaTime()
	{
		return s_renderer.getDeltaTime();
	}

	double getTime()
	{
		return glfwGetTime();
	}

	bool isKeyPressed(int key)
	{
		return InputController::IsKeyPressed(key);
	}

	bool isKeyDown(int key)
	{
		return InputController::IsKeyDown(key);
	}

	bool isKeyReleased(int key)
	{
		return InputController::IsKeyReleased(key);
	}

	bool isKeyUp(int key)
	{
		return !InputController::IsKeyDown(key);
	}

	bool isButtonPressed(int button)
	{
		return InputController::IsButtonPressed(button);
	}

	bool isButtonDown(int button)
	{
		return InputController::IsButtonDown(button);
	}

	bool isButtonReleased(int button)
	{
		return InputController::IsButtonReleased(button);
	}

	bool isButtonUp(int button)
	{
		return !InputController::IsButtonDown(button);
	}

	glm::vec2 getCursorPos()
	{
		return { InputController::GetCursorPosX(), InputController::GetCursorPosY() };
	}

	double getCursorPosX()
	{
		return InputController::GetCursorPosX();
	}

	double getCursorPosY()
	{
		return InputController::GetCursorPosY();
	}

	double getMouseWheelOffset()
	{
		return InputController::GetMouseWheelOffset();
	}

	uint32_t getFPS()
	{
		return InputController::GetFPS();
	}

	void window::create(int width, int height, std::string const& title, bool resizable, bool enableValidationLayers)
	{
		if (!s_window.isCreated())
		{
			s_window.create(width, height, title, resizable);
			s_renderer.create(enableValidationLayers);
		}
	}

	void window::waitEvents()
	{
		s_window.waitEvents();
	}

	bool window::shouldClose() noexcept
	{
		return s_shouldClose;
	}

	void window::close()
	{
		s_window.close();
	}

	std::string const& window::getTitle()
	{
		return lfWindow::GetTitle();
	}

	void window::setTitle(std::string_view title)
	{
		if (s_window.isCreated())
			lfWindow::SetTitle(title);
		else
			printf("[error] You can't set window title when it's not created\n");
	}

	glm::vec2 window::size()
	{
		return { lfWindow::GetWidth(), lfWindow::GetHeight() };
	}

	int window::width()
	{
		return lfWindow::GetWidth();
	}

	int window::height()
	{
		return lfWindow::GetHeight();
	}

	const char* window::getMonitorName()
	{
		return lfWindow::GetMonitorName();
	}

	glm::vec4 window::getMonitorWorkarea()
	{
		return lfWindow::GetMonitorWorkarea();
	}

	glm::vec2 window::getMonitorPhysicalSize()
	{
		return lfWindow::GetMonitorPhysicalSize();
	}

	glm::vec2 window::getMonitorPos()
	{
		return lfWindow::GetMonitorPos();
	}

	void renderer::begin(Camera& camera) noexcept
	{
		if (!s_window.isCreated())
		{
			printf("[error] You can't call 'beginRendering' function when window is not created\n");
			return;
		}

		s_cameraPtr = &camera;

		s_window.pollEvents();
		s_shouldClose = s_window.shouldClose();
		s_renderer.beginFrame(&camera);
		s_mesh.setCamera(&camera);
	}

	void renderer::end() noexcept
	{
		if (!s_window.isCreated())
		{
			printf("[error] You can't call 'endRendering' function when window is not created\n");
			return;
		}

		s_renderer.endFrame(s_mesh);

		if (s_shouldClose)
			s_renderer.waitIdle();
	}

	void renderer::rect(const Rect& rect, Color color, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, 0, rotation, color, color, color, color);
	}

	void renderer::rect(const Rect& rect, const Texture& texture, Color color, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, texture.getIndex(), rotation, color, color, color, color);
	}
	
	void renderer::rectGradientV(const Rect& rect, Color color1, Color color2, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, 0, rotation, color1, color1, color2, color2);
	}

	void renderer::rectGradientV(const Rect& rect, const Texture& texture, Color color1, Color color2, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, texture.getIndex(), rotation, color1, color1, color2, color2);
	}

	void renderer::rectGradientH(const Rect& rect, Color color1, Color color2, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, 0, rotation, color1, color2, color1, color2);
	}

	void renderer::rectGradientH(const Rect& rect, const Texture& texture, Color color1, Color color2, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, texture.getIndex(), rotation, color1, color2, color1, color2);
	}

	void renderer::rectGradient(const Rect& rect, Color color1, Color color2, Color color3, Color color4, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, 0, rotation, color1, color4, color2, color3);
	}

	void renderer::rectGradient(const Rect& rect, const Texture& texture, Color color1, Color color2, Color color3, Color color4, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, texture.getIndex(), rotation, color1, color4, color2, color3);
	}

	void renderer::text(const Font& font, std::string_view text, glm::vec2 position, float scale, Color color)
	{
		position = (position - window::size() / 2.f) / s_cameraPtr->zoom + s_cameraPtr->position - s_cameraPtr->offset;

		std::string_view::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			auto& ch = font.m_characters.at(*c);

			s_mesh.addText(
				{ position.x + ch.bearing.x * scale / s_cameraPtr->zoom, position.y - (ch.bearing.y) * scale / s_cameraPtr->zoom, ch.size.x * scale / s_cameraPtr->zoom, ch.size.y * scale / s_cameraPtr->zoom },
				ch.texture.getIndex(),
				color,
				color,
				color,
				color
			);

			position.x += (ch.advance >> 6) * scale / s_cameraPtr->zoom;
		}
	}

	void renderer::worldText(const Font& font, std::string_view text, glm::vec2 position, float scale, Color color)
	{
		std::string_view::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			auto& ch = font.m_characters.at(*c);

			s_mesh.addText(
				{ position.x + ch.bearing.x * scale, position.y - (ch.bearing.y) * scale, ch.size.x * scale, ch.size.y * scale },
				ch.texture.getIndex(),
				color,
				color,
				color,
				color
			);

			position.x += (ch.advance >> 6) * scale;
		}
	}

	void renderer::clearColor(Color color)
	{
		s_renderer.clearColor(color);
	}

	void renderer::setVsync(bool enabled)
	{
		s_renderer.setVsync(enabled);
	}
}