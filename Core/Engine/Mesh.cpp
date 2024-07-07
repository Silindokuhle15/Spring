#include "Mesh.h"

void Mesh::OnInit()
{
    ObjectLoader ob;
    ob.LoadObjectFromFile(m_OBJFilePath.c_str());
    NumVertices = static_cast<uint32_t>(ob.m_Positions.size());
    NumIndices = static_cast<uint32_t>(ob.m_VertexIndices.size());
    
    for (auto& i : ob.m_Positions)
    {
        m_Positions.push_back(i);
    }

    for (int i = 0; i < ob.m_VertexIndices.size(); i++)
    {
        uint32_t index = ob.m_VertexIndices[i];
        if (index < NumIndices)
        {
            glm::vec3 pos = ob.m_Positions[index];
            m_VertexIDs.push_back(index);
            m_VertexIndices.push_back(index);
        }
    }
    for (int i = 0; i < ob.m_TextureIndices.size(); i++)
    {
        uint32_t index = ob.m_TextureIndices[i];

        if (index < NumIndices)
        {
            glm::vec2 tex_coord = ob.m_TexCoords[index];
            m_TexCoords.push_back(tex_coord);
            m_TextureIndices.push_back(index);
        }
    }
    for (int i = 0; i < ob.m_NormalIndices.size(); i++)
    {
        uint32_t index = ob.m_NormalIndices[i];
        if (index < NumIndices)
        {
            glm::vec3 normal = ob.m_Normals[index];
            m_Normals.push_back(normal);
            m_NormalIndices.push_back(index);
        }
    }

    NumVertices = static_cast<uint32_t>(m_Positions.size());
    NumIndices  = static_cast<uint32_t>(m_VertexIndices.size());
    
    for (uint32_t i = 0; i < ob.m_VertexIndices.size(); i++)
    {
        glm::vec3 pos = m_Positions[m_VertexIndices[i]];
        glm::vec2 tex = m_TexCoords[m_TextureIndices[i]];
        uint32_t  ID  = m_VertexIDs[m_VertexIndices[i]];
        glm::vec3 norm = m_Normals[m_NormalIndices[i]];

        m_V.push_back({ pos, tex, ID, norm });
    }

    NumIndices = static_cast<uint32_t>(m_V.size());
}

void Mesh::OnUpdate(TimeStep ts)
{
    // Update the model transform 
    //m_Transform = glm::mat4(glm::translate(m_Transform, glm::vec3(0.0, 0.0, -0.10))); // Not the best solution, still needs to be updated,, all in due time
    m_Ts = ts;
}

Mesh Mesh::Batch(std::vector<Mesh>& meshes)
{
    unsigned int padd_value = 0;

    Mesh batchedMesh;

    for (auto& i : meshes)
    {
        for (auto& j : i.m_VertexIndices)
        {
            j += padd_value;
            batchedMesh.m_VertexIndices.push_back(j);
        }
        for (auto& pos : i.m_Positions)
        {
            batchedMesh.m_Positions.push_back(pos);
        }
        for (auto& tex : i.m_TexCoords)
        {
            batchedMesh.m_TexCoords.push_back(tex);
        }
        for (auto& vID : i.m_VertexIDs)
        {
            batchedMesh.m_VertexIDs.push_back(vID);
        }
        for (auto& norm : i.m_Normals)
        {
            batchedMesh.m_Normals.push_back(norm);
        }
        for (auto& v : i.m_V)
        {
            batchedMesh.m_V.push_back(v);
        }
        padd_value += i.NumVertices;
    }

    batchedMesh.NumVertices = static_cast<uint32_t>(batchedMesh.m_Positions.size());
    batchedMesh.NumIndices = static_cast<uint32_t>(batchedMesh.m_VertexIndices.size());
    batchedMesh.SetTransform(glm::translate(glm::mat4(1.0f), glm::vec3(+0.0f, 0.0, 0.0)));
    return batchedMesh;
}
