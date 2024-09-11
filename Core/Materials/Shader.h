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
public:
	Shader(const std::vector<ShaderInfo> info= {}) : m_Info{info} {}
	~Shader() {} 
	
	void OnInit();
	void Bind();
};

