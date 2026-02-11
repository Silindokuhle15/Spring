#pragma once
#include "UUID.h"
#include "Utility/ObjectLoader.h"
#include "Bound.h"
namespace primitives
{
    class Mesh
    {
    public:
        Mesh():
            m_MaterialGroupHandle{ AssetHandle{0,0} }
        {
        }
        Mesh(std::string file_path);
        Mesh(
            const std::vector<glm::vec3>& positions,
            const std::vector<glm::vec2>& texcoords,
            const std::vector<float>& ids,
            const std::vector<glm::vec3>& normals,
            const std::vector<Vertex>& vertices,
            const std::vector<uint64_t>& vertex_indices,
            const std::vector<uint64_t>& texture_indices,
            const std::vector<uint64_t>& normal_indices,
            const std::vector<Material>& materials
        );

        std::vector<glm::vec3 > m_Positions;
        std::vector<glm::vec2 > m_TexCoords;
        std::vector<float> m_VertexIDs;
        std::vector<glm::vec3 > m_Normals;
        std::vector<Vertex> m_V;
        std::vector<uint64_t> m_VertexIndices;
        std::vector<uint64_t> m_TextureIndices;
        std::vector<uint64_t> m_NormalIndices;
        std::vector<Material> m_Materials;
        AssetHandle m_MaterialGroupHandle;
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
        RenderComponent(uint64_t vertex_offset = 0, uint64_t index_offset = 0, uint64_t size = 0, uint64_t index_count = 0) :
            m_VertexBufferOffset{ vertex_offset },
            m_IndexBufferOffset{ index_offset },
            m_Size{ size },
            m_IndexCount{ index_count }
        {
        }
    };
    struct DestructComponent
    {
        uint64_t HQWORD;
        uint64_t LQWORD;
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

    Bound3D FindMinMax(const Mesh& mesh);
}