#include "Square.h"

void Square::OnInit()
{
    m_Positions = { {-1.0f, -1.0f, 0.0f}, { +1.0f, -1.0f, 0.0f }, {+1.0f, +1.0f, 0.0f}, { -1.0f, +1.0f, 0.0f} };
    m_TexCoords = { { 0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, { 0.0f, 1.0f}};
    m_Normals = { m_Color,  m_Color, m_Color, m_Color};
    m_VertexIDs = { 0, 1, 2, 3};
    m_VertexIndices = { 0, 1, 2, 3, 0, 2};
    // CHANGE THE VERTEX INDICES ORDER LATER

    m_Positions.shrink_to_fit();
    m_VertexIndices.shrink_to_fit();
 
    NumVertices = static_cast<uint32_t>(m_Positions.size());
    NumIndices = static_cast<uint32_t>(m_VertexIndices.size());

    for (uint64_t i = 0; i < NumIndices; i++)
    {
        uint64_t temp_index = m_VertexIndices[i];
        float id = (float)m_VertexIndices[i];
        m_V.push_back(Vertex{
            m_Positions[temp_index],
            m_TexCoords[temp_index],
            m_VertexIDs[id], 
            m_Normals[temp_index]
        });
    }
 }


void Square::OnUpdate(TimeStep ts)
{
    // Update the model transform 
    //m_Transform = glm::mat4(glm::translate(m_Transform, glm::vec3(0.0, 0.0, -0.10))); // Not the best solution, still needs to be updated,, all in due time
    for (auto& pos : m_Positions)
    {
        pos = glm::vec3(m_Transform * glm::vec4(pos, 1.0f));
    }
    m_Ts = ts;
}