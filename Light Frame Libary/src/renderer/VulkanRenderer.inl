#pragma once
#include "lf2d.hpp"

inline void lfRenderer::renderRect(int x, int y, int width, int height, lf2d::Color color)
{
	m_vertices.emplace_back({ x			, y			 }, color.normalizedVec3());
	m_vertices.emplace_back({ x + width	, y			 }, color.normalizedVec3());
	m_vertices.emplace_back({ x			, y + height }, color.normalizedVec3());

	m_vertices.emplace_back({ x	+ width	, y			 }, color.normalizedVec3());
	m_vertices.emplace_back({ x			, y	+ height }, color.normalizedVec3());
	m_vertices.emplace_back({ x	+ width	, y + height }, color.normalizedVec3());
}
