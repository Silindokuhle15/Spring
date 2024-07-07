#pragma once
#include "TextureBase.h"

class GL_Texture
{
public:
	// Textures and Samplers
	GLuint m_Texture;

	GL_Texture(GLuint id= 0xFFFFFFFF) : m_Texture{ id}
	{
	}
	operator GLuint() { return m_Texture; }

	void OnInit()
	{
	}

	void Bind()
	{
	}
	void OnUpdate() 
	{
	}
};