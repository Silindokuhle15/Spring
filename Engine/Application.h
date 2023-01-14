#pragma once
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include "Dependencies/imgui/imgui.h"
#include "Dependencies/imgui/backends/imgui_impl_glfw.h"
#include "Dependencies/imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include "Rendering/Shader.h"
#include <vector>

class Application
{
private:

public:

    std::vector<glm::vec3 > m_Vertices;
    std::vector<unsigned int> m_VertexIndices;

    // Vertex Arrays
    unsigned int m_VAO;

    // Uniforms
    int m_MVPlocation;
    int m_MVlocation;

    Shader m_Shader;
    unsigned int m_IndexCount;

    virtual void OnInit();
    virtual void OnUpdate();
    virtual void OnRender();
    virtual void OnDestroy();

    float AmbientColor[4] = { 0.5, 0.5, 0.5, 1.0 };

    virtual int GetMVPLocation() const { return m_MVPlocation; }
    virtual int GetMVLocation() const { return m_MVlocation; }

    virtual unsigned int GetShaderProgram() const { return m_Shader.GetShaderProgram(); }

};



