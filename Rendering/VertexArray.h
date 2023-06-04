#pragma once
#include "glew.h"
#include "glm.hpp"
#include <vector>

typedef enum
{
	Position, TexCoord, Normal
} VertexAttrib;

class VertexArray
{
	unsigned int m_VAO = 0;
	unsigned int m_DeltaLocation;
	int m_POSLocation;
	int m_UVLocation;
	int m_NORMLocation;

public:

	VertexArray() :m_VAO{ 0 }, m_POSLocation{ 0 }, m_UVLocation{ 0 }, m_NORMLocation{ 0 } {}
	~VertexArray();

	operator unsigned int() const { return m_VAO; }

	void CreateVertexArrayLayout(unsigned int prog);
	void CreateVertexArrayLayout(unsigned int prog, VertexAttrib attrib);
	void CreateVertexArrayLayout(unsigned int prog, std::vector<VertexAttrib> attribs);

	void OnInit();
	void Bind();
};

