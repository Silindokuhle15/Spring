#ifndef _RENDER_COMMAND_
#define _RENDER_COMMAND_
#include <cstdint>
#include "UUID.h"
#include "UniformBuffer.h"
class RenderCommand
{
public:
    uint64_t m_EntityID;
    uint64_t m_TargetBuffer;
    uint64_t m_VertexBufferOffset;
    uint64_t m_IndexBufferOffset;
    uint64_t m_CommandSize;
    uint64_t m_IndexCount;
    uint64_t m_PrimitiveType;
    AssetHandle m_ShaderHandle;
    AssetHandle m_MaterialHandle;
    UniformBuffer m_UniformBuffer;
};

#endif
