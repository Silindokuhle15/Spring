#include "Renderer.h"

void Renderer::SetUpForRendering()
{
    constexpr unsigned int MAX_NUM_VERTICES = 100000;
    unsigned int m_Width = 1920;
    unsigned int m_Height = 1080;

    m_FrameBuffer.SetHeight(m_Height);
    m_FrameBuffer.SetWidth(m_Width);

    m_FrameBuffer.OnCreate();
    m_FrameBuffer.Bind();


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT_AND_BACK);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);

    //m_VAO.OnInit();
    //m_VAO.Bind();

    glCreateBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    unsigned int vertex_buffer_size = (sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3)) * MAX_NUM_VERTICES;
    //glNamedBufferStorage(m_VertexBuffer, vertex_buffer_size, nullptr, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    glNamedBufferData(m_VertexBuffer, vertex_buffer_size, nullptr, GL_DYNAMIC_DRAW);

    unsigned int index_buffer_size = sizeof(unsigned int) * MAX_NUM_VERTICES;
    glCreateBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    //glNamedBufferStorage(m_IndexBuffer, index_buffer_size, nullptr, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    glNamedBufferData(m_IndexBuffer, index_buffer_size, nullptr, GL_DYNAMIC_DRAW);

    //unsigned int size = m_Width * m_Height * 4;
    //glCreateBuffers(1, &m_TexBuffer);
    //glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_TexBuffer);

    m_Material.OnInit();
    m_Material.Bind();
    
    std::vector<VertexAttrib> attribs;
    attribs.push_back(VertexAttrib::Position);
    attribs.push_back(VertexAttrib::TexCoord);
    attribs.push_back(VertexAttrib::Normal);
    m_VAO.CreateVertexArrayLayout(m_Material.m_MaterialID, attribs);

    m_VPlocation = glGetUniformLocation(m_Material.m_MaterialID, "VP");
    m_DeltaLocation = glGetUniformLocation(m_Material.m_MaterialID, "delta");

    m_ActiveScene->Process();
}

void Renderer::OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_CurrentIndexCount = 0;
    m_LastIndexCount = 0;
    //m_Material.Bind();
    //m_VAO.Bind();

    

    for(int i = 0; i < m_ActiveScene->m_Objects.size(); i++)
    {
        //m_Material.Bind();
        //m_VAO.Bind();
        
        m_CurrentIndexCount  = m_ActiveScene->m_Objects[i]->m_IndexCount;
        m_LastIndexCount += m_CurrentIndexCount;

        unsigned int pos_offset = (sizeof(glm::vec3) + sizeof(glm::vec2)) * m_LastIndexCount * i;
        unsigned int tex_offset = sizeof(glm::vec3) * m_CurrentIndexCount + pos_offset;
        unsigned int nom_offset = sizeof(glm::vec3) * m_CurrentIndexCount + tex_offset;
        unsigned int ind_offset = sizeof(unsigned int) * m_LastIndexCount * i;

        pos = (m_ActiveScene->m_positions);
        tex = (m_ActiveScene->m_texcoords);
        nom = (m_ActiveScene->m_normals);
        ind = (m_ActiveScene->m_indices);

        /*
        pos = (m_ActiveScene->m_Objects[i]->m_Positions);
        tex = std::move(m_ActiveScene->m_Objects[i]->m_TexCoords);
        nom = std::move(m_ActiveScene->m_Objects[i]->m_Normals);
        ind = std::move(m_ActiveScene->m_Objects[i]->m_VertexIndices);
        */
        if (m_IndexBufferSize < sizeof(ind))
        {
            m_IndexBufferSize += sizeof(ind);
            glNamedBufferSubData(m_VertexBuffer, pos_offset, sizeof(glm::vec3) * m_CurrentIndexCount, pos.data());
            glNamedBufferSubData(m_VertexBuffer, tex_offset, sizeof(glm::vec2) * m_CurrentIndexCount, tex.data());
            glNamedBufferSubData(m_VertexBuffer, nom_offset, sizeof(glm::vec3) * m_CurrentIndexCount, nom.data());
            glNamedBufferSubData(m_IndexBuffer, ind_offset, sizeof(unsigned int) * m_CurrentIndexCount, ind.data());

        }        
    }


    GLenum render_mode = m_PrimitiveModeWireFrame ? GL_LINE_STRIP : GL_TRIANGLE_STRIP;
    glDrawElements(render_mode, m_CurrentIndexCount, GL_UNSIGNED_INT, nullptr);


    glEndQuery(GL_SAMPLES_PASSED);
}
void Renderer::BindScene(std::shared_ptr<Scene> scene)
{
    m_ActiveScene = scene;
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
    glUniformMatrix4fv(m_VPlocation, 1, GL_FALSE, glm::value_ptr(m_ActiveScene->m_ActiveCamera->GetVP()));
    //glUniformMatrix4fv(m_Vlocation, 1, GL_FALSE, glm::value_ptr(m_ActiveScene->m_ActiveCamera->GetV()));
    glUniform1f(m_DeltaLocation, ts);
    m_ActiveScene->OnUpdate(ts);
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

void Renderer::BeginFrame()
{
    m_FrameBuffer.Bind();

    m_ActiveScene->m_ActiveCamera->Present();
}

void Renderer::EndFrame()
{
    unsigned int m_Width = 1920;
    unsigned int m_Height = 1080;

    GLbitfield flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, flags, GL_NEAREST);
}
