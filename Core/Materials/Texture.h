#pragma once
#include "GL/glew.h"

#include <fstream>
#include <string>
#include <iostream>

class Texture
{
public:

	// Textures and Samplers
	unsigned int m_Tex1;
	unsigned int m_Sampler;
	unsigned int m_Sampler1;
	unsigned int m_PlayButtonTex;

	unsigned int m_Width;
	unsigned int m_Height;
	int LoadTextureFromFile(const char* file_path, unsigned int* m_Tex);

	void OnInit();
	void Bind();
	void OnUpdate();
};

