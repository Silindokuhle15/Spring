#pragma once
#include "Common.h"
#include "Material.h"
#include <fbxsdk.h>


struct PerVertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;
};
class ObjectLoader
{
private:
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
	static std::vector<std::string> getWords(std::string s, const char * delim);
	virtual int LoadObjectFromFile(const char* file_path);
	virtual int LoadMaterialFromFile(const char* file_path);

	void ExtractDump(std::string dump, uint64_t object_index);

public:
	virtual void LoadObject(const char* file_path);
};

class OBJObjectLoader : public ObjectLoader
{
public:
	int LoadObjectFromFile(const char* file_path) override;
	int LoadMaterialFromFile(const char* file_path) override;

};

class FBXObjectLoader : public ObjectLoader
{
	FbxManager* m_pManager;
	FbxScene* m_pScene;
	std::string m_Title;

	const int TRIANGLE_VERTEX_COUNT = 3;
	// Four floats for every position.
	const int VERTEX_STRIDE = 4;
	// Three floats for every normal.
	const int NORMAL_STRIDE = 3;
	// Two floats for every UV.
	const int UV_STRIDE = 2;

	enum
	{
		VERTEX_VBO,
		NORMAL_VBO,
		UV_VBO,
		INDEX_VBO,
		VBO_COUNT,
	};

	// For every material, record the offsets in every VBO and triangle counts
	struct SubMesh
	{
		SubMesh() : IndexOffset(0), TriangleCount(0) {}

		int IndexOffset;
		int TriangleCount;
	};
public:
	std::vector<Vertex> m_MeshData;
	int LoadObjectFromFile(const char* file_path) override;
};
