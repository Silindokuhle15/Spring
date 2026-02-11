#include "Renderer.h"

void Renderer::SetUpForRendering()
{
    constexpr unsigned int MAX_NUM_VERTICES = 100000;

    glCreateBuffers(1, &m_VertexBuffer);
  
    unsigned int vertex_buffer_size = sizeof(Vertex) * MAX_NUM_VERTICES;
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glNamedBufferData(m_VertexBuffer, vertex_buffer_size, nullptr, GL_DYNAMIC_DRAW);

    unsigned int index_buffer_size = sizeof(unsigned int) * MAX_NUM_VERTICES;
    glCreateBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
    glNamedBufferData(m_IndexBuffer, index_buffer_size, nullptr, GL_DYNAMIC_DRAW);

    unsigned int material_buffer_size = sizeof(MTLMaterial) * 50;
    glCreateBuffers(1, &m_MaterialBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_MaterialBuffer);
    glNamedBufferData(m_MaterialBuffer, material_buffer_size, nullptr, GL_DYNAMIC_DRAW);

    unsigned int modelMatrix_buffer_size = sizeof(glm::mat4) * 10000;
    glCreateBuffers(1, &m_ModelMatrixInstanceBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ModelMatrixInstanceBuffer);
    glNamedBufferData(m_ModelMatrixInstanceBuffer, modelMatrix_buffer_size, nullptr, GL_DYNAMIC_DRAW);

    BufferLayout layout{
        {ShaderDataType::Float3, "Position"},
        {ShaderDataType::Float2, "TexCoord"},
        {ShaderDataType::Float,  "ID"},
        {ShaderDataType::Float3, "Normal"}
    };
    m_VAO.Bind();
    m_VAO.SetBufferLayout(layout);

}

void Renderer::UploadBuffer(const VertexBuffer& vertex_buffer) const
{
    glNamedBufferSubData(m_VertexBuffer, 0, vertex_buffer.Size(), vertex_buffer.m_Buffer.data());
}

void Renderer::Clear(GLbitfield flags) const
{
    glClear(flags);
}

void Renderer::Flush() const
{
    glFlush();
}

void Renderer::DrawInstanced(const RenderCommand& cmd , AssetManager& asset_manager, uint64_t instance_count) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDrawBuffer(GL_BACK);
    glUseProgram(0);
    UploadMaterialData(cmd, asset_manager);
    UploadUniformData(cmd, asset_manager);
    auto viewportX = cmd.m_Viewport[0];
    auto viewportY = cmd.m_Viewport[1];
    auto viewportW = cmd.m_Viewport[2];
    auto viewportH = cmd.m_Viewport[3];
    glViewport(viewportX, viewportY, viewportW, viewportH);
    
    if (!(cmd.m_FrameBufferHandle.m_LWORD == U64_DEF || cmd.m_FrameBufferHandle.m_LWORD == 0))
    {
        auto& frameBuffer = asset_manager.GetAsset<unsigned int>(cmd.m_FrameBufferHandle);
        auto& texture = asset_manager.GetAsset<TextureBase<GL_Texture>>(cmd.m_FrameBufferHandle);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glBindTexture(GL_TEXTURE_2D, texture.m_Texture);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
    }
    if (!(cmd.m_TextureHandle.m_LWORD == U64_DEF || cmd.m_TextureHandle.m_LWORD == 0))
    {
        auto& texture = asset_manager.GetAsset<TextureBase<GL_Texture>>(cmd.m_TextureHandle);
        glBindTextureUnit(0, texture.m_Texture);
    }
    glDrawArraysInstanced((GLenum)cmd.m_PrimitiveType, (GLint)cmd.m_IndexBufferOffset, (GLsizei)cmd.m_IndexCount, instance_count);
}

void Renderer::UploadMaterialData(const RenderCommand& cmd, AssetManager& asset_manager) const
{
    auto& shader = asset_manager.GetShader(cmd.m_ShaderHandle);
    auto& materialGroup = asset_manager.GetMaterial(cmd.m_MaterialHandle);
    auto shaderHandlePlatform = shader.GetHandle();
    shader.Bind();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_MaterialBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_MaterialBuffer);
    glShaderStorageBlockBinding(shaderHandlePlatform, 0, 0);
    
    for (auto index{0}; index < materialGroup.size(); index++)
    {
        auto& material = materialGroup[index];
        glm::vec4 KaNs{ 0 };
        glm::vec4 KdNi{ 0 };
        glm::vec4 KsD{ 0 };
        glm::vec4 KeIllum{ 0 };

        for (auto& materialComponent : material.m_Uniforms1f)
        {
            auto& name = materialComponent.first;
            auto& data = materialComponent.second;
            if (name == "Ns")
            {
                KaNs[3] = data.x;
            }
            if (name == "Ni")
            {
                KdNi[3] = data.x;
            }
            if (name == "d")
            {
                KsD[3] = data.x;
            }
            if (name == "illum")
            {
                KeIllum[3] = data.x;
            }
        }
        for(auto& materialComponent : material.m_Uniforms3f)
        {
            auto& name = materialComponent.first;
            auto& data = materialComponent.second;
            if (name == "Kd")
            {
                KaNs[0] = data.x;
                KaNs[1] = data.y;
                KaNs[2] = data.z;
            }
            if (name == "Ka")
            {
                KdNi[0] = data.x;
                KdNi[1] = data.y;
                KdNi[2] = data.z;
            }
            if (name == "Ks")
            {
                KsD[0] = data.x;
                KsD[1] = data.y;
                KsD[2] = data.z;;
            }
            if (name == "Ke")
            {
                KeIllum[0] = data.x;
                KeIllum[1] = data.y;
                KeIllum[2] = data.z;
            }
        }
        MTLMaterial tempMaterial{ KaNs, KdNi, KsD, KeIllum };
        GLint materialOffset = index * sizeof(MTLMaterial);
        glNamedBufferSubData(m_MaterialBuffer, materialOffset, sizeof(MTLMaterial), &tempMaterial);
    }
}

void Renderer::UploadUniformData(const RenderCommand& cmd, AssetManager& asset_manager) const
{
    auto& shader = asset_manager.GetShader(cmd.m_ShaderHandle);
    auto shaderHandlePlatform = shader.GetHandle();
    shader.Bind();

    for (auto& uniformFloat : cmd.m_UniformBuffer.m_FloatMap)
    {
        auto uniformLocation = glGetUniformLocation(shaderHandlePlatform, uniformFloat.first.c_str());
        glUniform1f(uniformLocation, uniformFloat.second);
    }
    for (auto& uniformFloat2 : cmd.m_UniformBuffer.m_Float2Map)
    {
        auto uniformLocation = glGetUniformLocation(shaderHandlePlatform, uniformFloat2.first.c_str());
        auto x = uniformFloat2.second.x;
        auto y = uniformFloat2.second.y;
        glUniform2f(uniformLocation, x, y);
    }
    for (auto& uniformFloat3 : cmd.m_UniformBuffer.m_Float3Map)
    {
        auto uniformLocation = glGetUniformLocation(shaderHandlePlatform, uniformFloat3.first.c_str());
        auto x = uniformFloat3.second.x;
        auto y = uniformFloat3.second.y;
        auto z = uniformFloat3.second.z;
        glUniform3f(uniformLocation, x, y, z);
    }
    for (auto& uniformFloat4 : cmd.m_UniformBuffer.m_Float4Map)
    {
        auto uniformLocation = glGetUniformLocation(shaderHandlePlatform, uniformFloat4.first.c_str());
        auto x = uniformFloat4.second.x;
        auto y = uniformFloat4.second.y;
        auto z = uniformFloat4.second.z;
        auto w = uniformFloat4.second.w;
        glUniform4f(uniformLocation, x, y, z, w);
    }
    for (auto& uniformMat4 : cmd.m_UniformBuffer.m_Mat4Map)
    {
        auto uniformLocation = glGetUniformLocation(shaderHandlePlatform, uniformMat4.first.c_str());
        auto mat = uniformMat4.second;
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat));
    }
}

void Renderer::DrawBuffer(std::vector<RenderCommand>& command_queue, VertexBuffer& vertex_buffer, AssetManager& asset_manager) const
{
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    UploadBuffer(vertex_buffer);

    for (auto& cmd : command_queue)
    {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        auto enableBits = cmd.m_EnableBits;
        if (enableBits & 0x1)
        {
            glDisable(GL_CULL_FACE);
        }
        if (enableBits & 0x10)
        {
            glDisable(GL_DEPTH_TEST);
        }
        DrawInstanced(cmd, asset_manager);
    }
}

void Renderer::DrawBufferInstanced(std::vector<RenderCommand>& command_queue, VertexBuffer& vertex_buffer, AssetManager& asset_manager)
{
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    UploadBuffer(vertex_buffer);
    m_InstanceGroups.clear();
    m_InstanceGroups.reserve(command_queue.size());
    m_InstanceGroupKeys.clear();
    m_InstanceGroupsMap.clear();

    for (auto& renderCommand : command_queue)
    {
        /*
        uint64_t indexCount = renderCommand.m_IndexCount;
        size_t index = 0;
        while (index < m_InstanceGroupKeys.size() && m_InstanceGroupKeys[index] != indexCount)
        {
            ++index;
        }
        
        if (index == m_InstanceGroupKeys.size())
        {
            m_InstanceGroupKeys.push_back(indexCount);
            m_InstanceGroups.emplace_back();
        }
        m_InstanceGroups[index].reserve(command_queue.size());
        m_InstanceGroups[index].push_back(std::move(renderCommand));
        */
        m_InstanceGroupsMap[renderCommand.m_IndexCount].push_back(
            std::move(renderCommand)
        );
    }

    for (auto& group : m_InstanceGroupsMap)
    {
        m_ModelMatrices.clear();
        m_ModelMatrices.reserve(131072);
        uint64_t glDisableBits{ 0 };
        AssetHandle shaderHandle{ 0,0 };
        AssetHandle materialHandle{ 0, 0 };
        for (auto& cmd : group.second)
        {
            shaderHandle = cmd.m_ShaderHandle;
            materialHandle = cmd.m_MaterialHandle;
            auto& uniformBuffer = cmd.m_UniformBuffer;
            auto& modelMatrix = uniformBuffer.m_Mat4Map["Model"];
            m_ModelMatrices.push_back(modelMatrix);
            glDisableBits = cmd.m_EnableBits;
        }

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        if (glDisableBits & 0x1)
        {
            glDisable(GL_CULL_FACE);
        }
        if (glDisableBits & 0x10)
        {
            glDisable(GL_DEPTH_TEST);
        }
        auto& shader = asset_manager.GetShader(shaderHandle);
        auto& materialGroup = asset_manager.GetMaterial(materialHandle);
        auto shaderHandlePlatform = shader.GetHandle();
        shader.Bind();

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ModelMatrixInstanceBuffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ModelMatrixInstanceBuffer);
        glNamedBufferSubData(m_ModelMatrixInstanceBuffer, 0, sizeof(glm::mat4) * m_ModelMatrices.size(), m_ModelMatrices.data());
        auto& front = group.second.front();
        uint64_t instanceCount = static_cast<uint64_t>(group.second.size());
        DrawInstanced(front, asset_manager, instanceCount);
    }
}

void Renderer::SetActiveCamera(std::shared_ptr<Camera> camera)
{
    m_pActiveCamera = camera;
}

void Renderer::OnUpdate(TimeStep ts)
{
    m_Ts = ts;
}

void Renderer::BeginFrame()
{
    Clear();
}

void Renderer::EndFrame()
{
    glFlush();
}
