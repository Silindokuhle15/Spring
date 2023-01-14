#pragma once
#include <fstream>
#include "Engine/BaseApplication.h"
#include "Shader.h"
#include "stb_image.h"

class Square : public Application
{
private:
    // Buffers
    unsigned int m_VertexBuffer;
    unsigned int m_IndexBuffer;
    unsigned int m_TexBuffer;

    // Textures and Samplers
    unsigned int m_Tex1;
    unsigned int m_Sampler;

    int m_SamplerLocation;

    // Vertex Array Attributes
    int m_VPOSlocation;
    int m_VTEXlocation;

    ShaderInfo m_Info[2] = { {"Shaders/Square/Vertexshader.glsl", GL_VERTEX_SHADER}, { "Shaders/Square/Fragmentshader.glsl", GL_FRAGMENT_SHADER } };

private:

    std::vector<glm::vec2 > m_Positions;
    std::vector<glm::vec2 > m_TexCoords;

public:

    std::vector<glm::vec2 > m_Vertices;
    std::vector<unsigned int> m_VertexIndices;

    // Vertex Arrays
    unsigned int m_VAO;

    // Uniforms
    int m_MVPlocation;
    int m_MVlocation;

    Shader m_Shader;
    unsigned int m_IndexCount;
    Square() : m_Shader{}
    {
    }

    void OnInit();
    void OnUpdate();
    void OnRender();
    void OnDestroy();

    unsigned int GetShaderProgram() const override { return m_Shader.GetShaderProgram(); }
};