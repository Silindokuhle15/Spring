#pragma once
#include "OpenGLFrameBuffer.h"
#include "Scene.h"
#include "VertexArray.h"
#include "Mesh.h"
#include <typeinfo>

class Renderer
{
public:

    GLenum render_mode;
    // Buffers
    unsigned int m_VertexBuffer[2];
    unsigned int m_IndexBuffer[2];
    unsigned int m_TexBuffer;

    // Vertex Arrays
    VertexArray m_VAO;

    std::vector<unsigned int> m_ModelLocations;
    std::vector<unsigned int> m_NormalMatrixLocations;

    // Uniforms
    int m_ModelLocation;
    int m_VPlocation;
    int m_Vlocation;
    int m_DeltaLocation;

    unsigned int m_CurrentIndexCount = 0;
    unsigned int m_LastIndexCount = 0;

    bool m_PrimitiveModeWireFrame = false;

    // Try Out the New Framebuffer stuff
    OpenGLFrameBuffer m_DrawFrame;
    OpenGLFrameBuffer m_ReadFrame;

    unsigned int m_IndexBufferSize = 0;

    std::shared_ptr<Scene> m_ActiveScene;

    int m_Samples;
    std::vector<GLuint> m_GLSamplers;
    void* m_pTextureBuffer = nullptr;
    void BeginFrame();

    void BindScene(std::shared_ptr<Scene> scene);

    void CreateImage();
    void EnableTesselation();

    void OnRender();
    void OnUpdate(TimeStep delta);

    void EndFrame();

    void SetUpForRendering();
    void UploadToOpenGL();
    void CreateOpenGLTexture(_TextureView& view, _TextureDescription & desc, GL_Texture & tex);
    template<typename PLATFORM>
    void CreateTexture(TextureBase<PLATFORM>& tex_base);

    Renderer() {}
    ~Renderer() {}

private:


    std::vector<TextureBase<GL_Texture>> m_Textures;
    std::vector<glm::mat4> m_ActiveTransforms;
    std::vector<Shader> m_ActiveShaders;
};

template<typename PLATFORM>
inline void Renderer::CreateTexture(TextureBase<PLATFORM>& tex_base)
{
    const std::type_info& info = typeid(tex_base);
    std::string type_name = info.name();
    if(type_name.compare("class GL_Texture") == 0)
    { 
        CreateTexture<GL_Texture>(tex_base);
    }
}
template<>
inline void Renderer::CreateTexture(TextureBase<GL_Texture>& tex_base)
{
    glCreateTextures(GL_TEXTURE_2D, 1, reinterpret_cast<GLuint*>(&tex_base.m_Texture));
    auto& tex = tex_base.m_Texture;

    GLenum format = GL_RGB8;

    auto& tex_data = tex_base.m_TextureData;
    GLuint width = static_cast<GLuint>(tex_base.m_Width);
    GLuint height = static_cast<GLuint>(tex_base.m_Height);
    auto data = tex_data.c_str();
    glTextureStorage2D(tex, 1, format, width, height);

    glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureSubImage2D(tex, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, (void*)(data));
}
