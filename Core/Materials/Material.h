#pragma once
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Material
{
public:
	ShaderInfo m_Info[2] = { {"C:/dev/Silindokuhle15/Spring/Assets/Shaders/RndVertexShader.glsl", GL_VERTEX_SHADER}, { "C:/dev/Silindokuhle15/Spring/Assets/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER } };
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
	Texture m_Texture;
	unsigned int m_MaterialID;

	void OnInit();
	void Bind();

	void OnUpdate();
	void LoadMTLFile(std::string path);

	Material(): 
		m_tex_path{""}, 
		mtl_path{ "C:/dev/Silindokuhle15/Spring/Assets/Objects/Tree/Lowpoly_tree_sample.mtl"}
	{
		OnInit();
	}

	Material(std::string tex_path, std::string mat_path) :
		m_tex_path{ tex_path },
		mtl_path{ mat_path }
	{
		OnInit();
	}
};