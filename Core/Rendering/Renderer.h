#pragma once
#include "Character.h"
#include "VertexArray.h"

class Renderer
{
public:
    // Buffers
    unsigned int m_VertexBuffer;
    unsigned int m_IndexBuffer;

    // Vertex Arrays
    VertexArray m_VAO;
    GLuint basicRenderProgram = 0;

    // Uniforms
    int m_LightLocation;
    int m_CameraEyeLocation;
    int m_ModelLocation;
    int m_VPLocation;
    int m_VLocation;
    int m_DeltaLocation;

    unsigned int m_CurrentIndexCount = 0;
    unsigned int m_LastIndexCount = 0;

    bool m_PrimitiveModeWireFrame = false;

    unsigned int m_IndexBufferSize = 0;

    std::shared_ptr<Scene> m_ActiveScene;

    void BeginFrame();

    void BindScene(std::shared_ptr<Scene> scene);

    void SetActiveCamera(std::shared_ptr<Camera> camera);

    void CreateImage();
    void EnableTesselation();

    void OnRender();
    void OnUpdate(TimeStep delta);

    void EndFrame();

    void SetUpForRendering();
    void UploadToOpenGL();
    void CreateRenderingProgram(Shader& shader_resource, GLuint program);
    void CreateOpenGLTexture(_TextureView& view, _TextureDescription & desc, GL_Texture & tex);
    template<typename PLATFORM>
    void CreateTexture(TextureBase<PLATFORM>& tex_base);

    Renderer() {}
    ~Renderer() {}

private:
    std::vector<TextureBase<GL_Texture>> m_Textures;
    std::vector<glm::mat4> m_ActiveTransforms;
    std::vector<Shader> m_ActiveShaders;
    std::shared_ptr<Camera> m_pActiveCamera;
};

template<typename PLATFORM>
inline void Renderer::CreateTexture(TextureBase<PLATFORM>& tex_base)
{
}
template<>
inline void Renderer::CreateTexture(TextureBase<GL_Texture>& tex_base)
{
    glCreateTextures(GL_TEXTURE_2D, 1, reinterpret_cast<GLuint*>(&tex_base.m_Texture));
    auto& tex = tex_base.m_Texture;

    GLenum format = GL_RGB8;

    GLuint width = static_cast<GLuint>(tex_base.m_Width);
    GLuint height = static_cast<GLuint>(tex_base.m_Height);

    glTextureStorage2D(tex, 1, format, width, height);

    glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto tex_data = tex_base.m_TextureData.data();
    glTextureSubImage2D(tex, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, (void*)(tex_data));
}
