#pragma once
#include "Utility/ObjectLoader.h"

class Mesh
{
public:
    Mesh()
    {}
    Mesh(std::string file_path) :
        m_OBJFilePath{ file_path }
    {
        OnInit();
    }
    Mesh(std::vector<Vertex> m_points)
    {
        this->m_V = m_points;
    }
    void OnInit();

    std::vector<glm::vec3 > m_Positions;
    std::vector<glm::vec2 > m_TexCoords;
    std::vector<glm::vec3 > m_Normals;
    std::vector<Vertex> m_V;
    std::vector<unsigned int> m_VertexIndices;
    std::vector<float> m_VertexIDs;
    std::vector<unsigned int> m_TextureIndices;
    std::vector<unsigned int> m_NormalIndices;
    std::vector<Material> m_Materials;

    std::vector<Mesh> m_SubMeshes;

private:
    std::string m_OBJFilePath;
};

