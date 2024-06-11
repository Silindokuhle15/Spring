#include "BaseApplication.h"

scripting::ScriptingEngine* BaseApplication::m_LuaEngine = nullptr;
std::unique_ptr<Renderer> BaseApplication::m_pActiveRenderer = nullptr;
template<class T>
std::unique_ptr<UILayer<T>> BaseApplication::m_pUILayer = nullptr;
std::shared_ptr<Scene> BaseApplication::m_Scene = nullptr;

template<class T>
T * BaseApplication::m_Window = nullptr;

TimeStep BaseApplication::ts;
bool BaseApplication::ExitWindow = false;

using WINDOW_BASE = Win32Window;
//using WINDOW_BASE = NGLFWwindow;

void BaseApplication::Run()
{
    WINDOW_BASE AppWindow(1920, 1080, "Spring Editor");

    m_Window<WINDOW_BASE> = &AppWindow;

    m_Window<WINDOW_BASE>->SetUpForRendering();

    //scripting::ScriptingEngine LuaEngine;
    //m_LuaEngine = &LuaEngine;

    //TestFonts();

    Scene square_scn;
    square_scn.OnInit();

    
    UILayer<WINDOW_BASE> ImGui_Layer(AppWindow);
    BaseApplication::m_pUILayer<WINDOW_BASE> = std::make_unique<UILayer<WINDOW_BASE>>(ImGui_Layer);
    BaseApplication::m_pUILayer<WINDOW_BASE>->OnInit();
    
    
    
    Renderer OpenGLrenderer;
    BaseApplication::m_pActiveRenderer = std::make_unique<Renderer>(OpenGLrenderer);
    BaseApplication::m_Scene = std::make_shared<Scene>(square_scn);
    BaseApplication::m_pActiveRenderer->BindScene(m_Scene);
    
    BaseApplication::m_pActiveRenderer->SetUpForRendering(); 
    //BaseApplication::m_pActiveRenderer->EnableTesselation();
    
    BaseApplication::m_pUILayer<WINDOW_BASE>->LoadScene(BaseApplication::m_Scene);

    while (!BaseApplication::ExitWindow)
    {
        //glfwGetFramebufferSize(BaseApplication::m_pWindow, &m_Width, &m_Height);
        //BaseApplication::m_LuaEngine->Run();
        m_Window<WINDOW_BASE>->StartTimer();

        BaseApplication::m_pUILayer<WINDOW_BASE>->BeginFrame();

        BaseApplication::m_pActiveRenderer->BeginFrame();

        BaseApplication::m_pActiveRenderer->OnRender();
        BaseApplication::m_pUILayer<WINDOW_BASE>->Enable();

        m_Window<WINDOW_BASE>->EndTimer();
        m_Window<WINDOW_BASE>->OnUpdate();

        ts = m_Window<WINDOW_BASE>->ts;
 
        BaseApplication::m_pActiveRenderer->OnUpdate(ts);
        
        BaseApplication::OnUpdate<WINDOW_BASE>();

        BaseApplication::m_pUILayer<WINDOW_BASE>->OnUpdate(ts);

        BaseApplication::m_pActiveRenderer->EndFrame();

        BaseApplication::m_pUILayer<WINDOW_BASE>->EndFrame();

        m_Window<WINDOW_BASE>->SwapBuffer();
    }

    ShutDown();
}

void BaseApplication::ShutDown()
{
    m_Window<WINDOW_BASE> = nullptr;
}

/*

void BaseApplication::TestFonts()
{

    auto AtlasGeneratorFunc = [](const char* fontFilename) {
        using namespace msdfgen;

        FreetypeHandle* ft = initializeFreetype();
        if (ft) {
            FontHandle* font = loadFont(ft, "C:\\Windows\\Fonts\\arialbd.ttf");
            if (font) {
                Shape shape;
                if (loadGlyph(shape, font, 'A')) {
                    shape.normalize();
                    //                      max. angle
                    edgeColoringSimple(shape, 3.0);
                    //          output width, height
                    Bitmap<float, 3> msdf(32, 32);
                    //                           scale, translation
                    SDFTransformation t(Projection(1.0, Vector2(4.0, 4.0)), Range(4.0));
                    generateMSDF(msdf, shape, t);
                    savePng(msdf, "output.png");
                }
                destroyFont(font);
            }
            deinitializeFreetype(ft);
        }
    };

    AtlasGeneratorFunc("");
}
*/

template<class T>
void BaseApplication::OnUpdate()
{
    for (auto v : m_Window<T>->m_SceneEventQueue.m_Queue)
    {
        MouseWheel* m = nullptr;
        int delta = 0;
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
               m_Scene->m_ActiveCamera->MoveForward();
            }
            else
            {
                m_Scene->m_ActiveCamera->MoveBackward();
            }
            break;
        // KEYBOARD
        case EventID::A:
            m_Scene->m_ActiveCamera->MoveLeft();
            break;
        case EventID::D:
            m_Scene->m_ActiveCamera->MoveRight();
            break;
        case EventID::Q:
            m_Scene->m_ActiveCamera->MoveForward();
            break;
        case EventID::S:
            m_Scene->m_ActiveCamera->MoveDown();
            break;
        case EventID::W:
            m_Scene->m_ActiveCamera->MoveUp();
            break;
        case EventID::Z:
            m_Scene->m_ActiveCamera->MoveBackward();
        }
        v->Resolve();
    }
}

void BaseApplication::AttachRenderer(std::unique_ptr<Renderer> Ren)
{
    BaseApplication::m_pActiveRenderer = std::move(Ren);
}
