#pragma once
#include <string>
#include <map>
#include <glm/glm.hpp>
#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace lf2d
{
	using Rect = glm::vec4;

	struct Color
	{
		uint8_t r, g, b, a;

		constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			: r{ r }, g{ g }, b{ b }, a{ a } {}

		consteval Color()
			: r{ 0 }, g{ 0 }, b{ 0 }, a{ 0 } {}

		glm::vec4 normalized() { return { r / 255.f, g / 255.f, b / 255.f, a / 255.f }; }
		
		static consteval Color Transparent() { return Color{   0,   0,   0,   0 }; }
		static consteval Color Black()		 { return Color{   0,   0,   0, 255 }; }
		static consteval Color White()		 { return Color{ 255, 255, 255, 255 }; }
		static consteval Color Lime()		 { return Color{   0, 255,   0, 255 }; }
		static consteval Color Yellow()		 { return Color{ 255, 255,   0, 255 }; }
		static consteval Color Aqua()		 { return Color{   0, 255, 255, 255 }; }
		static consteval Color Magenta()	 { return Color{ 255,   0, 255, 255 }; }
		static consteval Color Silver()		 { return Color{ 192, 192, 192, 255 }; }
		static consteval Color Maroon()		 { return Color{ 128,   0,   0, 255 }; }
		static consteval Color Olive()		 { return Color{ 128, 128,   0, 255 }; }
		static consteval Color Teal()		 { return Color{   0, 128, 128, 255 }; }
		static consteval Color Navy()		 { return Color{   0,   0, 128, 255 }; }
		static consteval Color LightGray()	 { return Color{ 200, 200, 200, 255 }; }
		static consteval Color Gray()		 { return Color{ 130, 130, 130, 255 }; }
		static consteval Color DarkGray()	 { return Color{  80,  80,  80, 255 }; }
		static consteval Color Gold()		 { return Color{ 255, 203,   0, 255 }; }
		static consteval Color Orange()		 { return Color{ 255, 161,   0, 255 }; }
		static consteval Color Pink()		 { return Color{ 255, 109, 194, 255 }; }
		static consteval Color Red()		 { return Color{ 230,  41,  55, 255 }; }
		static consteval Color Green()		 { return Color{   0, 228,  48, 255 }; }
		static consteval Color DarkGreen()	 { return Color{   0, 117,  44, 255 }; }
		static consteval Color LightBlue()	 { return Color{ 102, 191, 255, 255 }; }
		static consteval Color Blue()		 { return Color{   0, 121, 241, 255 }; }
		static consteval Color DarkBlue()	 { return Color{   0,  82, 172, 255 }; }
		static consteval Color Purple()		 { return Color{ 200, 122, 255, 255 }; }
		static consteval Color Violet()		 { return Color{ 135,  60, 190, 255 }; }
		static consteval Color DarkPurple()	 { return Color{ 112,  31, 126, 255 }; }
		static consteval Color Beige()		 { return Color{ 211, 176, 131, 255 }; }
		static consteval Color Brown()		 { return Color{ 127, 106,  79, 255 }; }
		static consteval Color DarkBrown()	 { return Color{  76,  63,  47, 255 }; }
	};

	struct Camera
	{
		glm::vec2 position{}, offset{};
		float zoom{1.f};

		inline glm::vec2 getPosWithOffset() { return position + offset; }
		inline glm::vec2 fromWorldToScreenPos(glm::vec2 const& v);
		inline glm::vec2 fromScreenToWorldPos(glm::vec2 const& v);
		inline lf2d::Rect getViewRect();
	};

	struct Texture
	{
	public:
		explicit Texture(std::string_view filepath, bool pixelated = false);
		Texture(void* buffer, size_t bufferSize, uint32_t width, uint32_t height);
		Texture(const Texture& other);
		inline int32_t getIndex() const { return m_index; }

	private:
		static int32_t s_currentTextureIndex;
		const int32_t m_index;
	};

	struct Font
	{
	public:
		explicit Font(std::string_view filepath, uint32_t textureResolution = 48);

	public:
		struct Character {
			Texture		 texture;
			glm::ivec2   size;
			glm::ivec2   bearing;
			uint32_t	 advance;
		};

	public:
		std::map<char, Character> m_characters;
	};

	namespace window
	{
		int width();
		int height();
		glm::vec2 size();
	}

	class Renderer
	{
	public:
#ifdef _WIN32
		Renderer(HWND window, bool enableValidationLayers = false);
#endif
		void begin(Camera& camera) noexcept;
		void end() noexcept;

		void updateViewport(int width, int height);

		void rect(const Rect& rect, Color color, glm::vec2 const origin = {}, float rotation = 0.f);
		void rect(const Rect& rect, const Texture& texture, Color color = Color::White(), glm::vec2 const origin = {}, float rotation = 0.f);
		void rectGradientV(const Rect& rect, Color color1, Color color2, glm::vec2 const origin = {}, float rotation = 0.f);
		void rectGradientV(const Rect& rect, const Texture& texture, Color color1, Color color2, glm::vec2 const origin = {}, float rotation = 0.f);
		void rectGradientH(const Rect& rect, Color color1, Color color2, glm::vec2 const origin = {}, float rotation = 0.f);
		void rectGradientH(const Rect& rect, const Texture& texture, Color color1, Color color2, glm::vec2 const origin = {}, float rotation = 0.f);
		void rectGradient(const Rect& rect, Color color1, Color color2, Color color3, Color color4, glm::vec2 const origin = {}, float rotation = 0.f);
		void rectGradient(const Rect& rect, const Texture& texture, Color color1, Color color2, Color color3, Color color4, glm::vec2 const origin = {}, float rotation = 0.f);

		void text(const Font& font, std::string_view text, glm::vec2 position, float scale = 1.f, Color color = Color::White());
		void worldText(const Font& font, std::string_view text, glm::vec2 position, float scale = 1.f, Color color = Color::White());

		void clearColor(Color color);
		void setVsync(bool enabled);
	};
}

inline glm::vec2 lf2d::Camera::fromWorldToScreenPos(glm::vec2 const& v)
{
	return {};// (v - this->getPosWithOffset())* zoom + window::size() / 2.f;
}

inline glm::vec2 lf2d::Camera::fromScreenToWorldPos(glm::vec2 const& v)
{
	return {};// (v - window::size() / 2.f) / zoom + this->getPosWithOffset();
}

inline lf2d::Rect lf2d::Camera::getViewRect()
{
	return {};// { fromScreenToWorldPos(glm::vec2{ 0.f }), lf2d::window::size() / zoom };
}
