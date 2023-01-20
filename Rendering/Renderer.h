#pragma once
//#include "Engine/BaseApplication.h"
#include "GL/glew.h"
#include "Scene.h"
#include "Engine/BaseApplication.h"

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
    int m_DeltaLocation;
    int m_SamplerLocation;

    bool m_PrimitiveModeWireFrame = false;
    ShaderInfo m_Info[2] = { {"Shaders/RndVertexShader.glsl", GL_VERTEX_SHADER}, { "Shaders/RndFragmentShader.glsl", GL_FRAGMENT_SHADER } };

public:

    Material m_Material;
    Renderer() {}
    ~Renderer() {}

    void SetUpForRendering();
    void OnRender(Scene* scene);
    unsigned int m_CurrentIndexCount;
    unsigned int m_LastIndexCount;
    //void OnUpdate();
    void OnUpdate(float delta);
};


