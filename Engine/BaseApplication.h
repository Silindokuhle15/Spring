#pragma once
#include "Application.h"
#include "Camera.h"
#include "Mouse.h"
#include "KeyBoard.h"
#include "UILayer.h"
#include "Renderer.h"
#include "Scene.h"
#include "TimeStep.h"

class Renderer;
class UILayer;

class BaseApplication
{
private:
	static void CreateMainWindow();
	
public:

	BaseApplication() = default;
	static void Run();
	static void ShutDown();
	static void AttachRenderer(std::unique_ptr<Renderer> Ren);

	static GLFWwindow* m_pWindow;
	static std::unique_ptr<Renderer> m_pActiveRenderer;
	static std::unique_ptr<UILayer> m_pUILayer;
	static std::shared_ptr<Scene> m_Scene;

	// maybe make the timestep a member of the base application to have access to it
	static TimeStep ts;

	static void error_callback(int error, const char* description);
	static void window_size_callback(GLFWwindow* window, int new_width, int new_height);
	static void window_close_call(GLFWwindow* window);

	// Camera Controlls
	static void MoveCameraForward();
	static void MoveCameraBackward();
	static void MoveCameraUp();
	static void MoveCameraDown();
	static void MoveCameraLeft();
	static void MoveCameraRight();
	static void RotateCamera(glm::vec3 rot_dir);
	static void FocusCamera(glm::vec3 rot_dir);

	static void CreateNewScene();

};