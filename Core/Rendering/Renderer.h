#pragma once
#include "Character.h"
#include "VertexArray.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "RenderCommand.h"

constexpr uint64_t U64_DEF = 14757395258967641292;
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

    void BeginFrame();
    void SetActiveCamera(std::shared_ptr<Camera> camera);

    void Draw(const RenderCommand& cmd, AssetManager& asset_manager) const;
    void DrawBuffer(const std::vector<RenderCommand>& command_queue, const VertexBuffer& vertex_buffer, AssetManager& asset_manager) const;
    void UploadMaterialData(const RenderCommand& cmd, AssetManager& asset_manager) const;
    void OnUpdate(TimeStep delta);

    void EndFrame();

    void SetUpForRendering();
    void UploadToOpenGL();
    void UploadBuffer(const VertexBuffer& vertex_buffer) const;
    void ClearVertexBuffer();
    void UpdateVertexBuffer(std::vector<Vertex>& new_vertices);
    void Clear(GLbitfield flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) const;
    void Flush() const;

    Renderer():
        m_Ts{0},
        m_VertexBuffer{0},
        m_IndexBuffer{0}
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