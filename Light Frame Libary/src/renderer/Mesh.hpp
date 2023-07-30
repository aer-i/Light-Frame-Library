#pragma once
#include "lf2d.hpp"
#include "VulkanBuffer.hpp"

struct Vertex
{
	Vertex() = default;
	Vertex(lf2d::vec2 position, lf2d::vec3 color);
	Vertex(const Vertex& other);
	Vertex(Vertex&& other);

	lf2d::vec2 position;
	lf2d::vec3 color;
};

class Mesh
{
public:
	void render(vk::CommandBuffer commandBuffer, VulkanBuffer& vertexBuffer);
	void setCamera(lf2d::Camera* camera);

	void addRect(lf2d::Rect const& rect, lf2d::Color color);
	void addRectGradient(lf2d::Rect const& rect, lf2d::Color color1, lf2d::Color color2, lf2d::Color color3, lf2d::Color color4);

private:
	lf2d::Camera* m_currentCamera;
	std::vector<Vertex> m_vertices;
	uint32_t m_totalObjectCount = 0;
	uint32_t m_renderedObjecCount = 0;
};