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

class Renderer;

class BaseApplication : public Application
{	
public:
	Win32Window m_AppWindow;
	bool m_ExitWindow;
	std::shared_ptr<Renderer> m_pActiveRenderer;
	UILayer<Win32Window> m_pUILayer;
	std::shared_ptr<Scene> m_Scene;
	AssetHandle m_LobbyMeshHandle;
	AssetHandle m_LobbyGraphicsShaderHandle;
	AssetHandle m_ParticleGraphicsShaderHandle;
	AssetManager m_AssetManager;
public:
	void Run() override;
	void OnUpdate();
	void ShutDown();
	void OnMouseMove(event::MouseMoveEvent& mouse_move) override;
	void OnKeyPress(event::KeyPressEvent& mouse_move) override;
	void DrawLobby(AssetManager& asset_manager);
	void DrawSceneCharacters(AssetManager& asset_manager);
	void DrawParticleSystems(AssetManager& asset_manager);
public:
	BaseApplication(uint64_t width, uint64_t height, const char* title);
	BaseApplication() = default;

};