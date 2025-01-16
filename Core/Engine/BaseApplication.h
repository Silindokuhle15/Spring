#pragma once
#include "Window.h"
#include "Camera.h"
#include "Mouse.h"
#include "KeyBoard.h"
#include "UILayer.h"
#include "Renderer.h"
#include "Scene.h"
#include "SceneEvent.h"
#include "Script.h"
#include "NGLFWwindow.h"
#include "FontMgr.h"
//#include <PxPhysicsAPI.h>

class Renderer;

class BaseApplication
{	
public:
	static bool ExitWindow;
	// maybe make the timestep a member of the base application to have access to it
	static TimeStep ts;
	template<class T>
	static T* m_Window;

	static std::shared_ptr<Renderer> m_pActiveRenderer;
	template<class T>
	static std::shared_ptr<UILayer<T>> m_pUILayer;
	static std::shared_ptr<Scene> m_Scene;

	static void Run();
	static void ShutDown();

	template<class T>
	static void OnUpdate();
	
	BaseApplication() = default;
};