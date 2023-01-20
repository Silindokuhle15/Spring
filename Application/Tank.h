#pragma once
#include "Application/Application.h"
#include "Engine/ObjectLoader.h"
class Tank :
    public Application
{
public:
    ShaderInfo m_Info[2] = { {"Shaders/RndVertexShader.glsl", GL_VERTEX_SHADER}, { "Shaders/RndFragmentShader.glsl", GL_FRAGMENT_SHADER } };

    void OnInit() override;
};

