#pragma once
#include "GL/glew.h"
#include "ShaderResource.h"

class Shader
{
	uint32_t m_Handle;
public:
	Shader();
	Shader(const ShaderResource& shader_resource);
	~Shader() {} 
	uint32_t GetHandle() const;
	void Bind() const;

public:
	static Shader CreateShader(const ShaderResource& resoure);
};

