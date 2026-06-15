#include "Mesh.h"

namespace primitives
{
    Mesh::Mesh(
        const std::vector<Vertex>& vertices,
        const std::vector<uint64_t>& vertex_indices
        //const std::vector<Material>& materials
        )
        :
        m_V{std::move(vertices)},
        m_VertexIndices{std::move(vertex_indices)}
        //m_Materials{std::move(materials)}
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