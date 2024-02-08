#include "Square.h"


void Square::OnInit()
{
    m_Positions = { {-1.0f, -1.0f, 0.0f}, { +1.0f, -1.0f, 0.0f }, {+1.0f, +1.0f, 0.0f}, { -1.0f, +1.0f, 0.0f} };
    m_TexCoords = { { 0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, { 0.0f, 1.0f}};
    m_Normals = { {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0} , {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0} };
    m_VertexIDs = { 1, 2, 3, 4 };
    m_VertexIndices = { 0, 1, 2, 3, 0, 2};
    m_Positions.shrink_to_fit();
    m_VertexIndices.shrink_to_fit();
 
    NumVertices = m_Positions.size();
    NumIndices = m_VertexIndices.size();

    for (int i = 0; i < NumVertices; i++)
    {
        m_V.push_back(Vertex{
            m_Positions[i],
            m_TexCoords[i],
            m_VertexIDs[i], 
            m_Normals[i] 
        });
    }
 }


void Square::OnUpdate(float ts)
{
    // Update the model transform 
    //m_Transform = glm::mat4(glm::translate(m_Transform, glm::vec3(0.0, 0.0, -0.10))); // Not the best solution, still needs to be updated,, all in due time
    for (auto& pos : m_Positions)
    {
        pos = glm::vec3(m_Transform * glm::vec4(pos, 1.0f));
    }
    m_Ts = ts;
}

void Square::MoveBackward()
{
    glm::vec3 translation = glm::vec3(0.0, 0.0, +1.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}

void Square::MoveForward()
{
    glm::vec3 translation = glm::vec3(0.0, 0.0, -1.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}

void Square::MoveLeft()
{
    glm::vec3 translation = glm::vec3(+1.0, 0.0, 0.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}

void Square::MoveRight()
{
    glm::vec3 translation = glm::vec3(-1.0, 0.0, 0.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}

void Square::MoveUp()
{
    glm::vec3 translation = glm::vec3(0.0, +1.0, 0.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}

void Square::MoveDown()
{
    glm::vec3 translation = glm::vec3(0.0, -1.0, 0.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}
