#include "Renderer.h"
#include "stb_image.h"

void Renderer::SetUpForRendering()
{
    constexpr unsigned int MAX_NUM_VERTICES = 1000000;
    unsigned int m_Width = 1920;
    unsigned int m_Height = 1080;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glCreateBuffers(1, &m_VertexBuffer);
  
    unsigned int vertex_buffer_size = sizeof(Vertex) * MAX_NUM_VERTICES;
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glNamedBufferData(m_VertexBuffer, vertex_buffer_size, nullptr, GL_DYNAMIC_DRAW);

    unsigned int index_buffer_size = sizeof(unsigned int) * MAX_NUM_VERTICES;
    glCreateBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glNamedBufferData(m_IndexBuffer, index_buffer_size, nullptr, GL_DYNAMIC_DRAW);

    BufferLayout layout{
        {ShaderDataType::Float3, "Position"},
        {ShaderDataType::Float2, "TexCoord"},
        {ShaderDataType::Float,  "ID"},
        {ShaderDataType::Float3, "Normal"}
    };
    m_VAO.Bind();
    m_VAO.SetBufferLayout(layout);

    CreateSkyboxCubeMap();
    CreateOpenGLFrameBuffer();
}

void Renderer::UploadToOpenGL()
{
    m_ActiveScene->m_Shaders[0].Bind();
    auto handle = m_ActiveScene->m_Shaders[0].GetHandle();
    auto view = m_pActiveCamera->GetV();
    auto proj = m_pActiveCamera->GetP();

    GLuint viewLocation = glGetUniformLocation(handle, "View");
    GLuint projectionLocation = glGetUniformLocation(handle, "Projection");
    GLuint posLocation = glGetUniformLocation(handle, "pos");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(proj));

    for (auto& ch : m_ActiveScene->m_Characters)
    {
        Character var{ ch, m_ActiveScene.get() };

        auto& phzx = var.GetComponent<physics::PhysicsState>();
        glUniform3f(posLocation, phzx.position.x, phzx.position.y, phzx.position.z);
        auto& mesh = var.GetComponent<Mesh>();
        auto& material = mesh.m_Materials[1];
        for (auto& materialComponent : material.m_Uniforms3f)
        {
            auto& name = materialComponent.first;
            auto& data = materialComponent.second;
            GLuint location = glGetUniformLocation(handle, materialComponent.first.c_str());
            glUniform3f(location, data.x, data.y, data.z);
        }
        glNamedBufferSubData(m_VertexBuffer, vBuffer_offset, sizeof(Vertex) * mesh.m_V.size(), mesh.m_V.data());
        vBuffer_offset += sizeof(Vertex) * mesh.m_V.size();

        glDrawArrays(GL_TRIANGLES, index_offset, mesh.m_V.size());
        index_offset += mesh.m_V.size();


        auto mousePos = UnProjectMouse(m_ActiveScene->GetMousePosition());
        std::vector<Vertex> line{{ {phzx.position}, { 0.0, 1.0 }, 0, { 0.0, 1.0, 0.0 } },
        { { mousePos }, { 0.0, 1.0 }, 0, { 0.0, 1.0, 0.0 } }};

        glNamedBufferSubData(m_VertexBuffer, vBuffer_offset, sizeof(Vertex) * line.size(), line.data());
        vBuffer_offset += sizeof(Vertex) * line.size();

        glDrawArrays(GL_LINES, index_offset, line.size());
        index_offset += line.size();
    }
}

void Renderer::DrawSkyboxBackground()
{
    m_SkyboxShader.Bind();
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);

    glBufferSubData(GL_ARRAY_BUFFER, vBuffer_offset, sizeof(Vertex) * m_SkyboxMesh.m_V.size(), (void*)m_SkyboxMesh.m_V.data());
    vBuffer_offset += sizeof(Vertex) * m_SkyboxMesh.m_V.size();
    glBindTextureUnit(0, m_SkyboxTexture.m_Texture);

    auto v = glm::mat4_cast(m_pActiveCamera->m_orientation);
    auto rot_location = glGetUniformLocation(m_SkyboxShader.GetHandle(), "CameraOrientation");
    glUniformMatrix4fv(rot_location, 1, GL_FALSE, glm::value_ptr(v));

    glDrawArrays(GL_TRIANGLES, static_cast<int>(index_offset), 36);
    index_offset += m_SkyboxMesh.m_V.size();

    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
}

void Renderer::CreateOpenGLTexture(TextureBase<GL_Texture>& tex_base)
{
    GLuint target = 0;
    GLenum format = 0;
    auto data = tex_base.m_TextureData.data();

    GLuint width = static_cast<GLuint>(tex_base.m_Width);
    GLuint height = static_cast<GLuint>(tex_base.m_Height);

    switch (tex_base.m_TextureTarget)
    {
    case _TextureTarget::TEXTURE_1D:
        glCreateTextures(GL_TEXTURE_1D, 1, &tex_base.m_Texture);
        glTextureStorage1D(tex_base.m_Texture, 1, format, width);
        break;
    case _TextureTarget::TEXTURE_2D:
        glCreateTextures(GL_TEXTURE_2D, 1, &tex_base.m_Texture);
        glTextureParameteri(tex_base.m_Texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(tex_base.m_Texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        switch (tex_base.m_TextureFormat)
        {
        case _TextureFormat::RGB8:
            glTextureStorage2D(tex_base.m_Texture, 1, GL_RGB8, width, height);
            break;
        case _TextureFormat::RGB32F:
            glTextureStorage2D(tex_base.m_Texture, 1, GL_RGB32F, width, height);
            break;
        case _TextureFormat::RGBA8:
            glTextureStorage2D(tex_base.m_Texture, 1, GL_RGBA8, width, height);
            break;
        case _TextureFormat::RGBA32F:
            glTextureStorage2D(tex_base.m_Texture, 1, GL_RGBA32F, width, height);
            break;
        }
        break;
    case _TextureTarget::TEXTURE_3D:
        glCreateTextures(GL_TEXTURE_3D, 1, &tex_base.m_Texture);
        glTextureStorage3D(tex_base.m_Texture, 1, format, width, height, 0);
        break;

    case _TextureTarget::TEXTURE_CUBE:
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &tex_base.m_Texture);

        switch (tex_base.m_TextureFormat)
        {
        case _TextureFormat::RGB8:
            glTextureStorage2D(tex_base.m_Texture, 1, GL_RGB8, width, height);
            break;
        case _TextureFormat::RGB32F:
            glTextureStorage2D(tex_base.m_Texture, 1, GL_RGB32F, width, height);
            break;
        case _TextureFormat::RGBA8:
            glBindTexture(GL_TEXTURE_CUBE_MAP, tex_base.m_Texture);
            glTexStorage2D(GL_TEXTURE_CUBE_MAP, 10, GL_RGBA8, width, height);
            //glTextureStorage2D(tex_base.m_Texture, 1, GL_RGBA8, width, height);
            break;
        case _TextureFormat::RGBA32F:
            glBindTexture(GL_TEXTURE_CUBE_MAP, tex_base.m_Texture);
            glTexStorage2D(GL_TEXTURE_CUBE_MAP, 10, GL_RGBA32F, width, height);
            //glTextureStorage2D(tex_base.m_Texture, 1, GL_RGBA32F, width, height);
            break;
        }
        break;
    }
}

void Renderer::CreateSkyboxCubeMap()
{
    std::vector<std::string> paths{
        "C:/dev/Astron Battles/Assets/Textures/CubeMaps/Lava/cell_2_3.png",
        "C:/dev/Astron Battles/Assets/Textures/CubeMaps/Lava/cell_2_1.png",
        "C:/dev/Astron Battles/Assets/Textures/CubeMaps/Lava/cell_3_2.png",
        "C:/dev/Astron Battles/Assets/Textures/CubeMaps/Lava/cell_1_2.png",
        "C:/dev/Astron Battles/Assets/Textures/CubeMaps/Lava/cell_2_2.png",
        "C:/dev/Astron Battles/Assets/Textures/CubeMaps/Lava/cell_2_4.png",
    };
    int width = 0;
    int height = 0;
    int channels = 0;
    int desired_channels = 4;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* right = stbi_load(paths[0].c_str(), &width, &height, &channels, desired_channels);
    stbi_uc* left = stbi_load(paths[1].c_str(), &width, &height, &channels, desired_channels);
    stbi_uc* top = stbi_load(paths[2].c_str(), &width, &height, &channels, desired_channels);
    stbi_uc* bottom = stbi_load(paths[3].c_str(), &width, &height, &channels, desired_channels);
    stbi_uc* front = stbi_load(paths[4].c_str(), &width, &height, &channels, desired_channels);
    stbi_uc* behind = stbi_load(paths[5].c_str(), &width, &height, &channels, desired_channels);

    m_SkyboxTexture.m_Height = static_cast<uint64_t>(width);
    m_SkyboxTexture.m_Width = static_cast<uint64_t>(height);

    m_SkyboxTexture.m_TextureFormat = _TextureFormat::RGBA8;
    m_SkyboxTexture.m_TextureSource = _TextureSource::GL_ATTACHMENT;
    m_SkyboxTexture.m_TextureTarget = _TextureTarget::TEXTURE_CUBE;

    CreateOpenGLTexture(m_SkyboxTexture);

    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, m_SkyboxTexture.m_Width, m_SkyboxTexture.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, right);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, m_SkyboxTexture.m_Width, m_SkyboxTexture.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, left);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, m_SkyboxTexture.m_Width, m_SkyboxTexture.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, top);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, m_SkyboxTexture.m_Width, m_SkyboxTexture.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, bottom);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, m_SkyboxTexture.m_Width, m_SkyboxTexture.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, front);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, m_SkyboxTexture.m_Width, m_SkyboxTexture.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, behind);

    stbi_image_free(right);
    stbi_image_free(left);
    stbi_image_free(top);
    stbi_image_free(bottom);
    stbi_image_free(front);
    stbi_image_free(behind);
}

void Renderer::CreateOpenGLFrameBuffer()
{
    CustomFrameBufferTexture.m_Height = static_cast<uint64_t>(1024);
    CustomFrameBufferTexture.m_Width = static_cast<uint64_t>(1024);

    CustomFrameBufferTexture.m_TextureFormat = _TextureFormat::RGBA32F;
    CustomFrameBufferTexture.m_TextureSource = _TextureSource::GL_ATTACHMENT;
    CustomFrameBufferTexture.m_TextureTarget = _TextureTarget::TEXTURE_2D;
    CreateOpenGLTexture(CustomFrameBufferTexture);

    CustomFrameBuffer.SetTexture(GL_COLOR_ATTACHMENT0, CustomFrameBufferTexture.m_Texture, 0);
    //glGenFramebuffers(1, &CustomFrameBuffer);
    //glNamedFramebufferTexture(CustomFrameBuffer, GL_COLOR_ATTACHMENT0, CustomFrameBufferTexture.m_Texture, 0);
}

const glm::vec3 Renderer::UnProjectMouse(const glm::vec2& mouse_position) const
{
    glm::vec4 clipSpacePos{ mouse_position.x, mouse_position.y, 1.0, 1.0 };
    auto invProj = glm::inverse(m_pActiveCamera->GetP());
    auto viewSpacePos = invProj * clipSpacePos;

    viewSpacePos.x /= viewSpacePos.w;
    viewSpacePos.y /= viewSpacePos.w;
    viewSpacePos.z /= viewSpacePos.w;
    viewSpacePos.w /= viewSpacePos.w;

    auto invView = glm::inverse(m_pActiveCamera->GetV());
    auto worldSpacePos = invView * viewSpacePos;

    return glm::vec3(worldSpacePos);
}

void Renderer::OnRender()
{
    GLbitfield flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    glClear(flags);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    DrawSkyboxBackground();
    UploadToOpenGL();
    glFlush();
}
void Renderer::BindScene(std::shared_ptr<Scene> scene)
{
    if (m_ActiveScene)
        m_ActiveScene.reset();
    m_ActiveScene = scene;
}

void Renderer::SetActiveCamera(std::shared_ptr<Camera> camera)
{
    m_pActiveCamera = camera;
}

void Renderer::OnUpdate(TimeStep ts)
{
}

void Renderer::CreateImage()
{
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
    vBuffer_offset = 0;
    index_offset = 0;
}

void Renderer::EndFrame()
{
}
