#pragma once
#include "TextureBase.h"

/*
class GL_Texture
{
public:
	// Textures and Samplers
	GLuint m_Texture;

	GL_Texture(GLuint id= 0xFFFFFFFF) : m_Texture{ id}
	{
	}
	GLuint operator() (){ return m_Texture; }
	operator GLuint() { return m_Texture; }
	operator GLuint*() { return &m_Texture; }

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
*/
using GL_Texture = GLuint;