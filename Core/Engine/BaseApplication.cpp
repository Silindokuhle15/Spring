#include "BaseApplication.h"

using WINDOW_BASE = Win32Window;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT BaseApplication::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplWin32_WndProcHandler(m_Hwnd, uMsg, wParam, lParam);
    double mouseX{ 0.0f }, mouseY{ 0.0f };
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_MOUSEMOVE:
        mouseX = 2.0 * (static_cast<double>(GET_X_LPARAM(lParam)) / 1920.0) - 1.0;
        mouseY = 1.0 - 2.0 * (static_cast<double>(GET_Y_LPARAM(lParam)) / 1080.0);
        m_pUILayer.SetMousePosition(glm::vec2{ mouseX, mouseY });
        break;
    default:
        return DefWindowProc(m_Hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

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
        //StartTimer();
        
        m_pUILayer.BeginFrame();
        m_pActiveRenderer->BeginFrame();
        m_pUILayer.Enable();
        m_pActiveRenderer->OnRender();

        m_Ts = 1.0f / 6.0f;
        m_Scene->OnUpdate(m_Ts);
        m_pActiveRenderer->OnUpdate(m_Ts);
        OnUpdate();
        m_pUILayer.OnUpdate(m_Ts);

        m_pActiveRenderer->EndFrame();
        m_pUILayer.EndFrame();

        //EndTimer();
        SwapBuffer();
    }
    ShutDown();
}

void BaseApplication::ShutDown()
{
}

void BaseApplication::OnUpdate()
{
    MSG msg = {};
    if (PeekMessage(&msg, m_Hwnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

BaseApplication::BaseApplication(uint64_t width, uint64_t height, const char* title)
    :
    Win32Window{width, height, title},
    m_ExitWindow{false},
    m_Scene{std::make_shared<Scene>("C:/dev/Silindokuhle15/Spring/Assets/Projects/Lobby.lua")},
    m_pUILayer{std::shared_ptr<BaseApplication>(this)}
{
}
