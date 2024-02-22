#include "BaseApplication.h"

GLFWwindow* BaseApplication::m_pWindow = nullptr;
Win32Window* BaseApplication::m_Win32Window = nullptr;
scripting::ScriptingEngine* BaseApplication::m_LuaEngine = nullptr;
std::unique_ptr<Renderer> BaseApplication::m_pActiveRenderer = nullptr;
std::unique_ptr<UILayer> BaseApplication::m_pUILayer = nullptr;
std::shared_ptr<Scene> BaseApplication::m_Scene = nullptr;

TimeStep BaseApplication::ts;
bool BaseApplication::ExitWindow = false;

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
    const char* glsl_version = "#version 300";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    m_pWindow = glfwCreateWindow(1920, 1080, "Spring Editor", NULL, NULL);
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

    /*

    glfwSetKeyCallback(BaseApplication::m_pWindow, KeyBoard::key_callback);
    glfwSetWindowSizeCallback(BaseApplication::m_pWindow, BaseApplication::window_size_callback);

    glfwSetMouseButtonCallback(BaseApplication::m_pWindow, Mouse::mouse_button_callback);
    glfwSetScrollCallback(BaseApplication::m_pWindow, Mouse::mouse_scroll_callback);
    glfwSetWindowCloseCallback(BaseApplication::m_pWindow, BaseApplication::window_close_call);

    */
    // START THE LUA ENGINE AFTER THE WINDOW HAS BEEN CREATED
}

void BaseApplication::Run()
{
    Win32Window AppWindow;

    m_Win32Window = &AppWindow;

    //m_Win32Window->SetUpForRendering();

    //scripting::ScriptingEngine LuaEngine;

    //m_LuaEngine = &LuaEngine;

    CreateMainWindow();
    Scene square_scn;
    square_scn.OnInit();
    
    
    const char* glsl_version = "#version 400";
    UILayer ImGui_Layer(BaseApplication::m_pWindow, AppWindow,  glsl_version);

    BaseApplication::m_pUILayer = std::make_unique<UILayer>(ImGui_Layer);
    BaseApplication::m_pUILayer->OnInit();
    
    Renderer OpenGLrenderer;
    BaseApplication::m_pActiveRenderer = std::make_unique<Renderer>(OpenGLrenderer);
    BaseApplication::m_Scene = std::make_shared<Scene>(square_scn);
    BaseApplication::m_pActiveRenderer->BindScene(m_Scene);
    
    BaseApplication::m_pActiveRenderer->SetUpForRendering(); 
    //BaseApplication::m_pActiveRenderer->EnableTesselation();
    
    int m_Width, m_Height;

    BaseApplication::m_pUILayer->LoadScene(BaseApplication::m_Scene);

    while (!BaseApplication::ExitWindow)
        //!glfwWindowShouldClose(BaseApplication::m_pWindow))
    {
        float time = 0;

        //glfwGetFramebufferSize(BaseApplication::m_pWindow, &m_Width, &m_Height);

        glfwPollEvents();
        //BaseApplication::m_LuaEngine->Run();

        BaseApplication::m_pUILayer->BeginFrame();

        BaseApplication::m_pActiveRenderer->BeginFrame();

        BaseApplication::m_pActiveRenderer->OnRender();
        BaseApplication::m_pUILayer->Enable();

        ts = (float)glfwGetTime() / 1000.0;

        //float now  = m_Win32Window->milliseconds_now();

        //ts = (now - time) / 1000000.0f;

        //m_Win32Window->OnUpdate();
 
        BaseApplication::m_pActiveRenderer->OnUpdate(ts);
        
        BaseApplication::OnUpdate();

        BaseApplication::m_pUILayer->OnUpdate(ts);

        BaseApplication::m_pActiveRenderer->EndFrame();

        BaseApplication::m_pUILayer->EndFrame();

        glfwSwapBuffers(BaseApplication::m_pWindow);

        //m_Win32Window->SwapBuffer();
    }

    ShutDown();
}

void BaseApplication::ShutDown()
{
    glfwDestroyWindow(BaseApplication::m_pWindow);
    glfwTerminate();
}

void BaseApplication::OnUpdate()
{
    for (auto & v : m_Win32Window->m_SceneEventQueue.m_Queue)
    {
        switch (v)
        {
        case EventID::None:
            break;
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
        v.Resolve();
    }
}

void BaseApplication::AttachRenderer(std::unique_ptr<Renderer> Ren)
{
    BaseApplication::m_pActiveRenderer = std::move(Ren);
}

void BaseApplication::error_callback(int error, const char* description)
{
    std::cerr << "Error : " << description << std::endl;
    //exit(EXIT_FAILURE);
}

void BaseApplication::window_size_callback(GLFWwindow* window, int new_width, int new_height) { }

void BaseApplication::window_close_call(GLFWwindow* window)
{
    //m_pUILayer = nullptr;
    BaseApplication::ExitWindow = true;
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
    //m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->Rotate(rot_dir);
}

void BaseApplication::FocusCamera(glm::vec3 rot_dir)
{
    //m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->Focus(rot_dir);
}

void BaseApplication::CreateNewScene()
{
    // Create A new Scene at set it as the active Scene??
}
