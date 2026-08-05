#pragma once
#include "Common.h"
#include "Vertex.h"
class VertexBuffer
{
public:
	uint32_t m_BufferOffset = 0;
	uint32_t m_BufferElementCount = 0;
	std::vector<primitives::Vertex> m_Buffer;
	std::vector<uint32_t> m_Indices;
	void Clear()
	{
		m_Buffer.clear();
	}
	void UpdateBuffer(std::vector<primitives::Vertex> buffer)
	{
		m_BufferOffset = static_cast<uint32_t>(sizeof(primitives::Vertex) * m_Buffer.size());
		m_BufferElementCount = static_cast<uint32_t>(m_Buffer.size());
		m_Buffer.insert(m_Buffer.end(), buffer.begin(), buffer.end());
		
	}
	uint32_t Size() const { return static_cast<uint32_t>(sizeof(primitives::Vertex) * m_Buffer.size()); }

	VertexBuffer(uint32_t initial_capacity = 65536)
	{
		m_Buffer.reserve(initial_capacity);
	}
};