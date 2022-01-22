#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stdafx.h"
#include "EventHandler.h"
#include "VertexData.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>


struct ShaderInfo
{
	std::string m_Path;
	const GLint m_ShaderType;
};
struct ObjectInfo
{
	enum ObjectType
	{
		OBJ, TXT
	};
	std::string m_Path;
	const ObjectType m_ObjectType;
};

class Renderer
{
private:
	//glm::mat4 m_RotationMatrix;
	//glm::mat4 m_ScalingMatrix;
	//glm::mat4 m_TranslationMatrix;	
	//glm::mat4 m_ProjectionMatrix;	
	//glm::mat4 m_ModevViewProjectMatrix;
	GLFWwindow* window;
	int m_InitialWindowWidth, m_InitialWindowHeight;
	unsigned int m_ShaderProgram;
	unsigned int vao, ibo, vbo;
	unsigned int m_Sampler;
	unsigned int m_TextureBuffers[20];

public:
	//Renderer(): 
		//m_TranslationMatrix{ glm::translate(glm::mat4(1.0f), glm::vec3(1.0f))},
		//m_RotationMatrix{ glm::rotate(glm::mat4(1.0f), 1.0f, glm::vec3(1.0f)) },
		//m_ScalingMatrix{ glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)) }
	//{


	//}
	Renderer() = default;
	Renderer(int t_width, int t_height) :
		window{ nullptr }, m_InitialWindowWidth{ t_width }, m_InitialWindowHeight{ t_height },
		m_ShaderProgram{ 0 }, vao{ 0 }, ibo{ 0 }, vbo{ 0 }, m_Sampler{}, m_TextureBuffers{}
	{} // Constructor Done

	~Renderer()
	{
		//glDeleteSamplers(1, &this->m_Sampler);
	}

	ReturnStatus InitTextures();
	ReturnStatus RenderInit();
	//	GLFWwindow* getWindowHandle() const { return thiss->window;}
	ReturnStatus  LoadObjectFromFile(ObjectInfo& t_ObjectInfo);
	ReturnStatus LoadShadersFromFile(std::vector<ShaderInfo>& t_ShaderInfo)
	{
		for (auto& shader : t_ShaderInfo)
		{
			std::ifstream FileReader(shader.m_Path.c_str(), std::ios::in);
			if (!FileReader.is_open())
			{
				std::cerr << "Failed to open " << shader.m_Path << "\n";
				return ReturnStatus::LOAD_SHADERS_FROM_FILE_FAIL;
			}
			std::string content = "";
			std::string line = "";
			while (!FileReader.eof())
			{
				std::getline(FileReader, line);
				content.append(line + "\n");
			}

			const char* ShaderSource = content.c_str();

			GLuint l_shader = glCreateShader(shader.m_ShaderType);
			glShaderSource(l_shader, 1, &ShaderSource, &shader.m_ShaderType);
			glCompileShader(l_shader);
			glAttachShader(m_ShaderProgram, l_shader);
			content.clear();
		}
		return ReturnStatus::LOAD_SHADERS_FROM_FILE_SUCCESS;
	}
	void Run();
};

