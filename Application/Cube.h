#pragma once
#include "Application/Application.h"
#include "Engine/BaseApplication.h"
#include "Engine/ObjectLoader.h"
#include "Materials/Shader.h"
#include "Rendering/Scene.h"

class Cube :
    public Application
{
private:
    // Buffers
    unsigned int m_VertexBuffer;
    unsigned int m_IndexBuffer;
    unsigned int m_TexBuffer;
    unsigned int m_DrawBuffer;

    // Textures and Samplers
    unsigned int m_Tex1;
    unsigned int m_Sampler;

    // Vertex Array Attributes
    int m_VTexlocation;
    int m_VPOSlocation;
    int m_VNORMlocation;

    int LoadTextureFromFile(const char* file_path);
    ObjectLoader ob;

    // Light and Shadiow Uniforms
    unsigned int m_AmbientColorLocation;
    unsigned int m_DiffuseColorLocation;
    unsigned int m_SpecularColortLocation;

    unsigned int m_PointLightLocation;

    ShaderInfo m_Info[2] = { {"Shaders/Cube/Vertexshader.glsl", GL_VERTEX_SHADER}, {"Shaders/Cube/Fragmentshader.glsl", GL_FRAGMENT_SHADER} };

public:

    // Vertex Arrays
    VertexArray m_VAO;

    Shader m_Shader;
    unsigned int m_IndexCount;
    unsigned int GetIndexBuffer() const { return m_IndexBuffer; }
    unsigned int GetVertexBuffer() const { return m_VertexBuffer; }

    void OnInit();
    void OnUpdate();
    void OnRender();
    void OnDestroy();

    void LoadTexture(const char* file_path);
    void EnableTesselation();

    unsigned int GetShaderProgram() const override { return m_Shader.GetShaderProgram(); }
};