#pragma once
#include "Dependencies/GLM/glm/glm.hpp"
#include "Dependencies/GLM/glm/ext.hpp"
#include "Layer.h"

class UILayer :
    public Layer
{
public:

    bool m_IsEnable = false;
    bool m_EnableLighting = false;
    bool m_EnableTexture = 0;

    bool m_RenderMode;
    unsigned int m_NumPrimitives;
    unsigned int m_NumIndices;

    int m_ActiveMaterial;
    int m_ActiveUniforms;
    //int m_CurrentProgram;


    float m_CameraPosition[3];

public:
    
    UILayer() : Layer() {}
    UILayer(GLFWwindow* win, const char* gl_ver);
    ~UILayer();

    void Enable() override;

    virtual void OnInit() override;
    virtual void BeginFrame() override;
    virtual void EndFrame() override;

};

