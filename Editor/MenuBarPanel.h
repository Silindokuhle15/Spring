#ifndef _MENU_BAR_PANEL_H_
#define _MENU_BAR_PANEL_H_
#include "Panel.h"

template<typename T>
class MenuBar : public Panel
{
public:

	void Run() override;

	MenuBar() : m_PanelName{ "MenuBar" } {}
	MenuBar(Layer* parent) : m_PanelName{ "MenuBar" }
	{
		OnInit(parent);
	}
	~MenuBar() {}

protected:
	std::string m_PanelName;
};


template<typename T>
inline void MenuBar<T>::Run()
{
	auto parent = reinterpret_cast<T*>(*(m_ParentLayer.get()));
	bool is_open = true;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "..."))
			{
				if (ImGui::IsItemClicked())
				{
					char buffer[512];
					ImGui::InputText("File Name", buffer, sizeof(char) * 512);
					std::string file_name(buffer);
					Project NewProject(file_name);

				}
			}
			if (ImGui::MenuItem("Open", "..."))
			{
				const char* filter = ".lua";
				parent->m_ActiveScene.reset();
				std::string file_name = parent->GetFileName(filter);
				//parent->m_ActiveScene = std::shared_ptr<Scene>(new Scene(file_name));
				//parent->m_ActiveRenderer->BindScene(parent->m_ActiveScene);
				//parent->LoadScene(parent->m_ActiveScene);
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Close", "..."))
			{
			}
			if (ImGui::MenuItem("Close Project", "..."))
			{
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Save Selected", "crtl+s"))
			{
				Serializer serializer;
				serializer.Serialize(parent->m_ActiveScene.get());
			}
			if (ImGui::MenuItem("Save All", "crtl+shift+s"))
			{
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Recent Files", "..."))
			{
			}
			if (ImGui::MenuItem("Recent Projects", "..."))
			{
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Import"))
			{
				if (ImGui::MenuItem("Image (.png)"))
				{
					const char* filter = ".png";
					std::string path = parent->GetFileName(filter);
					//parent->LoadImageFromFile(path, parent->m_ActiveRenderer->m_pTextureBuffer);
					//parent->m_ActiveRenderer->CreateOpenGLTexture(_TextureView & view, _TextureDescription & desc, GL_Texture & tex)

				}
				if (ImGui::MenuItem("Wavefront (.obj)"))
				{
					//if (ImGui::IsItemClicked())
					{
						const char* filter = ".obj";
						std::string obj_path = parent->GetFileName(filter);
						//parent->m_ActiveScene->AddNewItem<Mesh>(Mesh(obj_path));

					}
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Filmbox (.fbx)"))
				{
					//if (ImGui::IsItemClicked())
					{
						const char* filter = ".fbx";
						std::string fbx_path = parent->GetFileName(filter);
						//parent->m_ActiveScene->AddNewItem<Mesh>(Mesh(fbx_path));
					}
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Project"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

#endif