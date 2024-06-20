#include "VertexArray.h"

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void VertexArray::CreateVertexArrayLayout(uint32_t program, std::vector<VertexAttrib> attribs)
{
	for (auto attribute : attribs)
	{
		uint64_t offset = 0;
		GLint size = 3;
		std::string attribute_name = "";
		GLsizei stride = sizeof(Vertex);

		switch (attribute)
		{
		case VertexAttrib::Position:
			attribute_name = "Position";
			offset = offsetof(Vertex, pos);
			break;
		case VertexAttrib::TexCoord:
			attribute_name = "TexCoord";
			offset = offsetof(Vertex, tex);
			size = size - 1;
			break;
		case VertexAttrib::ID:
			attribute_name = "ID";
			offset = offsetof(Vertex, ID);
			size = size - 2;
			break;
		case VertexAttrib::Normal:
			attribute_name = "Normal";
			offset = offsetof(Vertex, norm);
			break;
		}

		GLuint m_LayoutLocation = glGetAttribLocation(program, attribute_name.c_str());
		glVertexAttribPointer(m_LayoutLocation, size, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
	}
}

void VertexArray::EnableAttribute(uint32_t program, const char * name)
{
	uint32_t m_LayoutLocation = glGetAttribLocation(program, name);
	glEnableVertexAttribArray(m_LayoutLocation);
}

void VertexArray::OnInit()
{
	glCreateVertexArrays(1, &m_VAO);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_VAO);
}
