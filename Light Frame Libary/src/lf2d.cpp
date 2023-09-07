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

	Font::Font(std::string_view filepath)
	{
		auto face = s_text.load(filepath);

		for (unsigned char c = 32; c < 127; c++)
		{
			// load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				printf("Failed to load glyph\n\n");
				continue;
			}

			Texture texture(face->glyph->bitmap.buffer, face->glyph->bitmap.width * face->glyph->bitmap.rows * 4, face->glyph->bitmap.width, face->glyph->bitmap.rows);

			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};

			m_characters.insert(std::pair<char, Character>(c, character));
		}

		FT_Done_Face(face);
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

	void renderer::rect(const Rect& rect, Color color)
	{
		s_mesh.addRect(rect, 0, color, color, color, color);
	}

	void renderer::rect(const Rect& rect, const Texture& texture, Color color)
	{
		s_mesh.addRect(rect, texture.getIndex(), color, color, color, color);
	}
	
	void renderer::rectGradientV(const Rect& rect, Color color1, Color color2)
	{
		s_mesh.addRect(rect, 0, color1, color1, color2, color2);
	}

	void renderer::rectGradientV(const Rect& rect, const Texture& texture, Color color1, Color color2)
	{
		s_mesh.addRect(rect, texture.getIndex(), color1, color1, color2, color2);
	}

	void renderer::rectGradientH(const Rect& rect, Color color1, Color color2)
	{
		s_mesh.addRect(rect, 0, color1, color2, color1, color2);
	}

	void renderer::rectGradientH(const Rect& rect, const Texture& texture, Color color1, Color color2)
	{
		s_mesh.addRect(rect, texture.getIndex(), color1, color2, color1, color2);
	}

	void renderer::rectGradient(const Rect& rect, Color color1, Color color2, Color color3, Color color4)
	{
		s_mesh.addRect(rect, 0, color1, color4, color2, color3);
	}

	void renderer::rectGradient(const Rect& rect, const Texture& texture, Color color1, Color color2, Color color3, Color color4)
	{
		s_mesh.addRect(rect, texture.getIndex(), color1, color4, color2, color3);
	}

	void renderer::text(const Font& font, std::string_view text, glm::vec2 position, float scale, Color color)
	{
		std::string_view::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			auto& ch = font.m_characters.at(*c);

			float xpos = position.x + ch.bearing.x * scale;
			float ypos = position.y - (ch.size.y - ch.bearing.y) * scale;

			float w = ch.size.x * scale;
			float h = ch.size.y * scale;

			rect({ xpos, ypos, w, h }, ch.texture);

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