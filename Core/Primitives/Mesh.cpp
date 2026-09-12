#include "Mesh.h"

namespace primitives
{
    Mesh::Mesh(
        const std::vector<Vertex>& vertices,
        const std::vector<uint32_t>& vertex_indices
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
    Mesh GenerateArrow(const glm::vec3 axis, const std::size_t num_segments, float shaft_start, float shaft_end, float shaft_radius, float head_radius, float tip_y)
    {
        if (axis == glm::vec3(0.0f))
        {
            throw std::runtime_error("Invalid Axis !!!");
        }
        const auto normalizedAxis = glm::normalize(axis);
        glm::vec3 reference;
        if (glm::abs(axis.y) < 0.999f)
        {
            reference = glm::vec3(0, 1, 0);
        }
        else
        {
            reference = glm::vec3(1, 0, 0);
        }
        const auto u = glm::normalize(glm::cross(reference, axis));
        const auto v = glm::normalize(glm::cross(axis, u));
        std::vector<primitives::Vertex> buffer;
        buffer.reserve( num_segments * 15);
        for (int index = 0; index < num_segments; index++)
        {
            const auto next = (index + 1) % num_segments;
            const float a0 = 2.0f * std::_Pi_val * index / num_segments;
            const float a1 = 2.0f * std::_Pi_val * next / num_segments;
            const glm::vec3 r0{ u * glm::cos(a0) + v * glm::sin(a0) };
            const glm::vec3 r1{ u * glm::cos(a1) + v * glm::sin(a1) };
            const Vertex shaftBottom0{ {axis * shaft_start + r0 * shaft_radius}, {0.0f, 1.0f}, {1.0f}, axis };
            const Vertex shaftBottom1{ {axis * shaft_start + r1 * shaft_radius}, {0.0f, 1.0f}, {1.0f}, axis };
            const Vertex shaftTop0{ {axis * shaft_end + r0 * shaft_radius}, {0.0f, 1.0f}, {1.0f}, axis };
            const Vertex shaftTop1{ {axis * shaft_end + r1 * shaft_radius}, {0.0f, 1.0f}, {1.0f}, axis };
            const Vertex headBase0{ {axis * shaft_end + r0 * head_radius}, {0.0f, 1.0f}, {1.0f}, axis };
            const Vertex headBase1{ {axis * shaft_end + r1 * head_radius}, {0.0f, 1.0f}, {1.0f}, axis };
            const Vertex tip{ {axis * tip_y},{0.0f, 1.0f},{1.0f}, axis };

            buffer.insert(buffer.end(),
                {
                    shaftBottom0, shaftTop0, shaftBottom1,
                    shaftBottom1, shaftTop0, shaftTop1
                });
            buffer.insert(buffer.end(),
                {
                    shaftTop0, headBase0, shaftTop1,
                    shaftTop1, headBase0, headBase1
                });
            buffer.insert(buffer.end(),
                {
                    headBase0, tip, headBase1
                });
        }
        const float dist = 0.25f;
        const float size = 0.5f;
        const float half = 0.5f * size;
        glm::vec3 center = -axis * dist + u * size + v * size;
        if (glm::abs(axis.x) > 0.999f)
        {
            center = -axis * dist - u * size + v * size;
        }
        const Vertex p0{ {center - u * half - v * half}, { 0.0f, 1.0f }, {1.0f},{axis} };
        const Vertex p1{ {center + u * half - v * half}, { 0.0f, 1.0f }, {1.0f},{axis} };
        const Vertex p2{ {center + u * half + v * half}, { 0.0f, 1.0f }, {1.0f},{axis} };
        const Vertex p3{ {center - u * half + v * half}, { 0.0f, 1.0f }, {1.0f},{axis} };
        buffer.insert(buffer.end(),
            {
                p0, p1, p2,
                p0, p2, p3
            });
        uint32_t indexCount{ 0 };
        std::vector<uint32_t> indexBuffer(buffer.size());
        for (const auto& v : buffer)
        {
            indexBuffer[indexCount] = indexCount;
            ++indexCount;
        }
        return Mesh(std::move(buffer), std::move(indexBuffer));
    }
}