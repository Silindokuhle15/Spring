#include "Renderer.h"

void Renderer::SetUpForRendering()
{
    constexpr unsigned int MAX_NUM_VERTICES = 1000000;
    unsigned int m_Width = 1920;
    unsigned int m_Height = 1080;

    m_DrawFrame.SetHeight(m_Height);
    m_DrawFrame.SetWidth(m_Width);

    m_DrawFrame.OnCreate();
    m_DrawFrame.Bind();


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CCW);

    glCreateBuffers(2, m_VertexBuffer);
  
    unsigned int vertex_buffer_size = sizeof(Vertex) * MAX_NUM_VERTICES;
    //glNamedBufferStorage(m_VertexBuffer, vertex_buffer_size, nullptr, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer[0]);
    glNamedBufferData(m_VertexBuffer[0], vertex_buffer_size, nullptr, GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    //glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer[1]);
    glNamedBufferData(m_VertexBuffer[1], vertex_buffer_size, nullptr, GL_DYNAMIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    unsigned int index_buffer_size = sizeof(unsigned int) * MAX_NUM_VERTICES;
    glCreateBuffers(2, m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer[0]);
    glNamedBufferData(m_IndexBuffer[0], index_buffer_size, nullptr, GL_STATIC_DRAW);

   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer[1]);
    glNamedBufferData(m_IndexBuffer[1], index_buffer_size, nullptr, GL_DYNAMIC_DRAW);
    //glNamedBufferStorage(m_IndexBuffer, index_buffer_size, nullptr, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    //unsigned int size = m_Width * m_Height * 4;
    //glCreateBuffers(1, &m_TexBuffer);
    //glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_TexBuffer);

    std::vector<VertexAttrib> attribs;
    attribs.push_back(VertexAttrib::Position);
    attribs.push_back(VertexAttrib::TexCoord);
    attribs.push_back(VertexAttrib::ID);
    attribs.push_back(VertexAttrib::Normal);

    m_VAO.OnInit();
    m_VAO.Bind();

    for (auto& mat : m_ActiveScene->m_Materials)
    {
        //mat.Bind();
        glUseProgram(mat.m_MaterialID);
        m_VAO.CreateVertexArrayLayout(mat.m_MaterialID, attribs);

        m_ModelLocation = glGetUniformLocation(mat.m_MaterialID, "Model");
        m_VPlocation = glGetUniformLocation(mat.m_MaterialID, "VP");
        m_DeltaLocation = glGetUniformLocation(mat.m_MaterialID, "delta");

    }
    m_ActiveScene->Process();
}

void Renderer::UploadToOpenGL()
{
    auto UploadBuffer3v = [&](std::vector<Mesh> buffer, GLenum render_mode, uint32_t vao, uint32_t vbo, uint32_t ibo, int model_location)
        {
            GLuint vBufferOffset = 0;                                                      // POSITIONS OFFSET INTO VERTEX ARRAY BUFFER
            GLuint iBufferOffset = 0;

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            for (size_t bIndex = 0; bIndex < buffer.size(); bIndex++)
            {
                glNamedBufferSubData(vbo, vBufferOffset, sizeof(Vertex) * buffer[bIndex].NumIndices, (void*)buffer[bIndex].m_V.data());
                vBufferOffset += sizeof(Vertex) * buffer[bIndex].NumIndices;

                glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(buffer[bIndex].m_Transform));
                glDrawArrays(render_mode, iBufferOffset, buffer[bIndex].NumIndices);
                iBufferOffset += buffer[bIndex].NumIndices;
            }
        };

    
    switch (m_ActiveScene->m_CurrentRenderMode)
    {
    case PrimitiveMode::WIRE_FRAME:
        render_mode = GL_LINE_LOOP;
        break;
    default:
        render_mode = GL_TRIANGLES;
    }
    UploadBuffer3v(m_ActiveScene->m_StaticGeometry, render_mode, m_VAO, m_VertexBuffer[0], m_IndexBuffer[0], m_ModelLocation);
    //UploadBuffer3v(m_ActiveScene->m_DynamicGeometry, render_mode, m_VAO, m_VertexBuffer[0], m_IndexBuffer[0], m_ModelLocation);
    //UploadBuffer3v(m_ActiveScene->m_MeshData, GL_TRIANGLES, m_VAO, m_VertexBuffer[0], m_IndexBuffer[0]);
}

void Renderer::OnRender()
{
    // REDESIGN THE ENTIRE RENDER LOOP
    CreateImage();
    UploadToOpenGL();

    GLbitfield flags = GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT;
    glBlitFramebuffer(0, 0, 1920, 1080, 0, 0, 1920, 1080, flags, GL_NEAREST);
    glEndQuery(GL_SAMPLES_PASSED);

}
void Renderer::BindScene(std::shared_ptr<Scene> scene)
{
    if (m_ActiveScene)
        m_ActiveScene.reset();
    m_ActiveScene = scene;
}

void Renderer::OnUpdate(TimeStep ts)
{
    glUniformMatrix4fv(m_VPlocation, 1, GL_FALSE, glm::value_ptr(m_ActiveScene->m_ActiveCamera.GetVP()));
    //glUniformMatrix4fv(m_Vlocation, 1, GL_FALSE, glm::value_ptr(m_ActiveScene->m_ActiveCamera->GetV()));
    glUniform1f(m_DeltaLocation, static_cast<GLclampf>(ts));
    m_ActiveScene->OnUpdate(ts);
}

void Renderer::CreateImage()
{
    GLclampf r = 0.02f, g = 0.019f, b = 0.092f, a = 1.0f;
    glClearColor(r, g, b, a);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    //glDrawBuffer(GL_DEPTH_ATTACHMENT);
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
    m_DrawFrame.Bind();
    //m_VAO.Bind();
    GLenum flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    glClear(flags);

    std::vector<std::string> m_keys = {
    "Position",
    "TexCoord",
    "ID",
    "Normal"
    };

    for (auto& mat : m_ActiveScene->m_Materials)
    {
        mat.Bind();
        for (auto& vao_attrib : m_keys)
        {
            m_VAO.EnableAttribute(mat.m_MaterialID, vao_attrib.c_str());
        }
    }
    m_ActiveScene->m_ActiveCamera.Present();
}

void Renderer::EndFrame()
{
    glFlush();
}
