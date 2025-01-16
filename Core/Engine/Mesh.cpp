#include "Mesh.h"

auto CalculateCurrentSizeOffset3fv = [](std::vector<std::vector<glm::vec3>>& buffer, size_t index)
    {
        size_t temp_size = 0;
        for (size_t inner_index = 0; inner_index < index; inner_index++)
        {
            temp_size += buffer[inner_index].size();
        }
        return index > 0 ? temp_size : 0;
    };

auto CalculateCurrentSizeOffset2fv = [](std::vector<std::vector<glm::vec2>>& buffer, size_t index)
    {
        size_t temp_size = 0;
        for (size_t inner_index = 0; inner_index < index; inner_index++)
        {
            temp_size += buffer[inner_index].size();
        }
        return index > 0 ? temp_size : 0;
    };


auto CalculateCurrentSizeOffset1fv = [](std::vector<std::vector<unsigned int>>& buffer, size_t index)
    {
        size_t temp_size = 0;
        for (size_t inner_index = 0; inner_index < index; inner_index++)
        {
            temp_size += buffer[inner_index].size();
        }
        return index > 0 ? temp_size : 0;
    };

void Mesh::OnInit()
{
    auto file_split = ObjectLoader::getWords(m_OBJFilePath, ".");
    auto len = file_split.size();
    auto& extension = file_split[len - 1];

    if (!extension.compare("obj"))
    {
        OBJObjectLoader ob;
        ob.LoadObjectFromFile(m_OBJFilePath.c_str());

        uint64_t current_number = 0;
        NumIndices = 0;
        NumVertices = 0;

        size_t number_of_objects = ob.m_ObjectNames.size();


        if (number_of_objects == 1)
        {
            m_Materials = ob.m_Materials;

            for (size_t object_index = 0; object_index < number_of_objects; object_index++)
            {
                for (auto& i : ob.m_Positions[object_index])
                {
                    m_Positions.push_back(i);
                }
                for (int i = 0; i < ob.m_VertexIDs[object_index].size(); i++)
                {
                    auto id_index = ob.m_VertexIDs[object_index][i];
                    m_VertexIDs.push_back(id_index);
                }
                for (int i = 0; i < ob.m_VertexIndices[object_index].size(); i++)
                {
                    uint32_t index = ob.m_VertexIndices[object_index][i];
                    //if (index < NumIndices)
                    {
                        glm::vec3 pos = ob.m_Positions[object_index][index];
                        m_VertexIndices.push_back(index);
                    }
                }
                for (int i = 0; i < ob.m_TextureIndices[object_index].size(); i++)
                {
                    uint32_t index = ob.m_TextureIndices[object_index][i];
                    //if (index < NumIndices)
                    {
                        glm::vec2 tex_coord = ob.m_TexCoords[object_index][index];
                        m_TexCoords.push_back(tex_coord);
                        m_TextureIndices.push_back(index);
                    }
                }
                for (int i = 0; i < ob.m_NormalIndices[object_index].size(); i++)
                {
                    uint32_t index = ob.m_NormalIndices[object_index][i];
                    //if (index < NumIndices)
                    {
                        glm::vec3 normal = ob.m_Normals[object_index][index];
                        m_Normals.push_back(normal);
                        m_NormalIndices.push_back(index);
                    }
                }

                for (uint32_t i = 0; i < ob.m_VertexIndices[object_index].size(); i++)
                {
                    glm::vec3 pos = m_Positions[m_VertexIndices[i]];

                    glm::vec2 tex = m_TexCoords[m_TextureIndices[i]];

                    float  ID = m_VertexIDs[m_VertexIndices[i]];

                    glm::vec3 norm = m_Normals[m_NormalIndices[i]];

                    m_V.push_back({ pos, tex, ID, norm });
                }
                NumIndices = static_cast<uint32_t>(m_V.size());
                NumVertices = static_cast<uint32_t>(m_Positions.size());

            }
        }
        else
        {
            for (size_t object_index = 0; object_index < number_of_objects; object_index++)
            {
                m_SubMeshes.push_back(Mesh());
            }
            for (size_t object_index = 0; object_index < number_of_objects; object_index++)
            {
                m_SubMeshes[object_index].m_Materials = ob.m_Materials;

                for (auto& i : ob.m_Positions[object_index])
                {
                    m_SubMeshes[object_index].m_Positions.push_back(i);
                }


                for (int i = 0; i < ob.m_VertexIDs[object_index].size(); i++)
                {
                    uint64_t id_index = ob.m_VertexIDs[object_index][i];
                    m_SubMeshes[object_index].m_VertexIDs.push_back(id_index);
                }
                current_number = CalculateCurrentSizeOffset3fv(ob.m_Positions, object_index);
                for (int i = 0; i < ob.m_VertexIndices[object_index].size(); i++)
                {
                    uint64_t v_index = ob.m_VertexIndices[object_index][i];
                    //if (index < NumIndices)
                    {
                        glm::vec3 pos = ob.m_Positions[object_index][v_index - current_number];
                        m_SubMeshes[object_index].m_VertexIndices.push_back(v_index);
                    }
                }
                current_number = CalculateCurrentSizeOffset2fv(ob.m_TexCoords, object_index);
                for (int i = 0; i < ob.m_TextureIndices[object_index].size(); i++)
                {
                    uint32_t index = ob.m_TextureIndices[object_index][i];

                    //if (index < NumIndices)
                    {
                        glm::vec2 tex_coord = ob.m_TexCoords[object_index][index - current_number];
                        m_SubMeshes[object_index].m_TexCoords.push_back(tex_coord);
                        m_SubMeshes[object_index].m_TextureIndices.push_back(index);
                    }
                }
                current_number = CalculateCurrentSizeOffset3fv(ob.m_Normals, object_index);
                for (int i = 0; i < ob.m_NormalIndices[object_index].size(); i++)
                {
                    uint32_t index = ob.m_NormalIndices[object_index][i];
                    //if (index < NumIndices)
                    {
                        glm::vec3 normal = ob.m_Normals[object_index][index - current_number];
                        m_SubMeshes[object_index].m_Normals.push_back(normal);
                        m_SubMeshes[object_index].m_NormalIndices.push_back(index);
                    }
                }

                for (uint32_t i = 0; i < ob.m_VertexIndices[object_index].size(); i++)
                {
                    current_number = CalculateCurrentSizeOffset3fv(ob.m_Positions, object_index);
                    glm::vec3 pos = m_SubMeshes[object_index].m_Positions[m_SubMeshes[object_index].m_VertexIndices[i] - current_number];
                    float  ID = m_SubMeshes[object_index].m_VertexIDs[m_SubMeshes[object_index].m_VertexIndices[i] - current_number];

                    current_number = CalculateCurrentSizeOffset2fv(ob.m_TexCoords, object_index);
                    glm::vec2 tex = m_SubMeshes[object_index].m_TexCoords[m_SubMeshes[object_index].m_TextureIndices[i] - current_number];

                    //current_number = CalculateCurrentSizeOffset1fv(ob.m_VertexIDs, object_index);

                    //float  ID = m_SubMeshes[object_index].m_VertexIDs[m_SubMeshes[object_index].m_VertexIDs[i] - current_number];

                    current_number = CalculateCurrentSizeOffset3fv(ob.m_Normals, object_index);
                    glm::vec3 norm = m_SubMeshes[object_index].m_Normals[m_SubMeshes[object_index].m_NormalIndices[i] - current_number];

                    m_SubMeshes[object_index].m_V.push_back({ pos, tex, ID, norm });
                }

                m_SubMeshes[object_index].NumIndices = static_cast<uint32_t>(m_SubMeshes[object_index].m_V.size());
                m_SubMeshes[object_index].NumVertices = static_cast<uint32_t>(m_SubMeshes[object_index].m_Positions.size());
            }
        }
    }
    else if (!extension.compare("fbx"))
    {
        FBXObjectLoader ob;
        ob.LoadObjectFromFile(m_OBJFilePath.c_str());
        m_V = ob.m_MeshData;
        NumIndices = m_V.size();
    }
   
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
    return batchedMesh;
}
