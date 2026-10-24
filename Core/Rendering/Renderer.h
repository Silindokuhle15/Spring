#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "Range.h"
#include "Character.h"
#include "VertexArray.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "RenderCommand.h"

namespace bufferIndices
{
    constexpr GLuint GPU_POSITION_BUFFER = 3;
    constexpr GLuint GPU_VELOCITY_BUFFER = 4;
    constexpr GLuint GPU_LIFETIME_AND_SIZE_BUFFER = 5;
    constexpr GLuint GPU_COLOR_BUFFER = 6;
    constexpr GLuint GPU_CONTROL_POINT_BUFFER = 7;
    constexpr GLuint GPU_KNOT_BUFFER = 8;
}
constexpr uint64_t U64_DEF = 14757395258967641292;

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

struct SplineBuffer
{
    uint32_t ControlPointBuffer;
    uint32_t KnotBuffer;
    glm::vec4* ControlPoints = nullptr;
    float* Knots = nullptr;
};

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
    unsigned int MAX_CONTROL_POINT_BUFFER_SIZE = 16384;
    unsigned int MAX_KNOT_BUFFER_SIZE = 32768;
    unsigned int m_VertexBuffer;
    unsigned int m_IndexBuffer;
    unsigned int m_MaterialBuffer;
    unsigned int m_ModelMatrixInstanceBuffer;
    VertexArray m_VAO;

    void BeginFrame();
    void SetActiveCamera(std::shared_ptr<Camera> camera);

    void DrawInstanced(const RenderCommand& cmd, AssetManager& asset_manager, uint32_t instance_count = 1) const;
    void DrawBuffer(std::vector<RenderCommand>& command_queue, VertexBuffer& vertex_buffer, AssetManager& asset_manager) const;
    void DrawBufferInstanced(std::vector<RenderCommand>& command_queue, VertexBuffer& vertex_buffer, AssetManager& asset_manager);
    void DrawParticles(std::vector<ParticleCommand>& command_queue, AssetManager& asset_manager) const;
    void UploadMaterialData(const RenderCommand& cmd, AssetManager& asset_manager) const;
    void UploadUniformData(const RenderCommand& cmd, AssetManager& asset_manager) const;
    void UploadUniformData(const ParticleCommand& cmd, AssetManager& asset_manager) const;
    void OnUpdate(float delta);

    void EndFrame();

    void UploadBuffer(const VertexBuffer& vertex_buffer) const;
    void Clear(GLbitfield flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) const;
    void Flush() const;

    uint32_t AllocateParticles(uint32_t num_particles);
    bool DeallocateParticles(primitives::Range range);
    void MergeFreeParticleRanges();
    void ParticleSystemSync(Scene* scene);

    uint32_t AllocateCurves(uint32_t num_curves);
    bool DeallocateCurves(primitives::Range range);
    void MergeFreeCurveRanges();

    Renderer();
    Renderer(const Renderer&) = delete;
    Renderer operator=(const Renderer&) = delete;
    ~Renderer();
    std::vector<RenderCommand> m_CommandBuffer;
private:
    uint32_t AllocateRange(uint32_t size, std::vector<primitives::Range>& allocated_ranges, std::vector<primitives::Range>& free_ranges) const;
    bool DeallocateRange(const primitives::Range& range, std::vector<primitives::Range>& allocated_ranges, std::vector<primitives::Range>& free_ranges) const;
    void MergeFreeRanges(std::vector<primitives::Range>& free_ranges) const;
    void DebugRanges(std::vector<primitives::Range>& allocated_ranges, std::vector<primitives::Range>& free_ranges) const;
private:
    void EnsureParticleAllocation(Scene* scene);
    void ReinstantiateParticleInstances(Scene* scene);
    void UpdateGPUParticleStates(Scene* scene);
private:
    std::shared_ptr<Camera> m_pActiveCamera;
    std::vector<std::vector<RenderCommand>> m_InstanceGroups;
    std::vector<int> m_InstanceGroupKeys;
    std::map<int, std::vector<RenderCommand>> m_InstanceGroupsMap;
    std::vector<glm::mat4> m_ModelMatrices;

    std::vector<primitives::Range> m_AllocatedParticleRanges;
    std::vector<primitives::Range> m_FreeParticleRanges;
    ParticleSystemBuffer m_GpuBuffers;

    std::vector<primitives::Range> m_AllocatedCurveRanges;
    std::vector<primitives::Range> m_FreeCurveRanges;
    SplineBuffer m_SplineBuffer;
};

#endif