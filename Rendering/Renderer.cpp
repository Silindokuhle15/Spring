#include "Renderer.h"

#define MAX_NUM_VERTICES 10000
void Renderer::SetUpForRendering()
{
    unsigned int m_Width = 1920;
    unsigned int m_Height = 1080;

    glViewport(0, 0, m_Width, m_Height);
    glEnable(GL_DEPTH_BUFFER_BIT);
    glDepthRange(.010f, 1000.f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_VAO.OnInit();
    m_VAO.Bind();

    glCreateBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    
    unsigned int vertex_buffer_size = (sizeof(glm::vec3) + sizeof(glm::vec2)) * MAX_NUM_VERTICES;
    glNamedBufferData(m_VertexBuffer, vertex_buffer_size, nullptr, GL_DYNAMIC_DRAW);

    unsigned int index_buffer_size = sizeof(unsigned int) * MAX_NUM_VERTICES;
    glCreateBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glNamedBufferData(m_IndexBuffer, index_buffer_size, nullptr, GL_DYNAMIC_DRAW);

    m_Shader.m_Info[0] = this->m_Info[0];
    m_Shader.m_Info[1] = this->m_Info[1];
    m_Shader.OnInit();

    std::vector<VertexAttrib> attribs;
    attribs.push_back(VertexAttrib::Position);
    attribs.push_back(VertexAttrib::TexCoord);
    m_VAO.CreateVertexArrayLayout(m_Shader.GetShaderProgram(), attribs);

    m_VPlocation = glGetUniformLocation(m_Shader.GetShaderProgram(), "VP");
    m_Vlocation = glGetUniformLocation(m_Shader.GetShaderProgram(), "V");
    m_DeltaLocation = glGetUniformLocation(m_Shader.GetShaderProgram(), "delta");
}

void Renderer::OnRender(Scene* scene)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_CurrentIndexCount = 0;
    m_LastIndexCount = 0;
    for (int i = 0; i < scene->m_SceneData.size(); i++)
    {
        m_Shader.Bind();
        m_VAO.Bind();

        m_CurrentIndexCount  = scene->m_SceneData[i]->m_IndexCount;
        m_LastIndexCount += m_CurrentIndexCount;
        glm::mat4 transformB = scene->m_SceneData[i]->m_Transform;

        unsigned int pos_offset = (sizeof(glm::vec3) + sizeof(glm::vec2)) * m_LastIndexCount * i;
        unsigned int tex_offset = sizeof(glm::vec3) * m_CurrentIndexCount + pos_offset;
        unsigned int ind_offset = sizeof(unsigned int) * m_LastIndexCount * i;

        unsigned int uniformLocation = glGetUniformLocation(m_Shader.GetShaderProgram(), "Model");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(transformB));
        glNamedBufferSubData(m_VertexBuffer, pos_offset, sizeof(glm::vec3) * m_CurrentIndexCount, scene->m_SceneData[i]->m_Positions.data());
        glNamedBufferSubData(m_VertexBuffer, tex_offset, sizeof(glm::vec2) * m_CurrentIndexCount, scene->m_SceneData[i]->m_TexCoords.data());

        glNamedBufferSubData(m_IndexBuffer, ind_offset, sizeof(unsigned int) * m_CurrentIndexCount, scene->m_SceneData[i]->m_VertexIndices.data());
        //glDrawElements(GL_TRIANGLE_STRIP, m_CurrentIndexCount , GL_UNSIGNED_INT, nullptr);
        GLenum render_mode = m_PrimitiveModeWireFrame ? GL_LINE_STRIP : GL_TRIANGLE_STRIP;
        glDrawElements(render_mode, m_CurrentIndexCount , GL_UNSIGNED_INT, nullptr);
    }
}

void Renderer::OnUpdate()
{
    glUseProgram(m_Shader.GetShaderProgram());
    glUniformMatrix4fv(m_VPlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetVP()));
    glUniformMatrix4fv(m_Vlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetV()));
}

void Renderer::OnUpdate(float ts)
{
    glUseProgram(m_Shader.GetShaderProgram());
    glUniformMatrix4fv(m_VPlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetVP()));
    glUniformMatrix4fv(m_Vlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetV()));
    glUniform1f(m_DeltaLocation, ts);

}
