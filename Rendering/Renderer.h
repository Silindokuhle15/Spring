#pragma once
//#include "Engine/BaseApplication.h"
#include "glew.h"
#include "Scene.h"
#include "BaseApplication.h"
#include "FrameBuffer.h"

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

    // Try Out the New Framebuffer stuff
    FrameBuffer m_FrameBuffer;

    unsigned int m_IndexBufferSize = 0;

    std::shared_ptr<Scene> m_ActiveScene;

    int m_Samples;

public:

    Material m_Material;
    Renderer() {}
    ~Renderer() {}

    void SetUpForRendering();
    void OnRender();
    void BindScene(std::shared_ptr<Scene> scene);
    //void OnUpdate();
    void OnUpdate(float delta);
    void EnableTesselation();

    void BeginFrame();
    void EndFrame();

    std::vector<glm::vec3> pos;
    std::vector<glm::vec2> tex;
    std::vector<glm::vec3> nom;
    std::vector<unsigned int> ind;
};


