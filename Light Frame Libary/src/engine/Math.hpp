#pragma once
#include "lf2d.hpp"

namespace lfMath
{
	struct mat4
	{
		lf2d::vec4 value[4];

		inline constexpr mat4(float s)
			: value{ {s, 0, 0, 0}, {0, s, 0, 0}, { 0, 0, s, 0}, {0, 0, 0, s} } {}

		template<typename T>
		inline constexpr mat4(T s)
			: value
				{ {static_cast<float>(s), 0, 0, 0},
				{0, static_cast<float>(s), 0, 0},
				{ 0, 0, static_cast<float>(s), 0},
				{0, 0, 0, static_cast<float>(s)} }
		{}

		inline constexpr lf2d::vec4& operator[](int i)
		{
			assert(i < 4);
			return value[i];
		}
	};

	template<typename T>
	constexpr mat4 ortho(T left, T right, T bottom, T top, T zNear, T zFar)
	{
		mat4 m{ 1.f };
		m[0][0] = 2.f / (right - left);
		m[1][1] = 2.f / (top - bottom);
		m[2][2] = -1.f / (zFar - zNear);
		m[3][0] = -(right + left) / (right - left);
		m[3][1] = -(top + bottom) / (top - bottom);
		m[3][2] = -zNear / (zFar - zNear);
		return m;
	}

	constexpr mat4 translate(mat4 const& m, lf2d::vec3 const& v)
	{
		mat4 result = m;
		//result[3] = m[0] * v[0] + m[1]
	}

	constexpr mat4 inverse()
	{
		//
		return { 1.f };
	}

}
