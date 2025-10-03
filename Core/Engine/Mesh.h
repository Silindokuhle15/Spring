#pragma once
#include "UUID.h"
#include "Utility/ObjectLoader.h"

class Mesh
{
public:
    Mesh()
    {}
    Mesh(std::string file_path);

    std::vector<glm::vec3 > m_Positions;
    std::vector<glm::vec2 > m_TexCoords;
    std::vector<glm::vec3 > m_Normals;
    std::vector<Vertex> m_V;
    std::vector<unsigned int> m_VertexIndices;
    std::vector<float> m_VertexIDs;
    std::vector<unsigned int> m_TextureIndices;
    std::vector<unsigned int> m_NormalIndices;
    std::vector<Material> m_Materials;
    AssetHandle m_MaterialHandle;

    std::vector<Mesh> m_SubMeshes;

private:
    std::string m_OBJFilePath;
};

struct RenderComponent
{
    uint64_t m_VertexBufferOffset;
    uint64_t m_IndexBufferOffset;
    uint64_t m_Size;
    uint64_t m_IndexCount;
public:
    RenderComponent(uint64_t vertex_offset = 0 , uint64_t index_offset =  0, uint64_t size = 0, uint64_t index_count = 0) :
        m_VertexBufferOffset{ vertex_offset },
        m_IndexBufferOffset{ index_offset },
        m_Size{ size },
        m_IndexCount{ index_count}
    {
    }
};

class MeshInstance
{
public:
    AssetHandle m_Handle;
    MeshInstance(const AssetHandle& handle) :
        m_Handle{ handle }
    {

    }
};