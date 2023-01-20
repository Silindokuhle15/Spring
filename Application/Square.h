#pragma once
#include <fstream>
#include "stb_image.h"
#include "Engine/BaseApplication.h"
#include "Materials/Shader.h"
#include "Rendering/VertexArray.h"

class Square : public Application
{
public:
    ShaderInfo m_Info[2] = { {"Shaders/Square/Vertexshader.glsl", GL_VERTEX_SHADER}, { "Shaders/Square/Fragmentshader.glsl", GL_FRAGMENT_SHADER } };
    Shader m_Shader;
    glm::mat4 m_Transform;
    Square() : m_Shader{}
    {
    }

    void OnInit();

    unsigned int GetShaderProgram() const override { return m_Shader.GetShaderProgram(); }
};