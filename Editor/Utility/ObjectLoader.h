#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

struct PerVertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
};
class ObjectLoader
{
private:
	void ExtractDump(std::string dump);
public:
	std::vector<glm::vec3> m_Positions;
	std::vector<glm::vec3> m_Normals;
	std::vector<glm::vec2> m_TexCoords;
	std::vector<unsigned int> m_VertexIndices;
	std::vector<unsigned int> m_VertexIDs;
	std::vector<unsigned int> m_TextureIndices;
	std::vector<unsigned int> m_NormalIndices;

	std::vector<std::string> getWords(std::string s, const char * delim);
	int LoadObjectFromFile(const char* file_path);
public:
	void LoadObject(const char* file_path);
};

