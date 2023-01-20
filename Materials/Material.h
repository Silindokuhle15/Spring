#pragma once
#include "Materials/Shader.h"
#include "Materials/Texture.h"

class Material
{
public:
	ShaderInfo m_Info[2] = { {"Shaders/RndVertexShader.glsl", GL_VERTEX_SHADER}, { "Shaders/RndFragmentShader.glsl", GL_FRAGMENT_SHADER } };

	Shader m_Shader;
	Texture m_Texture;
	unsigned int m_MaterialID;

	void OnInit();
	void Bind();

	void OnUpdate();
};

