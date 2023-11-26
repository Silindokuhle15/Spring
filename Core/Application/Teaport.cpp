#include "Teaport.h"

void Teaport::OnInit()
{
    ObjectLoader ob;
    ob.LoadObjectFromFile("Objects/Uta_teaport.obj");

    m_Positions = ob.m_Positions;
    m_TexCoords = ob.m_TexCoords;
    m_Normals = ob.m_Normals;

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

    Base::m_IndexCount = m_VertexIndices.size();
}


void Teaport::OnUpdate(float ts)
{
    // Update the model transform 
    //m_Transform = glm::mat4(glm::translate(m_Transform, glm::vec3(0.0, 0.0, -0.10))); // Not the best solution, still needs to be updated,, all in due time
    m_Ts = ts;
}

void Teaport::MoveBackward()
{
    glm::vec3 translation = glm::vec3(0.0, 0.0, +1.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}

void Teaport::MoveForward()
{
    glm::vec3 translation = glm::vec3(0.0, 0.0, -1.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}

void Teaport::MoveLeft()
{
    glm::vec3 translation = glm::vec3(+1.0, 0.0, 0.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}

void Teaport::MoveRight()
{
    glm::vec3 translation = glm::vec3(-1.0, 0.0, 0.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}

void Teaport::MoveUp()
{
    glm::vec3 translation = glm::vec3(0.0, +1.0, 0.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}

void Teaport::MoveDown()
{
    glm::vec3 translation = glm::vec3(0.0, -1.0, 0.0) * m_Speed * (float)m_Ts;
    m_Transform = glm::mat4(glm::translate(m_Transform, translation));
}