#pragma once
#include "Panel.h"
#include "Win32Window.h"
#include "NGLFWwindow.h"
#include <typeinfo>
#include "Renderer.h"

#define IMPORT_FROM_EDITBOX 1003

enum class ImGui_BeckEnd
{
    None, Win32, GLFW
};
template<class WindowBase>
class UILayer :
    public Layer
{
public:
    ImGui_BeckEnd m_ImguiBackEnd;
    unsigned int SelectedMesh;
    // Huh?
    std::shared_ptr<WindowBase> m_ParentWindow;
    std::shared_ptr<Scene> m_ActiveScene;
    std::shared_ptr<Renderer> m_ActiveRenderer;
    std::shared_ptr<glm::mat4> m_pActiveTransform;
    std::shared_ptr<Camera> m_pActiveCamera;

    // EDITOR CAMERA VARIABLES

    // Queries
    unsigned int m_Query;
    int m_Samples;
    int m_ResultAvailable;

    TimeStep m_Delta{ 0.0f };

    // OBJECT POINTERS

    // DISPLAY PANELS
    ComponentPanel<UILayer> m_ComponentPanel;
    MenuBar<UILayer> m_FileMenuBar;
    RenderPanel m_RenderPanel;
    StatsPanel m_StatsPanel;
    ContentBrowser m_ContentBrowser;


public:
    // PURE FUNCTIONS
    void Enable() override
    {
        m_ComponentPanel.Run();
        m_ContentBrowser.Run();
        m_RenderPanel.Run();
        m_StatsPanel.Run();
        m_FileMenuBar.Run();
    }

    void LoadSceneFromFile(std::string& path)
    {
        //Layer::LoadSceneFromFile(path);
    }

    void CreateSceneObjects()
    {
    }

    void BindRenderer(std::shared_ptr<Renderer> renderer)
    {
        m_ActiveRenderer = renderer;
        renderer->SetActiveCamera(m_pActiveCamera);
    }
    void LoadScene(std::shared_ptr<Scene> scene)
    {
        m_ActiveScene = scene;

        m_ComponentPanel.m_pEditorCamera = m_pActiveCamera;
        if (m_ComponentPanel.m_ActiveTransform)
            m_ComponentPanel.m_ActiveTransform.reset();
        m_ComponentPanel.m_ActiveTransform = m_pActiveTransform;
        if (m_ComponentPanel.m_ActiveScene)
            m_ComponentPanel.m_ActiveScene.reset();
        m_ComponentPanel.m_ActiveScene = m_ActiveScene;
        if (m_RenderPanel.m_ActiveScene)
            m_RenderPanel.m_ActiveScene.reset();
        m_RenderPanel.m_ActiveScene = m_ActiveScene;
    }

    // VIRTUAL FUNCTIONS
    virtual void BeginFrame() override
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
    virtual void EndFrame() override
    {
        glEndQuery(GL_SAMPLES_PASSED);
        // ImGui EndFrame
        // Render ImGui stuff
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    virtual void OnInit() override
    {
        m_pActiveCamera->OnInit();
    }
    virtual void OnUpdate(TimeStep ts) override
    {
        m_Delta = ts;
        //Update the Editor camera delta right here
        //m_pActiveCamera->Present();
        //m_pActiveCamera->OnUpdate(m_Delta);
        
    }

    std::shared_ptr<Camera>& GetLayerCamera()
    {
        return m_pActiveCamera;
    }
    std::string GetFileName(const char* filter)
    {
        auto temp_parent = m_ParentWindow.get();
        auto win32_window = reinterpret_cast<Win32Window*>(temp_parent);

        OPENFILENAME ofn;
        ZeroMemory(&ofn, sizeof(ofn));

        char szFile[_MAX_PATH];  // buffer for file name
        ZeroMemory(szFile, sizeof(szFile));

        // Initialize OPENFILENAME
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = win32_window->m_Hwnd;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.lpstrTitle = "Select the file to import from ... (use the file type filter)";
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // get a description of all readers registered in the FBX SDK
        ofn.lpstrFilter = filter;

        // Display the Open dialog box. 
        if (GetOpenFileName(&ofn) == false)
        {
            // user cancel
            //delete filter;
            return "";
        }
        // show the file name selected
        SetWindowText(GetDlgItem(win32_window->m_Hwnd, IMPORT_FROM_EDITBOX), szFile);

        return std::string(szFile);
        // Keep a copy of the file name
        //FBXSDK_strcpy(gszInputFile, _MAX_PATH, szFile);
    }
    // CONSTRUCTORS

    UILayer(WindowBase window) :
        m_ParentWindow{std::make_shared<WindowBase>(window)},
        m_pActiveCamera{new Camera{ 1920, 1080, 0.1f, 1000.0f } },
        m_ComponentPanel{this,  m_pActiveCamera},
        m_ContentBrowser{this},
        m_RenderPanel{this},
        m_StatsPanel{this},
        m_FileMenuBar{this}
    {
        //start the Imgui code here
        //Initialization Code
   
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); // Enable ImGui IO
        io.WantCaptureMouse;
        (void)io;

        const std::type_info& info = typeid(window);
        std::string type_name = info.name();
        
        
        if (type_name.compare("class Win32Window") == 0)
        {
            m_ImguiBackEnd = ImGui_BeckEnd::Win32;
            ImGui_ImplWin32_Init(window);
        }
        /*
        if (type_name.compare("class NGLFWwindow") == 0)
        {
            m_ImguiBackEnd = ImGui_BeckEnd::GLFW;
            ImGui_ImplGlfw_InitForOpenGL(window, true);
        }
        */

        const char* gl_ver = "#version 450";
        ImGui_ImplOpenGL3_Init(gl_ver);
        ImGui::StyleColorsDark();

        // Initialize the camera
        m_pActiveCamera->SetEye(glm::vec3{ 0.0, 1.0, -1.0 });
        m_pActiveCamera->SetCenter(glm::vec3{ 0.0, 1.0, 0.0 });

        
    }
    UILayer() : Layer() {}

    ~UILayer()
    {
        //ImGui Clean Up Code
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

};