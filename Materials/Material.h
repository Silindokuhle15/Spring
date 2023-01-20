#pragma once
#include "Materials/Shader.h"

class Material
{

public:

	Shader m_shader;

	unsigned int m_UniformLocation;
	void* m_Data;

	void OnInit();
	void Bind();
};

