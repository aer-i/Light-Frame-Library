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

Vertex::Vertex(Vertex&& other) noexcept
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

static glm::vec2 rotateAroundPoint(glm::vec2 p, glm::vec2 o, const float d)
{
	const float r = glm::radians(d);
	const float s = sinf(r);
	const float c = cosf(r);
	p.x -= o.x;
	p.y -= o.y;

	const float newx = p.x * c - p.y * s;
	const float newy = p.x * s + p.y * c;

	p.x = newx + o.x;
	p.y = newy + o.y;
	return p;
}

static glm::vec2 scaleAroundPoint(glm::vec2 p, glm::vec2 o, float z)
{
	p = (p - o) * z + o;

	return p;
}

void Mesh::addRect(lf2d::Rect const& rect, glm::vec2 const& origin, int textureIndex, float rotation, lf2d::Color color1, lf2d::Color color2, lf2d::Color color3, lf2d::Color color4, bool useCamera)
{
	auto s = lf2d::window::size();
	
	glm::vec2 xy   = { rect.x,			rect.y			};
	glm::vec2 xyz  = { rect.x + rect.z, rect.y			};
	glm::vec2 xyw  = { rect.x,			rect.y + rect.w };
	glm::vec2 xyzw = { rect.x + rect.z, rect.y + rect.w };

	if (rotation != 0.f)
	{
		xy   = rotateAroundPoint( xy,	origin, rotation);
		xyz  = rotateAroundPoint( xyz,	origin, rotation);
		xyw  = rotateAroundPoint( xyw,  origin, rotation);
		xyzw = rotateAroundPoint( xyzw, origin, rotation);
	}

	if (useCamera)
	{
		xy   -= m_currentCamera->getPosWithOffset();
		xyz  -= m_currentCamera->getPosWithOffset();
		xyw  -= m_currentCamera->getPosWithOffset();
		xyzw -= m_currentCamera->getPosWithOffset();

		const glm::vec2 cameraCenter = s / 2.f;

		if (m_currentCamera->rotation != 0.f)
		{
			xy   = rotateAroundPoint( xy,	cameraCenter, m_currentCamera->rotation);
			xyz  = rotateAroundPoint( xyz,	cameraCenter, m_currentCamera->rotation);
			xyw  = rotateAroundPoint( xyw,  cameraCenter, m_currentCamera->rotation);
			xyzw = rotateAroundPoint( xyzw, cameraCenter, m_currentCamera->rotation);
		}

		if (m_currentCamera->zoom != 1.0f)
		{
			xy   = scaleAroundPoint( xy,   cameraCenter, m_currentCamera->zoom);
			xyz  = scaleAroundPoint( xyz,  cameraCenter, m_currentCamera->zoom);
			xyw  = scaleAroundPoint( xyw,  cameraCenter, m_currentCamera->zoom);
			xyzw = scaleAroundPoint( xyzw, cameraCenter, m_currentCamera->zoom);
		}
	}

	m_vertices.push_back({ xy   / s, color1.normalized(), {0, 0}, textureIndex });
	m_vertices.push_back({ xyz  / s, color2.normalized(), {1, 0}, textureIndex });
	m_vertices.push_back({ xyzw / s, color4.normalized(), {1, 1}, textureIndex });
	m_vertices.push_back({ xyw  / s, color3.normalized(), {0, 1}, textureIndex });

	m_indices.emplace_back(m_renderedObjectCount * 4	);
	m_indices.emplace_back(m_renderedObjectCount * 4 + 1);
	m_indices.emplace_back(m_renderedObjectCount * 4 + 2);

	m_indices.emplace_back(m_renderedObjectCount * 4 + 2);
	m_indices.emplace_back(m_renderedObjectCount * 4 + 3);
	m_indices.emplace_back(m_renderedObjectCount * 4	);

	m_renderedObjectCount++;
	m_totalObjectCount++;
}

void Mesh::addText(lf2d::Rect const& rect, glm::vec2 const& origin, int textureIndex, float rotation, lf2d::Color color1, lf2d::Color color2, lf2d::Color color3, lf2d::Color color4, bool useCamera)
{
	auto s = lf2d::window::size();

	glm::vec2 xy =   { rect.x,			rect.y			};
	glm::vec2 xyz =  { rect.x + rect.z, rect.y			};
	glm::vec2 xyw =  { rect.x,			rect.y + rect.w };
	glm::vec2 xyzw = { rect.x + rect.z, rect.y + rect.w };

	if (rotation != 0.f)
	{
		xy   = rotateAroundPoint( xy,	origin, rotation);
		xyz  = rotateAroundPoint( xyz,	origin, rotation);
		xyw  = rotateAroundPoint( xyw,  origin, rotation);
		xyzw = rotateAroundPoint( xyzw, origin, rotation);
	}

	if (useCamera)
	{
		xy   -= m_currentCamera->getPosWithOffset();
		xyz  -= m_currentCamera->getPosWithOffset();
		xyw  -= m_currentCamera->getPosWithOffset();
		xyzw -= m_currentCamera->getPosWithOffset();

		const glm::vec2 cameraCenter = s / 2.f;

		if (m_currentCamera->rotation != 0.f)
		{
			xy   = rotateAroundPoint( xy,	cameraCenter, m_currentCamera->rotation);
			xyz  = rotateAroundPoint( xyz,	cameraCenter, m_currentCamera->rotation);
			xyw  = rotateAroundPoint( xyw,  cameraCenter, m_currentCamera->rotation);
			xyzw = rotateAroundPoint( xyzw, cameraCenter, m_currentCamera->rotation);
		}

		if (m_currentCamera->zoom != 1.0f)
		{
			xy   = scaleAroundPoint( xy,   cameraCenter, m_currentCamera->zoom);
			xyz  = scaleAroundPoint( xyz,  cameraCenter, m_currentCamera->zoom);
			xyw  = scaleAroundPoint( xyw,  cameraCenter, m_currentCamera->zoom);
			xyzw = scaleAroundPoint( xyzw, cameraCenter, m_currentCamera->zoom);
		}
	}

	m_textVertices.push_back({ xy   / s, color1.normalized(), {0, 0}, textureIndex });
	m_textVertices.push_back({ xyz  / s, color2.normalized(), {1, 0}, textureIndex });
	m_textVertices.push_back({ xyzw / s, color4.normalized(), {1, 1}, textureIndex });
	m_textVertices.push_back({ xyw  / s, color3.normalized(), {0, 1}, textureIndex });

	m_textIndices.emplace_back(m_renderedTextCount * 4	  );
	m_textIndices.emplace_back(m_renderedTextCount * 4 + 1);
	m_textIndices.emplace_back(m_renderedTextCount * 4 + 2);

	m_textIndices.emplace_back(m_renderedTextCount * 4 + 2);
	m_textIndices.emplace_back(m_renderedTextCount * 4 + 3);
	m_textIndices.emplace_back(m_renderedTextCount * 4	  );

	m_renderedTextCount++;
	m_totalObjectCount++;
}
