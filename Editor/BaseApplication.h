#pragma once
#include <GL/glew.h>
#include <GL/wglew.h>
#include "Win32Window.h"
#include "Camera.h"
#include "Mouse.h"
#include "KeyBoard.h"
#include "UILayer.h"
#include "Renderer.h"
#include "Scene.h"
#include "Script.h"
#include "FontMgr.h"
#include "BVH.h"
#include "Application.h"

namespace FILTER
{
    constexpr const char* DDS_FILTER = ".DDS";
    constexpr const char* LUA_FILTER = ".lua";
    constexpr const char* OBJ_FILTER = ".obj";
    constexpr const char* PAK_FILTER = ".pak";
    constexpr const char* PNG_FILTER = ".png";
}

constexpr float updateInterval = 1.0f / 60.0f;
class Renderer;

class BaseApplication : public Application
{	
public:
	Win32Window m_AppWindow;
	bool m_ExitWindow;
	int counter = 0;
	int counterLimit = 64;
	std::shared_ptr<Renderer> m_pActiveRenderer;
	UILayer<Win32Window> m_pUILayer;
	//void BeginFrame();
	void DrawMenuBarPanel();
    void DrawComponentsPanel();
    void DrawParticleSystemPanel();
    void DrawParticleSystemEditor();
	//void EndFrame();
	std::shared_ptr<Scene> m_Scene;
	AssetHandle m_LobbyMeshHandle;
	AssetHandle m_LobbyGraphicsShaderHandle;
	AssetHandle m_ParticleGraphicsShaderHandle;
	AssetManager m_AssetManager;

    //--------- COMPONENTS ----------//
    entt::entity selectedEntity;
    int selectedCharacter = -1;
    bool showAddCharacterMenu = false;
    bool showCharacterEditor = false;
    bool showEditCharacterMenu = false;

    bool characterAddComponent = false;
    bool characterRemoveComponent = false;
    std::string createNewCharacterMeshPath = "";
    //-------- PARTICLE SYSTEM -------//
    bool showAddParticleInstanceWindow = false;
    int selectedParticleSystem = -1;
    bool showParticleEditor = false;
    bool randomOrientation = false;
public:
	void Run() override;
	void OnUpdate();
	void ShutDown();
	void OnMouseMove(event::MouseMoveEvent& mouse_move) override;
	void OnKeyPress(event::KeyPressEvent& mouse_move) override;
    void OnPlaySound(primitives::PlaySoundRequest& sound_event);
	void DrawSceneCharacters(AssetManager& asset_manager);
	void DrawParticleSystems(AssetManager& asset_manager);
public:
	BaseApplication(uint32_t width, uint32_t height, const char* title);
	BaseApplication() = default;
    ~BaseApplication();

private:
    std::string GetFileName(const char* filter)
    {
        OPENFILENAME ofn;
        ZeroMemory(&ofn, sizeof(ofn));

        char szFile[_MAX_PATH];  // buffer for file name
        ZeroMemory(szFile, sizeof(szFile));

        // Initialize OPENFILENAME
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = m_AppWindow.m_WindowHandle;
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
        SetWindowText(GetDlgItem(m_AppWindow.m_WindowHandle, IMPORT_FROM_EDITBOX), szFile);
        return std::string(szFile);
    }
    bool showCreateNewSceneWindow = false;
    char* newSceneNameBuffer;
    char* meshPackBuffer;
    char* materialPackBuffer;
    char* texturePackBuffer;
    char* newParticleConfigName;
    bool showAssetManagerWindow = false;
};