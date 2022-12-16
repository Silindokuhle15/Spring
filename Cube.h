#pragma once
#include "Application.h"
#include "BaseApplication.h"
#include "ObjectLoader.h"

class Cube :
    public Application
{
private:
    // Buffers
    unsigned int m_VertexBuffer;
    unsigned int m_IndexBuffer;
    unsigned int m_TexBuffer;

    // Textures and Samplers
    unsigned int m_Tex1;
    unsigned int m_Sampler;

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
    unsigned int m_VertexShader;
    unsigned int m_FragmentShader;
    unsigned int m_Shaders[5];

    // Shader Programs
    unsigned int m_Program;
    unsigned int m_ShaderPrograms[5];

    // Separable Pipelines
    unsigned int m_ProgramPipeLine;

    int LoadTextureFromFile(const char* file_path);
    ObjectLoader ob;

    // Light and Shadiow Uniforms
    unsigned int m_AmbientColorLocation;
    unsigned int m_DiffuseColorLocation;
    unsigned int m_SpecularColortLocation;

    unsigned int m_PointLightLocation;

public:
    unsigned int GetIndexBuffer() const { return m_IndexBuffer; }
    unsigned int GetVertexBuffer() const { return m_VertexBuffer; }
    unsigned int GetProgram() const { return m_Program; }

    void OnInit();
    void OnUpdate();
    void OnRender();
    void OnDestroy();

    void create_shaders();
    void LoadTexture(const char* file_path);
    void EnableTesselation();
};

