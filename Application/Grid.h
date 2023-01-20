#pragma once
#include "Rendering/Scene.h"
#include <fstream>
#include "Engine/BaseApplication.h"
#include "Materials/Shader.h"
#include "Rendering/VertexArray.h"

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

    // Vertex Array Attributes
    int m_ModelLocation;

    ShaderInfo m_Info[2] = { {"Shaders/Grid/Vertexshader.glsl", GL_VERTEX_SHADER}, { "Shaders/Grid/Fragmentshader.glsl", GL_FRAGMENT_SHADER } };


private:
	unsigned int m_Rows;
	unsigned int m_Columns;

	std::vector<glm::vec3 > m_Positions;
	std::vector<glm::vec2 > m_TexCoords;
	std::vector<glm::vec3 > m_Normals;
	
   
public:

    std::vector<glm::vec3 > m_Vertices;
    std::vector<unsigned int> m_VertexIndices;

    // Vertex Arrays
    VertexArray m_VAO;
     

    // Uniforms
    int m_VPlocation;
    int m_Vlocation;

    Shader m_Shader;
    unsigned int m_IndexCount;
	Grid(unsigned int m, unsigned int n) : m_Rows{ m }, m_Columns{ n }, m_Shader{}
	{
	}
    Grid(unsigned int size) : m_Rows{ size }, m_Columns{ size }, m_Shader{} {}

    void OnInit();
    void OnUpdate();
    void OnRender();
    void OnDestroy();

    unsigned int GetShaderProgram() const override { return m_Shader.GetShaderProgram(); }
};