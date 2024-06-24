#pragma once
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <iostream>

class Material
{
public:
	std::string mtl_path;
	std::string m_tex_path;

	glm::vec3 m_Ka;
	glm::vec3 m_Kd;
	glm::vec3 m_Ks;
	float m_Ni;
	float m_Ns;

	int KaLocation;
	int KdLocation;
	int KsLocation;

	Shader m_Shader;
	TextureBase<GL_Texture> m_Texture;
	unsigned int m_MaterialID;

	void OnInit();
	void Bind();

	void OnUpdate();
	void LoadMTLFile(std::string path);

	Material(TextureBase<GL_Texture>& tex_base, std::string mat_path, Shader shader) :
		m_MaterialID{0},
		m_Texture{tex_base},
		mtl_path{ mat_path },
		m_Shader{shader}
	{
	}
};