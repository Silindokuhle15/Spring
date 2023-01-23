#pragma once
#include "Materials/Shader.h"
#include "Materials/Texture.h"
#include "Dependencies/GLM/glm/glm.hpp"
#include "Dependencies/GLM/glm/ext.hpp"

class Material
{
public:
	ShaderInfo m_Info[2] = { {"Shaders/RndVertexShader.glsl", GL_VERTEX_SHADER}, { "Shaders/RndFragmentShader.glsl", GL_FRAGMENT_SHADER } };
	//std::string mtl_path = "Objects/Crate/Crate1.mtl";
	std::string mtl_path = "Objects/Tank/mytank.mtl";

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
};