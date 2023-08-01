#include "pch.hpp"
#include "Mesh.hpp"

Vertex::Vertex(lf2d::vec2 position, lf2d::vec4 color)
{
	this->position = position;
	this->color = color;
}

Vertex::Vertex(const Vertex& other)
{
	this->position = other.position;
	this->color = other.color;
}

Vertex::Vertex(Vertex&& other)
{
	this->position = other.position;
	this->color = other.color;
}

void Mesh::render(vk::CommandBuffer commandBuffer, VulkanBuffer& vertexBuffer, VulkanBuffer& indexBuffer)
{
	m_totalObjectCount = 0;
	m_renderedObjectCount = 0;

	if (!m_vertices.empty())
	{
		uint32_t const vertexCount = static_cast<uint32_t>(m_vertices.size());
		uint32_t const indexCount = static_cast<uint32_t>(m_indices.size());

		if (vertexCount > vertexBuffer.count || vertexCount * 2 < vertexBuffer.count)
		{
			vertexBuffer.create(sizeof(Vertex) * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
			indexBuffer.create(sizeof(uint16_t) * indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
		}

		vertexBuffer.writeToBuffer(m_vertices.data());
		vertexBuffer.count = vertexCount;
		m_vertices.clear();

		indexBuffer.writeToBuffer(m_indices.data());
		indexBuffer.count = indexCount;
		m_indices.clear();

		constexpr vk::DeviceSize offsets[]{ 0 };
		commandBuffer.bindVertexBuffers(0, 1, vertexBuffer, offsets);
		commandBuffer.bindIndexBuffer(indexBuffer, 0, vk::IndexType::eUint16);
		commandBuffer.drawIndexed(indexCount, 1, 0, 0, 0);
	}
}

void Mesh::setCamera(lf2d::Camera* camera)
{
	m_currentCamera = camera;
}

void Mesh::addRect(lf2d::Rect const& rect, lf2d::Color color)
{
	if (rect.width < 1 || rect.height < 1)
	{
		spdlog::error("Rect width and height can't be negative");
		return;
	}


	float w = static_cast<float>(lf2d::getWindowWidth()), h = static_cast<float>(lf2d::getWindowHeight());

	// Little frustum culling
	if (   rect.x + rect.width  > (m_currentCamera->position.x - m_currentCamera->offset.x) + (-w / m_currentCamera->zoom + w)
		&& rect.x < (m_currentCamera->position.x - m_currentCamera->offset.x) + w / m_currentCamera->zoom
		&& rect.y + rect.height > (m_currentCamera->position.y - m_currentCamera->offset.y) + (-h / m_currentCamera->zoom + h)
		&& rect.y < (m_currentCamera->position.y - m_currentCamera->offset.y) + h / m_currentCamera->zoom)
	{
		m_vertices.push_back({ { rect.x / w,				rect.y / h}, color.normalized() });
		m_vertices.push_back({ {(rect.x + rect.width) / w,	rect.y / h}, color.normalized() });
		m_vertices.push_back({ {(rect.x + rect.width) / w, (rect.y + rect.height) / h}, color.normalized() });
		m_vertices.push_back({ { rect.x / w, (rect.height + rect.y) / h}, color.normalized() });

		m_indices.emplace_back(m_renderedObjectCount * 4	);
		m_indices.emplace_back(m_renderedObjectCount * 4 + 1);
		m_indices.emplace_back(m_renderedObjectCount * 4 + 2);

		m_indices.emplace_back(m_renderedObjectCount * 4 + 2);
		m_indices.emplace_back(m_renderedObjectCount * 4 + 3);
		m_indices.emplace_back(m_renderedObjectCount * 4	);

		m_renderedObjectCount++;
	}
	m_totalObjectCount++;
}

void Mesh::addRectGradient(lf2d::Rect const& rect, lf2d::Color color1, lf2d::Color color2, lf2d::Color color3, lf2d::Color color4)
{
	if (rect.width < 1 || rect.height < 1)
	{
		spdlog::error("Rect width and height can't be negative");
		return;
	}

	float w = static_cast<float>(lf2d::getWindowWidth()), h = static_cast<float>(lf2d::getWindowHeight());

	// Little frustum culling
	if (   rect.x + rect.width  > (m_currentCamera->position.x - m_currentCamera->offset.x) + (-w / m_currentCamera->zoom + w)
		&& rect.x < (m_currentCamera->position.x - m_currentCamera->offset.x) + w / m_currentCamera->zoom
		&& rect.y + rect.height > (m_currentCamera->position.y - m_currentCamera->offset.y) + (-h / m_currentCamera->zoom + h)
		&& rect.y < (m_currentCamera->position.y - m_currentCamera->offset.y) + h / m_currentCamera->zoom)
	{
		m_vertices.push_back({ { rect.x / w,				rect.y  / h}, color1.normalized() });
		m_vertices.push_back({ {(rect.x + rect.width) / w,	rect.y  / h}, color2.normalized() });
		m_vertices.push_back({ {(rect.x + rect.width) / w, (rect.y + rect.height) / h}, color4.normalized() });
		m_vertices.push_back({ { rect.x / w, (rect.height + rect.y) / h}, color3.normalized() });

		m_indices.emplace_back(m_renderedObjectCount * 4	);
		m_indices.emplace_back(m_renderedObjectCount * 4 + 1);
		m_indices.emplace_back(m_renderedObjectCount * 4 + 2);

		m_indices.emplace_back(m_renderedObjectCount * 4 + 2);
		m_indices.emplace_back(m_renderedObjectCount * 4 + 3);
		m_indices.emplace_back(m_renderedObjectCount * 4	);

		m_renderedObjectCount++;
	}
	m_totalObjectCount++;
}
