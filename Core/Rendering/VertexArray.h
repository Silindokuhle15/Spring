#pragma once
#include "GL/glew.h"
#include "Common.h"

class VertexArray
{
	unsigned int m_VAO = 0;
	unsigned int m_DeltaLocation;
	int m_POSLocation;
	int m_UUIDLocation;
	int m_UVLocation;
	int m_NORMLocation;

public:

	VertexArray() :m_VAO{ 0 }, m_POSLocation{ 0 }, m_UVLocation{ 0 }, m_NORMLocation{ 0 } {}
	~VertexArray();

	operator unsigned int() const { return m_VAO; }

	void CreateVertexArrayLayout(uint32_t prog, std::vector<VertexAttrib> attribs);

	void EnableAttribute(uint32_t program, const char* name);

	void OnInit();
	void Bind();
};

