#pragma once
#include "OpenGLFrameBuffer.h"
#include "Scene.h"
#include "VertexArray.h"
#include "Mesh.h"

typedef enum
{
    TRIANGLE_STRIP, WIRE_FRAME
} PrimitiveMode;

class Renderer
{
public:

    GLenum render_mode;
    // Buffers
    unsigned int m_VertexBuffer[2];
    unsigned int m_IndexBuffer[2];
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
    OpenGLFrameBuffer m_DrawFrame;
    OpenGLFrameBuffer m_ReadFrame;

    unsigned int m_IndexBufferSize = 0;

    std::shared_ptr<Scene> m_ActiveScene;

    int m_Samples;

    //Material m_Material;

    std::vector<glm::vec3> pos;
    std::vector<glm::vec2> tex;
    std::vector<unsigned int> ids;
    std::vector<glm::vec3> nom;
    std::vector<unsigned int> ind;
    
    void BeginFrame();

    void BindScene(std::shared_ptr<Scene> scene);

    void CreateImage();
    void EnableTesselation();

    void OnRender();
    void OnUpdate(float delta);

    void EndFrame();

    void SetUpForRendering();
    void UploadToOpenGL();

    Renderer() {}
    ~Renderer() {}

private:

    std::vector<Texture> m_Textures;
    //std::map<const char*, int> m_ActiveUniforms;
    std::vector<glm::mat4> m_ActiveTransforms;
    std::vector<Shader> m_ActiveShaders;
};


