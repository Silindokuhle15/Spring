#pragma once
#include "Character.h"
#include "VertexArray.h"

class Renderer
{
public:
    glm::vec2 m_MousePos;
    // Buffers
    unsigned int m_VertexBuffer;
    unsigned int m_IndexBuffer;
    GLuint index_offset = 0;
    GLuint vBuffer_offset = 0;

    // Vertex Arrays
    VertexArray m_VAO;
    GLuint basicRenderProgram = 0;
    GLuint m_SkyboxRenderingProgram = 0;
    GLuint CustomFrameBuffer = 0;
    TextureBase<GL_Texture> CustomFrameBufferTexture;
    TextureBase<GL_Texture> m_SkyboxTexture;
    std::vector<ShaderInfo> m_SkyboxShaderInfo;
    Shader m_SkyboxShader;
    Mesh m_SkyboxMesh;

    // Uniforms
    int m_LightLocation;
    int m_CameraEyeLocation;
    int m_ModelLocation;
    int m_VPLocation;
    int m_VLocation;
    int m_DeltaLocation;

    unsigned int m_CurrentIndexCount = 0;
    unsigned int m_LastIndexCount = 0;

    bool m_PrimitiveModeWireFrame = false;

    unsigned int m_IndexBufferSize = 0;

    std::shared_ptr<Scene> m_ActiveScene;

    void BeginFrame();

    void BindScene(std::shared_ptr<Scene> scene);

    void SetActiveCamera(std::shared_ptr<Camera> camera);

    void CreateImage();
    void EnableTesselation();

    void OnRender();
    void OnUpdate(TimeStep delta);

    void EndFrame();

    void SetUpForRendering();
    void UploadToOpenGL();
    void DrawSkyboxBackground();
    void CreateRenderingProgram(Shader& shader_resource, GLuint program);
    void CreateOpenGLTexture(_TextureView& view, _TextureDescription & desc, GL_Texture & tex);
    void CreateOpenGLTexture(TextureBase<GL_Texture>& tex_base);
    void CreateSkyboxCubeMap();
    void CreateOpenGLFrameBuffer();

    const glm::vec3 UnprojectMouse() const;

    Renderer():
        m_MousePos{0.0, 0.0},
        m_SkyboxShaderInfo{ 
        {"C:/dev/Silindokuhle15/Spring/Assets/Shaders/SkyBoxVertShader.glsl", GL_VERTEX_SHADER},
        {"C:/dev/Silindokuhle15/Spring/Assets/Shaders/SkyBoxFragShader.glsl", GL_FRAGMENT_SHADER}
        },
        m_SkyboxShader{m_SkyboxShaderInfo},
        m_SkyboxMesh{ "C:/dev/Silindokuhle15/Spring/Assets/Objects/Cube/Cube.obj" }
    {
        m_SkyboxMesh.OnInit();
    }
    ~Renderer() {}

private:
    std::vector<TextureBase<GL_Texture>> m_Textures;
    std::vector<glm::mat4> m_ActiveTransforms;
    std::vector<Shader> m_ActiveShaders;
    std::shared_ptr<Camera> m_pActiveCamera;
};
