#pragma once
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Layer.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "Window.h"
#include "NGLFWwindow.h"
#include <typeinfo>
#include "Renderer.h"

#define IMPORT_FROM_EDITBOX 1003

typedef enum class ImGui_BeckEnd
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

    // EDITOR CAMERA VARIABLES

    // Queries
    unsigned int m_Query;
    int m_Samples;
    int m_ResultAvailable;

    // OBJECT POINTERS
    std::shared_ptr<Camera> m_EditorCamera;
    std::shared_ptr<glm::mat4> m_ActiveTransform;

    // DISPLAY PANELS
    ComponentPanel<UILayer> m_ComponentPanel;
    RenderPanel<UILayer> m_RenderPanel;
    StatsPanel<UILayer> m_StatsPanel;
    ContentBrowser<UILayer> m_ContentBrowser;
    MenuBar<UILayer> m_FileMenuBar;


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

    void BindRenderer(std::shared_ptr<Renderer> renderer)
    {
        m_ActiveRenderer = renderer;
    }
    void LoadScene(std::shared_ptr<Scene> scene)
    {
        m_ActiveScene = std::shared_ptr<Scene>(scene);

        SelectedMesh = 0;

        //if (!(scene->m_StaticGeometry.empty() || scene->m_DynamicGeometry.empty()))
        if (!(scene->m_MeshData.empty()))
        {
            m_ActiveTransform = std::shared_ptr<glm::mat4>(&scene->m_MeshData[SelectedMesh].m_Transform);

            m_EditorCamera = std::shared_ptr<Camera>(scene->m_ActiveCamera);
            m_ComponentPanel.m_EditorCamera = m_EditorCamera;
            if (m_ComponentPanel.m_ActiveTransform)
                m_ComponentPanel.m_ActiveTransform.reset();
            m_ComponentPanel.m_ActiveTransform = m_ActiveTransform;
            if (m_ComponentPanel.m_ActiveScene)
                m_ComponentPanel.m_ActiveScene.reset();
            m_ComponentPanel.m_ActiveScene = m_ActiveScene;
            if (m_RenderPanel.m_ActiveScene)
                m_RenderPanel.m_ActiveScene.reset();
            m_RenderPanel.m_ActiveScene = m_ActiveScene;

        }
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
        // TO do Add some code here bro
        //m_ActiveScene = 1;
        glGenQueries(1, &m_Query);
    }
    virtual void OnUpdate(float ts) override
    {
        m_Delta = ts;
        //Update the Editor camera delta right here
        m_EditorCamera->OnUpdate(m_Delta);
    }

    virtual std::string GetFileName() override
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
        //const char* filter = GetReaderOFNFilters();
        const char* filter = ".fbx";
        ofn.lpstrFilter = filter;

        // Display the Open dialog box. 
        if (GetOpenFileName(&ofn) == false)
        {
            // user cancel
            delete filter;
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
        m_ComponentPanel{},
        m_ContentBrowser{},
        m_RenderPanel{},
        m_StatsPanel{},
        m_FileMenuBar{this}
    {
        //start the Imgui code here
        //Initialization Code
   
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); // Enable ImGui IO
        io.WantCaptureMouse;
        (void)io;
        // Dark Theme lemme see 
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