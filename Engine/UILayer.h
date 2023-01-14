#pragma once
#include "Layer.h"

class UILayer :
    public Layer
{
private:

    bool m_IsEnable = false;
public:
    
    UILayer() : Layer() {}
    UILayer(GLFWwindow* win, const char* gl_ver);
    ~UILayer();

    void Enable() override;

    virtual void OnInit() override;
    virtual void BeginFrame() override;
    virtual void EndFrame() override;
};

