#pragma once
//#include "Engine/BaseApplication.h"
#include "GL/glew.h"
#include <fstream>
#include "Scene.h"
#include "Engine/BaseApplication.h"

class Renderer
{
private:
    // Vertex Arrays
    unsigned int m_VAO;

    // Vertex Array Attributes
    int m_VTexlocation;
    int m_VPOSlocation;
    int m_VNORMlocation;

    // Uniforms
    int m_MVPlocation;
    int m_MVlocation;

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

public:

    Renderer() :
        m_VAO{ 0 }, m_VTexlocation{ 0 },
        m_VPOSlocation{ 0 }, m_VNORMlocation{ 0 },
        m_MVPlocation{ 0 }, m_MVlocation{ 0 }, m_Shaders{ 0 },
        m_Program{ 0 }, m_ShaderPrograms{ 0 } {}
    ~Renderer() {}

    void SetUpForRendering();
    void OnRender(Scene* scene);

    void OnUpdate();
};

