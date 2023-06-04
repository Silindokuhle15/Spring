#pragma once
#include "glm.hpp"
#include "ext.hpp"
#include "Layer.h"
#include "BaseApplication.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"

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


    int m_ActiveScene;
    int m_ActiveMaterial;
    int m_ActiveUniforms;

    int m_NumberOfCamera = 1;
    float m_CameraPosition[3] = { 0.0, 0.0, 0.0 };


    float m_PointLightPosition[3] = { 0.0, 0.0, 0.0 };

    float m_LightColor[3] = { 1.0f, 1.0f, 1.0f };
    float m_SkyColor[3] = { 0.0, 0.0, 0.0 };
    float m_GroundColor[3] = { 0.0, 0.0, 0.0 };
    float m_Factor = 0.0;

    // Queries
    unsigned int m_Query;
    int m_Samples;
    int m_ResultAvailable;
public:
    
    UILayer() : Layer() {}
    UILayer(GLFWwindow* win, const char* gl_ver);
    ~UILayer();

    void Enable() override;

    virtual void OnInit() override;
    virtual void BeginFrame() override;
    virtual void EndFrame() override;
    virtual void OnUpdate(float ts) override;


    TimeStep m_Delta;
    std::shared_ptr<PerspectiveCamera> m_EditorCamera;

};

