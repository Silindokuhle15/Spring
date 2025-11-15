#include "Renderer.h"

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

}

void Renderer::UploadToOpenGL()
{
    glNamedBufferSubData(m_VertexBuffer, 0, m_LargeVertexBuffer.Size(), m_LargeVertexBuffer.m_Buffer.data());
}

void Renderer::UploadBuffer(const VertexBuffer& vertex_buffer) const
{
    glNamedBufferSubData(m_VertexBuffer, 0, vertex_buffer.Size(), vertex_buffer.m_Buffer.data());
}

void Renderer::ClearVertexBuffer()
{
    m_LargeVertexBuffer.Clear();
}

void Renderer::UpdateVertexBuffer(std::vector<Vertex>& new_vertices)
{
    m_LargeVertexBuffer.UpdateBuffer(new_vertices);
}

void Renderer::Clear(GLbitfield flags) const
{
    glClear(flags);
}

void Renderer::Flush() const
{
    glFlush();
}

void Renderer::Draw(const RenderCommand& cmd , AssetManager& asset_manager) const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(0);
    UploadMaterialData(cmd, asset_manager);
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
    }
    if (!(cmd.m_TextureHandle.m_LWORD == U64_DEF || cmd.m_TextureHandle.m_LWORD == 0))
    {
        auto& texture = asset_manager.GetAsset<TextureBase<GL_Texture>>(cmd.m_TextureHandle);
        glBindTextureUnit(0, texture.m_Texture);
    }
    glDrawArrays((GLenum)cmd.m_PrimitiveType, (GLint)cmd.m_IndexBufferOffset, (GLsizei)cmd.m_IndexCount);
}

void Renderer::UploadMaterialData(const RenderCommand& cmd, AssetManager& asset_manager) const
{
    auto& shader = asset_manager.GetShader(cmd.m_ShaderHandle);
    auto& material = asset_manager.GetMaterial(cmd.m_MaterialHandle);
    auto shaderHandlePlatform = shader.GetHandle();
    shader.Bind();
    for (auto& materialComponent : material.m_Uniforms1f)
    {
        auto& name = materialComponent.first;
        auto& data = materialComponent.second;
        GLuint location = glGetUniformLocation(shaderHandlePlatform, materialComponent.first.c_str());
        glUniform1f(location, data.x);
    }
    for (auto& materialComponent : material.m_Uniforms2f)
    {
        auto& name = materialComponent.first;
        auto& data = materialComponent.second;
        GLuint location = glGetUniformLocation(shaderHandlePlatform, materialComponent.first.c_str());
        glUniform2f(location, data.x, data.y);
    }
    for (auto& materialComponent : material.m_Uniforms3f)
    {
        auto& name = materialComponent.first;
        auto& data = materialComponent.second;
        GLuint location = glGetUniformLocation(shaderHandlePlatform, materialComponent.first.c_str());
        glUniform3f(location, data.x, data.y, data.z);
    }
    for (auto& materialComponent : material.m_Uniforms4f)
    {
        auto& name = materialComponent.first;
        auto& data = materialComponent.second;
        GLuint location = glGetUniformLocation(shaderHandlePlatform, materialComponent.first.c_str());
        glUniform4f(location, data.x, data.y, data.z, data.w);
    }

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

void Renderer::DrawBuffer(const std::vector<RenderCommand>& command_queue, const VertexBuffer& vertex_buffer, AssetManager& asset_manager) const
{
    UploadBuffer(vertex_buffer);
    for (auto& cmd : command_queue)
    {
        Draw(cmd, asset_manager);
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
