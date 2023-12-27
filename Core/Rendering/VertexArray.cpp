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
	unsigned int stride = 0;

	switch (attrib)
	{
		case VertexAttrib::Position:
	{
		m_POSLocation = glGetAttribLocation(prog, "Position");
		glVertexAttribPointer(m_POSLocation, 3, GL_FLOAT, GL_FALSE,
			stride, (void*)0);
		glEnableVertexAttribArray(m_POSLocation);
		break;
	}		
	case VertexAttrib::TexCoord:
	{
		m_UVLocation = glGetAttribLocation(prog, "TexCoord");
		glVertexAttribPointer(m_UVLocation, 2, GL_FLOAT, GL_FALSE,
			stride, (void*)(sizeof(glm::vec3)));
		glEnableVertexAttribArray(m_UVLocation);
		break;
	}
	case VertexAttrib::UUID:
	{
		unsigned int offset = sizeof(glm::vec3) + sizeof(glm::vec2);
		m_UUIDLocation = glGetAttribLocation(prog, "ID");
		glVertexAttribPointer(m_UUIDLocation, 1, GL_UNSIGNED_INT, GL_FALSE,
			stride, (void*)(0 + offset));
		glEnableVertexAttribArray(m_UUIDLocation);
		break;
	}
	case VertexAttrib::Normal:
	{
		m_NORMLocation = glGetAttribLocation(prog, "Normal");
		unsigned int size = sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(unsigned int);
		glVertexAttribPointer(m_NORMLocation, 3, GL_FLOAT, GL_TRUE,
			stride, (void*)(0 + size));
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

void VertexArray::EnableAttribute(const char* attribute_name, unsigned int size)
{
	int active_program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &active_program);
	
	int location = glGetAttribLocation(active_program, attribute_name);
	unsigned int stride = 0;
	unsigned int offset = stride * location;
	glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	glEnableVertexArrayAttrib(active_program, location);
}

void VertexArray::OnInit()
{
	glCreateVertexArrays(1, &m_VAO);
}

void VertexArray::Bind()
{
	glBindVertexArray(m_VAO);
}
