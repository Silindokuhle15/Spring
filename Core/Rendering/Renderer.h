#pragma once
#include "Character.h"
#include "VertexArray.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "UniformBuffer.h"

class RenderCommand
{
public:
    uint64_t m_EntityID;
    uint64_t m_TargetBuffer;
    uint64_t m_VertexBufferOffset;
    uint64_t m_IndexBufferOffset;
    uint64_t m_CommandSize;
    uint64_t m_IndexCount;
    //std::vector<LayoutInfo> m_Uniforms;
    AssetHandle m_ShaderHandle;
    AssetHandle m_MaterialHandle;
};

class Renderer
{
public:
    //Testing
    std::vector<LayoutInfo> uniforms
    {
        {ShaderDataType::Float, "delta"},
        {ShaderDataType::Float3,"pos"},
        {ShaderDataType::Mat4,  "View"},
        {ShaderDataType::Mat4,  "Projection"}
    };

    UniformBuffer m_UniformBuffer;
public:
    TimeStep m_Ts;
    VertexBuffer m_LargeVertexBuffer;
    unsigned int m_VertexBuffer;
    unsigned int m_IndexBuffer;
    GLuint index_offset = 0;
    GLuint vBuffer_offset = 0;

    VertexArray m_VAO;
    FrameBuffer CustomFrameBuffer;
    TextureBase<GL_Texture> CustomFrameBufferTexture;
    TextureBase<GL_Texture> m_SkyboxTexture;
    ShaderResource m_SkyboxShaderInfo;
    Shader m_SkyboxShader;
    Mesh m_SkyboxMesh;

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
    void Draw(RenderCommand& cmd);
    void OnUpdate(TimeStep delta);

    void EndFrame();

    void SetUpForRendering();
    void UploadToOpenGL();
    void BeginCommandRecording();
    void DrawSkyboxBackground();
    void CreateOpenGLTexture(TextureBase<GL_Texture>& tex_base);
    void CreateSkyboxCubeMap();
    void CreateOpenGLFrameBuffer();

    const glm::vec3 UnProjectMouse(const glm::vec2 & mouse_position) const;

    Renderer():
        m_Ts{0},
        m_SkyboxShaderInfo
        {
            {
                ShaderInfo{"C:/dev/Spring/Assets/Shaders/SkyBoxVertShader.glsl", ShaderType::VERTEX},
                ShaderInfo{"C:/dev/Spring/Assets/Shaders/SkyBoxFragShader.glsl", ShaderType::PIXEL}
            }
        },
        m_SkyboxShader{m_SkyboxShaderInfo},
        m_SkyboxMesh{ "C:/dev/Spring/Assets/Objects/Cube/Cube.obj" },
        CustomFrameBuffer{}
    {
        Material testMaterial(m_SkyboxShaderInfo);
    }
    ~Renderer() {}

    std::vector<RenderCommand> m_CommandBuffer;
private:
    std::vector<TextureBase<GL_Texture>> m_Textures;
    std::vector<glm::mat4> m_ActiveTransforms;
    std::vector<Shader> m_ActiveShaders;
    std::shared_ptr<Camera> m_pActiveCamera;
};

class RenderManager
{
public:
    
};