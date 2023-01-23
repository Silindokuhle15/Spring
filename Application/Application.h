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
#include "Materials/Material.h"
#include "Materials/Shader.h"
#include "Materials/Texture.h"
#include <vector>
#include "Rendering/VertexArray.h"

class Application
{
public:
    std::vector<glm::vec3 > m_Positions;
    std::vector<glm::vec2 > m_TexCoords;
    std::vector<glm::vec3 > m_Normals;
    std::vector<unsigned int> m_VertexIndices;
    std::vector<unsigned int> m_TextureIndices;

    // Vertex Arrays
    VertexArray m_VAO;
    Shader m_Shader;
    Texture m_Tex;
    glm::mat4 m_Transform;
    // Uniforms
    int m_ModelLocation;

    virtual void OnInit();

    float AmbientColor[4] = { 0.5, 0.5, 0.5, 1.0 };
    volatile unsigned int m_IndexCount;

    virtual int GetModelLocation() const { return m_ModelLocation; }
    virtual unsigned int GetShaderProgram() const { return m_Shader.GetShaderProgram(); }
    virtual void SetTransform(const glm::mat4& transform) noexcept { m_Transform = transform; }
};
typedef Application Base;



