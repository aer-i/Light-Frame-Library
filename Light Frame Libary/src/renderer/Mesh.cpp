#include "pch.hpp"
#include "Mesh.hpp"

Vertex::Vertex(glm::vec2 position, glm::vec4 color)
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
			indexBuffer.create(sizeof(uint32_t) * indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
		}

		vertexBuffer.writeToBuffer(m_vertices.data());
		vertexBuffer.count = vertexCount;
		m_vertices.clear();

		indexBuffer.writeToBuffer(m_indices.data());
		indexBuffer.count = indexCount;
		m_indices.clear();

		constexpr vk::DeviceSize offsets[]{ 0 };
		commandBuffer.bindVertexBuffers(0, 1, vertexBuffer, offsets);
		commandBuffer.bindIndexBuffer(indexBuffer, 0, vk::IndexType::eUint32);
		commandBuffer.drawIndexed(indexCount, 1, 0, 0, 0);
	}
}

void Mesh::setCamera(lf2d::Camera* camera)
{
	m_currentCamera = camera;
}

void Mesh::addRect(lf2d::Rect const& rect, lf2d::Color color)
{
	if (rect.z < 1 || rect.w < 1)
	{
		printf("[error] Rect width and height can't be negative\n");
		return;
	}

	float width = static_cast<float>(lf2d::window::width()), height = static_cast<float>(lf2d::window::height());
	
	if (   rect.x + rect.z > (	   0 - m_currentCamera->offset.x) / m_currentCamera->zoom + m_currentCamera->position.x
		&& rect.x <			 ( width - m_currentCamera->offset.x) / m_currentCamera->zoom + m_currentCamera->position.x
		&& rect.y + rect.w > (	   0 - m_currentCamera->offset.y) / m_currentCamera->zoom + m_currentCamera->position.y
		&& rect.y <			 (height - m_currentCamera->offset.y) / m_currentCamera->zoom + m_currentCamera->position.y)
	{
		m_vertices.push_back({ { rect.x			  / width,  rect.y			 / height},	color.normalized() });
		m_vertices.push_back({ {(rect.x + rect.z) / width,	rect.y			 / height},	color.normalized() });
		m_vertices.push_back({ {(rect.x + rect.z) / width, (rect.y + rect.w) / height}, color.normalized() });
		m_vertices.push_back({ { rect.x			  / width, (rect.w + rect.y) / height},	color.normalized() });

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
	if (rect.z < 1 || rect.w < 1)
	{
		printf("[error] Rect width and height can't be negative\n");
		return;
	}
		
	float width = static_cast<float>(lf2d::window::width()), height = static_cast<float>(lf2d::window::height());

	if (   rect.x + rect.z > (	   0 - m_currentCamera->offset.x) / m_currentCamera->zoom + m_currentCamera->position.x
		&& rect.x <			 ( width - m_currentCamera->offset.x) / m_currentCamera->zoom + m_currentCamera->position.x
		&& rect.y + rect.w > (	   0 - m_currentCamera->offset.y) / m_currentCamera->zoom + m_currentCamera->position.y
		&& rect.y <			 (height - m_currentCamera->offset.y) / m_currentCamera->zoom + m_currentCamera->position.y)
	{
		m_vertices.push_back({ { rect.x			  / width,  rect.y			 / height},	color1.normalized() });
		m_vertices.push_back({ {(rect.x + rect.z) / width,	rect.y			 / height},	color2.normalized() });
		m_vertices.push_back({ {(rect.x + rect.z) / width, (rect.y + rect.w) / height}, color4.normalized() });
		m_vertices.push_back({ { rect.x			  / width, (rect.w + rect.y) / height},	color3.normalized() });

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
