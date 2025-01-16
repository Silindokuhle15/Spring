#include "Renderer.h"

void Renderer::SetUpForRendering()
{
    constexpr unsigned int MAX_NUM_VERTICES = 1000000;
    unsigned int m_Width = 1920;
    unsigned int m_Height = 1080;

    //m_DrawFrame.SetHeight(m_Height);
    //m_DrawFrame.SetWidth(m_Width);

    //m_DrawFrame.OnCreate();
    //m_DrawFrame.Bind();


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_STENCIL_TEST);
    //glEnable(GL_SCISSOR_TEST);
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

    // CREATE THE MATERIALS
    for (size_t index = 0; index < m_ActiveScene->m_Shaders.size(); index++)
    {
        GLuint program = glCreateProgram();
        //m_ActiveScene->m_Materials[index].m_MaterialID = glCreateProgram();
        //m_ActiveScene->m_Materials[index].OnInit();
        //m_GLSamplers.push_back(static_cast<GLuint>(index));

        for (size_t k = 0; k < m_ActiveScene->m_Shaders[index].m_Info.size(); k++)
        {

            GLenum shader_type = m_ActiveScene->m_Shaders[index].m_Info[k].ShaderType;
            auto shader = glCreateShader(shader_type);
            m_ActiveScene->m_Shaders[index].OnInit();

            auto shader_source = m_ActiveScene->m_Shaders[index].m_ShaderSource[k].c_str();
            glShaderSource(shader, 1, &shader_source, NULL);
            glCompileShader(shader);

            glAttachShader(program, shader);
        }


        glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
        glLinkProgram(program);
        m_RenderPrograms.push_back(program);

        //CreateTexture<GL_Texture>(m_ActiveScene->m_Materials[index].m_Texture);
    }

    for (auto& program : m_RenderPrograms)
    {
        glUseProgram(program);
        m_VAO.CreateVertexArrayLayout(program, attribs);

        m_LightLocation = glGetUniformLocation(program, "LightPosition");
        m_CameraEyeLocation = glGetUniformLocation(program, "CameraEye");
        m_ModelLocation = glGetUniformLocation(program, "Model");
        m_VPlocation = glGetUniformLocation(program, "VP");
        m_DeltaLocation = glGetUniformLocation(program, "delta");

    }

    auto func = [this](std::vector<Mesh> buffer)
        {
            for (int i = 0; i < buffer.size(); i++)
            {
                int m_ActiveMaterial = 0;
                glGetIntegerv(GL_CURRENT_PROGRAM, &m_ActiveMaterial); // m_CurrentProgram shoubd be the currently bound Material ID
                uint32_t model_location = glGetUniformLocation(m_ActiveMaterial, "Model");
                uint32_t normal_matrix_location = glGetUniformLocation(m_ActiveMaterial, "NormalMatrix");

                m_ModelLocations.push_back(model_location);
                m_NormalMatrixLocations.push_back(normal_matrix_location);
            }
        };

    // CREATE OPENGL TEXTURE SAMPLERS
    for (size_t index = 0; index < m_ActiveScene->m_Materials.size(); index++)
    {
        GLuint sampler_index = static_cast<GLuint>(index);
        glGenSamplers(1, &m_GLSamplers[sampler_index]);
        glBindSampler(sampler_index, m_GLSamplers[sampler_index]);


        glSamplerParameteri(m_GLSamplers[sampler_index], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(m_GLSamplers[sampler_index], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    //

    const char*  data =
        "0xc0" "0xc0""0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0"
        "0xc0" "0xc0""0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0"
        "0xc0" "0xc0""0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0"
        "0xc0" "0xc0""0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0"
        "0xc0" "0xc0""0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0"
        "0xc0" "0xc0""0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0"
        "0xc0" "0xc0""0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0"
        "0xc0" "0xc0""0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0"
        "0xc0" "0xc0""0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0"
        "0xc0" "0xc0""0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0" "0xc0"
        ;
    _TextureView view{};
    view.m_Height = 10;
    view.m_Width = 10;
    view.m_TextureData.push_back((float*)(data));
    _TextureDescription desc{};

    desc.m_TextureFormat = _TextureFormat::RGB8;
    desc.m_TextureSource = _TextureSource::GL_ATTACHMENT;
    desc.m_TextureTarget = _TextureTarget::TEXTURE_2D;

    
    m_Textures.push_back(TextureBase<GL_Texture>(desc, view));

    const char * new_data = 
        "0xE0" "0xE""0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0"
        "0xE0" "0xE""0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0"
        "0xE0" "0xE""0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0"
        "0xE0" "0xE""0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0"
        "0xE0" "0xE""0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0"
        "0xE0" "0xE""0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0"
        "0xE0" "0xE""0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0"
        "0xE0" "0xE""0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0"
        "0xE0" "0xE""0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0"
        "0xE0" "0xE""0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0" "0xE0"
        ;
    m_Textures.push_back(TextureBase<GL_Texture>(desc, view));

    CreateTexture<GL_Texture>(m_Textures[0]);
    CreateTexture<GL_Texture>(m_Textures[1]);
    m_Textures[0].OnInit();
    m_Textures[1].OnInit();


    glBindImageTexture(1, m_Textures[0].m_Texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(2, m_Textures[1].m_Texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8UI);

    auto eye = m_ActiveScene->m_pActiveCamera->GetEye();
    glm::vec3 end = glm::vec3(0.0, 0.0, 0.0);
    Ray test_ray(eye, end);
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

                if (buffer[bIndex].m_SubMeshes.size() > 0)
                {
                    for (auto& sub_mesh : buffer[bIndex].m_SubMeshes)
                    {
                        glNamedBufferSubData(vbo, vBufferOffset, sizeof(Vertex) * sub_mesh.NumIndices, (void*)sub_mesh.m_V.data());
                        vBufferOffset += sizeof(Vertex) * sub_mesh.NumIndices;

                        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(sub_mesh.m_Transform));
                        glDrawArrays(render_mode, iBufferOffset, sub_mesh.NumIndices);
                        iBufferOffset += sub_mesh.NumIndices;
                    }
                }
                    glNamedBufferSubData(vbo, vBufferOffset, sizeof(Vertex) * buffer[bIndex].NumIndices, (void*)buffer[bIndex].m_V.data());
                    vBufferOffset += sizeof(Vertex) * buffer[bIndex].NumIndices;

                    glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(buffer[bIndex].m_Transform));
                    glDrawArrays(render_mode, iBufferOffset, buffer[bIndex].NumIndices);
                    iBufferOffset += buffer[bIndex].NumIndices;

            }
        };

    render_mode = GL_TRIANGLE_STRIP;
    std::vector<Mesh> temp_vec;

    
    for (auto& ch : m_ActiveScene->m_Characters)
    {
        auto mesh = ch->GetComponent<Mesh>();
        temp_vec.push_back(*mesh);
    }
    
    UploadBuffer3v(temp_vec, render_mode, m_VAO, m_VertexBuffer[0], m_IndexBuffer[0], m_ModelLocation);

    //UploadBuffer3v(m_ActiveScene->m_DynamicGeometry, render_mode, m_VAO, m_VertexBuffer[0], m_IndexBuffer[0], m_ModelLocation);
    //UploadBuffer3v(m_ActiveScene->m_MeshData, GL_TRIANGLES, m_VAO, m_VertexBuffer[0], m_IndexBuffer[0]);
}

void Renderer::CreateOpenGLTexture(_TextureView& view, _TextureDescription& desc, GL_Texture& tex)
{    
    glCreateTextures(GL_TEXTURE_2D, 1, &tex);

    GLenum format = GL_RGB8;

    GLuint width = 10, height = 10;
    void* data = nullptr;
  
    glTextureStorage2D(tex, 1, format, width, height);

    glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureSubImage2D(tex, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

}

void Renderer::OnRender()
{
    // REDESIGN THE ENTIRE RENDER LOOP
    CreateImage();
    UploadToOpenGL();

    GLbitfield flags = GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT;
    //glBlitFramebuffer(0, 0, 1920, 1080, 0, 0, 1920, 1080, flags, GL_NEAREST);

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
    glUniformMatrix4fv(m_VPlocation, 1, GL_FALSE, glm::value_ptr(m_ActiveScene->m_pActiveCamera->GetVP()));

    auto pos = m_ActiveScene->m_pActiveCamera->GetEye();
    GLfloat pos_3fv[] = { pos.x, pos.y, pos.z };
    glUniform3fv(m_CameraEyeLocation, 3, pos_3fv);

    auto light_pos = glm::vec3(0.0, 15.0, 0.0);
    //float light_3fv[] = { light_pos.x, light_pos.y, light_pos.z };
    GLfloat light_3fv[] = { 0.0f, 10.5f,  0.0f};
    glUniform3fv(m_LightLocation, 3, light_3fv);

    glUniform1f(m_DeltaLocation, static_cast<GLclampf>(ts));

    m_ActiveScene->OnUpdate(ts);

    auto func = [this, ts](std::vector<Mesh> buffer)
        {
            for (int i = 0; i < buffer.size(); i++)
            {
                buffer[i].OnUpdate(ts);
                unsigned int model_location = m_ModelLocations[i];
                unsigned int normal_matrix_location = m_NormalMatrixLocations[i];

                glm::mat4 transform = buffer[i].m_Transform;
                glm::mat4 model_view = transform * m_ActiveScene->m_pActiveCamera->GetV();

                glm::mat4 normal_matrix = glm::transpose(glm::inverse(model_view));

                glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(transform));
                glUniformMatrix4fv(normal_matrix_location, 1, GL_FALSE, glm::value_ptr(normal_matrix));
            }

            // UPDATE THE MATERIALS

            for (auto& program : m_RenderPrograms)
            {
                //mtl.OnUpdate();
                GLuint KaLocation = glGetUniformLocation(program, "AmbientColor");
                GLuint KdLocation = glGetUniformLocation(program, "DiffuseColor");
                GLuint KsLocation = glGetUniformLocation(program, "SpecularColor");

                //glProgramUniform3fv(mtl.m_MaterialID, KdLocation, 1, glm::value_ptr(mtl.m_Kd));
                //glProgramUniform3fv(mtl.m_MaterialID, KaLocation, 1, glm::value_ptr(mtl.m_Ka));
                //glProgramUniform3fv(mtl.m_MaterialID, KsLocation, 1, glm::value_ptr(mtl.m_Ks));

            }

            // UPDATE OPENGL SAMPLER UNIFORMS
            for (size_t index=0; index  < m_GLSamplers.size(); index++)
            {
                GLuint sampler_unit = static_cast<GLuint>(index);
                GLuint sampler = static_cast<GLuint>(m_GLSamplers[sampler_unit]);
                glUniform1i(sampler, sampler_unit);
            }
        };

    //func(m_ActiveScene->m_StaticGeometry);
    //func(m_ActiveScene->m_DynamicGeometry);
    //func(m_ActiveScene->m_MeshData);
    for (auto& tex : m_Textures)
    {
        tex.OnUpdate();
    }
}

void Renderer::CreateImage()
{
    GLclampf r = 0.02f, g = 0.019f, b = 0.092f, a = 1.0f;
    glClearColor(r, g, b, a);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    //glDrawBuffer(GL_COLOR_ATTACHMENT1);
    //glDrawBuffer(GL_COLOR_ATTACHMENT2);
    glDrawBuffer(GL_DEPTH_ATTACHMENT);
    //glDrawBuffer(GL_FRONT);
    //glReadBuffer(GL_COLOR_ATTACHMENT1);
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
    //m_DrawFrame.Bind();
    //m_VAO.Bind();
    GLenum flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    glClear(flags);

    std::vector<std::string> m_keys = {
    "Position",
    "TexCoord",
    "ID",
    "Normal"
    };

    for (size_t index = 0; index < m_RenderPrograms.size(); index++)
    {
        GLuint texture_slot = static_cast<GLuint>(index);
        glUseProgram(m_RenderPrograms[index]);
        GLuint KaLocation = glGetUniformLocation(m_RenderPrograms[index], "AmbientColor");
        GLuint KdLocation = glGetUniformLocation(m_RenderPrograms[index], "DiffuseColor");
        GLuint KsLocation = glGetUniformLocation(m_RenderPrograms[index], "SpecularColor");
        //glBindTextureUnit(texture_slot, m_ActiveScene->m_Materials[index].m_Texture.m_Texture.m_Texture);

        for (auto& vao_attrib : m_keys)
        {
            m_VAO.EnableAttribute(m_RenderPrograms[index], vao_attrib.c_str());
        }
    }
}

void Renderer::EndFrame()
{
    glFlush();
}
