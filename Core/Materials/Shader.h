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
	std::vector<std::string> m_ShaderSource;

	// Shader Programs
	std::vector<uint64_t> m_Shader;

public:
	Shader(const std::vector<ShaderInfo> info) : m_Info{info} {}
	~Shader() { /*glDeleteShader(m_Shader[0]);   glDeleteShader(m_Shader[1]); */} //glDeleteProgram(m_ShaderProgram); }
	
	void OnInit(unsigned int mat_id);
	void Bind();
};

