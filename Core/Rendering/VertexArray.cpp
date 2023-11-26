#include "VertexArray.h"

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void VertexArray::CreateVertexArrayLayout(unsigned int active_program)
{
	m_POSLocation = glGetAttribLocation(active_program, "Position");

	glVertexAttribPointer(m_POSLocation, 3, GL_FLOAT, GL_FALSE,
		sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(m_POSLocation);

}

void VertexArray::CreateVertexArrayLayout(unsigned int prog, VertexAttrib attrib)
{
	switch (attrib)
	{
		case VertexAttrib::Position:
	{
		m_POSLocation = glGetAttribLocation(prog, "Position");
		glVertexAttribPointer(m_POSLocation, 3, GL_FLOAT, GL_FALSE,
			sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(m_POSLocation);
		break;
	}	case VertexAttrib::TexCoord:
	{
		m_UVLocation = glGetAttribLocation(prog, "TexCoord");
		glVertexAttribPointer(m_UVLocation, 2, GL_FLOAT, GL_TRUE,
			sizeof(glm::vec2), (void*)(sizeof(glm::vec3)));
		glEnableVertexAttribArray(m_UVLocation);
		break;
	}	case VertexAttrib::Normal:
	{
		m_NORMLocation = glGetAttribLocation(prog, "Normal");
		unsigned int size = sizeof(glm::vec3) + sizeof(glm::vec2);
		glVertexAttribPointer(m_NORMLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)(size));
		glEnableVertexAttribArray(m_NORMLocation);
		break;
	}
	}
}

void VertexArray::CreateVertexArrayLayout(unsigned int prog, std::vector<VertexAttrib> attribs)
{
	for (auto i : attribs)
	{
		CreateVertexArrayLayout(prog, i);
	}
}

void VertexArray::OnInit()
{
	glCreateVertexArrays(1, &m_VAO);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_VAO);
}
