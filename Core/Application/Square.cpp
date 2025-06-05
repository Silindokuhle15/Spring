#include "Square.h"

void Square::OnInit()
{
    m_Positions = { {-1.0f, -1.0f, 0.0f}, { +1.0f, -1.0f, 0.0f }, {+1.0f, +1.0f, 0.0f}, { -1.0f, +1.0f, 0.0f} };
    m_TexCoords = { { 0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, { 0.0f, 1.0f}};
    m_VertexIDs = { 0, 1, 2, 3};
    m_VertexIndices = { 2, 3, 0, 1, 2, 0};
    // CHANGE THE VERTEX INDICES ORDER LATER

    m_Positions.shrink_to_fit();
    m_VertexIndices.shrink_to_fit();

    for (uint64_t i = 0; i < m_VertexIndices.size(); i++)
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