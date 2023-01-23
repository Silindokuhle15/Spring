#pragma once
#include "Application/Application.h"
#include "Viewing/Camera.h"
#include "Mouse.h"
#include "KeyBoard.h"
#include "LightAndShadow/PointLight.h"
#include "UILayer.h"
#include "Rendering/Renderer.h"
#include "TimeStep.h"

class Renderer;

class BaseApplication
{
private:
	static void CreateMainWindow();
	
public:
	static void Run();
	static void AttachCamera(Camera* cam);
	static void AddPointLight(PointLight * point_light);
	static void AttachRenderer(Renderer * Ren);

	static Camera* cam_ptr;
	static GLFWwindow* m_pWindow;
	static PointLight* m_PointLight;
	static Renderer * m_pActiveRenderer;
	static UILayer* m_pUILayer;

	static void error_callback(int error, const char* description);

	static void window_size_callback(GLFWwindow* window, int new_width, int new_height);
	static void window_close_call(GLFWwindow* window);
};