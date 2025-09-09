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
private:
    glm::vec2 m_MousePosition;
public:
    ImGui_BeckEnd m_ImguiBackEnd;
    // Huh?
    std::shared_ptr<Win32Window> m_ParentWindow;
    std::shared_ptr<Scene> m_ActiveScene;
    std::shared_ptr<Renderer> m_ActiveRenderer;
    std::shared_ptr<glm::mat4> m_pActiveTransform;
    std::shared_ptr<Camera> m_pActiveCamera;

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
        glm::vec2 deltaMouse = m_MousePosition  - m_ActiveScene->GetMousePosition();
        glm::quat deltaQuat = glm::quat(glm::vec3(deltaMouse.y * -0.0f * ts, deltaMouse.x * 0.5f * ts, 0.0f));
        m_pActiveCamera->m_orientation = glm::normalize(deltaQuat * m_pActiveCamera->m_orientation);
        m_pActiveCamera->Present();
        m_pActiveCamera->OnUpdate(m_Delta);
        m_ActiveScene->SetMousePosition(m_MousePosition);
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

    UILayer(std::shared_ptr<Win32Window> window) :
        m_ParentWindow{window},
        m_ImguiBackEnd{ ImGui_BeckEnd::Win32 },
        m_MousePosition{window->GetWidth()/2.0, window->GetHeight()/2.0},
        m_pActiveCamera{ new Camera{ 1920, 1080, 0.01f, 10000.0f } },
        m_ComponentPanel{ this,  m_pActiveCamera },
        m_ContentBrowser{ this },
        m_RenderPanel{ this },
        m_StatsPanel{ this },
        m_FileMenuBar{ this }
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


        if (type_name.compare("class std::shared_ptr<class Win32Window>") == 0)
        {
            m_ImguiBackEnd = ImGui_BeckEnd::Win32;            
            ImGui_ImplWin32_Init(m_ParentWindow->m_Hwnd);
        }
        const char* gl_ver = "#version 450";
        ImGui_ImplOpenGL3_Init(gl_ver);
        ImGui::StyleColorsDark();
    }
    UILayer() : Layer() {}
    void SetMousePosition(const glm::vec2& mouse_position)
    {
        m_MousePosition = mouse_position;
    }

    const glm::vec2& GetMousePosition() const
    {
        return m_MousePosition;
    }

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