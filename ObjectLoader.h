#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

// Wanna create this component to read .obj/ 3D files dyanamically
struct PerVertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
};
class ObjectLoader
{
public:
	std::vector<glm::vec3> m_Positions;
	std::vector<glm::vec3> m_Normals;
	std::vector<glm::vec2> m_TexCoords;
	std::vector<unsigned int> m_VertexIndices;

	std::vector<PerVertex> m_Vertices;

	int LoadObjectFromFile(const char* file_path);
public:
	void LoadObject(const char* file_path);
};

