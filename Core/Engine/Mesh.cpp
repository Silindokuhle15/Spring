#include "Mesh.h"

ObjectLoader Mesh::ob = ObjectLoader();

void Mesh::OnInit()
{
    ob.LoadObjectFromFile(m_OBJFilePath.c_str());

    m_Positions = ob.m_Positions;
    m_TexCoords = ob.m_TexCoords;
    m_Normals = ob.m_Normals;
    m_VertexIDs = ob.m_VertexIDs;

    ob.m_VertexIndices.erase(ob.m_VertexIndices.begin());
    ob.m_TextureIndices.erase(ob.m_TextureIndices.begin());
    ob.m_NormalIndices.erase(ob.m_NormalIndices.begin());
    m_VertexIndices = ob.m_VertexIndices;
    m_TextureIndices = ob.m_TextureIndices;
    m_NormalIndices = ob.m_NormalIndices;

    m_Positions.shrink_to_fit();
    m_VertexIndices.shrink_to_fit();
    m_TextureIndices.shrink_to_fit();
    m_NormalIndices.shrink_to_fit();
    m_VertexIDs.shrink_to_fit();

    NumVertices = m_Positions.size();
    NumIndices = m_VertexIndices.size();
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
        padd_value += i.NumVertices;
    }

    batchedMesh.NumVertices = batchedMesh.m_Positions.size();
    batchedMesh.NumIndices = batchedMesh.m_VertexIndices.size();
    batchedMesh.SetTransform(glm::translate(glm::mat4(1.0f), glm::vec3(+0.0f, 0.0, 0.0)));
    return batchedMesh;
}
