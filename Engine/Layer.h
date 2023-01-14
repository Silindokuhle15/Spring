#pragma once
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include "Dependencies/imgui/imgui.h"
#include "Dependencies/imgui/backends/imgui_impl_glfw.h"
#include "Dependencies/imgui/backends/imgui_impl_opengl3.h"

class Layer
{

public:

	Layer() = default;
	~Layer() = default;

	virtual void Enable() = 0;
	virtual void OnInit() = 0;
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
};

