#include "VertexArray.h"
VertexArray::VertexArray():
	m_VAO{0}
{
	glCreateVertexArrays(1, &m_VAO);
}
VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void VertexArray::SetBufferLayout(const BufferLayout& buffer_layout) const
{
	auto stride = BufferLayout::GetLayoutStride(buffer_layout);
	GLuint attribIndex{ 0 };
	for (auto& attrib : buffer_layout.GetBufferLayoutInfo())
	{
		auto attribOffset = BufferLayout::GetLayoutInfoOffset(buffer_layout, attrib);
		auto attribElementCount = BufferLayout::GetLayoutInfoElementCount(attrib);
		auto attribElementSize = BufferLayout::GetLayoutInfoElementSize(attrib);
		glVertexAttribPointer(attribIndex, attribElementCount, attribElementSize, GL_FALSE, stride, (const void*)attribOffset);
		//glEnableVertexArrayAttrib(m_VAO, attribIndex);
		glEnableVertexAttribArray(attribIndex);
		++attribIndex;
	}
}

void VertexArray::Bind()
{
	glBindVertexArray(m_VAO);
}