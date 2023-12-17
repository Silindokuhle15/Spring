#pragma once
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Layer.h"
#include "BaseApplication.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"

class UILayer :
    public Layer
{
public:

    unsigned int SelectedMesh;
    // Huh?
    std::shared_ptr<Scene> m_ActiveScene;

    // EDITOR CAMERA VARIABLES

    TimeStep m_Delta;

    // Queries
    unsigned int m_Query;
    int m_Samples;
    int m_ResultAvailable;

    // OBJECT POINTERS
    std::shared_ptr<PerspectiveCamera> m_EditorCamera;
    std::shared_ptr<glm::mat4> m_ActiveTransform;

    // DISPLAY PANELS
    ComponentPanel<Panel> m_ComponentPanel;
    RenderPanel<Panel> m_RenderPanel;
    StatsPanel<Panel> m_StatsPanel;
    ContentBrowser<Panel> m_ContentBrowser;


public:
    // PURE FUNCTIONS
    void Enable() override;
    void LoadScene(std::shared_ptr<Scene> scene);

    // VIRTUAL FUNCTIONS
    virtual void BeginFrame() override;
    virtual void EndFrame() override;
    virtual void OnInit() override;
    virtual void OnUpdate(float ts) override;
    // CONSTRUCTORS
    UILayer() : Layer() {}
    UILayer(GLFWwindow* win, const char* gl_ver);
    ~UILayer();
};