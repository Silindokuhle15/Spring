#pragma once
#include "Scene.h"
#include <fstream>
#include "Engine/BaseApplication.h"
#include "Shader.h"

class Grid : public Application
{
private:
    // Buffers
    unsigned int m_VertexBuffer;
    unsigned int m_IndexBuffer;
    unsigned int m_TexBuffer;

    // Textures and Samplers
    unsigned int m_Tex1;
    unsigned int m_Sampler;

    // Vertex Arrays
    unsigned int m_VAO;

    // Vertex Array Attributes
    int m_VPOSlocation;

    // Uniforms
    int m_MVPlocation;
    int m_MVlocation;

    ShaderInfo m_Info[2] = { {"Shaders/Grid/Vertexshader.glsl", GL_VERTEX_SHADER}, { "Shaders/Grid/Fragmentshader.glsl", GL_FRAGMENT_SHADER } };
    Shader m_VShader;

private:
	unsigned int m_Rows;
	unsigned int m_Columns;

	std::vector<glm::vec3 > m_Vertices;
	std::vector<glm::vec3 > m_Positions;
	std::vector<glm::vec2 > m_TexCoords;
	std::vector<glm::vec3 > m_Normals;
	std::vector<unsigned int> m_VertexIndices;

public:
	Grid(unsigned int m, unsigned int n) : m_Rows{ m }, m_Columns{ n }, m_VShader{}
	{
	}
    Grid(unsigned int size) : m_Rows{ size }, m_Columns{ size }, m_VShader{} {}

    void OnInit();
    void OnUpdate();
    void OnRender();
    void OnDestroy();
};