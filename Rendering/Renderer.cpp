#include "Renderer.h"

void Renderer::SetUpForRendering()
{
    unsigned int m_Width = 1920;
    unsigned int m_Height = 1080;

    glViewport(0, 0, m_Width, m_Height);
    glEnable(GL_DEPTH_BUFFER_BIT);
    glDepthRange(.010f, 1000.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*
    glCreateVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glCreateBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    struct PerVertex
    {
        glm::vec3 pos;
        glm::vec2 tex;
        glm::vec3 norm;
    };
    unsigned int size = m_Width * m_Height * sizeof(struct PerVertex);
    glNamedBufferStorage(m_VertexBuffer, size, nullptr, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT);

    glCreateBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glNamedBufferStorage(m_IndexBuffer, 1000, nullptr, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT);

    //create_shaders();
    m_Shader.m_Info[0] = this->m_Info[0];
    m_Shader.m_Info[1] = this->m_Info[1];
    m_Shader.OnInit();

    //glUseProgram(m_Shader.GetShaderProgram());

    m_VPOSlocation = glGetAttribLocation(m_Shader.GetShaderProgram(), "Position");
    //m_VTEXlocation = glGetAttribLocation(m_Shader.GetShaderProgram(), "TexCoord");

    m_MVPlocation = glGetUniformLocation(m_Shader.GetShaderProgram(), "MVP");
    m_MVlocation = glGetUniformLocation(m_Shader.GetShaderProgram(), "MV");
    //m_SamplerLocation = glGetUniformLocation(m_Shader.GetShaderProgram(), "u_Texture");

    glVertexAttribPointer(m_VPOSlocation, 2, GL_FLOAT, GL_FALSE,
        sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(m_VPOSlocation);

    //glVertexAttribPointer(m_VTEXlocation, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(sizeof(glm::vec2)));
    //glEnableVertexAttribArray(m_VTEXlocation);
    */

}

void Renderer::OnRender(Scene* scene)
{
    /*
    unsigned int vertex_buffer_size = sizeof(scene->m_VertexData.data());
    unsigned int index_buffer_size = sizeof(scene->m_IndexData.data());
    glNamedBufferSubData(m_VertexBuffer, 0, vertex_buffer_size, scene->m_VertexData.data());
    glNamedBufferSubData(m_IndexBuffer, 0, index_buffer_size, scene->m_IndexData.data());

    glDrawElements(GL_TRIANGLE_STRIP, scene->m_IndexData.size(), GL_UNSIGNED_INT, nullptr);
    */

    for (int i = 0; i < scene->m_SceneData.size(); i++)
    {
        scene->m_SceneData[i]->OnUpdate();
        scene->m_SceneData[i]->OnRender();
    }
}

void Renderer::OnUpdate()
{
    /*
    glUniformMatrix4fv(m_MVPlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetMVP()));
    glUniformMatrix4fv(m_MVlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetMV()));
    */
}
