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


struct Configuration {
    msdf_atlas::ImageType imageType;
    msdf_atlas::ImageFormat imageFormat;
    msdf_atlas::YDirection yDirection;
    int width, height;
    double emSize;
    msdfgen::Range pxRange;
    double angleThreshold;
    double miterLimit;
    bool pxAlignOriginX, pxAlignOriginY;
    struct {
        int cellWidth, cellHeight;
        int cols, rows;
        bool fixedOriginX, fixedOriginY;
    } grid;
    void (*edgeColoring)(msdfgen::Shape&, double, unsigned long long);
    bool expensiveColoring;
    unsigned long long coloringSeed;
    msdf_atlas::GeneratorAttributes generatorAttributes;
    bool preprocessGeometry;
    bool kerning;
    int threadCount;
    const char* arteryFontFilename;
    const char* imageFilename;
    const char* jsonFilename;
    const char* csvFilename;
    const char* shadronPreviewFilename;
    const char* shadronPreviewText;
};

void BaseApplication::Run()
{
    WINDOW_BASE AppWindow(1920, 1080, "Spring Editor");

    m_Window<WINDOW_BASE> = &AppWindow;

    m_Window<WINDOW_BASE>->SetUpForRendering();

    TestFonts();

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
    ImGui_Layer.m_ActiveRenderer->SetUpForRendering();

    while (!BaseApplication::ExitWindow)
    {
        m_Window<WINDOW_BASE>->StartTimer();
        BaseApplication::m_pUILayer<WINDOW_BASE>->BeginFrame();

        BaseApplication::m_pActiveRenderer->BeginFrame();

        BaseApplication::m_pActiveRenderer->OnRender();
        BaseApplication::m_pUILayer<WINDOW_BASE>->Enable();

        m_Window<WINDOW_BASE>->EndTimer();
        m_Window<WINDOW_BASE>->OnUpdate();

        m_Window<WINDOW_BASE>->ts = 20.f / 60.0f;
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


template <typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GEN_FN>
bool makeAtlas(const std::vector<msdf_atlas::GlyphGeometry>& glyphs, const std::vector<msdf_atlas::FontGeometry>& fonts, const Configuration& config) {
    msdf_atlas::ImmediateAtlasGenerator<S, N, GEN_FN, msdf_atlas::BitmapAtlasStorage<T, N> > generator(config.width, config.height);
    generator.setAttributes(config.generatorAttributes);
    generator.setThreadCount(config.threadCount);
    generator.generate(glyphs.data(), glyphs.size());
    msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>) generator.atlasStorage();

    bool success = true;

    if (config.imageFilename) {
        if (saveImage(bitmap, config.imageFormat, config.imageFilename, config.yDirection))
            fputs("Atlas image file saved.\n", stderr);
        else {
            success = false;
            fputs("Failed to save the atlas as an image file.\n", stderr);
        }
    }
    return success;
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

                
                //msdf_atlas::FontGeometry fg;
                //msdf_atlas::GlyphGeometry gg;

                //auto result = fg.loadGlyphRange(font, 1.0, 1, 5);
                //result = gg.load(font, 1.0, 0);
                
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
                m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveUp();
            }
            else
            {
                m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveDown();
            }
            break;

        case EventID::MOUSEMOVE:
            mv = reinterpret_cast<MouseMove*>(v);
            if (mv->GetX() / 1920 < 0.5)
            {
                if (mv->GetY() / 1080 < 0.5)
                {
                    m_pUILayer<WINDOW_BASE>->m_ActiveCamera.SetCenter(glm::normalize(glm::vec3(-(1 - 2 * (mv->GetX() / 1920.0f)), -(-1 + 2 * (mv->GetY() / 1080.0f)), 1.0)));
                }
                else
                {
                    m_pUILayer<WINDOW_BASE>->m_ActiveCamera.SetCenter(glm::normalize(glm::vec3(-(1 - 2 * (mv->GetX() / 1920.0f)), -(1 - 2 * (mv->GetY() / 1080.0f)), 1.0)));
                }
            }
            else {

                if (mv->GetY() / 1080 < 0.5)
                {
                    m_pUILayer<WINDOW_BASE>->m_ActiveCamera.SetCenter(glm::normalize(glm::vec3(-(-1 + 2 * (mv->GetX() / 1920.0f)), -(-1 + 2 * (mv->GetY() / 1080.0f)), 1.0)));
                }
                else
                {
                    m_pUILayer<WINDOW_BASE>->m_ActiveCamera.SetCenter(glm::normalize(glm::vec3(-(-1 + 2 * (mv->GetX() / 1920.0f)), -(1 - 2 * (mv->GetY() / 1080.0f)), 1.0)));
                }
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
            m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveBackward();
            break;
        case EventID::S:
            m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveDown();
            break;
        case EventID::W:
            m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveUp();
            break;
        case EventID::Z:
            m_pUILayer<WINDOW_BASE>->m_ActiveCamera.MoveForward();
            break;
        case EventID::ESCAPE:
            BaseApplication::ExitWindow = true;
            break;
        }
        v->Resolve();
    }
}
