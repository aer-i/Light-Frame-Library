#pragma once
#include "lf2d.hpp"
#include "VulkanFrame.hpp"

struct Vertex
{
	Vertex() = default;
	Vertex(glm::vec2 position, glm::vec4 color, glm::vec2 uv, int32_t textureIndex);
	Vertex(const Vertex& other);
	Vertex(Vertex&& other) noexcept;
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
	uint32_t getTotalObjectCount() const { return m_tempTotalObjectCount; }
	uint32_t getRenderedObjectCount() const { return m_tempTotalRenderedObjectCount; }

	void render(VulkanFrame* frame);
	void renderText(VulkanFrame* frame);
	void setCamera(lf2d::Camera* camera);
	
	void addRect(lf2d::Rect const& rect, glm::vec2 const& origin, int textureIndex, float rotation, lf2d::Color color1, lf2d::Color color2, lf2d::Color color3, lf2d::Color color4);
	void addText(lf2d::Rect const& rect, glm::vec2 const& origin, int textureIndex, float rotation, lf2d::Color color1, lf2d::Color color2, lf2d::Color color3, lf2d::Color color4);

private:
	lf2d::Camera* m_currentCamera;
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
	std::vector<Vertex> m_textVertices;
	std::vector<uint32_t> m_textIndices;
	uint32_t m_totalObjectCount = 0;
	uint32_t m_renderedObjectCount = 0;
	uint32_t m_renderedTextCount = 0;

	uint32_t m_tempTotalObjectCount = 0;
	uint32_t m_tempTotalRenderedObjectCount = 0;
};