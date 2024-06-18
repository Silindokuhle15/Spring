#pragma once
#include "GL/glew.h"
#include <string>
#include <fstream>
#include <vector>


struct ShaderInfo
{
	std::string FilePath;
	GLenum ShaderType;
};

class Shader
{
public:
	std::vector<ShaderInfo> m_Info;

private:

	// Shader Programs
	unsigned int m_Shader[2];

	// Separable Pipelines
	unsigned int m_ProgramPipeLine;
	
public:
	Shader(const std::vector<ShaderInfo> info) : m_Info{info} {}
	~Shader() { glDeleteShader(m_Shader[0]);   glDeleteShader(m_Shader[1]); } //glDeleteProgram(m_ShaderProgram); }
	
	void OnInit(unsigned int mat_id);
	void Bind();

	//unsigned int GetShaderProgram() const { return m_ShaderProgram; }

};

