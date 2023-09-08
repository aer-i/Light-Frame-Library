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

void Mesh::render(VulkanFrame* frame)
{
	m_tempTotalObjectCount = m_totalObjectCount;
	m_totalObjectCount = 0;
	m_tempTotalRenderedObjectCount = m_renderedObjectCount + m_renderedTextCount;
	m_renderedObjectCount = 0;
	m_renderedTextCount = 0;

	if (!m_vertices.empty())
	{
		uint32_t const vertexCount = static_cast<uint32_t>(m_vertices.size());
		uint32_t const indexCount = static_cast<uint32_t>(m_indices.size());

		if (vertexCount > frame->vertexBuffer.count || vertexCount * 2 < frame->vertexBuffer.count)
		{
			frame->vertexBuffer.create(sizeof(Vertex) * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
			frame->vertexBuffer.map();
			frame->indexBuffer.create(sizeof(uint32_t) * indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
			frame->indexBuffer.map();
		}

		frame->vertexBuffer.writeToBuffer(m_vertices.data());
		frame->vertexBuffer.count = vertexCount;
		m_vertices.clear();

		frame->indexBuffer.writeToBuffer(m_indices.data());
		frame->indexBuffer.count = indexCount;
		m_indices.clear();

		constexpr vk::DeviceSize offsets[]{ 0 };
		frame->commandBuffer.bindVertexBuffers(0, 1, frame->vertexBuffer, offsets);
		frame->commandBuffer.bindIndexBuffer(frame->indexBuffer, 0, vk::IndexType::eUint32);
		frame->commandBuffer.drawIndexed(indexCount, 1, 0, 0, 0);
	}
}

void Mesh::renderText(VulkanFrame* frame)
{
	if (!m_textVertices.empty())
	{
		uint32_t const textVertexCount = static_cast<uint32_t>(m_textVertices.size());
		uint32_t const textIndexCount = static_cast<uint32_t>(m_textIndices.size());

		if (textVertexCount > frame->textVertexBuffer.count || textVertexCount * 2 < frame->textVertexBuffer.count)
		{
			frame->textVertexBuffer.create(sizeof(Vertex) * textVertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
			frame->textVertexBuffer.map();
			frame->textIndexBuffer.create(sizeof(uint32_t) * textIndexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
			frame->textIndexBuffer.map();
		}

		frame->textVertexBuffer.writeToBuffer(m_textVertices.data());
		frame->textVertexBuffer.count = textVertexCount;
		m_textVertices.clear();

		frame->textIndexBuffer.writeToBuffer(m_textIndices.data());
		frame->textIndexBuffer.count = textIndexCount;
		m_textIndices.clear();

		constexpr vk::DeviceSize offsets[]{ 0 };
		frame->commandBuffer.bindVertexBuffers(0, 1, frame->textVertexBuffer, offsets);
		frame->commandBuffer.bindIndexBuffer(frame->textIndexBuffer, 0, vk::IndexType::eUint32);
		frame->commandBuffer.drawIndexed(textIndexCount, 1, 0, 0, 0);
	}
}

void Mesh::setCamera(lf2d::Camera* camera)
{
	m_currentCamera = camera;
}

void Mesh::addRect(lf2d::Rect const& rect, int textureIndex, lf2d::Color color1, lf2d::Color color2, lf2d::Color color3, lf2d::Color color4)
{
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

void Mesh::addText(lf2d::Rect const& rect, int textureIndex, lf2d::Color color1, lf2d::Color color2, lf2d::Color color3, lf2d::Color color4)
{
	float width = static_cast<float>(lf2d::window::width()), height = static_cast<float>(lf2d::window::height());

	{
		m_textVertices.push_back({ { rect.x			  / width,  rect.y			 / height},	color1.normalized(), {0, 0}, textureIndex });
		m_textVertices.push_back({ {(rect.x + rect.z) / width,	rect.y			 / height},	color2.normalized(), {1, 0}, textureIndex });
		m_textVertices.push_back({ {(rect.x + rect.z) / width, (rect.y + rect.w) / height}, color4.normalized(), {1, 1}, textureIndex });
		m_textVertices.push_back({ { rect.x			  / width, (rect.w + rect.y) / height},	color3.normalized(), {0, 1}, textureIndex });

		m_textIndices.emplace_back(m_renderedTextCount * 4	);
		m_textIndices.emplace_back(m_renderedTextCount * 4 + 1);
		m_textIndices.emplace_back(m_renderedTextCount * 4 + 2);

		m_textIndices.emplace_back(m_renderedTextCount * 4 + 2);
		m_textIndices.emplace_back(m_renderedTextCount * 4 + 3);
		m_textIndices.emplace_back(m_renderedTextCount * 4	);

		m_renderedTextCount++;
	}
	m_totalObjectCount++;
}
