#pragma once
#include "lf2d.hpp"
#include "VulkanBuffer.hpp"

struct Vertex
{
	Vertex() = default;
	Vertex(lf2d::vec2 const& position, lf2d::vec3 const& color);
	Vertex(const Vertex& other);

	lf2d::vec2 position;
	lf2d::vec3 color;
};

class Mesh
{
public:
	void render(vk::CommandBuffer commandBuffer, VulkanBuffer& vertexBuffer);

	void addRect(lf2d::Rect const& rect, lf2d::Color color);

private:
	std::vector<Vertex> m_vertices;
};