#pragma once
#include "Application.h"
#include "Viewing/Camera.h"
#include "Mouse.h"
#include "KeyBoard.h"
#include "LightAndShadow/PointLight.h"

class BaseApplication
{
private:
	
public:
	static void Run(Application*  App);
	static void AttachCamera(Camera* cam);
	static void AddPointLight(PointLight * point_light);

	static Camera* cam_ptr;
	static GLFWwindow* m_pWindow;
	static PointLight* m_PointLight;

	static void error_callback(int error, const char* description);

	static void window_size_callback(GLFWwindow* window, int new_width, int new_height);
};

