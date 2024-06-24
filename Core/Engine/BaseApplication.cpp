#include "BaseApplication.h"

std::shared_ptr<Renderer> BaseApplication::m_pActiveRenderer = nullptr;
template<class T>
std::shared_ptr<UILayer<T>> BaseApplication::m_pUILayer = nullptr;
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

    //TestFonts();

    std::string path = "C:/dev/Silindokuhle15/Spring/Assets/Projects/Lobby.lua";
    Scene square_scn(path);
    std::shared_ptr<Scene> pscene = std::make_shared<Scene>(square_scn);
    
    UILayer<WINDOW_BASE> ImGui_Layer(AppWindow);

    ImGui_Layer.LoadSceneFromFile(path);
    pscene->OnInit();
    ImGui_Layer.LoadScene(pscene);
    ImGui_Layer.CreateSceneObjects();

    BaseApplication::m_pUILayer<WINDOW_BASE> = std::shared_ptr<UILayer<WINDOW_BASE>>(&ImGui_Layer);
    BaseApplication::m_pUILayer<WINDOW_BASE>->OnInit();
    
    BaseApplication::m_Scene = pscene;
    Renderer OpenGLrenderer;
    std::shared_ptr<Renderer> pOpenGLRenderer = std::shared_ptr<Renderer>(&OpenGLrenderer);
    BaseApplication::m_pActiveRenderer = pOpenGLRenderer;
    BaseApplication::m_pActiveRenderer->BindScene(pscene);

    ImGui_Layer.BindRenderer(pOpenGLRenderer);
    //BaseApplication::m_pUILayer<WINDOW_BASE>->BindRenderer(pOpenGLRenderer);
    //BaseApplication::m_pUILayer<WINDOW_BASE>->LoadScene(BaseApplication::m_Scene);
    BaseApplication::m_pActiveRenderer->SetUpForRendering();

    while (!BaseApplication::ExitWindow)
    {
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
                    //savePng(msdf, "output.png");
                }
                destroyFont(font);
            }
            deinitializeFreetype(ft);
        }
    };

    AtlasGeneratorFunc("");
}


template<class T>
void BaseApplication::OnUpdate()
{
    for (auto v : m_Window<T>->m_SceneEventQueue.m_Queue)
    {
        MouseWheel* m = nullptr;
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
                m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveForward();
            }
            else
            {
                m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveBackward();
            }
            break;
        // KEYBOARD
        case EventID::A:
            m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveLeft();
            break;
        case EventID::D:
            m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveRight();
            break;
        case EventID::Q:
            m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveForward();
            break;
        case EventID::S:
            m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveDown();
            break;
        case EventID::W:
            m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveUp();
            break;
        case EventID::Z:
            m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveBackward();
        }
        v->Resolve();
    }
}
