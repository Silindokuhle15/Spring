#pragma once
#include "Application/Application.h"
#include "Engine/ObjectLoader.h"
class Moon :
    public Application
{
public:
    glm::mat4 m_Transform;

    void OnInit() override;
};

