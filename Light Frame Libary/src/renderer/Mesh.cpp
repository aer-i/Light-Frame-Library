#include "pch.hpp"
#include "Mesh.hpp"

Vertex::Vertex(lf2d::vec2 const& position, lf2d::vec3 const& color)
{
	this->position = position;
	this->color = color;
}

Vertex::Vertex(const Vertex& other)
{
	this->position = other.position;
	this->color = other.color;
}

void Mesh::render(vk::CommandBuffer commandBuffer, VulkanBuffer& vertexBuffer)
{
	if (!m_vertices.empty())
	{
		if (m_vertices.size() > vertexBuffer.count)
		{
			vertexBuffer.create(sizeof(Vertex) * m_vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		}

		vertexBuffer.count = static_cast<uint32_t>(m_vertices.size());
		memcpy(vertexBuffer.mapped, m_vertices.data(), vertexBuffer.size);
		m_vertices.clear();

		constexpr vk::DeviceSize offsets[]{ 0 };
		commandBuffer.bindVertexBuffers(0, 1, vertexBuffer, offsets);
		commandBuffer.draw(vertexBuffer.count, 1, 0, 0);
	}

}

void Mesh::addRect(lf2d::Rect const& rect, lf2d::Color color)
{
	if (rect.width < 1 || rect.height < 1)
	{
		spdlog::error("Rect width and height can't be negative");
		return;
	}

	int w = lf2d::getWindowWidth(), h = lf2d::getWindowHeight();

	m_vertices.reserve(m_vertices.capacity() + 6);

	m_vertices.push_back({ {rect.x / w, rect.y / h}, color.normalizedVec3() });
	m_vertices.push_back({ {(rect.x + rect.width) / w, rect.y / h}, color.normalizedVec3() });
	m_vertices.push_back({ {rect.x / w, (rect.y + rect.height) / h}, color.normalizedVec3() });

	m_vertices.push_back({ {rect.x / w, (rect.y + rect.height) / h}, color.normalizedVec3() });
	m_vertices.push_back({ {(rect.x + rect.width) / w, rect.y / h}, color.normalizedVec3() });
	m_vertices.push_back({ {(rect.x + rect.width) / w, (rect.y + rect.height) / h}, color.normalizedVec3() });
}

void Mesh::addRectGradient(lf2d::Rect const& rect, lf2d::Color color1, lf2d::Color color2, lf2d::Color color3, lf2d::Color color4)
{
	if (rect.width < 1 || rect.height < 1)
	{
		spdlog::error("Rect width and height can't be negative");
		return;
	}

	int w = lf2d::getWindowWidth(), h = lf2d::getWindowHeight();

	m_vertices.reserve(m_vertices.capacity() + 6);

	m_vertices.push_back({ {rect.x / w, rect.y / h}, color1.normalizedVec3() });
	m_vertices.push_back({ {(rect.x + rect.width) / w, rect.y / h}, color2.normalizedVec3() });
	m_vertices.push_back({ {rect.x / w, (rect.y + rect.height) / h}, color3.normalizedVec3() });

	m_vertices.push_back({ {rect.x / w, (rect.y + rect.height) / h}, color3.normalizedVec3() });
	m_vertices.push_back({ {(rect.x + rect.width) / w, rect.y / h}, color2.normalizedVec3() });
	m_vertices.push_back({ {(rect.x + rect.width) / w, (rect.y + rect.height) / h}, color4.normalizedVec3() });
}
