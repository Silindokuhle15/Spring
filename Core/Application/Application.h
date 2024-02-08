#pragma once
#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include "glm/glm.hpp"
#include "Material.h"
#include <vector>
#include "VertexArray.h"
#include "TimeStep.h"

class Application
{
public:
    // Declare the Serializable Data first

    unsigned int NumVertices;
    unsigned int NumIndices;

    volatile unsigned int m_IndexCount;
    volatile float m_Speed = 1.0;
    glm::mat4 m_Transform;

    std::vector<glm::vec3 > m_Positions;
    std::vector<glm::vec2 > m_TexCoords;
    std::vector<glm::vec3 > m_Normals;
    std::vector<Vertex> m_V;
    std::vector<unsigned int> m_VertexIndices;
    std::vector<unsigned int> m_VertexIDs;
    std::vector<unsigned int> m_TextureIndices;
    std::vector<unsigned int> m_NormalIndices;

    // Fields don't necessarily need to be saved in the .scn file
    // Vertex Arrays
    VertexArray m_VAO;
    TimeStep m_Ts;
    int m_ModelLocation;

    virtual void OnInit()  = 0;
    virtual void OnUpdate(float ts) = 0;
    virtual int GetModelLocation() const { return m_ModelLocation; }
    virtual void SetTransform(const glm::mat4& transform) noexcept 
    { 
        for (auto& pos : m_Positions)
        {
            pos = glm::vec3(transform * glm::vec4(pos, 1.0f));
        }
        m_Transform = transform; 
    }

    // Now get the object moving
    virtual void MoveBackward() = 0;
    virtual void MoveForward()  = 0;
    virtual void MoveLeft()  = 0;
    virtual void MoveRight()  = 0;
    virtual void MoveUp() = 0;
    virtual void MoveDown() = 0;

};
typedef Application Base;



