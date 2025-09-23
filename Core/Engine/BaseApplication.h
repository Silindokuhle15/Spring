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

class Renderer;

class BaseApplication : public Win32Window
{	
public:
	bool m_ExitWindow;
	std::shared_ptr<Renderer> m_pActiveRenderer;
	UILayer<BaseApplication> m_pUILayer;
	std::shared_ptr<Scene> m_Scene;
public:
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
public:
	void Run();
	void ShutDown();
	void OnUpdate() override;
public:
	BaseApplication(uint64_t width, uint64_t height, const char* title);
	BaseApplication() = default;

};