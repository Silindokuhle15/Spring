#include "Shader.h"
#include <cassert>

Shader::Shader():
    m_Handle{0}
{
}

Shader::Shader(const std::vector<ShaderType>& types, const std::vector<std::string>& sources)
{
    assert(types.size() == sources.size());
    m_Handle = glCreateProgram();
    for (auto index = 0; index < types.size(); index++)
    {
        GLenum glShaderType{ 0 };
        switch (types[index])
        {
        case ShaderType::COMPUTE:
            glShaderType = GL_COMPUTE_SHADER;
            break;
        case ShaderType::VERTEX:
            glShaderType = GL_VERTEX_SHADER;
            break;
        case ShaderType::PIXEL:
            glShaderType = GL_FRAGMENT_SHADER;
            break;
        }
        auto shader = glCreateShader(glShaderType);
        auto shader_source = sources[index].c_str();
        glShaderSource(shader, 1, &shader_source, NULL);
        glCompileShader(shader);
        glAttachShader(m_Handle, shader);
    }
    glLinkProgram(m_Handle);
}

Shader::Shader(const ShaderResource& shader_resource)
{
    m_Handle = glCreateProgram();
    for (size_t k = 0; k < shader_resource.GetShaderInfo().size(); k++)
    {
        auto shader_type = shader_resource.GetShaderInfo()[k].shaderType;
        GLenum glShaderType{ 0 };
        switch (shader_type)
        {
            case ShaderType::COMPUTE:
                glShaderType = GL_COMPUTE_SHADER;
                break;
            case ShaderType::VERTEX:
                glShaderType = GL_VERTEX_SHADER;
                break;
            case ShaderType::PIXEL:
                glShaderType = GL_FRAGMENT_SHADER;
                break;
        }
        auto shader = glCreateShader(glShaderType);
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

