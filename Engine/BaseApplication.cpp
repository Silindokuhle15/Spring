#include "BaseApplication.h"
#include "Grid.h"
#include "Cube.h"
#include "Square.h"
#include "Teaport.h"
#include "Tank.h"
#include "Moon.h"

GLFWwindow* BaseApplication::m_pWindow = nullptr;
std::unique_ptr<Renderer> BaseApplication::m_pActiveRenderer = nullptr;
std::unique_ptr<UILayer> BaseApplication::m_pUILayer = nullptr;
std::shared_ptr<Scene> BaseApplication::m_Scene = nullptr;


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

    m_pWindow = glfwCreateWindow(1920, 1080, "Simple example", NULL, NULL);
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
    glfwSetWindowCloseCallback(BaseApplication::m_pWindow, BaseApplication::window_close_call);


}

void BaseApplication::Run()
{
    CreateMainWindow();
    
    Moon moon;
    moon.OnInit();
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
    moon.SetTransform(scale * trans);
    
   
    Teaport Uta_teaport;
    Uta_teaport.OnInit();
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
    trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.0, 0.0));
    Uta_teaport.SetTransform(scale);
   
    // Initial Trasform
    // Rotate about y axis 180 degrees
    Tank tank;
    tank.OnInit();
    tank.SetTransform(glm::rotate(glm::mat4(1.0f), glm::radians(+180.0f), glm::vec3(0.0, 1.0, 0.0)));
    
    Cube cb;
    cb.OnInit();
    cb.SetTransform(glm::translate(glm::mat4(1.0f), glm::vec3(+0.20f, 0.0, 0.0)));
 
    Square sq;
    sq.OnInit();
    sq.SetTransform(glm::translate(glm::mat4(1.0f), glm::vec3(-0.20f, 0.0, 0.0)));

    Scene square_scn;
    square_scn.AddToScene(&sq);

    Scene cube_scn;
    cube_scn.AddToScene(&cb);

    Scene tank_scn;
    tank_scn.AddToScene(&tank);

    Scene teaport_scn;
    teaport_scn.AddToScene(&Uta_teaport);

    Scene moon_scn;
    moon_scn.AddToScene(&moon);

    Scene scenes[]{square_scn, cube_scn, tank_scn, teaport_scn};

    PerspectiveCamera pCam;
    pCam.m_center = glm::vec3(0.0f);
    pCam.m_eye = glm::vec3(0.0f, 0.0, 5.0);
    pCam.m_up = glm::vec3(0.0, 1.0, 0.0);
    
    std::shared_ptr<PerspectiveCamera> cam = std::make_shared<PerspectiveCamera>(pCam);

    const char* glsl_version = "#version 450 core";
    UILayer ImGui_Layer(BaseApplication::m_pWindow, glsl_version);

    BaseApplication::m_pUILayer = std::make_unique<UILayer>(ImGui_Layer);
    BaseApplication::m_pUILayer->OnInit();

    Renderer OpenGLrenderer;
    BaseApplication::m_pActiveRenderer = std::make_unique<Renderer>(OpenGLrenderer);
    
    unsigned int active_scene = m_pUILayer->m_ActiveScene;
    BaseApplication::m_Scene = std::make_shared<Scene>(scenes[0]);
    BaseApplication::m_pActiveRenderer->BindScene(m_Scene);

    BaseApplication::m_pActiveRenderer->m_ActiveScene->AttachCamera(std::make_shared<PerspectiveCamera>(pCam));
    BaseApplication::m_pActiveRenderer->SetUpForRendering();
    

    BaseApplication::m_pUILayer->m_EditorCamera = BaseApplication::m_pActiveRenderer->m_ActiveScene->m_ActiveCamera;
    int m_Width, m_Height;

    while (!glfwWindowShouldClose(BaseApplication::m_pWindow))
    {
        float time = 0;

        glfwGetFramebufferSize(BaseApplication::m_pWindow, &m_Width, &m_Height);

        glfwPollEvents();

        int new_scene = BaseApplication::m_pUILayer->m_ActiveScene;
        if (new_scene != active_scene)
        {
            BaseApplication::m_Scene = std::make_shared<Scene>(scenes[new_scene]);
            BaseApplication::m_pActiveRenderer->m_ActiveScene->AttachCamera(std::make_shared<PerspectiveCamera>(pCam));
            BaseApplication::m_pActiveRenderer->BindScene(m_Scene);
            BaseApplication::m_pUILayer->m_EditorCamera = BaseApplication::m_pActiveRenderer->m_ActiveScene->m_ActiveCamera;
            m_Scene->Process();
        }

        BaseApplication::m_pUILayer->BeginFrame();
        BaseApplication::m_pActiveRenderer->BeginFrame();

        TimeStep ts;

        BaseApplication::m_pActiveRenderer->OnRender();

        ts = (float)glfwGetTime() / 1000.0;
        BaseApplication::m_pActiveRenderer->OnUpdate(ts);
        BaseApplication::m_pUILayer->OnUpdate(ts);
 

        BaseApplication::m_pUILayer->Enable();

        BaseApplication::m_pUILayer->m_NumIndices = BaseApplication::m_pActiveRenderer->m_LastIndexCount;
        BaseApplication::m_pUILayer->m_NumPrimitives = BaseApplication::m_pActiveRenderer->m_Samples;
        BaseApplication::m_pActiveRenderer->m_PrimitiveModeWireFrame = BaseApplication::m_pUILayer->m_RenderMode;

        BaseApplication::m_pActiveRenderer->EndFrame();

        BaseApplication::m_pUILayer->EndFrame();

        glfwSwapBuffers(BaseApplication::m_pWindow);
    }

    ShutDown();
}

void BaseApplication::ShutDown()
{
    glfwDestroyWindow(BaseApplication::m_pWindow);
    glfwTerminate();
}

void BaseApplication::AttachRenderer(std::unique_ptr<Renderer> Ren)
{
    BaseApplication::m_pActiveRenderer = std::move(Ren);
}

void BaseApplication::error_callback(int error, const char* description)
{
    std::cerr << "Error : " << description << std::endl;
}

void BaseApplication::window_size_callback(GLFWwindow* window, int new_width, int new_height) { }

void BaseApplication::window_close_call(GLFWwindow* window)
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}


void BaseApplication::MoveCameraForward()
{
    m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->MoveForward();
}

void BaseApplication::MoveCameraBackward()
{
    m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->MoveBackward();
}

void BaseApplication::MoveCameraUp()
{
    m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->MoveUp();
}

void BaseApplication::MoveCameraDown()
{
    m_pActiveRenderer->m_ActiveScene-> m_ActiveCamera->MoveDown();
}

void BaseApplication::MoveCameraLeft()
{
    m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->MoveLeft();
}

void BaseApplication::MoveCameraRight()
{
    m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->MoveRight();
}

void BaseApplication::RotateCamera(glm::vec3 rot_dir)
{
    m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->Rotate(rot_dir);
}

void BaseApplication::FocusCamera(glm::vec3 rot_dir)
{
    m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->Focus(rot_dir);
}

void BaseApplication::CreateNewScene()
{
    // Create A new Scene at set it as the active Scene??
}
