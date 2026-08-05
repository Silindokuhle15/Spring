#ifndef _RENDER_COMMAND_
#define _RENDER_COMMAND_
#include <cstdint>
#include "UUID.h"
#include "UniformBuffer.h"
class RenderCommand
{
public:
    uint64_t m_EnableBits;
    uint64_t m_EntityID;
    uint32_t m_TargetBuffer;
    uint32_t m_VertexBufferOffset;
    uint32_t m_IndexBufferOffset;
    uint32_t m_CommandSize;
    uint32_t m_IndexCount;
    uint32_t m_PrimitiveType;
    uint32_t m_Viewport[4];
    AssetHandle m_ShaderHandle;
    AssetHandle m_MaterialHandle;
    AssetHandle m_TextureHandle;
    AssetHandle m_FrameBufferHandle;
    UniformBuffer m_UniformBuffer;
};

class ParticleCommand
{
public:
    uint32_t m_BufferOffset;
    uint32_t m_NumParticles;
    AssetHandle m_ShaderHandle;
    AssetHandle m_TextureHandle;
    UniformBuffer m_UniformBuffer;
};

#endif
