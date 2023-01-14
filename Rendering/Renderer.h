#pragma once
//#include "Engine/BaseApplication.h"
#include "GL/glew.h"
#include <fstream>
#include "Scene.h"
#include "Engine/BaseApplication.h"
#include "Cube.h"
#include "Grid.h"
class Renderer
{
private:
    // Buffers
    unsigned int m_VertexBuffer;
    unsigned int m_IndexBuffer;
    unsigned int m_TexBuffer;

    // Vertex Arrays
    unsigned int m_VAO;

    // Uniforms
    int m_MVPlocation;
    int m_MVlocation;

    // Vertex Array Attributes
    int m_VTexlocation;
    int m_VPOSlocation;
    int m_VNORMlocation;

    // Shaders 
    unsigned int m_Shaders[5];

    // Shader Programs
    unsigned int m_Program;
    unsigned int m_ShaderPrograms[5];

    // Separable Pipelines
    unsigned int m_ProgramPipeLine;

    // Light and Shadiow Uniforms
    unsigned int m_AmbientColorLocation;
    unsigned int m_DiffuseColorLocation;
    unsigned int m_SpecularColortLocation;

    unsigned int m_PointLightLocation;

    ShaderInfo m_Info[2] = { {"Shaders/Square/Vertexshader.glsl", GL_VERTEX_SHADER}, { "Shaders/Square/Fragmentshader.glsl", GL_FRAGMENT_SHADER } };

public:

    Shader m_Shader;
    Renderer() :
        m_VAO{ 0 }, m_VTexlocation{ 0 },
        m_VPOSlocation{ 0 }, m_VNORMlocation{ 0 },
        m_Shaders{ 0 }, m_Program{ 0 }, m_ShaderPrograms{ 0 } {}
    ~Renderer() {}

    void SetUpForRendering();
    void OnRender(Scene* scene);

    void OnUpdate();
};


