#include "Square.h"

void Square::OnInit()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glm::vec2 pos[] = { {-1.0f, -1.0f}, { +1.0f, -1.0f }, {+1.0f, +1.0f}, { -1.0f, +1.0f} };
    glm::vec2 tex[] = { { 0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, { 0.0f, 1.0f} };
    unsigned int indices[] = { 0,1,2, 2, 3,0 };

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

    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    unsigned int size = sizeof(glm::vec2) * m_Positions.size();
    size += sizeof(glm::vec2) * m_TexCoords.size();

    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec2) * m_Positions.size(), m_Positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_Positions.size(), sizeof(glm::vec2) * m_TexCoords.size(),m_TexCoords.data());

    glGenBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_VertexIndices.size(), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * m_VertexIndices.size(), m_VertexIndices.data());

    //create_shaders();
    m_Shader.m_Info[0] = this->m_Info[0];
    m_Shader.m_Info[1] = this->m_Info[1];
    m_Shader.OnInit();

    m_VPOSlocation = glGetAttribLocation(m_Shader.GetShaderProgram(), "Position");
    m_VTEXlocation = glGetAttribLocation(m_Shader.GetShaderProgram(), "TexCoord");
    

    m_MVPlocation = glGetUniformLocation(m_Shader.GetShaderProgram(), "MVP");

    glVertexAttribPointer(m_VPOSlocation, 2, GL_FLOAT, GL_FALSE,
        sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(m_VPOSlocation);

    glVertexAttribPointer(m_VTEXlocation, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(sizeof(glm::vec2)));
    glEnableVertexAttribArray(m_VTEXlocation);

    m_IndexCount = m_VertexIndices.size();


    glCreateTextures(1, 1, &m_Tex1);
    glBindTexture(GL_TEXTURE_2D, m_Tex1);




}


void Square::OnUpdate()
{
    //glBindVertexArray(m_VAO);
    glUseProgram(m_Shader.GetShaderProgram());
    glUniformMatrix4fv(m_MVPlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetMVP()));
    //glUniform1i(m_SamplerLocation, 0);
}

void Square::OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawElements(GL_TRIANGLE_STRIP, m_VertexIndices.size(), GL_UNSIGNED_INT, NULL);
}

void Square::OnDestroy()
{
}