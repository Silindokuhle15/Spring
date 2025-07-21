#pragma once
#include "GL/glew.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuizmo.h"
#include "imgui_impl_win32.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <string>
#include <set>
#include "Scene.h"
#include "Project.h"
#include "TimeStep.h"
#include "Script.h"

static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

class Layer
{
public:

	virtual ~Layer()
	{

	};

	virtual void Enable() = 0;
	virtual void OnInit() = 0;
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void OnUpdate(TimeStep ts) = 0;

	//virtual void LoadImageFromFile(std::string& path, _TextureView& image_data);
	//virtual void LoadSceneFromFile(std::string& path);
	//virtual std::string GetFileName(const char* filter) = 0;
};
