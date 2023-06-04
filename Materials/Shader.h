#pragma once
#include "glew.h"
#include <string>
#include <fstream>


struct ShaderInfo
{
	const char* FilePath;
	GLenum ShaderType;
};

class Shader
{
public:
	ShaderInfo m_Info[2];

private:

	// Shader Programs
	unsigned int m_Shader[2];
	unsigned int m_ShaderProgram;

	// Separable Pipelines
	unsigned int m_ProgramPipeLine;
	
public:
	Shader() : m_Info{ {nullptr, GL_INVALID_ENUM },{ nullptr, GL_INVALID_ENUM } } {}
	Shader(const ShaderInfo info[2]) : m_Info{ info[0], info[1]} {}
	~Shader() {   glDeleteShader(m_Shader[0]);   glDeleteShader(m_Shader[1]); glDeleteProgram(m_ShaderProgram); }
	
	void OnInit(unsigned int mat_id);
	void Bind();

	unsigned int GetShaderProgram() const { return m_ShaderProgram; }

};

