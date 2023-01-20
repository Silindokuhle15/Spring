#include "Square.h"


void Square::OnInit()
{
    m_VAO.OnInit();
    m_VAO.Bind();

    glm::vec3 pos[] = { {-1.0f, -1.0f, 0.0f}, { +1.0f, -1.0f, 0.0f }, {+1.0f, +1.0f, 0.0f}, { -1.0f, +1.0f, 0.0f} };
    glm::vec2 tex[] = { { 0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, { 0.0f, 1.0f} };
    unsigned int indices[] = { 3,0, 1, 1, 2,3 };

    for (int i = 0; i < 4; i++)
    {
        m_Positions.push_back(pos[i]);
        m_TexCoords.push_back(tex[i]);
    }
    for (int i = 0; i < 6; i++)
    {
        m_VertexIndices.push_back(indices[i]);
    }

    m_Positions.shrink_to_fit();
    m_VertexIndices.shrink_to_fit();
    
    std::vector<VertexAttrib> attribs;
    attribs.push_back(VertexAttrib::Position);
    attribs.push_back(VertexAttrib::TexCoord);
    m_VAO.CreateVertexArrayLayout(m_Shader.GetShaderProgram(), attribs);

    Base::m_IndexCount = m_VertexIndices.size();   
}