#pragma once
//#include "Engine/BaseApplication.h"
#include "GL/glew.h"
#include <fstream>
#include "Scene.h"
#include "Engine/BaseApplication.h"
#include "Application/Cube.h"
#include "Application/Grid.h"

typedef enum
{
    TRIANGLE_STRIP, WIRE_FRAME
} PrimitiveMode;

class Renderer
{
public:
    // Buffers
    unsigned int m_VertexBuffer;
    unsigned int m_IndexBuffer;
    unsigned int m_TexBuffer;

    // Vertex Arrays
    VertexArray m_VAO;

    // Uniforms
    int m_VPlocation;
    int m_Vlocation;

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
    unsigned int m_DeltaLocation;

    bool m_PrimitiveModeWireFrame = false;
    ShaderInfo m_Info[2] = { {"Shaders/RndVertexShader.glsl", GL_VERTEX_SHADER}, { "Shaders/RndFragmentShader.glsl", GL_FRAGMENT_SHADER } };

public:

    Shader m_Shader;
    Renderer() : m_VTexlocation{ 0 },
        m_VPOSlocation{ 0 }, m_VNORMlocation{ 0 },
        m_Shaders{ 0 }, m_Program{ 0 }, m_ShaderPrograms{ 0 } {}
    ~Renderer() {}

    void SetUpForRendering();
    void OnRender(Scene* scene);
    unsigned int m_CurrentIndexCount;
    unsigned int m_LastIndexCount;
    void OnUpdate();
    void OnUpdate(float delta);
};


