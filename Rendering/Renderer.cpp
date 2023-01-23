#include "Renderer.h"

void Renderer::SetUpForRendering()
{
    constexpr unsigned int MAX_NUM_VERTICES = 0xffffff;
    unsigned int m_Width = 1920;
    unsigned int m_Height = 1080;

    glViewport(0, 0, m_Width, m_Height);
    glEnable(GL_DEPTH_BUFFER_BIT);
    glDepthRange(.010f, 1000.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT_AND_BACK);
    glCullFace(GL_BACK);
    //glFrontFace(GL_CW);

    m_VAO.OnInit();
    m_VAO.Bind();

    glCreateBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    unsigned int vertex_buffer_size = (sizeof(glm::vec3) + sizeof(glm::vec2)) * MAX_NUM_VERTICES;
    //glNamedBufferStorage(m_VertexBuffer, vertex_buffer_size, nullptr, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    glNamedBufferData(m_VertexBuffer, vertex_buffer_size, nullptr, GL_DYNAMIC_DRAW);

    unsigned int index_buffer_size = sizeof(unsigned int) * MAX_NUM_VERTICES;
    glCreateBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glNamedBufferStorage(m_IndexBuffer, index_buffer_size, nullptr, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    //glNamedBufferData(m_IndexBuffer, index_buffer_size, nullptr, GL_DYNAMIC_DRAW);

    //unsigned int size = m_Width * m_Height * 4;
    //glCreateBuffers(1, &m_TexBuffer);
    //glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_TexBuffer);

    m_Material.OnInit();
    m_Material.Bind();
    
    std::vector<VertexAttrib> attribs;
    attribs.push_back(VertexAttrib::Position);
    attribs.push_back(VertexAttrib::TexCoord);
    m_VAO.CreateVertexArrayLayout(m_Material.m_MaterialID, attribs);

    m_VPlocation = glGetUniformLocation(m_Material.m_MaterialID, "VP");
    m_DeltaLocation = glGetUniformLocation(m_Material.m_MaterialID, "delta");

}

void Renderer::OnRender(Scene* scene)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_CurrentIndexCount = 0;
    m_LastIndexCount = 0;
    m_Material.Bind();
    m_VAO.Bind();
    
    std::vector<glm::vec3> pos;
    std::vector<glm::vec2> tex;

    for (int i = 0; i < scene->m_SceneData.size(); i++)
    {
        //m_Material.Bind();
        //m_VAO.Bind();
        
        m_CurrentIndexCount  = scene->m_SceneData[i]->m_IndexCount;
        m_LastIndexCount += m_CurrentIndexCount;
        glm::mat4 transformB = scene->m_SceneData[i]->m_Transform;

        unsigned int pos_offset = (sizeof(glm::vec3) + sizeof(glm::vec2)) * m_LastIndexCount * i;
        unsigned int tex_offset = sizeof(glm::vec3) * m_CurrentIndexCount + pos_offset;
        unsigned int ind_offset = sizeof(unsigned int) * m_LastIndexCount * i;

        unsigned int uniformLocation = glGetUniformLocation(m_Material.m_MaterialID, "Model");

        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(transformB));
        
        pos = std::move(scene->m_SceneData[i]->m_Positions);
        tex = std::move(scene->m_SceneData[i]->m_TexCoords);

        //glNamedBufferSubData(m_IndexBuffer, ind_offset, sizeof(unsigned int) * m_CurrentIndexCount, scene->m_SceneData[i]->m_VertexIndices.data());
        
        unsigned int * ind = (unsigned int*)glMapNamedBuffer(m_IndexBuffer, GL_READ_WRITE);
        for (int j = 0; j < m_CurrentIndexCount; j++)
        {
            ind[j] = scene->m_SceneData[i]->m_VertexIndices[j];
        }
        glUnmapNamedBuffer(m_IndexBuffer);

        glNamedBufferSubData(m_VertexBuffer, pos_offset, sizeof(glm::vec3) * m_CurrentIndexCount, pos.data());
        glNamedBufferSubData(m_VertexBuffer, tex_offset, sizeof(glm::vec2) * m_CurrentIndexCount, tex.data());
          
        GLenum render_mode = m_PrimitiveModeWireFrame ? GL_LINE_STRIP : GL_TRIANGLE_STRIP;
        glDrawElements(render_mode, m_CurrentIndexCount, GL_UNSIGNED_INT, nullptr);
    }
}
/*
void Renderer::OnUpdate()
{
    glUseProgram(m_Shader.GetShaderProgram());
    glUniformMatrix4fv(m_VPlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetVP()));
    glUniformMatrix4fv(m_Vlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetV()));
}
*/
void Renderer::OnUpdate(float ts)
{
    //m_Material.Bind();
    glUniformMatrix4fv(m_VPlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetVP()));
    //glUniformMatrix4fv(m_Vlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetV()));
    glUniform1f(m_DeltaLocation, ts);
    m_Material.OnUpdate();
}

void Renderer::EnableTesselation()
{
     float patch_inner[2] = { 5.0, 5.0 };
     float patch_outer[4] = { 6.0, 5.0, 8.0, 5.0 };
     glPatchParameteri(GL_PATCH_VERTICES, 36);
     glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, patch_inner);
     glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, patch_outer);
     glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
}
