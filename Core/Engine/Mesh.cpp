#include "Mesh.h"


void Mesh::OnInit() {
    // Extract extension
    auto file_split = ObjectLoader::getWords(m_OBJFilePath, ".");
    if (file_split.empty())
        return;

    const std::string& extension = file_split.back();

    if (extension == "obj") {
        OBJObjectLoader loader;
        if (loader.LoadObjectFromFile(m_OBJFilePath.c_str()) != 0) {
            std::cerr << "Failed to load OBJ file: " << m_OBJFilePath << std::endl;
            return;
        }

        m_Materials = loader.m_Materials;

        size_t object_count = loader.m_ObjectNames.size();

        for (size_t i = 0; i < object_count; ++i) {
            Mesh sub;

            const auto& positions = loader.m_Positions[i];
            const auto& texCoords = loader.m_TexCoords[i];
            const auto& normals = loader.m_Normals[i];
            const auto& vIndices = loader.m_VertexIndices[i];
            const auto& tIndices = loader.m_TextureIndices[i];
            const auto& nIndices = loader.m_NormalIndices[i];
            const auto& vertexIDs = loader.m_VertexIDs[i];

            for (size_t j = 0; j < vIndices.size(); ++j) {
                Vertex vert;
                vert.pos = positions[vIndices[j]];

                vert.tex = (j < tIndices.size() && !texCoords.empty())
                    ? texCoords[tIndices[j]]
                    : glm::vec2(0.0f);

                vert.norm = (j < nIndices.size() && !normals.empty())
                    ? normals[nIndices[j]]
                    : glm::vec3(0.0f);

                vert.ID = (j < vertexIDs.size())
                    ? static_cast<float>(vertexIDs[j])
                    : 0.0f;

                sub.m_V.push_back(vert);
                sub.m_VertexIndices.push_back(static_cast<unsigned int>(j));
            }

            sub.m_Materials = m_Materials;
            m_SubMeshes.push_back(std::move(sub));
        }

        if (m_SubMeshes.size() == 1) {
            const Mesh& submesh = m_SubMeshes.front();

            // Clear existing mesh data to ensure we're overwriting the data
            m_Positions.clear();
            m_TexCoords.clear();
            m_Normals.clear();
            m_VertexIndices.clear();
            m_TextureIndices.clear();
            m_NormalIndices.clear();
            m_VertexIDs.clear();
            m_V.clear();

            // Copy data from the submesh into the main mesh
            m_Positions = submesh.m_Positions;
            m_TexCoords = submesh.m_TexCoords;
            m_Normals = submesh.m_Normals;
            m_VertexIndices = submesh.m_VertexIndices;
            m_TextureIndices = submesh.m_TextureIndices;
            m_NormalIndices = submesh.m_NormalIndices;
            m_VertexIDs = submesh.m_VertexIDs;
            m_V = submesh.m_V;

            // Optionally copy materials if needed (if not already set)
            if (m_Materials.empty()) {
                m_Materials = submesh.m_Materials;
            }

            // Clear the submeshes vector
            m_SubMeshes.clear();
        }
    }
    else if (!extension.compare("fbx"))
    {
        FBXObjectLoader ob;
        ob.LoadObjectFromFile(m_OBJFilePath.c_str());
        m_V = ob.m_MeshData;
    }
    else {
        std::cerr << "[Mesh::OnInit] Unsupported format: ." << extension << std::endl;
        // You can add support for other formats like:
        // - LoadFBX()
        // - LoadGLTF()
    }
}

/*
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
*/