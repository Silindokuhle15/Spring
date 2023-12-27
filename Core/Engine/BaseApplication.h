#pragma once
#include "Application.h"
#include "Camera.h"
#include "Mouse.h"
#include "KeyBoard.h"
#include "UILayer.h"
#include "Renderer.h"
#include "Scene.h"
#include "TimeStep.h"
//#include "PerspectiveCamera.h"
#include "ScriptingEngine.h"

//#include <glm/gtx/transform.hpp >

class Renderer;
class UILayer;

class BaseApplication
{
private:
	static void CreateMainWindow();
	
public:

	static bool ExitWindow;
	// maybe make the timestep a member of the base application to have access to it
	static TimeStep ts;

	static GLFWwindow* m_pWindow;
	static scripting::ScriptingEngine m_LuaEngine;

	static std::unique_ptr<Renderer> m_pActiveRenderer;
	static std::unique_ptr<UILayer> m_pUILayer;
	static std::shared_ptr<Scene> m_Scene;

	static void error_callback(int error, const char* description);
	static void window_size_callback(GLFWwindow* window, int new_width, int new_height);
	static void window_close_call(GLFWwindow* window);

	// Camera Controlls
	static void MoveCameraBackward();
	static void MoveCameraForward();
	static void MoveCameraDown();
	static void MoveCameraUp();
	static void MoveCameraLeft();
	static void MoveCameraRight();
	static void RotateCamera(glm::vec3 rot_dir);
	static void FocusCamera(glm::vec3 rot_dir);

	static void AttachRenderer(std::unique_ptr<Renderer> Ren);
	static void CreateNewScene();
	static void Run();
	static void ShutDown();
	
	BaseApplication() = default;
};