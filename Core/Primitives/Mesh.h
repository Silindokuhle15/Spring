#pragma once
#include "UUID.h"
#include "Material.h"
#include "Vertex.h"
#include "Bound.h"
#include "entt.hpp"
namespace primitives
{
    class Mesh
    {
    public:
        Mesh():
            m_MaterialGroupHandle{ AssetHandle{0,0} }
        {
        }
        Mesh(
            const std::vector<Vertex>& vertices,
            const std::vector<uint64_t>& vertex_indices
            //const std::vector<Material>& materials
        );

        std::vector<Vertex> m_V;
        std::vector<uint64_t> m_VertexIndices;
        std::vector<Material> m_Materials;
        AssetHandle m_MaterialGroupHandle;
        std::vector<Mesh> m_SubMeshes;
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

    struct Parent
    {
        entt::entity ParentEntity;
        bool Active[4];
        uint64_t NumChildren;
    };

    struct PlaySoundRequest
    {
        entt::entity ParentEntity;
        bool Active[4];
        AssetHandle SoundID;
    };

    Bound3D FindMinMax(const Mesh& mesh);
}