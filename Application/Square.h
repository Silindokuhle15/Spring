#pragma once
#include "Application/Application.h"

class Square : public Application
{
public:
    glm::mat4 m_Transform;

    void OnInit();
};