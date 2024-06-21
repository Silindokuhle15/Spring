#pragma once
#include "GL/glew.h"
#include <fstream>
#include <string>
#include <iostream>

#include "TextureBase.h"


class GL_Texture
{
public:
	// Textures and Samplers
    std::string m_Path;
	GLuint m_Texture;
    GLuint m_TextureSlot;
	GLuint m_Sampler;
    GLuint m_SamplerUnit;

    
    GL_Texture(std::string path = "")
        :
        m_Path{ path },
        m_Texture{ 0xFFFFFFFF },m_TextureSlot{ 0xFFFFFFFF },
        m_Sampler{ 0xFFFFFFFF },m_SamplerUnit{ 0xFFFFFFFF }
    {

    }

	void OnInit()
	{
        const char* m_Source = m_Path.c_str();
        int res = LoadTextureFromFile(m_Source, &m_Texture);

        glGenSamplers(1, &m_Sampler);
        glBindSampler(m_SamplerUnit, m_Sampler);

        glSamplerParameteri(m_Sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(m_Sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void Bind(GLuint slot)
	{
        glBindTextureUnit(slot, m_Texture);
	}
	void OnUpdate() 
	{
       glUniform1i(m_Sampler, m_SamplerUnit);
	}
    int LoadTextureFromFile(const char* file_path, GLuint* m_Tex);

};