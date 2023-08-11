#include "pch.hpp"
#include "Mesh.hpp"

Vertex::Vertex(glm::vec2 position, glm::vec4 color, glm::vec2 uv, int32_t textureIndex)
	: position{ position }, color{ color }, uv{ uv }, textureIndex{ textureIndex }
{
}

Vertex::Vertex(const Vertex& other)
	: position{ other.position }, color{ other.color }, uv{ other.uv }, textureIndex{ other.textureIndex }
{
}

Vertex::Vertex(Vertex&& other)
	: position{ other.position }, color{ other.color }, uv{ other.uv }, textureIndex{ other.textureIndex }
{
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
			vertexBuffer.create(sizeof(Vertex) * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
			vertexBuffer.map();
			indexBuffer.create(sizeof(uint32_t) * indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
			indexBuffer.map();
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


void Mesh::addRect(lf2d::Rect const& rect, int textureIndex, lf2d::Color color1, lf2d::Color color2, lf2d::Color color3, lf2d::Color color4)
{
	if (rect.z < 1 || rect.w < 1)
	{
		printf("[error] Rect width and height can't be negative or equal to zero\n");
		return;
	}
		
	float width = static_cast<float>(lf2d::window::width()), height = static_cast<float>(lf2d::window::height());

	if (   rect.x + rect.z > (-(width  / 2.f)) / m_currentCamera->zoom + m_currentCamera->getPosWithOffset().x
		&& rect.x <			 ( (width  / 2.f)) / m_currentCamera->zoom + m_currentCamera->getPosWithOffset().x
		&& rect.y + rect.w > (-(height / 2.f)) / m_currentCamera->zoom + m_currentCamera->getPosWithOffset().y
		&& rect.y <			 ( (height / 2.f)) / m_currentCamera->zoom + m_currentCamera->getPosWithOffset().y)
	{
		m_vertices.push_back({ { rect.x			  / width,  rect.y			 / height},	color1.normalized(), {0, 0}, textureIndex });
		m_vertices.push_back({ {(rect.x + rect.z) / width,	rect.y			 / height},	color2.normalized(), {1, 0}, textureIndex });
		m_vertices.push_back({ {(rect.x + rect.z) / width, (rect.y + rect.w) / height}, color4.normalized(), {1, 1}, textureIndex });
		m_vertices.push_back({ { rect.x			  / width, (rect.w + rect.y) / height},	color3.normalized(), {0, 1}, textureIndex });

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
