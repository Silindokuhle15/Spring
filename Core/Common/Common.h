#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <string>
#include <iostream>
typedef struct
{
	glm::vec3 pos;
	glm::vec2 tex;
	uint32_t  ID;
	glm::vec3 norm;
} Vertex;


enum class VertexAttrib
{
	Position, TexCoord, ID, Normal
};