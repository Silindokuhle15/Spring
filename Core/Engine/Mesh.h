#pragma once
#include "Common.h"
#include "Utility/ObjectLoader.h"

class Mesh
{
public:
    Mesh() {}
    Mesh(std::string file_path) :
        m_OBJFilePath{ file_path },
        m_Color{glm::vec3(1.0f, 1.0f, 1.0f)},
        m_Transform{glm::mat4(1.0f)}
    {
        OnInit();
    }
    Mesh(std::vector<Vertex> m_points, glm::mat4 transform=glm::mat4(1.0f)):
        m_Transform{ transform},
        m_Color{ glm::vec3(1.0f, 1.0f, 1.0f) }
    {
        this->m_V = m_points;
        this->NumIndices = static_cast<uint32_t>(m_points.size());
        this->NumVertices = static_cast<uint32_t>(m_points.size());
    }
    virtual void OnInit();
    virtual void OnUpdate(TimeStep ts);

    static Mesh Batch(std::vector<Mesh>& meshes);

    unsigned int NumVertices;
    unsigned int NumIndices;
    volatile float m_Speed = 1.0;
    glm::mat4 m_Transform;
    glm::vec3 m_Color;

    std::vector<glm::vec3 > m_Positions;
    std::vector<glm::vec2 > m_TexCoords;
    std::vector<glm::vec3 > m_Normals;
    std::vector<Vertex> m_V;
    std::vector<unsigned int> m_VertexIndices;
    std::vector<unsigned int> m_VertexIDs;
    std::vector<unsigned int> m_TextureIndices;
    std::vector<unsigned int> m_NormalIndices;

    std::vector<Mesh> m_SubMeshes;

    TimeStep m_Ts;
    virtual void SetTransform(const glm::mat4& transform) noexcept
    {
        m_Transform = transform;
    }

    virtual void SetColor(const glm::vec3& color) noexcept
    {
        m_Color = color;
    }

private:
    std::string m_OBJFilePath;
};

