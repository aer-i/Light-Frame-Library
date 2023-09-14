#include "pch.hpp"
#include "lf2d.hpp"
#include "renderer/VulkanRenderer.hpp"
#include "renderer/Text.hpp"

static lfRenderer s_renderer;
static Mesh s_mesh;
static Text s_text;
static lf2d::Camera* s_cameraPtr;
static int s_width, s_height;
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

#ifdef _WIN32
	Renderer::Renderer(HWND window, bool enableValidationLayers)
#endif
	{
		s_renderer.create(window, enableValidationLayers);
	}

	void Renderer::begin(Camera& camera) noexcept
	{
		s_cameraPtr = &camera;
		s_renderer.beginFrame(&camera);
		s_mesh.setCamera(&camera);
	}

	void Renderer::end() noexcept
	{
		s_renderer.endFrame(s_mesh);

		if (s_shouldClose)
			s_renderer.waitIdle();
	}

	void Renderer::updateViewport(int width, int height)
	{
		s_width = width;
		s_height = height;
	}

	void Renderer::rect(const Rect& rect, Color color, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, 0, rotation, color, color, color, color);
	}

	void Renderer::rect(const Rect& rect, const Texture& texture, Color color, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, texture.getIndex(), rotation, color, color, color, color);
	}
	
	void Renderer::rectGradientV(const Rect& rect, Color color1, Color color2, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, 0, rotation, color1, color1, color2, color2);
	}

	void Renderer::rectGradientV(const Rect& rect, const Texture& texture, Color color1, Color color2, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, texture.getIndex(), rotation, color1, color1, color2, color2);
	}

	void Renderer::rectGradientH(const Rect& rect, Color color1, Color color2, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, 0, rotation, color1, color2, color1, color2);
	}

	void Renderer::rectGradientH(const Rect& rect, const Texture& texture, Color color1, Color color2, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, texture.getIndex(), rotation, color1, color2, color1, color2);
	}

	void Renderer::rectGradient(const Rect& rect, Color color1, Color color2, Color color3, Color color4, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, 0, rotation, color1, color4, color2, color3);
	}

	void Renderer::rectGradient(const Rect& rect, const Texture& texture, Color color1, Color color2, Color color3, Color color4, glm::vec2 const origin, float rotation)
	{
		s_mesh.addRect(rect, texture.getIndex(), rotation, color1, color4, color2, color3);
	}

	void Renderer::text(const Font& font, std::string_view text, glm::vec2 position, float scale, Color color)
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

	void Renderer::worldText(const Font& font, std::string_view text, glm::vec2 position, float scale, Color color)
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

	void Renderer::clearColor(Color color)
	{
		s_renderer.clearColor(color);
	}

	void Renderer::setVsync(bool enabled)
	{
		s_renderer.setVsync(enabled);
	}
}

int lf2d::window::width()
{
	return s_width;
}

int lf2d::window::height()
{
	return s_height;
}

glm::vec2 lf2d::window::size()
{
	return { s_width, s_height };
}
