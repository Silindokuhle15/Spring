#include "Shader.h"
#include <stdexcept>

Shader::Shader(const std::vector<ShaderType>& types, const std::vector<std::string>& sources):
    m_Handle{ Shader::InvalidProgram }
{
    if (types.size() != sources.size())
    {
        throw std::runtime_error("Shader: resource mismatch\n");
    }
    if (types.empty())
    {
        throw std::runtime_error("Shader: resource contains no stages\n");
    }
    GLuint program = glCreateProgram();
    try
    {
        for (std::size_t index = 0; index < types.size(); index++)
        {
            const auto& shaderType = types[index];
            const auto& shaderSource = sources[index];
            auto shader = Shader::CompileShaderSource(shaderType, shaderSource);
            glAttachShader(program, shader);
            glDeleteShader(shader);
        }
        Shader::LinkShaderProgram(program);
    }
    catch (...)
    {
        glDeleteProgram(program);
        throw;
    }
    m_Handle = program;
}

Shader::~Shader()
{
    if (m_Handle != Shader::InvalidProgram)
    {
        glDeleteProgram(m_Handle);
    }
}

Shader::Shader(Shader&& other) noexcept :
    m_Handle{ other.m_Handle }
{
    other.m_Handle = Shader::InvalidProgram;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    if (this != &other)
    {
        if (m_Handle != Shader::InvalidProgram)
        {
            glDeleteProgram(m_Handle);
        }
        m_Handle = other.m_Handle;
        other.m_Handle = Shader::InvalidProgram;
    }
    return *this;
}

uint32_t Shader::GetHandle() const
{
    return m_Handle;
}

void Shader::Bind() const
{
    if (m_Handle == Shader::InvalidProgram)
    {
        throw std::runtime_error("Shader: Cannot Bind Invalid Program\n");
    }
    glUseProgram(m_Handle);
}

GLenum Shader::ToGLShaderType(const ShaderType& type)
{
    switch (type)
    {
    case ShaderType::COMPUTE:
        return GL_COMPUTE_SHADER;
    case ShaderType::VERTEX:
        return GL_VERTEX_SHADER;
    case ShaderType::PIXEL:
        return GL_FRAGMENT_SHADER;
    }
    throw std::runtime_error("Unsupported Shader Type\n");
}

GLuint Shader::CompileShaderSource(const ShaderType& shader_type, const std::string& source)
{
    GLenum glShaderType = Shader::ToGLShaderType(shader_type);
    GLuint shader = glCreateShader(glShaderType);
    const auto shaderSource = source.c_str();
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);
    GLint success{ 0 };
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLint logLength{ 0 };
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(logLength, '\0');
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        glDeleteShader(shader);
        throw std::runtime_error("Shader:CompileShaderSource Failed\n" + log);
    }
    return shader;
}

void Shader::LinkShaderProgram(GLuint& program_handle)
{
    glLinkProgram(program_handle);
    GLint success{ 0 };
    glGetProgramiv(program_handle, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLint logLength{ 0 };
        glGetProgramiv(program_handle, GL_INFO_LOG_LENGTH, &logLength);
        std::string log(logLength, '\0');
        glGetProgramInfoLog(program_handle, logLength, nullptr, log.data());
        throw std::runtime_error("Shader:LinkShaderProgram Failed\n" + log);
    }
}

