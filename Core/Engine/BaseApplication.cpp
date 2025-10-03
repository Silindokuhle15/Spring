#include "BaseApplication.h"

void BaseApplication::Run()
{
    m_Scene->LoadSceneFromFile();
    m_Scene->OnInit();
    m_Scene->OnCreateSceneObjects();

    m_pUILayer.LoadScene(m_Scene);
    m_pUILayer.OnInit();
    
    m_pActiveRenderer = std::make_shared<Renderer>();
    m_pActiveRenderer->BindScene(m_Scene);

    m_pUILayer.BindRenderer(m_pActiveRenderer);
    m_pUILayer.m_ActiveRenderer->SetUpForRendering();
    m_pUILayer.m_pActiveCamera->SetEye(glm::vec3(0, 2, -2));
    m_pUILayer.m_pActiveCamera->SetCenter(glm::vec3(0, 0, 2));
    m_pUILayer.m_pActiveCamera->SetOrientation(glm::quat(0, -1, 0, 0));
    m_pUILayer.m_pActiveCamera->OnInit();

    while (!m_ExitWindow)
    {
        m_pUILayer.BeginFrame();
        m_pActiveRenderer->BeginFrame();

        m_pUILayer.Enable();
        m_pActiveRenderer->UploadToOpenGL();
        m_pActiveRenderer->BeginCommandRecording();
        m_pActiveRenderer->OnRender();

        m_AppWindow.OnUpdate();
        m_Scene->OnUpdate(m_AppWindow.m_Ts);
        m_pActiveRenderer->OnUpdate(m_AppWindow.m_Ts);
        m_pUILayer.OnUpdate(m_AppWindow.m_Ts);

        m_pActiveRenderer->EndFrame();
        m_pUILayer.EndFrame();

        m_AppWindow.SwapBuffer();
    }
    ShutDown();
}

void BaseApplication::ShutDown()
{
}

void BaseApplication::OnMouseMove(event::MouseMoveEvent& event)
{
}

BaseApplication::BaseApplication(uint64_t width, uint64_t height, const char* title)
    :
    m_AppWindow{width, height, title},
    m_ExitWindow{false},
    m_Scene{std::make_shared<Scene>("C:/dev/Spring/Assets/Projects/Lobby.lua")},
    m_pUILayer{std::shared_ptr<Win32Window>(&m_AppWindow)}
{
    m_AppWindow.OnEvent = &event::Dispatcher::Dispatch;
    event::Dispatcher::RegisterListener(*this);
    event::Dispatcher::RegisterListener(m_pUILayer);
}
