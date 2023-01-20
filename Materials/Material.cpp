#include "Material.h"

void Material::OnInit()
{
	m_MaterialID = glCreateProgram();
	m_Shader.m_Info[0] = this->m_Info[0];
	m_Shader.m_Info[1] = this->m_Info[1];

	m_Shader.OnInit(m_MaterialID);
	m_Texture.OnInit();

	glProgramParameteri(m_MaterialID, GL_PROGRAM_SEPARABLE, GL_TRUE);
	glLinkProgram(m_MaterialID);
}

void Material::Bind()
{
	glUseProgram(m_MaterialID);
	//m_Shader.Bind();
	m_Texture.Bind();
}

void Material::OnUpdate()
{
	m_Texture.OnUpdate();
}
