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


    unsigned int vertex_buffer_size = sizeof(GL_In) * MAX_NUM_VERTICES;
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
    attribs.push_back(VertexAttrib::UUID);
    attribs.push_back(VertexAttrib::Normal);
    m_VAO.CreateVertexArrayLayout(m_Material.m_MaterialID, attribs);

    m_VPlocation = glGetUniformLocation(m_Material.m_MaterialID, "VP");
    m_DeltaLocation = glGetUniformLocation(m_Material.m_MaterialID, "delta");

    m_ActiveScene->Process();
}

void Renderer::UploadToOpenGL()
{
    // UPLOAD DATA TO OPENGL
    for (auto& i : m_ActiveScene->m_MeshData)
    {
        int index = &i - &m_ActiveScene->m_MeshData[0];
        // Keep track of the buffer size
        // Make sure to properly offset the data

        GLuint size1 = sizeof(glm::vec3) * i.m_Positions.size();                // POSITIONS TO BE SENT TO OPENGL
        GLuint size2 = sizeof(glm::vec2) * i.m_TexCoords.size();                // TEXCOORDS TO BE SENT TO OPENGL
        GLuint size3 = sizeof(unsigned int) * i.m_VertexIDs.size();             // GUID TO BE SENT TO OPENGL
        GLuint size4 = sizeof(glm::vec3) * i.m_Normals.size();                  // NORMALS TO BE SENT TO OPENGL
        GLuint size5 = sizeof(unsigned int) * i.NumIndices;                      // INDICES TO BE SENT TO OPENGL

        // YOU HAVE THE INDEX IN THE ARRAY
        // YOU ALSO HAVE THE NUMBER OF ALL THE ELEMENTS IN THE ARRAY
        // YOU KNOW WHERE YOU ARE CURRENT LOOKING IN THE ARRAY
        // YOU HAVE TO FIND OUT HOW MANY MESHES IN THE ARRAY COME BEFORE ME, THAT IS GIVEN BY INDEX,
        // FIND OUT THE ACCUMULATIVE SIZE OF THE PRIOR DATA
        GLuint offset1 = 0;                                                     // POSITIONS OFFSET INTO VERTEX ARRAY BUFFER
        offset1 += index <= 0 ? 0: sizeof(GL_In)* m_ActiveScene->m_MeshData[index - 1].NumVertices;     
        GLuint offset2 = 0;                                                     // TEXCOORDS OFFSET INTO VERTEX ARRAY BUFFER
        GLuint offset3 = 0;                                                     // GUID OFFSET INTO VERTEX ARRAY BUFFER
        GLuint offset4 = 0;                                                     // NORMALS OFFSET INTO VERTEX ARRAY BUFFER
        GLuint offset5 = 0;                                                     // INDICES OFFSET INTO INDEX BUFFER

        //offset1 += sizeof(glm::vec3) * m_ActiveScene->m_MeshData[index - 1].NumVertices;
        offset2 += offset1 + size1;
        offset3 += offset2 + size2;
        offset4 += offset3 + size3;
        //offset5 += sizeof(unsigned int) * m_ActiveScene->m_MeshData[index - 1].NumIndices;

        glBufferSubData(GL_ARRAY_BUFFER, offset1, size1, (void*)i.m_Positions.data());
        glBufferSubData(GL_ARRAY_BUFFER, offset2, size2, (void*)i.m_TexCoords.data());
        glBufferSubData(GL_ARRAY_BUFFER, offset3, size3, (void*)i.m_VertexIDs.data());
        glBufferSubData(GL_ARRAY_BUFFER, offset4, size4, (void*)i.m_Normals.data());

        offset5 += index <= 0 ? 0 : sizeof(unsigned int) * m_ActiveScene->m_MeshData[index - 1].NumIndices;

        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset5, size5, (void*)i.m_VertexIndices.data());
        
        //m_LastIndexCount += i.NumIndices;

        //render_mode = m_PrimitiveModeWireFrame ? GL_LINE_STRIP : GL_TRIANGLE_STRIP;

        glDrawElements(render_mode, i.NumIndices, GL_UNSIGNED_INT, nullptr);
    }
}

void Renderer::OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //m_Material.Bind();
    //m_VAO.Bind();

    // REDESIGN THE ENTIRE RENDER LOOP
    render_mode = GL_TRIANGLES;
    UploadToOpenGL();
    glEndQuery(GL_SAMPLES_PASSED);
}
void Renderer::BindScene(std::shared_ptr<Scene> scene)
{
    m_ActiveScene = scene;
}

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
