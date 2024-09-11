#pragma once
#include "Layer.h"
#include "Window.h"
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

    // EDITOR CAMERA VARIABLES

    // Queries
    unsigned int m_Query;
    int m_Samples;
    int m_ResultAvailable;

    // OBJECT POINTERS

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
        
        if (m_ActiveScene)
        {
            m_ActiveScene->m_CurrentRenderMode = m_StatsPanel.selected_mode ? PrimitiveMode::WIRE_FRAME : PrimitiveMode::TRIANGLE_STRIP;
        }
    }

    void LoadSceneFromFile(std::string& path) override
    {
        Layer::LoadSceneFromFile(path);
    }

    void CreateSceneObjects() override
    {
        Layer::m_pActiveCamera = std::shared_ptr<Camera>(&m_ActiveCamera);
        m_pActiveCamera->OnInit();
        m_ActiveScene->m_pActiveCamera = Layer::m_pActiveCamera;
        std::string texture_data = "";

        std::vector<ShaderInfo> m_shaderInfo;
        for (size_t index = 0; index < shader_paths.size(); index += 2)
        {
            m_shaderInfo = { ShaderInfo{ shader_paths[index], GL_VERTEX_SHADER },
                ShaderInfo{ shader_paths[index + 1], GL_FRAGMENT_SHADER } };
            Shader temp_shader(m_shaderInfo);
            //temp_shader.OnInit();
            m_ActiveScene->m_Shaders.push_back(temp_shader);

            //std::string& texture_path = shader_paths[index + 2];

            //_TextureView view{};
            //Layer::LoadImageFromFile(texture_path, view);

            //_TextureDescription desc{};
            //desc.m_TextureSource = _TextureSource::FILE;
            //desc.m_TextureFormat = _TextureFormat::RGB8;
            //desc.m_TextureTarget = _TextureTarget::TEXTURE_2D;

            //TextureBase<GL_Texture> base_tex{desc, view };

            //std::string& mtl_path = shader_paths[index + 3];
            //m_ActiveScene->m_Materials.push_back(Material(base_tex, mtl_path, m_shaderInfo));

            m_shaderInfo.clear();

        }

        for (auto& mat : m_ActiveScene->m_Materials)
        {
            mat.OnInit();
        }
        auto dummy_pos = glm::vec3(1.0f);
        auto dummy_color = glm::vec3(1.0f);
        m_ActiveScene->m_Lights.push_back(
            PointLight(
                glm::vec3(dummy_pos.x, dummy_pos.y, dummy_pos.z),
                glm::vec3(dummy_color.x, dummy_color.y, dummy_color.z)
            )
        );

        for (auto& s_mesh : static_mesh_paths)
        {
            m_ActiveScene->LoadMeshData(s_mesh.c_str(), 0);
        }

        for (auto& d_mesh : dynamic_mesh_paths)
        {
            m_ActiveScene->LoadMeshData(d_mesh.c_str(), 1);
        }

        m_SelectedMesh = 0;
        //m_SelectedBuffer = (m_ActiveScene->m_StaticGeometry.size() > 0 ? 0 : (m_ActiveScene->m_DynamicGeometry.size() > 0 ? 1 : 2));
        m_SelectedBuffer = 0;
        /*
        switch (m_SelectedBuffer)
        {
        case 0:
            m_pActiveTransform = std::shared_ptr<glm::mat4>(&(m_ActiveScene->m_StaticGeometry[m_SelectedMesh].m_Transform));
            break;
        case 1:
            m_pActiveTransform = std::shared_ptr<glm::mat4>(&(m_ActiveScene->m_DynamicGeometry[m_SelectedMesh].m_Transform));
            break;
        case 2:
            m_pActiveTransform = std::shared_ptr<glm::mat4>(&(m_ActiveScene->m_MeshData[m_SelectedMesh].m_Transform));
            break;
        }
        */
    }

    void BindRenderer(std::shared_ptr<Renderer> renderer)
    {
        m_ActiveRenderer = renderer;
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
        // TO do Add some code here bro
        //m_ActiveScene = 1;
        glGenQueries(1, &m_Query);
    }
    virtual void OnUpdate(TimeStep ts) override
    {
        m_Delta = ts;
        //Update the Editor camera delta right here
        m_ActiveCamera.Present();
        m_ActiveCamera.OnUpdate(m_Delta);
        
    }

    virtual std::shared_ptr<Camera>& GetLayerCamera() override
    {
        return m_pActiveCamera;
    }
    virtual std::string GetFileName(const char* filter) override
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
        m_ComponentPanel{this},
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