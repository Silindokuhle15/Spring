#include "Mesh.h"

ObjectLoader Mesh::ob = ObjectLoader();
//TinyWrapper Mesh::TWr = TinyWrapper();

void Mesh::OnInit()
{
    ob.LoadObjectFromFile(m_OBJFilePath.c_str());
    //TWr.LoadObjectFromFile(m_OBJFilePath.c_str());
    //m_Positions = ob.m_Positions;
    NumVertices = ob.m_Positions.size();
    NumIndices  = ob.m_VertexIndices.size();
    //m_VertexIndices = ob.m_VertexIndices;
    //m_TextureIndices = ob.m_TextureIndices;
    //m_NormalIndices = ob.m_NormalIndices;
    //m_TexCoords = ob.m_TexCoords;
    //m_Normals = ob.m_Normals;
    //m_VertexIDs = ob.m_VertexIDs;
    
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
            //m_Positions.push_back(pos);
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
    //m_Positions.shrink_to_fit();
    //m_TexCoords.shrink_to_fit();
    //m_VertexIDs.shrink_to_fit();
    //m_Normals.shrink_to_fit();

    NumVertices = m_Positions.size();
    NumIndices  = m_VertexIndices.size();
    
    for (int i = 0; i < NumIndices; i++)
    {
        glm::vec3 pos = m_Positions[m_VertexIndices[i]];
        glm::vec2 tex = m_TexCoords[m_TextureIndices[i]];
        uint32_t  ID  = m_VertexIDs[m_VertexIndices[i]];
        glm::vec3 norm = m_Normals[m_NormalIndices[i]];

        m_V.push_back({ pos, tex, ID, norm });
    }
}

void Mesh::OnUpdate(float ts)
{
    // Update the model transform 
    //m_Transform = glm::mat4(glm::translate(m_Transform, glm::vec3(0.0, 0.0, -0.10))); // Not the best solution, still needs to be updated,, all in due time
    m_Ts = ts;
}

void Mesh::MoveBackward()
{
    glm::vec3 translation = glm::vec3(0.0, 0.0, +1.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}

void Mesh::MoveForward()
{
    glm::vec3 translation = glm::vec3(0.0, 0.0, -1.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}

void Mesh::MoveLeft()
{
    glm::vec3 translation = glm::vec3(+1.0, 0.0, 0.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}

void Mesh::MoveRight()
{
    glm::vec3 translation = glm::vec3(-1.0, 0.0, 0.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}

void Mesh::MoveUp()
{
    glm::vec3 translation = glm::vec3(0.0, +1.0, 0.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}

void Mesh::MoveDown()
{
    glm::vec3 translation = glm::vec3(0.0, -1.0, 0.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
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

    batchedMesh.NumVertices = batchedMesh.m_Positions.size();
    batchedMesh.NumIndices = batchedMesh.m_VertexIndices.size();
    batchedMesh.SetTransform(glm::translate(glm::mat4(1.0f), glm::vec3(+0.0f, 0.0, 0.0)));
    return batchedMesh;
}
