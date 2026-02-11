#pragma once
#include "Panel.h"
#include "Win32Window.h"
#include <typeinfo>
#include "Renderer.h"
#include <Commdlg.h>
#include "Event.h"
#include "ComponentsPanel.h"
#include "ContentBrowserPanel.h"
#include "MenuBarPanel.h"
#include "RenderPanel.h"
#include "StatisticsPanel.h"

#define IMPORT_FROM_EDITBOX 1003

template<typename T>
class UILayer :
    public Layer
{
private:
    glm::vec2 m_MousePosition;
    glm::vec2 m_MouseSpeedScale;
public:
    std::shared_ptr<Win32Window> m_ParentWindow;
    std::shared_ptr<Scene> m_ActiveScene;
    std::shared_ptr<Renderer> m_ActiveRenderer;
    std::shared_ptr<glm::mat4> m_pActiveTransform;
    std::shared_ptr<Camera> m_pActiveCamera;

    TimeStep m_Delta{ 0.0f };
    ComponentPanel<UILayer> m_ComponentPanel;
    MenuBar<UILayer> m_FileMenuBar;
    RenderPanel m_RenderPanel;
    StatsPanel m_StatsPanel;
    ContentBrowser m_ContentBrowser;

public:
    void SetMouseSpeedScale(const glm::vec2& scale_xy)
    {
        m_MouseSpeedScale = scale_xy;
    }
    const glm::vec2 GetMouseSpeedScale() const
    {
        return m_MouseSpeedScale;
    }
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

    virtual void BeginFrame() override
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }
    virtual void EndFrame() override
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    virtual void OnInit() override
    {
    }
    void OnMouseMove(event::MouseMoveEvent& event) override
    {
        auto x = static_cast<float>(event.GetX());
        auto y = static_cast<float>(event.GetY());
        glm::vec2 mousePos{
            (2.0 * (x / 1920.0) - 1.0),
            (1.0 - 2.0 * (y / 1080.0))
        };
        SetMousePosition(mousePos);
    }
    virtual void OnUpdate(TimeStep ts) override
    {
        m_Delta = ts;
        if (m_ActiveScene != nullptr)
        {
            glm::vec2 deltaMouse = m_MousePosition - m_ActiveScene->GetMousePosition();
            glm::quat deltaQuat = glm::quat(glm::vec3(deltaMouse.y * m_MouseSpeedScale.x * ts, deltaMouse.x * m_MouseSpeedScale.y * ts, 0.0f));
            m_pActiveCamera->SetOrientation(glm::normalize(deltaQuat * m_pActiveCamera->m_orientation));
            m_pActiveCamera->Present();
            m_pActiveCamera->OnUpdate(m_Delta);
            m_ActiveScene->SetMousePosition(m_MousePosition);
        }
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

    UILayer(std::shared_ptr<T> window) :
        m_ParentWindow{window},
        m_MousePosition{0.5f, 0.5f},
        m_MouseSpeedScale{0.0f, 10.0f},
        m_pActiveCamera{ new Camera{ 1920, 1080, 0.01f, 10000.0f } },
        m_ComponentPanel{ this,  m_pActiveCamera },
        m_ContentBrowser{ this },
        m_RenderPanel{ this },
        m_StatsPanel{ this },
        m_FileMenuBar{ this }
    {
        //start the Imgui code here

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); // Enable ImGui IO
        io.WantCaptureMouse;
        (void)io;

        const std::type_info& info = typeid(window);
        std::string type_name = info.name();

        //if (type_name.compare("class std::shared_ptr<class BaseApplication>") == 0)
        if (type_name.compare("class std::shared_ptr<class Win32Window>") == 0)
        {
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
        ImGui_ImplWin32_Shutdown();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
    }
};