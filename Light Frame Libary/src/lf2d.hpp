#pragma once
#include <string>
#include <cassert>

namespace lf2d
{
	struct vec2
	{
		float x = 0, y = 0;

		inline constexpr float operator[](int i)
		{
			assert(i >= 0 && i < 2);

			switch (i)
			{
			case 0:
				return x;
			case 1:
				return y;
			default:
				return 0;
			}
		}

		vec2() = default;

		inline constexpr vec2(float x, float y)
			: x{ x }, y{ y } {}

		inline constexpr vec2(float scalar)
			: x{ scalar }, y{ scalar } {}

		template<typename A, typename B>
		inline constexpr vec2(A x, B y)
			: x{ static_cast<float>(x) }, y{ static_cast<float>(y) } {}

		template<typename T>
		inline constexpr vec2(T scalar)
			: x{ static_cast<float>(scalar) }, y{ static_cast<float>(scalar) } {}

		inline constexpr vec2(vec2 const& v)
			: x{ v.x }, y{ v.y } {}

		inline constexpr vec2(vec2&& v)
			: x{ v.x }, y{ v.y } {}

		vec2& operator=(vec2 const&) = default;
		vec2& operator=(vec2&& v) = default;

		inline constexpr vec2& operator+=(vec2 const& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		template<typename T>
		inline constexpr vec2& operator+=(T scalar)
		{
			x += static_cast<float>(scalar);
			y += static_cast<float>(scalar);
			return *this;
		}

		inline constexpr vec2& operator-=(vec2 const& v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		template<typename T>
		inline constexpr vec2& operator-=(T scalar)
		{
			x -= static_cast<float>(scalar);
			y -= static_cast<float>(scalar);
			return *this;
		}

		inline constexpr vec2& operator*=(vec2 const& v)
		{
			x *= v.x;
			y *= v.y;
			return *this;
		}

		template<typename T>
		inline constexpr vec2& operator*=(T scalar)
		{
			x *= static_cast<float>(scalar);
			y *= static_cast<float>(scalar);
			return *this;
		}

		inline constexpr vec2& operator/=(vec2 const& v)
		{
			x /= v.x;
			y /= v.y;
			return *this;
		}

		template<typename T>
		inline constexpr vec2& operator/=(T scalar)
		{
			x /= static_cast<float>(scalar);
			y /= static_cast<float>(scalar);
			return *this;
		}

		inline vec2& operator%=(vec2 const& v)
		{
			x = fmod(x, v.x);
			y = fmod(y, v.y);
			return *this;
		}

		template<typename T>
		inline constexpr vec2& operator%=(T scalar)
		{
			x = fmod(x, static_cast<float>(scalar));
			y = fmod(y, static_cast<float>(scalar));
			return *this;
		}

		template<typename T>
		inline constexpr vec2 operator%(T scalar)
		{
			return {
				fmod(x, static_cast<float>(scalar)),
				fmod(y, static_cast<float>(scalar))
			};
		}

		inline vec2 operator%(vec2 const& v)
		{
			return {
				fmod(x, v.x),
				fmod(y, v.y)
			};
		}

		inline constexpr vec2 operator+(vec2 const& v)
		{
			return vec2(x + v.x , y + v.y);
		}

		template<typename T>
		inline constexpr vec2 operator+(T scalar)
		{
			return vec2(x + static_cast<float>(scalar), y + static_cast<float>(scalar));
		}

		inline constexpr vec2 operator-(vec2 const& v)
		{
			return vec2(x - v.x, y - v.y);
		}

		template<typename T>
		inline constexpr vec2 operator-(T scalar)
		{
			return vec2(x - static_cast<float>(scalar), y - static_cast<float>(scalar));
		}

		inline constexpr vec2 operator*(vec2 const& v)
		{
			return vec2(x * v.x, y * v.y);
		}

		template<typename T>
		inline constexpr vec2 operator*(T scalar)
		{
			return vec2(scalar * x, scalar * y);
		}

		inline constexpr vec2 operator/(vec2 const& v)
		{
			return vec2(x / v.x, y / v.y);
		}

		template<typename T>
		inline constexpr vec2 operator/(T scalar)
		{
			return vec2(x / scalar, y / scalar);
		}

		inline constexpr bool operator==(vec2 const& v) const
		{
			return ((x - v.x) < std::numeric_limits<float>::epsilon())
				&& ((y - v.y) < std::numeric_limits<float>::epsilon());
		}

		inline constexpr bool operator!=(vec2 const& v)
		{
			return !(*this == v);
		}

		inline constexpr vec2 operator-()
		{
			return { -x, -y };
		}

		inline constexpr vec2 operator+()
		{
			return { x, y };
		}

		inline constexpr vec2& operator++()
		{
			++x;
			++y;
			return *this;
		}

		inline constexpr vec2 operator++(int)
		{
			vec2 result{ *this };
			++*this;
			return result;
		}

		inline constexpr vec2& operator--()
		{
			--x;
			--y;
			return *this;
		}

		inline constexpr vec2 operator--(int)
		{
			vec2 result{ *this };
			--*this;
			return result;
		}
	};

	struct vec3
	{
		float x = 0, y = 0, z = 0;

		inline constexpr float operator[](int i)
		{
			assert(i >= 0 && i < 3);

			switch (i)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			default:
				return 0;
			}
		}

		vec3() = default;

		inline constexpr vec3(float x, float y, float z)
			: x{ x }, y{ y }, z{ z } {}

		inline constexpr vec3(float scalar)
			: x{ scalar }, y{ scalar }, z{ scalar } {}

		template<typename T>
		inline constexpr vec3(T scalar)
			: x{ static_cast<float>(scalar) }, y{ static_cast<float>(scalar) }, z{ static_cast<float>(scalar) } {}

		template<typename A, typename B, typename C>
		inline constexpr vec3(A x, B y, C z)
			: x{ static_cast<float>(x) }, y{ static_cast<float>(y) }, z{ static_cast<float>(z) } {}

		inline constexpr vec3(vec3 const& v)
			: x{ v.x }, y{ v.y }, z{ v.z } {}

		inline constexpr vec3(vec3&& v)
			: x{ v.x }, y{ v.y }, z{ v.z } {}

		vec3& operator=(vec3 const&) = default;
		vec3& operator=(vec3&& v) = default;

		inline constexpr vec3& operator+=(vec3 const& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		template<typename T>
		inline constexpr vec3& operator+=(T scalar)
		{
			x += static_cast<float>(scalar);
			y += static_cast<float>(scalar);
			z += static_cast<float>(scalar);
			return *this;
		}

		inline constexpr vec3& operator-=(vec3 const& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		template<typename T>
		inline constexpr vec3& operator-=(T scalar)
		{
			x -= static_cast<float>(scalar);
			y -= static_cast<float>(scalar);
			z -= static_cast<float>(scalar);
			return *this;
		}

		inline constexpr vec3& operator*=(vec3 const& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}

		template<typename T>
		inline constexpr vec3& operator*=(T scalar)
		{
			x *= static_cast<float>(scalar);
			y *= static_cast<float>(scalar);
			z *= static_cast<float>(scalar);
			return *this;
		}

		inline constexpr vec3& operator/=(vec3 const& v)
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}

		template<typename T>
		inline constexpr vec3& operator/=(T scalar)
		{
			x /= static_cast<float>(scalar);
			y /= static_cast<float>(scalar);
			z /= static_cast<float>(scalar);
			return *this;
		}

		inline vec3& operator%=(vec3 const& v)
		{
			x = fmod(x, v.x);
			y = fmod(y, v.y);
			z = fmod(z, v.z);
			return *this;
		}

		template<typename T>
		inline constexpr vec3& operator%=(T scalar)
		{
			x = fmod(x, static_cast<float>(scalar));
			y = fmod(y, static_cast<float>(scalar));
			z = fmod(z, static_cast<float>(scalar));
			return *this;
		}

		template<typename T>
		inline constexpr vec3 operator%(T scalar)
		{
			return {
				fmod(x, static_cast<float>(scalar)),
				fmod(y, static_cast<float>(scalar)),
				fmod(z, static_cast<float>(scalar))
			};
		}

		inline vec3 operator%(vec3 const& v)
		{
			return {
				fmod(x, v.x),
				fmod(y, v.y),
				fmod(z, v.z)
			};
		}

		inline constexpr vec3 operator+(vec3 const& v)
		{
			return vec3(x + v.x, y + v.y, z + v.z);
		}

		template<typename T>
		inline constexpr vec3 operator+(T scalar)
		{
			return vec3(x + static_cast<float>(scalar), y + static_cast<float>(scalar), z + static_cast<float>(scalar));
		}

		inline constexpr vec3 operator-(vec3 const& v)
		{
			return vec3(x - v.x, y - v.y, z - v.z);
		}

		template<typename T>
		inline constexpr vec3 operator-(T scalar)
		{
			return vec3(x - static_cast<float>(scalar), y - static_cast<float>(scalar), z - static_cast<float>(scalar));
		}

		inline constexpr vec3 operator*(vec3 const& v)
		{
			return vec3(x * v.x, y * v.y, z * v.z);
		}

		template<typename T>
		inline constexpr vec3 operator*(T scalar)
		{
			return vec3(scalar * x, scalar * y, scalar * z);
		}

		inline constexpr vec3 operator/(vec3 const& v)
		{
			return vec3(x / v.x, y / v.y, z / v.z);
		}

		template<typename T>
		inline constexpr vec3 operator/(T scalar)
		{
			return vec3(x / scalar, y / scalar, z / scalar);
		}

		inline constexpr bool operator==(vec3 const& v) const
		{
			return ((x - v.x) < std::numeric_limits<float>::epsilon())
				&& ((y - v.y) < std::numeric_limits<float>::epsilon())
				&& ((z - v.z) < std::numeric_limits<float>::epsilon());
		}

		inline constexpr bool operator!=(vec3 const& v)
		{
			return !(*this == v);
		}

		inline constexpr vec3 operator-()
		{
			return { -x, -y, -z };
		}

		inline constexpr vec3 operator+()
		{
			return { x, y, z };
		}

		inline constexpr vec3& operator++()
		{
			++x;
			++y;
			++z;
			return *this;
		}

		inline constexpr vec3 operator++(int)
		{
			vec3 result{ *this };
			++*this;
			return result;
		}

		inline constexpr vec3& operator--()
		{
			--x;
			--y;
			--z;
			return *this;
		}

		inline constexpr vec3 operator--(int)
		{
			vec3 result{ *this };
			--*this;
			return result;
		}
	};

	struct vec4
	{
		float x = 0, y = 0, z = 0, w = 0;

		inline constexpr float operator[](int i)
		{
			assert(i >= 0 && i < 4);

			switch (i)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			default: 
				return 0;
			}

		}

		vec4() = default;

		inline constexpr vec4(float x, float y, float z, float w)
			: x{ x }, y{ y }, z{ z }, w{ w } {}

		inline constexpr vec4(float scalar)
			: x{ scalar }, y{ scalar }, z{ scalar }, w{ scalar } {}

		template<typename T>
		inline constexpr vec4(T scalar)
			: x{ static_cast<float>(scalar) }, y{ static_cast<float>(scalar) }, z{ static_cast<float>(scalar) }, w{ static_cast<float>(scalar) } {}

		template<typename A, typename B, typename C, typename D>
		inline constexpr vec4(A x, B y, C z, D w)
			: x{ static_cast<float>(x) }, y{ static_cast<float>(y) }, z{ static_cast<float>(z) }, w{ static_cast<float>(w) } {}

		inline constexpr vec4(vec4 const& v)
			: x{ v.x }, y{ v.y }, z{ v.z }, w{ v.w } {}

		inline constexpr vec4(vec4&& v)
			: x{ v.x }, y{ v.y }, z{ v.z }, w{ v.w } {}

		vec4& operator=(vec4 const&) = default;
		vec4& operator=(vec4&& v) = default;

		inline constexpr vec4& operator+=(vec4 const& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}

		template<typename T>
		inline constexpr vec4& operator+=(T scalar)
		{
			x += static_cast<float>(scalar);
			y += static_cast<float>(scalar);
			z += static_cast<float>(scalar);
			w += static_cast<float>(scalar);
			return *this;
		}

		inline constexpr vec4& operator-=(vec4 const& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}

		template<typename T>
		inline constexpr vec4& operator-=(T scalar)
		{
			x -= static_cast<float>(scalar);
			y -= static_cast<float>(scalar);
			z -= static_cast<float>(scalar);
			w -= static_cast<float>(scalar);
			return *this;
		}

		inline constexpr vec4& operator*=(vec4 const& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			w *= v.w;
			return *this;
		}

		template<typename T>
		inline constexpr vec4& operator*=(T scalar)
		{
			x *= static_cast<float>(scalar);
			y *= static_cast<float>(scalar);
			z *= static_cast<float>(scalar);
			w *= static_cast<float>(scalar);
			return *this;
		}

		inline constexpr vec4& operator/=(vec4 const& v)
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;
			w /= v.w;
			return *this;
		}

		template<typename T>
		inline constexpr vec4& operator/=(T scalar)
		{
			x /= static_cast<float>(scalar);
			y /= static_cast<float>(scalar);
			z /= static_cast<float>(scalar);
			w /= static_cast<float>(scalar);
			return *this;
		}

		inline vec4& operator%=(vec4 const& v)
		{
			x = fmod(x, v.x);
			y = fmod(y, v.y);
			z = fmod(z, v.z);
			w = fmod(w, v.w);
			return *this;
		}

		template<typename T>
		inline constexpr vec4& operator%=(T scalar)
		{
			x = fmod(x, static_cast<float>(scalar));
			y = fmod(y, static_cast<float>(scalar));
			z = fmod(z, static_cast<float>(scalar));
			w = fmod(w, static_cast<float>(scalar));
			return *this;
		}

		template<typename T>
		inline constexpr vec4 operator%(T scalar)
		{
			return {
				fmod(x, static_cast<float>(scalar)),
				fmod(y, static_cast<float>(scalar)),
				fmod(z, static_cast<float>(scalar)),
				fmod(w, static_cast<float>(scalar))
			};
		}

		inline vec4 operator%(vec4 const& v)
		{
			return {
				fmod(x, v.x),
				fmod(y, v.y),
				fmod(z, v.z),
				fmod(w, v.w)
			};
		}

		inline constexpr vec4 operator+(vec4 const& v)
		{
			return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
		}

		template<typename T>
		inline constexpr vec4 operator+(T scalar)
		{
			return vec4(x + static_cast<float>(scalar), y + static_cast<float>(scalar), z + static_cast<float>(scalar), w + static_cast<float>(scalar));
		}

		inline constexpr vec4 operator-(vec4 const& v)
		{
			return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
		}

		template<typename T>
		inline constexpr vec4 operator-(T scalar)
		{
			return vec4(x - static_cast<float>(scalar), y - static_cast<float>(scalar), z - static_cast<float>(scalar), w - static_cast<float>(scalar));
		}

		inline constexpr vec4 operator*(vec4 const& v)
		{
			return vec4(x * v.x, y * v.y, z * v.z, w * v.w);
		}

		template<typename T>
		inline constexpr vec4 operator*(T scalar)
		{
			return vec4(scalar * x, scalar * y, scalar * z, scalar * w);
		}

		inline constexpr vec4 operator/(vec4 const& v)
		{
			return vec4(x / v.x, y / v.y, z / v.z, w / v.w);
		}

		template<typename T>
		inline constexpr vec4 operator/(T scalar)
		{
			return vec4(x / scalar, y / scalar, z / scalar, w / scalar);
		}

		inline constexpr bool operator==(vec4 const& v) const
		{
			return ((x - v.x) < std::numeric_limits<float>::epsilon())
				&& ((y - v.y) < std::numeric_limits<float>::epsilon())
				&& ((z - v.z) < std::numeric_limits<float>::epsilon())
				&& ((w - v.w) < std::numeric_limits<float>::epsilon());
		}

		inline constexpr bool operator!=(vec4 const& v)
		{
			return !(*this == v);
		}

		inline constexpr vec4 operator-()
		{
			return { -x, -y, -z, -w };
		}

		inline constexpr vec4 operator+()
		{
			return { x, y, z, w };
		}

		inline constexpr vec4& operator++()
		{
			++x;
			++y;
			++z;
			++w;
			return *this;
		}

		inline constexpr vec4 operator++(int)
		{
			vec4 result{ *this };
			++*this;
			return result;
		}

		inline constexpr vec4& operator--()
		{
			--x;
			--y;
			--z;
			--w;
			return *this;
		}

		inline constexpr vec4 operator--(int)
		{
			vec4 result{ *this };
			--*this;
			return result;
		}
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
		float zoom{1.f};
	};

	int getWindowWidth();
	int getWindowHeight();
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

	double getCursorPosX();
	double getCursorPosY();

	double getMouseWheelOffset();
	
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