#ifndef _CONTENT_BROWSER_PANEH_H_
#define _CONTENT_BROWSER_PANEH_H_
#include <filesystem>
#include "Panel.h"

class ContentBrowser : public Panel
{
public:
	void ReadDirTree(const std::filesystem::path& pathToShow, int level);

	void DisplayDirTree(const std::filesystem::path& pathToShow, int level);

	void Run() override;

	ContentBrowser() :
		m_PanelName{ "Content Browser" }
	{

	}
	ContentBrowser(Layer* parent_layer) :
		m_PanelName{ "Content Browser" }
	{
		OnInit(parent_layer);
	}
	~ContentBrowser()
	{

	}

private:

	std::filesystem::path root_path;
	std::filesystem::path current_path;

	std::string filename;
	std::string stem;
	std::set<std::string> m_DirContent;
	std::set<std::filesystem::path> m_DirEntries;
	std::string m_PanelName;
};

inline void ContentBrowser::DisplayDirTree(const std::filesystem::path& pathToShow, int level)
{
	// GET THE PARENT WINDOW SIZE
	auto current_window_rect = ImGui::GetCurrentWindow()->Rect();
	//auto current_window_rect = ImGui::GetCurrentContext()->CurrentWindow->InnerRect();

	float x = current_window_rect.GetWidth();
	float y = current_window_rect.GetHeight();

	float a = 0.4f;
	float b = 0.9f;

	current_path = pathToShow;

	ReadDirTree(current_path, level);

	if (ImGui::BeginChild("First Window", ImVec2(x * a, b * y), true))
	{
		ImGui::Text(pathToShow.string().c_str());

		for (auto& i : m_DirEntries)
		{
			std::string dir_stem;

			if (i.has_stem())
			{
				dir_stem = i.stem().string();
			}

			if (ImGui::TreeNode(dir_stem.c_str()))
			{
				//current_path = root_path;
				root_path = current_path;

				if (ImGui::IsItemClicked() && ImGui::IsItemHovered())
				{
					current_path = i;
					//current_path /= dir_stem;
					//pathToShow /= dir_stem;

					//ImGui::TreePush();
					//ImGui::TreePop();
					DisplayDirTree(i, level);
				}
				ImGui::TreePop();
			}
		}
	}

	//m_DirContent.clear();
	//m_DirEntries.clear();
	ImGui::EndChild();

	ImGui::SameLine();

	if (ImGui::BeginChild("Second Window", ImVec2((x * (1 - 1.09f * a)), b * y), true))
	{
		ImGui::Text(current_path.string().c_str());
		for (auto& i : m_DirContent)
		{
			ImGui::Text(i.c_str());
		}
	}
	ImGui::EndChild();

}

inline void ContentBrowser::ReadDirTree(const std::filesystem::path& pathToShow, int level)
{
	if (std::filesystem::exists(pathToShow) && std::filesystem::is_directory(pathToShow))
	{
		for (const auto& entry : std::filesystem::directory_iterator(pathToShow))
		{
			//auto filename = entry.path().filename();
			filename = entry.path().string();
			stem = entry.path().stem().string();

			// MAKE THE PATH RELATIVE

			if (std::filesystem::is_directory(entry.status()))
			{
				// DIRECTORY LEVEL
				m_DirEntries.insert(entry.path());
			}
			else if (std::filesystem::is_regular_file(entry.status()))
			{
				if (entry.path().has_extension())
				{
					auto name = stem + entry.path().extension().string();
					m_DirContent.insert(name.c_str());
				}
				else
				{
					m_DirContent.insert(stem.c_str());
				}
			}
		}
	}
}

inline void ContentBrowser::Run()
{

	ImGui::Begin(m_PanelName.c_str());

	std::string dummy_path = "C:/dev/Silindokuhle15/Spring/Assets";
	root_path = std::filesystem::path(dummy_path);
	//DisplayDirTree(root_path, 0);

	ImGui::End();
}

#endif