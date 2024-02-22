#include "UILayer.h"

UILayer::UILayer(GLFWwindow* win, HWND m_Hwnd, const char* gl_ver)
{
    //start the Imgui code here
    //Initialization Code
    m_ImguiBackEnd = ImGui_BeckEnd::GLFW;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); // Enable ImGui IO
    (void)io;

    // Dark Theme lemme see 

    switch (m_ImguiBackEnd)
    {
    case ImGui_BeckEnd::Win32:
        ImGui_ImplWin32_Init(&m_Hwnd);
        break;
    case ImGui_BeckEnd::GLFW:
        ImGui_ImplGlfw_InitForOpenGL(win, true);
        break;
    }
    ImGui_ImplOpenGL3_Init(gl_ver);
    ImGui::StyleColorsDark();
    //m_ActiveScene = 2;
}


UILayer::~UILayer()
{
    //ImGui Clean Up Code
    //ImGui_ImplGlfw_Shutdown();
    switch (m_ImguiBackEnd)
    {
    case ImGui_BeckEnd::Win32:
        ImGui_ImplWin32_Shutdown();
        break;
    case ImGui_BeckEnd::GLFW:
        ImGui_ImplGlfw_Shutdown();
        break;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}
void UILayer::Enable()
{
    m_ComponentPanel.Run();
    m_ContentBrowser.Run();
    m_RenderPanel.Run();
    m_StatsPanel.Run();
    m_FileMenuBar.Run();
}

void UILayer::OnInit()
{
    // TO do Add some code here bro
    //m_ActiveScene = 1;

    glGenQueries(1, &m_Query);
}

void UILayer::BeginFrame()
{
    // ImGui BeginFrame
    ImGui_ImplOpenGL3_NewFrame();

    switch (m_ImguiBackEnd)
    {
    case ImGui_BeckEnd::GLFW:
        ImGui_ImplGlfw_NewFrame();
        break;

    case ImGui_BeckEnd::Win32:
        ImGui_ImplWin32_NewFrame();
        break;
    }

    ImGui::NewFrame();
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::BeginFrame(); // ImGuizmo Begin Frame

    // Begin the Query the Samples Renders
    glBeginQuery(GL_SAMPLES_PASSED, m_Query);
}

void UILayer::EndFrame()
{
    glEndQuery(GL_SAMPLES_PASSED);
    // ImGui EndFrame
    // Render ImGui stuff
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UILayer::OnUpdate(float ts)
{
    m_Delta = ts;
    //Update the Editor camera delta right here
    m_EditorCamera->OnUpdate(m_Delta);
 }

void UILayer::LoadScene(std::shared_ptr<Scene> scene)
{
    m_ActiveScene = std::shared_ptr<Scene>(scene);
    
    SelectedMesh = 0;

   // m_ActiveTransform = std::shared_ptr<glm::mat4>(&scene->m_Objects[0]->m_Transform);
    if (!(scene->m_StaticGeometry.empty() || scene->m_DynamicGeometry.empty()))
    {
        m_ActiveTransform = std::shared_ptr<glm::mat4>(&scene->m_DynamicGeometry[SelectedMesh].m_Transform);

        m_EditorCamera = std::shared_ptr<Camera>(scene->m_ActiveCamera);
        m_ComponentPanel.m_EditorCamera = m_EditorCamera;
        m_ComponentPanel.m_ActiveTransform = m_ActiveTransform;
        m_ComponentPanel.m_ActiveScene = m_ActiveScene;
        m_RenderPanel.m_ActiveScene = m_ActiveScene;

    }
}
