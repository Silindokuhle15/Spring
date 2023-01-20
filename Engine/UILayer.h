#pragma once
#include "Layer.h"

class UILayer :
    public Layer
{
public:

    bool m_IsEnable = false;
    bool m_RenderMode;
public:
    
    UILayer() : Layer() {}
    UILayer(GLFWwindow* win, const char* gl_ver);
    ~UILayer();

    void Enable() override;

    virtual void OnInit() override;
    virtual void BeginFrame() override;
    virtual void EndFrame() override;

};

