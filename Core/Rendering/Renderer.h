#pragma once
#include "Character.h"
#include "VertexArray.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "RenderCommand.h"

class Renderer
{
public:
    std::vector<LayoutInfo> uniforms
    {
        {ShaderDataType::Float, "delta"},
        {ShaderDataType::Float3,"pos"},
        {ShaderDataType::Mat4,  "View"},
        {ShaderDataType::Mat4,  "Projection"}
    };
public:
    TimeStep m_Ts;
    VertexBuffer m_LargeVertexBuffer;
    unsigned int m_VertexBuffer;
    unsigned int m_IndexBuffer;

    VertexArray m_VAO;
    FrameBuffer CustomFrameBuffer;
    TextureBase<GL_Texture> CustomFrameBufferTexture;
    TextureBase<GL_Texture> m_SkyboxTexture;
    ShaderResource m_SkyboxShaderInfo;
    Shader m_SkyboxShader;
    Mesh m_SkyboxMesh;

    std::shared_ptr<Scene> m_ActiveScene;

    void BeginFrame();

    void BindScene(std::shared_ptr<Scene> scene);

    void SetActiveCamera(std::shared_ptr<Camera> camera);

    void CreateImage();
    void EnableTesselation();

    void Draw(const RenderCommand& cmd) const;
    void DrawBuffer(const std::vector<RenderCommand>& command_queue, const VertexBuffer& vertex_buffer) const;
    void OnUpdate(TimeStep delta);

    void EndFrame();

    void SetUpForRendering();
    void UploadToOpenGL();
    void UploadBuffer(const VertexBuffer& vertex_buffer) const;
    void ClearVertexBuffer();
    void UpdateVertexBuffer(std::vector<Vertex>& new_vertices);
    void LoadMeshRenderData();
    void DrawSkyboxBackground();
    void CreateOpenGLTexture(TextureBase<GL_Texture>& tex_base);
    void CreateSkyboxCubeMap();
    void CreateOpenGLFrameBuffer();
    void Clear(GLbitfield flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) const;
    void Flush() const;

    Renderer():
        m_Ts{0},
        m_SkyboxShaderInfo
        {
            {
                ShaderInfo{"C:/dev/Spring/Assets/Shaders/SkyBoxVertShader.glsl", ShaderType::VERTEX},
                ShaderInfo{"C:/dev/Spring/Assets/Shaders/SkyBoxFragShader.glsl", ShaderType::PIXEL}
            }
        },
        m_SkyboxShader{m_SkyboxShaderInfo},
        m_SkyboxMesh{ "C:/dev/Spring/Assets/Objects/Cube/Cube.obj" },
        CustomFrameBuffer{}
    {
    }
    ~Renderer() {}
    std::vector<RenderCommand> m_CommandBuffer;
private:
    std::vector<TextureBase<GL_Texture>> m_Textures;
    std::vector<glm::mat4> m_ActiveTransforms;
    std::vector<Shader> m_ActiveShaders;
    std::shared_ptr<Camera> m_pActiveCamera;
};

class RenderManager
{
public:
    
};