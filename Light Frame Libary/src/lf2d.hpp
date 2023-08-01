#pragma once
#include <string>

namespace lf2d
{
	struct vec2
	{
		float x, y;
	};

	struct vec3
	{
		float x, y, z;
	};

	struct vec4
	{
		float x, y, z, w;
	};

	struct Rect
	{
		float x, y, width, height;
	};

	struct Color
	{
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			: r{r}, g{g}, b{b}, a{a} {}

		constexpr Color()
			: r{ 0 }, g{ 0 }, b{ 0 }, a{ 0 } {}

		vec4 normalized() { return { r / 255.f, g / 255.f, b / 255.f, a / 255.f }; }

		uint8_t r, g, b, a;
	};

	struct Camera
	{
		vec2 position{}, offset{};
		float zoom{1.f}, rotation{};
	};

	int getWindowWidth();
	int getWindowHeight();
	float getDeltaTime();
	double getTime();

	bool isKeyPressed(int key);
	bool isKeyDown(int key);
	bool isKeyReleased(int key);
	bool isKeyUp(int key);
	
	class Renderer
	{
	public:
		Renderer() = default;
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

		void beginRendering(Camera& camera);
		void endRendering();
		
		void renderRect(const Rect& rect, Color color);
		void renderRectGradientV(const Rect& rect, Color color1, Color color2);
		void renderRectGradientH(const Rect& rect, Color color1, Color color2);
		void renderRectGradient(const Rect& rect, Color color1, Color color2, Color color3, Color color4);

		void clearColor(Color color);
		void setVsync(bool enabled);

		void createWindow(int width, int height, std::string const& title, bool resizable, bool enableValidationLayers = false);
		bool windowShouldClose();
	};


	namespace Key
	{
		constexpr int Space = 32;
		constexpr int Apostrophe = 39;
		constexpr int Comma = 44;
		constexpr int Minus = 45;
		constexpr int Period = 46;
		constexpr int Slash = 47;
		constexpr int n0 = 48;
		constexpr int n1 = 49;
		constexpr int n2 = 50;
		constexpr int n3 = 51;
		constexpr int n4 = 52;
		constexpr int n5 = 53;
		constexpr int n6 = 54;
		constexpr int n7 = 55;
		constexpr int n8 = 56;
		constexpr int n9 = 57;
		constexpr int Semicolon = 59;
		constexpr int Equal = 61;
		constexpr int A = 65;
		constexpr int B = 66;
		constexpr int C = 67;
		constexpr int D = 68;
		constexpr int E = 69;
		constexpr int F = 70;
		constexpr int G = 71;
		constexpr int H = 72;
		constexpr int I = 73;
		constexpr int J = 74;
		constexpr int K = 75;
		constexpr int L = 76;
		constexpr int M = 77;
		constexpr int N = 78;
		constexpr int O = 79;
		constexpr int P = 80;
		constexpr int Q = 81;
		constexpr int R = 82;
		constexpr int S = 83;
		constexpr int T = 84;
		constexpr int U = 85;
		constexpr int V = 86;
		constexpr int W = 87;
		constexpr int X = 88;
		constexpr int Y = 89;
		constexpr int Z = 90;
		constexpr int LeftBracket = 91;
		constexpr int Backslash = 92;
		constexpr int RightBracket = 93;
		constexpr int GraveAccent = 96;
		constexpr int World1 = 161;
		constexpr int World2 = 162;
		constexpr int Escape = 256;
		constexpr int Enter = 257;
		constexpr int Tab = 258;
		constexpr int Backspace = 259;
		constexpr int Insert = 260;
		constexpr int Delete = 261;
		constexpr int Right = 262;
		constexpr int Left = 263;
		constexpr int Down = 264;
		constexpr int Up = 265;
		constexpr int PageUp = 266;
		constexpr int PageDown = 267;
		constexpr int Home = 268;
		constexpr int End = 269;
		constexpr int CapsLock = 280;
		constexpr int ScrollLock = 281;
		constexpr int NumLock = 282;
		constexpr int PrintScreen = 283;
		constexpr int Pause = 284;
		constexpr int F1 = 290;
		constexpr int F2 = 291;
		constexpr int F3 = 292;
		constexpr int F4 = 293;
		constexpr int F5 = 294;
		constexpr int F6 = 295;
		constexpr int F7 = 296;
		constexpr int F8 = 297;
		constexpr int F9 = 298;
		constexpr int F10 = 299;
		constexpr int F11 = 300;
		constexpr int F12 = 301;
		constexpr int F13 = 302;
		constexpr int F14 = 303;
		constexpr int F15 = 304;
		constexpr int F16 = 305;
		constexpr int F17 = 306;
		constexpr int F18 = 307;
		constexpr int F19 = 308;
		constexpr int F20 = 309;
		constexpr int F21 = 310;
		constexpr int F22 = 311;
		constexpr int F23 = 312;
		constexpr int F24 = 313;
		constexpr int F25 = 314;
		constexpr int kp0 = 320;
		constexpr int kp1 = 321;
		constexpr int kp2 = 322;
		constexpr int kp3 = 323;
		constexpr int kp4 = 324;
		constexpr int kp5 = 325;
		constexpr int kp6 = 326;
		constexpr int kp7 = 327;
		constexpr int kp8 = 328;
		constexpr int kp9 = 329;
		constexpr int kpDecimal = 330;
		constexpr int kpDivide = 331;
		constexpr int kpMultiply = 332;
		constexpr int kpSubtract = 333;
		constexpr int kpAdd = 334;
		constexpr int kpEnter = 335;
		constexpr int kpEqual = 336;
		constexpr int LeftShift = 340;
		constexpr int leftControl = 341;
		constexpr int LeftAlt = 342;
		constexpr int LeftSuper = 343;
		constexpr int RightShift = 344;
		constexpr int RightControl = 345;
		constexpr int RightAlt = 346;
		constexpr int RightSuper = 347;
		constexpr int Menu = 348;
		constexpr int Last = Menu;
	};
}

#define Color_Transparent {   0,   0,   0,   0 }
#define Color_Black		  {   0,   0,   0, 255 }
#define Color_White		  { 255, 255, 255, 255 }
#define Color_Lime		  {   0, 255,   0, 255 }
#define Color_Yellow	  { 255, 255,   0, 255 }
#define Color_Aqua		  {   0, 255, 255, 255 }
#define Color_Magenta	  { 255,   0, 255, 255 }
#define Color_Silver	  { 192, 192, 192, 255 }
#define Color_Maroon	  { 128,   0,   0, 255 }
#define Color_Olive		  { 128, 128,   0, 255 }
#define Color_Teal		  {   0, 128, 128, 255 }
#define Color_Navy		  {   0,   0, 128, 255 }
#define Color_LightGray   { 200, 200, 200, 255 }
#define Color_Gray        { 130, 130, 130, 255 }
#define Color_DarkGray    {  80,  80,  80, 255 }
#define Color_Gold        { 255, 203,   0, 255 }  
#define Color_Orange      { 255, 161,   0, 255 }  
#define Color_Pink        { 255, 109, 194, 255 }
#define Color_Red         { 230,  41,  55, 255 }
#define Color_Green       {   0, 228,  48, 255 }   
#define Color_DarkGreen   {   0, 117,  44, 255 }   
#define Color_LightBlue   { 102, 191, 255, 255 }
#define Color_Blue        {   0, 121, 241, 255 }  
#define Color_DarkBlue    {   0,  82, 172, 255 }   
#define Color_Purple      { 200, 122, 255, 255 }
#define Color_Violet      { 135,  60, 190, 255 } 
#define Color_DarkPurple  { 112,  31, 126, 255 } 
#define Color_Beige       { 211, 176, 131, 255 }
#define Color_Brown       { 127, 106,  79, 255 } 
#define Color_DarkBrown   {  76,  63,  47, 255 }  