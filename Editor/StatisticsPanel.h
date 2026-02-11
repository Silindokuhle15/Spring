#ifndef _STATISTICS_PANEL_H_
#define _STATISTICS_PANEL_H_
#include "Panel.h"

class StatsPanel : public Panel
{
public:
	bool m_RenderMode;
	unsigned int m_NumPrimitives;
	unsigned int m_NumIndices;

	int selected_mode = 0;
	int selected_attr = 0;

	void Run() override;
	StatsPanel() :
		m_PanelName{ "Stats Panel" },
		selected_mode{ 0 },
		selected_attr{ 0 }
	{
	}
	StatsPanel(Layer* parent_layer) :
		m_PanelName{ "Stats Panel" }
	{
		OnInit(parent_layer);
	}

	~StatsPanel()
	{

	}

private:
	std::string m_PanelName;
};



inline void StatsPanel::Run()
{
	ImGui::Begin(m_PanelName.c_str());
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Separator();

	const char* names[] = { "TRIANGLES", "LINES" };
	// Showing a menu with toggles
	if (ImGui::Button("Render Mode"))
		ImGui::OpenPopup("render_mode_popup");
	ImGui::SameLine();
	ImGui::TextUnformatted(selected_mode == -1 ? "<None>" : names[selected_mode]);
	if (ImGui::BeginPopup("render_mode_popup"))
	{
		for (int i = 0; i < IM_ARRAYSIZE(names); i++)
			if (ImGui::Selectable(names[i]))
				selected_mode = i;
		ImGui::EndPopup();
	}
	const char* attr[] = { "Base Color", "TexCoord", "Normal", "Textured" };

	if (ImGui::Button("Output Attribute"))
		ImGui::OpenPopup("output_attribute_popup");
	ImGui::SameLine();
	ImGui::TextUnformatted(selected_attr == -1 ? "<None>" : attr[selected_attr]);
	if (ImGui::BeginPopup("output_attribute_popup"))
	{
		for (int i = 0; i < IM_ARRAYSIZE(attr); i++)
			if (ImGui::Selectable(attr[i]))
				selected_attr = i;
		ImGui::EndPopup();
	}

	ImGui::End();
}

#endif