#include "BaseApplication.h"

std::shared_ptr<Renderer> BaseApplication::m_pActiveRenderer = nullptr;
std::shared_ptr<UILayer<Win32Window>> BaseApplication::m_pUILayer = nullptr;
//std::shared_ptr<Scene> BaseApplication::m_Scene = nullptr;
Scene* BaseApplication::m_Scene = nullptr;

Win32Window * BaseApplication::m_Window = nullptr;

TimeStep BaseApplication::ts;
bool BaseApplication::ExitWindow = false;

using WINDOW_BASE = Win32Window;

void BaseApplication::Run()
{
    Win32Window AppWindow(1920, 1080, "Spring Editor");

    m_Window = &AppWindow;

    m_Window->SetUpForRendering();  

    UILayer<WINDOW_BASE> ImGui_Layer(AppWindow);

    ImGui_Layer.LoadScene(std::shared_ptr<Scene>(BaseApplication::m_Scene));

    BaseApplication::m_pUILayer = std::shared_ptr<UILayer<WINDOW_BASE>>(&ImGui_Layer);
    BaseApplication::m_pUILayer->OnInit();
    
    Renderer OpenGLrenderer;
    std::shared_ptr<Renderer> pOpenGLRenderer = std::shared_ptr<Renderer>(&OpenGLrenderer);
    BaseApplication::m_pActiveRenderer = pOpenGLRenderer;
    BaseApplication::m_pActiveRenderer->BindScene(std::shared_ptr<Scene>(BaseApplication::m_Scene));

    ImGui_Layer.BindRenderer(pOpenGLRenderer);
    ImGui_Layer.m_ActiveRenderer->SetUpForRendering();
    ImGui_Layer.m_pActiveCamera->SetEye(glm::vec3(0, 2, -2));
    ImGui_Layer.m_pActiveCamera->SetCenter(glm::vec3(0, 0, 12));
    ImGui_Layer.m_pActiveCamera->OnInit();

    while (!BaseApplication::ExitWindow)
    {
        m_Window->StartTimer();
        BaseApplication::m_pUILayer->BeginFrame();

        BaseApplication::m_pActiveRenderer->BeginFrame();
        BaseApplication::m_pUILayer->Enable();
        BaseApplication::m_pUILayer->m_pActiveCamera->Present();
        BaseApplication::m_pActiveRenderer->OnRender();

        BaseApplication::m_Scene->OnUpdate(m_Window->ts);
        BaseApplication::m_pActiveRenderer->OnUpdate(m_Window->ts);
        
        BaseApplication::OnUpdate();

        BaseApplication::m_pUILayer->OnUpdate(m_Window->ts);

        BaseApplication::m_pActiveRenderer->EndFrame();

        BaseApplication::m_pUILayer->EndFrame();

        m_Window->SwapBuffer();

        m_Window->EndTimer();
        m_Window->OnUpdate();
        std::cout << "Frame Time :" << m_Window->ts << std::endl;
    }
    ShutDown();
}

void BaseApplication::ShutDown()
{
    m_Window = nullptr;
}

void BaseApplication::OnUpdate()
{
    for (auto v : m_Window->m_SceneEventQueue.m_Queue)
    {
        MouseWheel* m = nullptr;
        MouseMove* mv = nullptr;
        int64_t delta = 0;
        switch (v->m_ID)
        {
        case EventID::None:
            break;
        // MOUSE
        case EventID::L_MOUSEDOWN:
            break;
        case EventID::M_MOUSEDOWN:
            break;
        case EventID::R_MOUSEDOWN:
            break;

        case EventID::MOUSEWHEEL:
            
            m = reinterpret_cast<MouseWheel*>(v);
            delta = m->m_WheelDelta;
            if (delta > 0) 
            {
                //m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveUp();
            }
            else
            {
                //m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveDown();
            }
            break;

        case EventID::MOUSEMOVE:
            mv = reinterpret_cast<MouseMove*>(v);
            m_pActiveRenderer->m_MousePos.x = mv->GetX();
            m_pActiveRenderer->m_MousePos.y = mv->GetY();
            if (mv->GetX() / 1920 < 0.5)
            {
                if (mv->GetY() / 1080 < 0.5)
                {
                    //m_pUILayer->m_pActiveCamera->SetCenter(glm::normalize(glm::vec3(-(1 - 2 * (mv->GetX() / 1920.0f)), -(-1 + 2 * (mv->GetY() / 1080.0f)), 1.0)));
                }
                else
                {
                    //m_pUILayer->m_pActiveCamera->SetCenter(glm::normalize(glm::vec3(-(1 - 2 * (mv->GetX() / 1920.0f)), -(1 - 2 * (mv->GetY() / 1080.0f)), 1.0)));
                }
            }
            else {

                if (mv->GetY() / 1080 < 0.5)
                {
                    //m_pUILayer->m_pActiveCamera->SetCenter(glm::normalize(glm::vec3(-(-1 + 2 * (mv->GetX() / 1920.0f)), -(-1 + 2 * (mv->GetY() / 1080.0f)), 1.0)));
                }
                else
                {
                    //m_pUILayer->m_pActiveCamera->SetCenter(glm::normalize(glm::vec3(-(-1 + 2 * (mv->GetX() / 1920.0f)), -(1 - 2 * (mv->GetY() / 1080.0f)), 1.0)));
                }
            }
            break;
        // KEYBOARD
        case EventID::A:
            //m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveLeft();
            break;
        case EventID::D:
            //m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveRight();
            break;
        case EventID::Q:
            //m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveBackward();
            break;
        case EventID::S:
            //m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveDown();
            break;
        case EventID::W:
            //m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveUp();
            break;
        case EventID::Z:
            //m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveForward();
            break;
        case EventID::ESCAPE:
            BaseApplication::ExitWindow = true;
            break;
        }
        v->Resolve();
    }
}
