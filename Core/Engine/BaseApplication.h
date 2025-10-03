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
#include "Event.h"

class Renderer;

class BaseApplication : public event::IEventListener
{	
public:
	//std::shared_ptr<Win32Window> m_AppWindow;
	Win32Window m_AppWindow;
	bool m_ExitWindow;
	std::shared_ptr<Renderer> m_pActiveRenderer;
	UILayer<Win32Window> m_pUILayer;
	std::shared_ptr<Scene> m_Scene;
public:
	void Run();
	void ShutDown();
	void OnMouseMove(event::MouseMoveEvent& mouse_move) override;
public:
	BaseApplication(uint64_t width, uint64_t height, const char* title);
	BaseApplication() = default;

};