#pragma once
#include "color.h"

class VertexData
{
private :
	vec3 m_Position;
	color::color m_Color;
public:
	VertexData(const vec3& t_pos, const color::color& t_col):
		m_Position{ t_pos }, m_Color{ t_col }{} // Constructor
	~VertexData() {} // Destructor...



};