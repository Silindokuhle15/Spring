#ifndef _RENDER_PANEL_H_
#define _RENDER_PANEL_H_
#include "Panel.h"

class RenderPanel : public Panel
{
public:
	std::shared_ptr<Scene> m_ActiveScene;

	void Run() override;
	void ReInstantiateBuffers();
	RenderPanel() :
		m_PanelName{ "Render Panel" }
	{
	}

	RenderPanel(Layer* parent_layer) :
		m_PanelName{ "Render Panel" }
	{
		OnInit(parent_layer);
	}
	~RenderPanel()
	{
	}
private:
	std::string m_PanelName;
};

inline void RenderPanel::Run()
{
	auto& activeScene = m_ActiveScene;
	if (activeScene != nullptr)
	{
		ImGui::Begin(m_PanelName.c_str());


		ImGui::End();
	}
}

#endif