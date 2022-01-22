#pragma once
#include <GLFW/glfw3.h>
#include "stdafx.h"
class EventHandler
{
private:
	GLFWwindow** window;
public:
	EventHandler() = default;
	~EventHandler() = default;

	ReturnStatus EventHandlerInit();
	static void  KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	static void WindowResizeCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		int NewWindowWidth, NewWindowHeight;
		glfwGetWindowSize(window, &NewWindowWidth, &NewWindowHeight);
		glfwSetWindowSize(window, NewWindowWidth, NewWindowHeight);
	}

	static void CursorPositionCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		double cursor_x = 0, cursor_y = 0;
		glfwGetCursorPos(window, &cursor_x, &cursor_y);
	}
};

