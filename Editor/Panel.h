#ifndef _PANEL_H_
#define _PANEL_H_
#include "Layer.h"
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
#include "Project/Project.h"
#include "Utility/Serializer.h"

class Panel
{
public:
	std::shared_ptr<Layer*> m_ParentLayer;
	uint64_t m_Width, m_Height;
	Panel() : m_Width{ 1920 }, m_Height{ 1080 } {}
	Panel(uint64_t width, uint64_t height) : m_Width{ width }, m_Height{ height }
	{

	}
	virtual void OnInit(Layer* parent)
	{
		m_ParentLayer = std::make_shared<Layer*>(parent);
	}

	virtual void Run() = 0;
	~Panel() = default;

protected:
	std::string m_PanelName;
};

#endif