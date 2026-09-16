#pragma once
#include "GL/glew.h"
#include "ShaderResource.h"

class Shader
{
	uint32_t m_Handle = Shader::InvalidProgram;
public:
	Shader() = default;
	Shader(const std::vector<ShaderType>& types, const std::vector<std::string>& sources);
	~Shader();
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader(Shader&& other) noexcept;
	Shader& operator=(Shader&& other) noexcept;
	uint32_t GetHandle() const;
	void Bind() const;
public:
	static constexpr GLuint InvalidProgram = 0;
	static GLenum ToGLShaderType(const ShaderType& type);
	static GLuint CompileShaderSource(const ShaderType& shader_type, const std::string& source);
	static void LinkShaderProgram(GLuint& program_handle);
};

