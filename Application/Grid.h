#pragma once
#include "Scene.h"
#include <fstream>
#include "BaseApplication.h"
#include "Shader.h"
#include "VertexArray.h"

class Grid : public Application
{
public:
    unsigned int m_Rows = 0, m_Columns = 0;

    Grid(unsigned int size) : m_Rows{ size }, m_Columns{ size } {}

    glm::mat4 m_Transform;

    void OnInit();
};