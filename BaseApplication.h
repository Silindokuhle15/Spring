#pragma once
#include "Application.h"
#include "Camera.h"
#include "Mouse.h"
#include "KeyBoard.h"

class BaseApplication
{
private:
	
public:
	static void Run(Application*  App);
	static void AttachCamera(Camera* cam);

	static Camera* cam_ptr;
	static GLFWwindow* m_pWindow;

	static void error_callback(int error, const char* description);

	static void window_size_callback(GLFWwindow* window, int new_width, int new_height);
};

