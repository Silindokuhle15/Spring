#pragma once
#include "GL/glew.h"
#include "BufferLayout.h"

class VertexArray
{
	GLuint m_VAO = 0;
public:
	VertexArray();
	~VertexArray();
	operator GLuint() const { return m_VAO; }
	void SetBufferLayout(const BufferLayout& buffer_layout) const;
	void Bind();
};

