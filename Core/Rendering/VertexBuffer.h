#pragma once
#include "Common.h"

class VertexBuffer
{
public:
	uint64_t m_BufferOffset = 0;
	uint64_t m_BufferElementCount = 0;
	std::vector<Vertex> m_Buffer;
	std::vector<uint32_t> m_Indices;
	void Clear()
	{
		m_Buffer.clear();
	}
	void UpdateBuffer(std::vector<Vertex> buffer)
	{
		m_BufferOffset = static_cast<uint64_t>(sizeof(Vertex) * m_Buffer.size());
		m_BufferElementCount = static_cast<uint64_t>(m_Buffer.size());
		m_Buffer.insert(m_Buffer.end(), buffer.begin(), buffer.end());
		
	}
	uint64_t Size() const { return static_cast<uint64_t>(sizeof(Vertex) * m_Buffer.size()); }

	VertexBuffer(uint64_t initial_capacity = 65536)
	{
		m_Buffer.reserve(initial_capacity);
	}
};