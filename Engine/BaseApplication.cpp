#include "BaseApplication.h"
#include "Application/Grid.h"
#include "Application/Cube.h"
#include "Application/Square.h"
#include "Application/Teaport.h"
#include "Application/Tank.h"

Camera* BaseApplication::cam_ptr = nullptr;
GLFWwindow* BaseApplication::m_pWindow = nullptr;
PointLight* BaseApplication::m_PointLight = nullptr;
Renderer* BaseApplication::m_pActiveRenderer = nullptr;
UILayer* BaseApplication::m_pUILayer = nullptr;

void BaseApplication::CreateMainWindow()
{

    if (!glfwInit())
        exit(EXIT_FAILURE);

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 450 core";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    m_pWindow = glfwCreateWindow(1920, 1057, "Simple example", NULL, NULL);
    if (!m_pWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_pWindow);

    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        std::cerr << "glfwMakeContextCurrent failed\n";
        return;
    }
    glfwSwapInterval(1);

    glfwSetErrorCallback(BaseApplication::error_callback);

    glfwSetKeyCallback(BaseApplication::m_pWindow, KeyBoard::key_callback);
    glfwSetWindowSizeCallback(BaseApplication::m_pWindow, BaseApplication::window_size_callback);

    glfwSetMouseButtonCallback(BaseApplication::m_pWindow, Mouse::mouse_button_callback);
    glfwSetScrollCallback(BaseApplication::m_pWindow, Mouse::mouse_scroll_callback);
    //glfwSetWindowCloseCallback(BaseApplication::m_pWindow, MainWindow::Window_Close_Callback);
}

void BaseApplication::Run()
{
    CreateMainWindow();

    const char* glsl_version = "#version 450 core";

    UILayer ImGui_Layer(BaseApplication::m_pWindow, glsl_version);

    BaseApplication::m_pUILayer = &ImGui_Layer;

    BaseApplication::cam_ptr->OnCreate();
    BaseApplication::m_pActiveRenderer->SetUpForRendering();
    
    Scene grd_scn;

    Grid grd(8);
    Cube cb;
  
    Teaport Uta_teaport;
    Uta_teaport.OnInit();
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(+.2f, 0.0, 0.0));
    Uta_teaport.SetTransform(scale * trans);
    grd_scn.AddToScene(&Uta_teaport);

    Tank tank;
    tank.OnInit();
    tank.SetTransform(glm::mat4(2.0f));
    //grd_scn.AddToScene(&tank);

    Square sq;
    sq.OnInit();
    sq.SetTransform(glm::translate(glm::mat4(1.0f), glm::vec3(-.2f, 0.0, 0.0)));
    //grd_scn.AddToScene(&sq);
    
    int m_Width, m_Height;

    while (!glfwWindowShouldClose(BaseApplication::m_pWindow))
    {
        float time = 0;

        glfwGetFramebufferSize(BaseApplication::m_pWindow, &m_Width, &m_Height);

        glfwPollEvents();
        
        BaseApplication::m_pUILayer->BeginFrame();

        TimeStep ts;
     
        BaseApplication::cam_ptr->Present();

        ts = (float)glfwGetTime()/1000.0;

        BaseApplication::m_pActiveRenderer->OnUpdate(ts);
        BaseApplication::m_pActiveRenderer->OnRender(&grd_scn);
        
        BaseApplication::m_pUILayer->Enable();

        BaseApplication::m_pActiveRenderer->m_PrimitiveModeWireFrame = BaseApplication::m_pUILayer->m_RenderMode;

        BaseApplication::m_pUILayer->EndFrame();

        glfwSwapBuffers(BaseApplication::m_pWindow);
    }

    BaseApplication::m_pUILayer->~UILayer();

    glfwDestroyWindow(BaseApplication::m_pWindow);

    glfwTerminate();
}

void BaseApplication::AttachCamera(Camera* cam)
{
    int m_Width  = 1920;
    int m_Height = 1080;

    cam->SetWidth(m_Width);
    cam->SetHeight(m_Height);
    cam->OnCreate();

    BaseApplication::cam_ptr = cam;
}

void BaseApplication::AddPointLight(PointLight * point_light)
{
    BaseApplication::m_PointLight = point_light;
}

void BaseApplication::AttachRenderer(Renderer* Ren)
{
    BaseApplication::m_pActiveRenderer = Ren;
}

void BaseApplication::error_callback(int error, const char* description)
{
    std::cerr << "Error : " << description << std::endl;
}

void BaseApplication::window_size_callback(GLFWwindow* window, int new_width, int new_height) { }