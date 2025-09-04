#pragma once
#include "Win32Window.h"
#include "Camera.h"
#include "Mouse.h"
#include "KeyBoard.h"
#include "UILayer.h"
#include "Renderer.h"
#include "Scene.h"
#include "SceneEvent.h"
#include "Script.h"
//#include "NGLFWwindow.h"
#include "FontMgr.h"

class Renderer;

class BaseApplication
{	
public:
	static bool ExitWindow;
	static TimeStep ts;
	static Win32Window* m_Window;

	static std::shared_ptr<Renderer> m_pActiveRenderer;
	static std::shared_ptr<UILayer<Win32Window>> m_pUILayer;
	//static std::shared_ptr<Scene> m_Scene;
	static Scene* m_Scene;

	static void Run();
	static void ShutDown();
	static void OnUpdate();
	
	BaseApplication() = default;
};