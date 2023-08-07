#pragma once
#include <string>
#include <glm/glm.hpp>

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

		inline constexpr glm::vec2 fromWorldToScreenPos(glm::vec2 const& v)
		{
			return (v - position) * zoom + offset;
		}

		inline constexpr glm::vec2 fromScreenToWorldPos(glm::vec2 const& v)
		{
			return (v - offset) / zoom + position;
		}
	};

	float getDeltaTime();
	double getTime();

	bool isKeyPressed(int key);
	bool isKeyDown(int key);
	bool isKeyReleased(int key);
	bool isKeyUp(int key);

	bool isButtonPressed(int button);
	bool isButtonDown(int button);
	bool isButtonReleased(int button);
	bool isButtonUp(int button);

	glm::vec2 getCursorPos();
	double getCursorPosX();
	double getCursorPosY();

	double getMouseWheelOffset();

	namespace window
	{
		void create(int width, int height, std::string const& title, bool resizable, bool enableValidationLayers = false);
		void waitEvents();
		bool shouldClose();
		void close();
		std::string const& getTitle();
		void setTitle(std::string_view title);
		glm::vec2 size();
		int width();
		int height();
		const char* getMonitorName();
		glm::vec4 getMonitorWorkarea();
		glm::vec2 getMonitorPhysicalSize();
		glm::vec2 getMonitorPos();
	}
	
	namespace renderer
	{
		void beginRendering(Camera& camera);
		void endRendering();
	
		void renderRect(const Rect& rect, Color color);
		void renderRectGradientV(const Rect& rect, Color color1, Color color2);
		void renderRectGradientH(const Rect& rect, Color color1, Color color2);
		void renderRectGradient(const Rect& rect, Color color1, Color color2, Color color3, Color color4);

		void clearColor(Color color);
		void setVsync(bool enabled);
	}

	namespace Key
	{
		constexpr int Space			 = 32;
		constexpr int Apostrophe	 = 39;
		constexpr int Comma			 = 44;
		constexpr int Minus			 = 45;
		constexpr int Period		 = 46;
		constexpr int Slash			 = 47;
		constexpr int n0			 = 48;
		constexpr int n1			 = 49;
		constexpr int n2			 = 50;
		constexpr int n3			 = 51;
		constexpr int n4			 = 52;
		constexpr int n5			 = 53;
		constexpr int n6			 = 54;
		constexpr int n7			 = 55;
		constexpr int n8			 = 56;
		constexpr int n9			 = 57;
		constexpr int Semicolon		 = 59;
		constexpr int Equal			 = 61;
		constexpr int A				 = 65;
		constexpr int B				 = 66;
		constexpr int C				 = 67;
		constexpr int D				 = 68;
		constexpr int E				 = 69;
		constexpr int F				 = 70;
		constexpr int G				 = 71;
		constexpr int H				 = 72;
		constexpr int I				 = 73;
		constexpr int J				 = 74;
		constexpr int K				 = 75;
		constexpr int L				 = 76;
		constexpr int M				 = 77;
		constexpr int N				 = 78;
		constexpr int O				 = 79;
		constexpr int P				 = 80;
		constexpr int Q				 = 81;
		constexpr int R				 = 82;
		constexpr int S				 = 83;
		constexpr int T				 = 84;
		constexpr int U				 = 85;
		constexpr int V				 = 86;
		constexpr int W				 = 87;
		constexpr int X				 = 88;
		constexpr int Y				 = 89;
		constexpr int Z				 = 90;
		constexpr int LeftBracket	 = 91;
		constexpr int Backslash		 = 92;
		constexpr int RightBracket	 = 93;
		constexpr int GraveAccent	 = 96;
		constexpr int World1		 = 161;
		constexpr int World2		 = 162;
		constexpr int Escape		 = 256;
		constexpr int Enter			 = 257;
		constexpr int Tab			 = 258;
		constexpr int Backspace		 = 259;
		constexpr int Insert		 = 260;
		constexpr int Delete		 = 261;
		constexpr int Right			 = 262;
		constexpr int Left			 = 263;
		constexpr int Down			 = 264;
		constexpr int Up			 = 265;
		constexpr int PageUp		 = 266;
		constexpr int PageDown		 = 267;
		constexpr int Home			 = 268;
		constexpr int End			 = 269;
		constexpr int CapsLock		 = 280;
		constexpr int ScrollLock	 = 281;
		constexpr int NumLock		 = 282;
		constexpr int PrintScreen	 = 283;
		constexpr int Pause			 = 284;
		constexpr int F1			 = 290;
		constexpr int F2			 = 291;
		constexpr int F3			 = 292;
		constexpr int F4			 = 293;
		constexpr int F5			 = 294;
		constexpr int F6			 = 295;
		constexpr int F7			 = 296;
		constexpr int F8			 = 297;
		constexpr int F9			 = 298;
		constexpr int F10			 = 299;
		constexpr int F11			 = 300;
		constexpr int F12			 = 301;
		constexpr int F13			 = 302;
		constexpr int F14			 = 303;
		constexpr int F15			 = 304;
		constexpr int F16			 = 305;
		constexpr int F17			 = 306;
		constexpr int F18			 = 307;
		constexpr int F19			 = 308;
		constexpr int F20			 = 309;
		constexpr int F21			 = 310;
		constexpr int F22			 = 311;
		constexpr int F23			 = 312;
		constexpr int F24			 = 313;
		constexpr int F25			 = 314;
		constexpr int kp0			 = 320;
		constexpr int kp1			 = 321;
		constexpr int kp2			 = 322;
		constexpr int kp3			 = 323;
		constexpr int kp4			 = 324;
		constexpr int kp5			 = 325;
		constexpr int kp6			 = 326;
		constexpr int kp7			 = 327;
		constexpr int kp8			 = 328;
		constexpr int kp9			 = 329;
		constexpr int kpDecimal		 = 330;
		constexpr int kpDivide		 = 331;
		constexpr int kpMultiply	 = 332;
		constexpr int kpSubtract	 = 333;
		constexpr int kpAdd			 = 334;
		constexpr int kpEnter		 = 335;
		constexpr int kpEqual		 = 336;
		constexpr int LeftShift		 = 340;
		constexpr int leftControl	 = 341;
		constexpr int LeftAlt		 = 342;
		constexpr int LeftSuper		 = 343;
		constexpr int RightShift	 = 344;
		constexpr int RightControl	 = 345;
		constexpr int RightAlt		 = 346;
		constexpr int RightSuper	 = 347;
		constexpr int Menu			 = 348;
		constexpr int Last			 = Menu;
	};

	namespace Button
	{
		constexpr int Left			 = 0;
		constexpr int Right			 = 1;
		constexpr int Middle		 = 2;
		constexpr int Back			 = 3;
		constexpr int Forward		 = 4;
	}
}

