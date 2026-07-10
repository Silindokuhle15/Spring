#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "Character.h"
#include "VertexArray.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "RenderCommand.h"

struct ParticleRange
{
    uint32_t Offset;
    uint32_t Size;
};

struct ParticleSystemBuffer
{
    uint32_t LifetimeAndSizeBuffer;
    uint32_t PositionBuffer;
    uint32_t VelocityBuffer;
    uint32_t ColorBuffer;

    glm::vec4* LifetimeAndSize = nullptr;
    glm::vec4* Positions = nullptr;
    glm::vec4* Velocities = nullptr;
    glm::vec4* Color = nullptr;
};

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
    int counter;
    int counterLimit;
    float m_Ts;
    unsigned int MAX_CHARACTER_BUFFER_SIZE = 16384;
    unsigned int MAX_INSTANCEBATCH_BUFFER_SIZE = 1024;
    unsigned int MAX_MATERIALBATCH_BUFFER_SIZE = 64;
    unsigned int MAX_PARTICLE_BUFFER_SIZE = 16384;
    unsigned int m_VertexBuffer;
    unsigned int m_IndexBuffer;
    unsigned int m_MaterialBuffer;
    unsigned int m_ModelMatrixInstanceBuffer;
    VertexArray m_VAO;

    void BeginFrame();
    void SetActiveCamera(std::shared_ptr<Camera> camera);

    void DrawInstanced(const RenderCommand& cmd, AssetManager& asset_manager, uint64_t instance_count = 1) const;
    void DrawBuffer(std::vector<RenderCommand>& command_queue, VertexBuffer& vertex_buffer, AssetManager& asset_manager) const;
    void DrawBufferInstanced(std::vector<RenderCommand>& command_queue, VertexBuffer& vertex_buffer, AssetManager& asset_manager);
    void DrawParticles(std::vector<ParticleCommand>& command_queue, AssetManager& asset_manager);
    void UploadMaterialData(const RenderCommand& cmd, AssetManager& asset_manager) const;
    void UploadUniformData(const RenderCommand& cmd, AssetManager& asset_manager) const;
    void UploadUniformData(const ParticleCommand& cmd, AssetManager& asset_manager) const;
    void OnUpdate(TimeStep delta);

    void EndFrame();

    void UploadBuffer(const VertexBuffer& vertex_buffer) const;
    void Clear(GLbitfield flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) const;
    void Flush() const;

    void ParticleSystemSync(Scene * scene);
    uint32_t AllocateParticles(uint32_t num_particles);
    bool DeallocateParticles(ParticleRange range);
    void MergeFreeRanges();
    void DebugParticleRanges();

    Renderer();
    ~Renderer();
    std::vector<RenderCommand> m_CommandBuffer;

private:
    std::vector<TextureBase<GL_Texture>> m_Textures;
    std::vector<glm::mat4> m_ActiveTransforms;
    std::vector<Shader> m_ActiveShaders;
    std::shared_ptr<Camera> m_pActiveCamera;

    std::vector<std::vector<RenderCommand>> m_InstanceGroups;
    std::map<int, std::vector<RenderCommand>> m_InstanceGroupsMap;
    std::vector<int> m_InstanceGroupKeys;
    std::vector<glm::mat4> m_ModelMatrices;

    ParticleSystemBuffer m_GpuBuffers;
    std::vector<ParticleRange> m_AllocatedRanges;
    std::vector<ParticleRange> m_FreeRanges;

};

#endif