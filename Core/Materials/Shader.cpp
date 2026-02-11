#include "Shader.h"

Shader::Shader():
    m_Handle{0}
{
}

Shader::Shader(const ShaderResource& shader_resource)
{
    m_Handle = glCreateProgram();
    for (size_t k = 0; k < shader_resource.GetShaderInfo().size(); k++)
    {
        auto shader_type = shader_resource.GetShaderInfo()[k].shaderType;
        GLenum glShader_type{ 0 };
        switch (shader_type)
        {
            case ShaderType::COMPUTE:
                glShader_type = GL_COMPUTE_SHADER;
                break;
            case ShaderType::VERTEX:
                glShader_type = GL_VERTEX_SHADER;
                break;
            case ShaderType::PIXEL:
                glShader_type = GL_FRAGMENT_SHADER;
                break;
        }
        auto shader = glCreateShader(glShader_type);
        auto shader_source = shader_resource.GetShaderSources()[k].c_str();
        glShaderSource(shader, 1, &shader_source, NULL);
        glCompileShader(shader);

        glAttachShader(m_Handle, shader);
    }
    glLinkProgram(m_Handle);
}

uint32_t Shader::GetHandle() const
{
    return m_Handle;
}

void Shader::Bind() const
{
    glUseProgram(m_Handle);
}

Shader Shader::CreateShader(const ShaderResource& resource)
{
    return Shader(resource);
}

