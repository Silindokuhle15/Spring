
#include "BaseApplication.h"
#include "Application.h"
#include "Rendering/Renderer.h"
#include "Rendering/Grid.h"
#include "Rendering/Cube.h"

Application* BaseApplication::App = nullptr;
Camera* BaseApplication::cam_ptr = nullptr;
GLFWwindow* BaseApplication::m_pWindow = nullptr;
PointLight* BaseApplication::m_PointLight = nullptr;
Renderer* BaseApplication::m_pActiveRenderer = nullptr;

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

    //start the Imgui code here
    //Initialization Code

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); // Enable ImGui IO
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(BaseApplication::m_pWindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Dark Theme lemme see 
    ImGui::StyleColorsDark();
}

void BaseApplication::Run()
{
    CreateMainWindow();

    BaseApplication::cam_ptr->OnCreate();
    BaseApplication::m_pActiveRenderer->SetUpForRendering();
    
    Scene grd_scn;

    Grid grd(8);
    grd.OnInit();

    Cube cb;

    BaseApplication::App = &cb;
    //BaseApplication::App->OnInit();

    grd_scn.AddToScence(&grd);

    bool should_open = true;

    static float cam_pos[3];
    static float light_pos[3];

    int m_Width, m_Height;

    while (!glfwWindowShouldClose(BaseApplication::m_pWindow))
    {
        glfwGetFramebufferSize(BaseApplication::m_pWindow, &m_Width, &m_Height);

        glfwPollEvents();
        
        // ImGui BeginFrame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
     
        BaseApplication::cam_ptr->Present();

        //BaseApplication::App->OnUpdate();
        //BaseApplication::App->OnRender();
        BaseApplication::m_pActiveRenderer->OnRender(&grd_scn);
        //BaseApplication::m_pActiveRenderer->OnUpdate();
        

        /*
  
        ImGui::Begin("File | Edit");
        ImGui::MenuItem("(demo menu)", NULL, false, false);
        if (ImGui::MenuItem("New")) {}
        if (ImGui::MenuItem("Open", "Ctrl+O")) {}
        if (ImGui::BeginMenu("Open Recent"))
        {
            ImGui::MenuItem("fish_hat.c");
            ImGui::MenuItem("fish_hat.inl");
            ImGui::MenuItem("fish_hat.h");

            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Save", "Ctrl+S")) {}
        if (ImGui::MenuItem("Save As..")) {}
        
        ImGui::End();


        ImGui::Begin("Stats And Performance");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Begin("Animation panel");
        ImGui::End();

        ImGui::Begin("Utilities");
        ImGui::Text("Viewing and Transformation");

        cam_pos[0] = BaseApplication::cam_ptr->GetPosition().x;
        cam_pos[1] = BaseApplication::cam_ptr->GetPosition().y;
        cam_pos[2] = BaseApplication::cam_ptr->GetPosition().z;

        ImGui::SliderFloat3("Camera Position", cam_pos, 0.0, 100.0);
        ImGui::Separator();

        ImGui::Text("Lights and Shadow");
        light_pos[0] = BaseApplication::m_PointLight->GetPosition().x;
        light_pos[1] = BaseApplication::m_PointLight->GetPosition().y;
        light_pos[2] = BaseApplication::m_PointLight->GetPosition().z;
        ImGui::SliderFloat3("Light Position",light_pos, 0.0, 100.);
        ImGui::End();

        */
        
        // ImGui EndFrame
        // Render ImGui stuff
        ImGui::Render();

        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(BaseApplication::m_pWindow);
    }
    // ImGui Clean Up Code
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(BaseApplication::m_pWindow);

    glfwTerminate();
}

void BaseApplication::AttachCamera(Camera* cam)
{
    int m_Width  = 1920;
    int m_Height = 1057;

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