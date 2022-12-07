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

    // Shaders 
    unsigned int m_VertexShader;
    unsigned int m_FragmentShader;
    unsigned int m_TessControlShader;
    unsigned int m_TessEvaluationShader;

    // Shader Programs
    unsigned int m_VertexShaderProgram;
    unsigned int m_FragmentShaderProgram;
    unsigned int m_TessControlShaderProgram;
    unsigned int m_TessEvaluationShaderProgram;
    unsigned int m_Program;

    // Separable Pipelines
    unsigned int m_ProgramPipeLine;

    int LoadTextureFromFile(const char* file_path);
    ObjectLoader ob;

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

