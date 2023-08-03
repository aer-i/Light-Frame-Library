#pragma once

namespace lf2d
{
	inline constexpr float vec2::operator[](int i)
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

	template<typename A, typename B>
	inline constexpr vec2::vec2(A x, B y)
		: x{ static_cast<float>(x) }, y{ static_cast<float>(y) }
	{
		
	}

	inline constexpr vec2::vec2(float x, float y)
		: x{ x }, y{ y }
	{
		
	}

	template<typename T>
	inline constexpr vec2::vec2(T scalar)
		: x{ static_cast<float>(scalar) }, y{ static_cast<float>(scalar) }
	{
		
	}

	inline constexpr vec2::vec2(float scalar)
		: x{ scalar }, y{ scalar }
	{
		
	}

	inline constexpr vec2::vec2(vec2 const& v)
		: x{ v.x }, y{ v.y }
	{
		
	}

	inline constexpr vec2::vec2(vec2&& v)
		: x{ v.x }, y{ v.y }
	{
		
	}

	inline constexpr vec2& vec2::operator+=(vec2 const& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	template<typename T>
	inline constexpr vec2& vec2::operator+=(T scalar)
	{
		x += static_cast<float>(scalar);
		y += static_cast<float>(scalar);
		return *this;
	}

	inline constexpr vec2& vec2::operator-=(vec2 const& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	template<typename T>
	inline constexpr vec2& vec2::operator-=(T scalar)
	{
		x -= static_cast<float>(scalar);
		y -= static_cast<float>(scalar);
		return *this;
	}

	inline constexpr vec2& vec2::operator*=(vec2 const& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	template<typename T>
	inline constexpr vec2& vec2::operator*=(T scalar)
	{
		x *= static_cast<float>(scalar);
		y *= static_cast<float>(scalar);
		return *this;
	}

	inline constexpr vec2& vec2::operator/=(vec2 const& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	template<typename T>
	inline constexpr vec2& vec2::operator/=(T scalar)
	{
		x /= static_cast<float>(scalar);
		y /= static_cast<float>(scalar);
		return *this;
	}

	inline vec2& vec2::operator%=(vec2 const& v)
	{
		x = fmod(x, v.x);
		y = fmod(y, v.y);
		return *this;
	}

	template<typename T>
	inline constexpr vec2& vec2::operator%=(T scalar)
	{
		x = fmod(x, static_cast<float>(scalar));
		y = fmod(y, static_cast<float>(scalar));
		return *this;
	}

	inline constexpr vec2& vec2::operator++()
	{
		++x;
		++y;
		return *this;
	}

	inline constexpr vec2 vec2::operator++(int)
	{
		vec2 result{ *this };
		++*this;
		return result;
	}

	inline constexpr vec2& vec2::operator--()
	{
		--x;
		--y;
		return *this;
	}

	inline constexpr vec2 vec2::operator--(int)
	{
		vec2 result{ *this };
		--*this;
		return result;
	}

	inline constexpr vec2 operator+(vec2 const& v)
	{
		return v;
	}

	inline constexpr vec2 operator-(vec2 const& v)
	{
		return { -v.x, -v.y };
	}

	inline constexpr vec2 operator+(vec2 const& lhs, vec2 const& rhs)
	{
		return vec2(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	template <typename T>
	inline constexpr vec2 operator+(vec2 const& v, T scalar)
	{
		return vec2(v.x + scalar, v.y + scalar);
	}

	template <typename T>
	inline constexpr vec2 operator+(T scalar, vec2 const& v)
	{
		return vec2(scalar + v.x, scalar + v.y);
	}

	inline constexpr vec2 operator-(vec2 const& lhs, vec2 const& rhs)
	{
		return vec2(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	template <typename T>
	inline constexpr vec2 operator-(vec2 const& v, T scalar)
	{
		return vec2(v.x - scalar, v.y - scalar);
	}

	template <typename T>
	inline constexpr vec2 operator-(T scalar, vec2 const& v)
	{
		return vec2(scalar - v.x, scalar - v.y);
	}

	inline constexpr vec2 operator*(vec2 const& lhs, vec2 const& rhs)
	{
		return vec2(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	template <typename T>
	inline constexpr vec2 operator*(vec2 const& v, T scalar)
	{
		return vec2(v.x * scalar, v.y * scalar);
	}

	template <typename T>
	inline constexpr vec2 operator*(T scalar, vec2 const& v)
	{
		return vec2(scalar * v.x, scalar * v.y);
	}

	inline constexpr vec2 operator/(vec2 const& lhs, vec2 const& rhs)
	{
		return vec2(lhs.x / rhs.x, lhs.y / rhs.y);
	}

	template <typename T>
	inline constexpr vec2 operator/(vec2 const& v, T scalar)
	{
		return vec2(v.x / scalar, v.y / scalar);
	}

	template <typename T>
	inline constexpr vec2 operator/(T scalar, vec2 const& v)
	{
		return vec2(scalar / v.x, scalar / v.y);
	}

	inline constexpr vec2 operator%(vec2 const& lhs, vec2 const& rhs)
	{
		return {
			fmod(lhs.x, rhs.x),
			fmod(lhs.y, rhs.y)
		};
	}

	template <typename T>
	inline constexpr vec2 operator%(vec2 const& v, T scalar)
	{
		return {
			fmod(v.x, static_cast<float>(scalar)),
			fmod(v.y, static_cast<float>(scalar))
		};
	}

	template <typename T>
	inline constexpr vec2 operator%(T scalar, vec2 const& v)
	{
		return {
			fmod(static_cast<float>(scalar), v.x),
			fmod(static_cast<float>(scalar), v.y)
		};
	}

	inline constexpr bool operator==(vec2 const& lhs, vec2 const& rhs)
	{
		return ((lhs.x - rhs.x) < std::numeric_limits<float>::epsilon())
			&& ((lhs.y - rhs.y) < std::numeric_limits<float>::epsilon());
	}

	inline constexpr bool operator!=(vec2 const& lhs, vec2 const& rhs)
	{
		return !(lhs == rhs);
	}

	inline constexpr float vec3::operator[](int i)
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

	template<typename A, typename B, typename C>
	inline constexpr vec3::vec3(A x, B y, C z)
		: x{ static_cast<float>(x) }, y{ static_cast<float>(y) }, z{ static_cast<float>(z) }
	{
		
	}

	inline constexpr vec3::vec3(float x, float y, float z)
		: x{ x }, y{ y }, z{ z }
	{
		
	}

	template<typename T>
	inline constexpr vec3::vec3(T scalar)
		: x{ static_cast<float>(scalar) }, y{ static_cast<float>(scalar) }, z{ static_cast<float>(scalar) }
	{
		
	}

	inline constexpr vec3::vec3(float scalar)
		: x{ scalar }, y{ scalar }, z{ scalar }
	{
		
	}

	inline constexpr vec3::vec3(vec3 const& v)
		: x{ v.x }, y{ v.y }, z{ v.z }
	{
		
	}

	inline constexpr vec3::vec3(vec3&& v)
		: x{ v.x }, y{ v.y }, z{ v.z }
	{
		
	}

	template<typename T>
	inline constexpr vec3& vec3::operator+=(T scalar)
	{
		x += static_cast<float>(scalar);
		y += static_cast<float>(scalar);
		z += static_cast<float>(scalar);
		return *this;
	}

	inline constexpr vec3& vec3::operator+=(vec3 const& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}


	template<typename T>
	inline constexpr vec3& vec3::operator-=(T scalar)
	{
		x -= static_cast<float>(scalar);
		y -= static_cast<float>(scalar);
		z -= static_cast<float>(scalar);
		return *this;
	}

	inline constexpr vec3& vec3::operator-=(vec3 const& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	template<typename T>
	inline constexpr vec3& vec3::operator*=(T scalar)
	{
		x *= static_cast<float>(scalar);
		y *= static_cast<float>(scalar);
		z *= static_cast<float>(scalar);
		return *this;
	}

	inline constexpr vec3& vec3::operator*=(vec3 const& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}


	template<typename T>
	inline constexpr vec3& vec3::operator/=(T scalar)
	{
		x /= static_cast<float>(scalar);
		y /= static_cast<float>(scalar);
		z /= static_cast<float>(scalar);
		return *this;
	}

	inline constexpr vec3& vec3::operator/=(vec3 const& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	template<typename T>
	inline constexpr vec3& vec3::operator%=(T scalar)
	{
		x = fmod(x, static_cast<float>(scalar));
		y = fmod(y, static_cast<float>(scalar));
		z = fmod(z, static_cast<float>(scalar));
		return *this;
	}

	inline vec3& vec3::operator%=(vec3 const& v)
	{
		x = fmod(x, v.x);
		y = fmod(y, v.y);
		z = fmod(z, v.z);
		return *this;
	}


	inline constexpr vec3& vec3::operator++()
	{
		++x;
		++y;
		++z;
		return *this;
	}

	inline constexpr vec3 vec3::operator++(int)
	{
		vec3 result{ *this };
		++*this;
		return result;

	}


	inline constexpr vec3& vec3::operator--()
	{
		--x;
		--y;
		--z;
		return *this;
	}

	inline constexpr vec3 vec3::operator--(int)
	{
		vec3 result{ *this };
		--*this;
		return result;
	}

	inline constexpr vec3 operator+(vec3 const& v)
	{
		return v;
	}

	inline constexpr vec3 operator-(vec3 const& v)
	{
		return { -v.x, -v.y, -v.z };
	}

	inline constexpr vec3 operator+(vec3 const& lhs, vec3 const& rhs)
	{
		return vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}

	template <typename T>
	inline constexpr vec3 operator+(vec3 const& v, T scalar)
	{
		return vec3(v.x + scalar, v.y + scalar, v.z + scalar);
	}

	template <typename T>
	inline constexpr vec3 operator+(T scalar, vec3 const& v)
	{
		return vec3(scalar + v.x, scalar + v.y, scalar + v.z);
	}

	inline constexpr vec3 operator-(vec3 const& lhs, vec3 const& rhs)
	{
		return vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}

	template <typename T>
	inline constexpr vec3 operator-(vec3 const& v, T scalar)
	{
		return vec3(v.x - scalar, v.y - scalar, v.z - scalar);
	}

	template <typename T>
	inline constexpr vec3 operator-(T scalar, vec3 const& v)
	{
		return vec3(scalar - v.x, scalar - v.y, scalar - v.z);
	}

	inline constexpr vec3 operator*(vec3 const& lhs, vec3 const& rhs)
	{
		return vec3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
	}

	template <typename T>
	inline constexpr vec3 operator*(vec3 const& v, T scalar)
	{
		return vec3(v.x * scalar, v.y * scalar, v.z * scalar);
	}

	template <typename T>
	inline constexpr vec3 operator*(T scalar, vec3 const& v)
	{
		return vec3(scalar * v.x, scalar * v.y, scalar * v.z);
	}

	inline constexpr vec3 operator/(vec3 const& lhs, vec3 const& rhs)
	{
		return vec3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
	}

	template <typename T>
	inline constexpr vec3 operator/(vec3 const& v, T scalar)
	{
		return vec3(v.x / scalar, v.y / scalar, v.z / scalar);
	}

	template <typename T>
	inline constexpr vec3 operator/(T scalar, vec3 const& v)
	{
		return vec3(scalar / v.x, scalar / v.y, scalar / v.z);
	}

	inline constexpr vec3 operator%(vec3 const& lhs, vec3 const& rhs)
	{
		return {
			fmod(lhs.x, rhs.x),
			fmod(lhs.y, rhs.y),
			fmod(lhs.z, rhs.z)
		};
	}

	template <typename T>
	inline constexpr vec3 operator%(vec3 const& v, T scalar)
	{
		return {
			fmod(v.x, static_cast<float>(scalar)),
			fmod(v.y, static_cast<float>(scalar)),
			fmod(v.z, static_cast<float>(scalar))
		};
	}

	template <typename T>
	inline constexpr vec3 operator%(T scalar, vec3 const& v)
	{
		return {
			fmod(static_cast<float>(scalar), v.x),
			fmod(static_cast<float>(scalar), v.y),
			fmod(static_cast<float>(scalar), v.z)
		};
	}

	inline constexpr bool operator==(vec3 const& lhs, vec3 const& rhs)
	{
		return ((lhs.x - rhs.x) < std::numeric_limits<float>::epsilon())
			&& ((lhs.y - rhs.y) < std::numeric_limits<float>::epsilon())
			&& ((lhs.z - rhs.z) < std::numeric_limits<float>::epsilon());
	}

	inline constexpr bool operator!=(vec3 const& lhs, vec3 const& rhs)
	{
		return !(lhs == rhs);
	}

	inline constexpr float vec4::operator[](int i)
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

	template<typename A, typename B, typename C, typename D>
	inline constexpr vec4::vec4(A x, B y, C z, D w)
		: x{ static_cast<float>(x) }, y{ static_cast<float>(y) }, z{ static_cast<float>(z) }, w{ static_cast<float>(w) }
	{

	}

	inline constexpr vec4::vec4(float x, float y, float z, float w)
		: x{ x }, y{ y }, z{ z }, w{ w }
	{

	}

	template<typename T>
	inline constexpr vec4::vec4(T scalar)
		: x{ static_cast<float>(scalar) }, y{ static_cast<float>(scalar) }, z{ static_cast<float>(scalar) }, w{ static_cast<float>(scalar) }
	{

	}

	inline constexpr vec4::vec4(float scalar)
		: x{ scalar }, y{ scalar }, z{ scalar }, w{ scalar }
	{

	}

	inline constexpr vec4::vec4(vec4 const& v)
		: x{ v.x }, y{ v.y }, z{ v.z }, w{ v.w }
	{

	}

	inline constexpr vec4::vec4(vec4&& v)
		: x{ v.x }, y{ v.y }, z{ v.z }, w { v.w }
	{

	}

	template<typename T>
	inline constexpr vec4& vec4::operator+=(T scalar)
	{
		x += static_cast<float>(scalar);
		y += static_cast<float>(scalar);
		z += static_cast<float>(scalar);
		w += static_cast<float>(scalar);
		return *this;
	}

	inline constexpr vec4& vec4::operator+=(vec4 const& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}


	template<typename T>
	inline constexpr vec4& vec4::operator-=(T scalar)
	{
		x -= static_cast<float>(scalar);
		y -= static_cast<float>(scalar);
		z -= static_cast<float>(scalar);
		w -= static_cast<float>(scalar);
		return *this;
	}

	inline constexpr vec4& vec4::operator-=(vec4 const& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	template<typename T>
	inline constexpr vec4& vec4::operator*=(T scalar)
	{
		x *= static_cast<float>(scalar);
		y *= static_cast<float>(scalar);
		z *= static_cast<float>(scalar);
		w *= static_cast<float>(scalar);
		return *this;
	}

	inline constexpr vec4& vec4::operator*=(vec4 const& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}


	template<typename T>
	inline constexpr vec4& vec4::operator/=(T scalar)
	{
		x /= static_cast<float>(scalar);
		y /= static_cast<float>(scalar);
		z /= static_cast<float>(scalar);
		w /= static_cast<float>(scalar);
		return *this;
	}

	inline constexpr vec4& vec4::operator/=(vec4 const& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
		return *this;
	}

	template<typename T>
	inline constexpr vec4& vec4::operator%=(T scalar)
	{
		x = fmod(x, static_cast<float>(scalar));
		y = fmod(y, static_cast<float>(scalar));
		z = fmod(z, static_cast<float>(scalar));
		w = fmod(z, static_cast<float>(scalar));
		return *this;
	}

	inline vec4& vec4::operator%=(vec4 const& v)
	{
		x = fmod(x, v.x);
		y = fmod(y, v.y);
		z = fmod(z, v.z);
		w = fmod(w, v.w);
		return *this;
	}


	inline constexpr vec4& vec4::operator++()
	{
		++x;
		++y;
		++z;
		++w;
		return *this;
	}

	inline constexpr vec4 vec4::operator++(int)
	{
		vec4 result{ *this };
		++*this;
		return result;
	}


	inline constexpr vec4& vec4::operator--()
	{
		--x;
		--y;
		--z;
		--w;
		return *this;
	}

	inline constexpr vec4 vec4::operator--(int)
	{
		vec4 result{ *this };
		--*this;
		return result;
	}

	inline constexpr vec4 operator+(vec4 const& v)
	{
		return v;
	}

	inline constexpr vec4 operator-(vec4 const& v)
	{
		return { -v.x, -v.y, -v.z, - v.w };
	}

	inline constexpr vec4 operator+(vec4 const& lhs, vec4 const& rhs)
	{
		return vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
	}

	template <typename T>
	inline constexpr vec4 operator+(vec4 const& v, T scalar)
	{
		return vec4(v.x + scalar, v.y + scalar, v.z + scalar, v.w + scalar);
	}

	template <typename T>
	inline constexpr vec4 operator+(T scalar, vec4 const& v)
	{
		return vec4(scalar + v.x, scalar + v.y, scalar + v.z, scalar + v.w);
	}

	inline constexpr vec4 operator-(vec4 const& lhs, vec4 const& rhs)
	{
		return vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
	}

	template <typename T>
	inline constexpr vec4 operator-(vec4 const& v, T scalar)
	{
		return vec4(v.x - scalar, v.y - scalar, v.z - scalar, v.w - scalar);
	}

	template <typename T>
	inline constexpr vec4 operator-(T scalar, vec4 const& v)
	{
		return vec4(scalar - v.x, scalar - v.y, scalar - v.z, scalar - v.w);
	}

	inline constexpr vec4 operator*(vec4 const& lhs, vec4 const& rhs)
	{
		return vec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
	}

	template <typename T>
	inline constexpr vec4 operator*(vec4 const& v, T scalar)
	{
		return vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
	}

	template <typename T>
	inline constexpr vec4 operator*(T scalar, vec4 const& v)
	{
		return vec4(scalar * v.x, scalar * v.y, scalar * v.z, scalar * v.w);
	}

	inline constexpr vec4 operator/(vec4 const& lhs, vec4 const& rhs)
	{
		return vec4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
	}

	template <typename T>
	inline constexpr vec4 operator/(vec4 const& v, T scalar)
	{
		return vec4(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);
	}

	template <typename T>
	inline constexpr vec4 operator/(T scalar, vec4 const& v)
	{
		return vec4(scalar / v.x, scalar / v.y, scalar / v.z, scalar / v.w);
	}

	inline constexpr vec4 operator%(vec4 const& lhs, vec4 const& rhs)
	{
		return {
			fmod(lhs.x, rhs.x),
			fmod(lhs.y, rhs.y),
			fmod(lhs.z, rhs.z),
			fmod(lhs.w, rhs.w)
		};
	}

	template <typename T>
	inline constexpr vec4 operator%(vec4 const& v, T scalar)
	{
		return {
			fmod(v.x, static_cast<float>(scalar)),
			fmod(v.y, static_cast<float>(scalar)),
			fmod(v.z, static_cast<float>(scalar)),
			fmod(v.w, static_cast<float>(scalar))
		};
	}

	template <typename T>
	inline constexpr vec4 operator%(T scalar, vec4 const& v)
	{
		return {
			fmod(static_cast<float>(scalar), v.x),
			fmod(static_cast<float>(scalar), v.y),
			fmod(static_cast<float>(scalar), v.z),
			fmod(static_cast<float>(scalar), v.w)
		};
	}

	inline constexpr bool operator==(vec4 const& lhs, vec4 const& rhs)
	{
		return ((lhs.x - rhs.x) < std::numeric_limits<float>::epsilon())
			&& ((lhs.y - rhs.y) < std::numeric_limits<float>::epsilon())
			&& ((lhs.z - rhs.z) < std::numeric_limits<float>::epsilon())
			&& ((lhs.w - rhs.w) < std::numeric_limits<float>::epsilon());
	}

	inline constexpr bool operator!=(vec4 const& lhs, vec4 const& rhs)
	{
		return !(lhs == rhs);
	}
}
