#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Material.h"
struct PerVertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
};
class ObjectLoader
{
private:
	void ExtractDump(std::string dump, uint64_t object_index);
public:
	std::vector<std::vector<glm::vec3>> m_Positions;
	std::vector<std::vector<glm::vec3>> m_Normals;
	std::vector<std::vector<glm::vec2>> m_TexCoords;
	std::vector<std::vector<unsigned int>> m_VertexIndices;
	std::vector<std::vector<unsigned int>> m_VertexIDs;
	std::vector<std::vector<unsigned int>> m_TextureIndices;
	std::vector<std::vector<unsigned int>> m_NormalIndices;
	std::vector<Material> m_Materials;
	std::vector<std::string> m_MaterialPaths;
	std::vector<std::string> m_MaterialNames;
	std::vector<std::string> m_ObjectNames;
	std::vector<std::string> m_Surfaces;
	std::vector<std::string> getWords(std::string s, const char * delim);
	int LoadObjectFromFile(const char* file_path);
	int LoadMaterialFromFile(const char* file_path);

public:
	void LoadObject(const char* file_path);
};

