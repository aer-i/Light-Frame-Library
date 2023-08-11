#pragma once
#include "lf2d.hpp"
#include "VulkanBuffer.hpp"

struct Vertex
{
	Vertex() = default;
	Vertex(glm::vec2 position, glm::vec4 color, glm::vec2 uv, int32_t textureIndex);
	Vertex(const Vertex& other);
	Vertex(Vertex&& other);
	Vertex& operator=(const Vertex&) = default;
	Vertex& operator=(Vertex&&) = default;

	glm::vec2 position;
	glm::vec4 color;
	glm::vec2 uv;
	int32_t	  textureIndex;
};

class Mesh
{
public:
	void render(vk::CommandBuffer commandBuffer, VulkanBuffer& vertexBuffer, VulkanBuffer& indexBuffer);
	void setCamera(lf2d::Camera* camera);
	
	void addRect(lf2d::Rect const& rect, int textureIndex, lf2d::Color color1, lf2d::Color color2, lf2d::Color color3, lf2d::Color color4);

private:
	lf2d::Camera* m_currentCamera;
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
	uint32_t m_totalObjectCount = 0;
	uint32_t m_renderedObjectCount = 0;
};