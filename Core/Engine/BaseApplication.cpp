#include "BaseApplication.h"

std::shared_ptr<Renderer> BaseApplication::m_pActiveRenderer = nullptr;
std::shared_ptr<UILayer<Win32Window>> BaseApplication::m_pUILayer = nullptr;
std::shared_ptr<Scene> BaseApplication::m_Scene = nullptr;
//Scene* BaseApplication::m_Scene = nullptr;
//Win32Window * BaseApplication::m_Window = nullptr;
std::shared_ptr<Win32Window> BaseApplication::m_Window = nullptr;

TimeStep BaseApplication::ts;
bool BaseApplication::ExitWindow = false;

using WINDOW_BASE = Win32Window;

void BaseApplication::Run()
{
    m_Window = std::make_shared<Win32Window>(1920, 1080, "Spring Editor");

    BaseApplication::m_Scene = std::make_shared<Scene>("C:/dev/Silindokuhle15/Spring/Assets/Projects/Lobby.lua");
    BaseApplication::m_Scene->LoadSceneFromFile();
    BaseApplication::m_Scene->OnInit();
    BaseApplication::m_Scene->OnCreateSceneObjects();

    BaseApplication::m_pUILayer = std::make_shared<UILayer<WINDOW_BASE>>(m_Window);
    BaseApplication::m_pUILayer->LoadScene(std::shared_ptr<Scene>(BaseApplication::m_Scene));
    BaseApplication::m_pUILayer->OnInit();
    
    BaseApplication::m_pActiveRenderer = std::make_shared<Renderer>();
    BaseApplication::m_pActiveRenderer->BindScene(std::shared_ptr<Scene>(BaseApplication::m_Scene));

    BaseApplication::m_pUILayer->BindRenderer(m_pActiveRenderer);
    BaseApplication::m_pUILayer->m_ActiveRenderer->SetUpForRendering();
    BaseApplication::m_pUILayer->m_pActiveCamera->SetEye(glm::vec3(0, 2, -2));
    BaseApplication::m_pUILayer->m_pActiveCamera->SetCenter(glm::vec3(0, 0, 12));
    BaseApplication::m_pUILayer->m_pActiveCamera->OnInit();

    while (!BaseApplication::ExitWindow)
    {
        //m_Window->StartTimer();
        
        BaseApplication::m_pUILayer->BeginFrame();

        BaseApplication::m_pActiveRenderer->BeginFrame();
        BaseApplication::m_pUILayer->Enable();
        BaseApplication::m_pActiveRenderer->OnRender();

        m_Window->m_Ts = 1.0f / 6.0f;
        BaseApplication::m_Scene->OnUpdate(m_Window->m_Ts);
        BaseApplication::m_pActiveRenderer->OnUpdate(m_Window->m_Ts);
        BaseApplication::OnUpdate();
        BaseApplication::m_pUILayer->OnUpdate(m_Window->m_Ts);

        BaseApplication::m_pActiveRenderer->EndFrame();
        BaseApplication::m_pUILayer->EndFrame();

        m_Window->SwapBuffer();
        //m_Window->EndTimer();
        m_Window->OnUpdate();
    }
    ShutDown();
}

void BaseApplication::ShutDown()
{
    m_Window = nullptr;
}

void BaseApplication::OnUpdate()
{
    auto mousePosition = m_Window->GetMousePosition();
    m_pUILayer->SetMousePosition(glm::vec2(mousePosition[0], mousePosition[1]));
}
