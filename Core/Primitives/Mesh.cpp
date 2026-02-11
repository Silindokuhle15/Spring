#include "Mesh.h"

namespace primitives
{
    Mesh::Mesh(std::string file_path) :
        m_OBJFilePath{ file_path }
    {
        auto file_split = getWords(m_OBJFilePath, ".");
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
            // - LoadFBX()
            // - LoadGLTF()
        }
    }
    Mesh::Mesh(
        const std::vector<glm::vec3>& positions, 
        const std::vector<glm::vec2>& texcoords, 
        const std::vector<float>& ids, 
        const std::vector<glm::vec3>& normals,
        const std::vector<Vertex>& vertices,
        const std::vector<uint64_t>& vertex_indices,
        const std::vector<uint64_t>& texture_indices,
        const std::vector<uint64_t>& normal_indices,
        const std::vector<Material>& materials)
        :
        m_Positions{std::move(positions)},
        m_TexCoords{std::move(texcoords)},
        m_VertexIDs{std::move(ids)},
        m_Normals{std::move(normals)},
        m_V{std::move(vertices)},
        m_VertexIndices{std::move(vertex_indices)},
        m_TextureIndices{std::move(texture_indices)},
        m_NormalIndices{std::move(normal_indices)},
        m_Materials{std::move(materials)}
    {
    }
    
    Bound3D FindMinMax(const Mesh& mesh)
    {
        std::vector<float> xAxis;
        std::vector<float> yAxis;
        std::vector<float> zAxis;
        
        xAxis.reserve(mesh.m_V.size());
        yAxis.reserve(mesh.m_V.size());
        zAxis.reserve(mesh.m_V.size());

        if (mesh.m_SubMeshes.size() > 0)
        {
            for (auto& sub_mesh : mesh.m_SubMeshes)
            {
                for (auto& v : sub_mesh.m_V)
                {
                    glm::vec3 pos_new = v.pos;
                    xAxis.push_back(pos_new.x);
                    yAxis.push_back(pos_new.y);
                    zAxis.push_back(pos_new.z);
                }
            }
        }
        for (auto& v : mesh.m_V)
        {
            glm::vec3 pos_new = v.pos;
            xAxis.push_back(pos_new.x);
            yAxis.push_back(pos_new.y);
            zAxis.push_back(pos_new.z);
        }

        std::sort(xAxis.begin(), xAxis.end());
        std::sort(yAxis.begin(), yAxis.end());
        std::sort(zAxis.begin(), zAxis.end());

        auto xMin = xAxis.front();
        auto yMin = yAxis.front();
        auto zMin = zAxis.front();

        auto xMax = xAxis.back();
        auto yMax = yAxis.back();
        auto zMax = zAxis.back();
        return Bound3D{ xMin, yMin, zMin, xMax, yMax, zMax };
    }
}