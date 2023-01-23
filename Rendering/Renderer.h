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

    unsigned int m_CurrentIndexCount = 0;
    unsigned int m_LastIndexCount = 0;

    bool m_PrimitiveModeWireFrame = false;

public:

    Material m_Material;
    Renderer() {}
    ~Renderer() {}

    void SetUpForRendering();
    void OnRender(Scene* scene);
    //void OnUpdate();
    void OnUpdate(float delta);
    void EnableTesselation();
};


