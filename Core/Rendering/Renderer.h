#pragma once
#include "OpenGLFrameBuffer.h"
#include "Scene.h"
#include "VertexArray.h"
#include "Mesh.h"

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

    std::vector<unsigned int> m_ModelLocations;
    std::vector<unsigned int> m_NormalMatrixLocations;

    // Uniforms
    int m_ModelLocation;
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
    std::vector<GLuint> m_GLSamplers;

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
    void OnUpdate(TimeStep delta);

    void EndFrame();

    void SetUpForRendering();
    void UploadToOpenGL();

    Renderer() {}
    ~Renderer() {}

private:


    std::vector<TextureBase<GL_Texture>> m_Textures;
    //std::map<const char*, int> m_ActiveUniforms;
    std::vector<glm::mat4> m_ActiveTransforms;
    std::vector<Shader> m_ActiveShaders;
};


