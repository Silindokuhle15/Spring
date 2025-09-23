#include "Layer.h"

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


template<class T>
class ComponentPanel : public Panel
{
public:
	int32_t create_handle = 0;

	// SETTING VARIABLES
	int m_EnableLighting = 1;
	bool m_EnableTexture = 0;
	bool m_IsEnable = false;
	// IMGUIZMO
	bool m_UseGizmoWindow = false;
	bool m_EnableImGuizmo = false;

	// Huh?
	float m_CameraSpeed;
	std::shared_ptr<Scene> m_ActiveScene;
	int m_ActiveMaterial;
	int m_ActiveUniforms;

	// EDITOR CAMERA VARIABLES
	float m_CameraPosition[3] = { 0.0, 0.0, 0.0 };
	int m_NumberOfCamera = 1;
	float m_MouseSpeedScalerX{ 0.5f };
	float m_MouseSpeedScalerY{ 0.5f };

	// LIGHT 
	float m_Factor = 0.0;
	float m_GroundColor[3] = { 0.0, 0.0, 0.0 };
	float m_LightColor[3] = { 1.0f, 1.0f, 1.0f };
	float m_SkyColor[3] = { 0.0, 0.0, 0.0 };
	float m_PointLightPosition[3] = { 0.0, 0.0, 0.0 };

	// OBJECT POINTERS
	std::shared_ptr<Camera> m_pEditorCamera;
	std::shared_ptr<glm::mat4> m_ActiveTransform;

	void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition);

	void Run() override;

	ComponentPanel() :
		m_PanelName{ "Components Panel" }
	{
	}
	ComponentPanel(Layer* parent, std::shared_ptr<Camera> layer_camera)
		:
		m_PanelName{ "Components Panel" }
	{
		OnInit(parent);
		m_pEditorCamera = layer_camera;
	}
	~ComponentPanel()
	{

	}
private:
	std::string m_PanelName;
};

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



class RenderPanel : public Panel
{
public:
	uint8_t* m_Pixels;

	std::shared_ptr<Scene> m_ActiveScene;

	void Run() override;
	RenderPanel() :
		m_PanelName{ "Render Panel" }
	{
		// DO something;
		m_Pixels = new uint8_t[1080 * 1920 * 4];
	}

	RenderPanel(Layer* parent_layer) :
		m_PanelName{ "Render Panel" }
	{
		m_Pixels = new uint8_t[1080 * 1920 * 4];
		OnInit(parent_layer);
	}
	~RenderPanel()
	{
		if (m_Pixels)
		{
			delete[] m_Pixels;
		}
	}

private:
	std::string m_PanelName;
};


class StatsPanel : public Panel
{
public:
	// hmm?
	bool m_RenderMode;
	unsigned int m_NumPrimitives;
	unsigned int m_NumIndices;

	int selected_mode = 0;
	int selected_attr = 0;

	// Queries
	unsigned int m_Query;
	int m_Samples;
	int m_ResultAvailable;

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

template<class T>
inline void ComponentPanel<T>::EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
{
	auto parent_layer = reinterpret_cast<T*>(*(m_ParentLayer.get()));
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;

	if (ImGui::IsKeyPressed(ImGuiKey_T))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(ImGuiKey_E))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	if (ImGui::RadioButton("Universal", mCurrentGizmoOperation == ImGuizmo::UNIVERSAL))
		mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;
	float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
	ImGui::InputFloat3("Tr", matrixTranslation);
	ImGui::InputFloat3("Rt", matrixRotation);
	ImGui::InputFloat3("Sc", matrixScale);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

	if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}
	if (ImGui::IsKeyPressed(ImGuiKey_S))
		useSnap = !useSnap;
	ImGui::Checkbox("##UseSnap", &useSnap);
	ImGui::SameLine();

	switch (mCurrentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
		ImGui::InputFloat3("Snap", &snap[0]);
		break;
	case ImGuizmo::ROTATE:
		ImGui::InputFloat("Angle Snap", &snap[0]);
		break;
	case ImGuizmo::SCALE:
		ImGui::InputFloat("Scale Snap", &snap[0]);
		break;
	}
	ImGui::Checkbox("Bound Sizing", &boundSizing);
	if (boundSizing)
	{
		ImGui::PushID(3);
		ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
		ImGui::SameLine();
		ImGui::InputFloat3("Snap", boundsSnap);
		ImGui::PopID();
	}

	ImGuiIO& io = ImGui::GetIO();
	float viewManipulateRight = io.DisplaySize.x;
	float viewManipulateTop = 0;
	static ImGuiWindowFlags gizmoWindowFlags = 0;
	if (m_UseGizmoWindow)
	{
		ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(400, 20), ImGuiCond_Appearing);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
		ImGui::Begin("Gizmo", 0, gizmoWindowFlags);
		ImGuizmo::SetDrawlist();
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
		viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
		viewManipulateTop = ImGui::GetWindowPos().y;
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;
	}
	else
	{
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
	}

	//ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
	//ImGuizmo::DrawCubes(cameraView, cameraProjection, &objectMatrix[0][0], gizmoCount);
	//ImGuizmo::DrawCubes(cameraView, cameraProjection, (float*)glm::value_ptr(*m_ActiveTransform), gizmoCount);
	ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

	glm::vec3 displacement = parent_layer->m_ActiveCamera.GetEye() - glm::vec3(0);
	float distance = glm::dot(displacement, displacement);
	distance = glm::sqrt(distance);

	ImGuizmo::ViewManipulate(cameraView, distance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

	if (m_UseGizmoWindow)
	{
		ImGui::End();
		ImGui::PopStyleColor(1);
	}
}

template<class T>
inline void ComponentPanel<T>::Run()
{
	auto parent_layer = reinterpret_cast<T*>(*(m_ParentLayer.get()));

	ImGui::Begin(m_PanelName.c_str());
	ImGui::Text("Viewing and Transformation");
	ImGui::Separator();

	ImGui::Text("Number Of Cameras : %d", m_NumberOfCamera);
	float cam_pos[] = { m_pEditorCamera->m_eye.x,m_pEditorCamera->m_eye.y, m_pEditorCamera->m_eye.z };
	ImGui::SliderFloat3("Eye", cam_pos, -50.0, 50.0);
	m_pEditorCamera->SetEye(glm::vec3{ cam_pos[0], cam_pos[1], cam_pos[2] });

	float cam_center[] = { m_pEditorCamera->m_center.x,m_pEditorCamera->m_center.y, m_pEditorCamera->m_center.z };
	ImGui::SliderFloat3("Center", cam_center, -50.0, 50.0);
	m_pEditorCamera->SetCenter(glm::vec3{ cam_center[0], cam_center[1], cam_center[2] });

	float cam_up[] = { m_pEditorCamera->m_up.x,m_pEditorCamera->m_up.y, m_pEditorCamera->m_up.z };
	ImGui::SliderFloat3("Up", cam_up, -50.0, 50.0);
	m_pEditorCamera->SetUp(glm::vec3{ cam_up[0], cam_up[1], cam_up[2] });

	auto& camOrientation = m_pEditorCamera->GetOrientation();
	float cam_orientation[] = { camOrientation.w ,camOrientation.x, camOrientation.y, camOrientation.z, };
	ImGui::SliderFloat4("orientation", cam_orientation, -1.0f, 1.0f);
	m_pEditorCamera->SetOrientation(glm::quat{ cam_orientation[0],cam_orientation[1], cam_orientation[2], cam_orientation[3] });

	ImGui::SliderFloat("Camera Speed", (float*)&m_pEditorCamera->m_Speed, 0.0, 1.0f, "%.2f", 0);
	ImGui::Separator();

	float mouseSpeedScaleX{ parent_layer->GetMouseSpeedScale().x }, mouseSpeedScaleY{ parent_layer->GetMouseSpeedScale().y };
	ImGui::SliderFloat("Mouse Speed Scale X", &mouseSpeedScaleX, 0.0f, 10.0f);
	ImGui::SliderFloat("Mouse Speed Scale Y", &mouseSpeedScaleY, 0.0f, 10.0f);
	parent_layer->SetMouseSpeedScale(glm::vec2{ mouseSpeedScaleX, mouseSpeedScaleY });
	ImGui::Separator();

	ImGui::Text("Edit Transform Component");
	const char* attr[] = { "Mesh", "Grid" };

	if (ImGui::Button("Create..."))
		ImGui::OpenPopup("Create..._popup");
	//ImGui::SameLine();
	//ImGui::TextUnformatted(create_handle == -1 ? "<None>" : attr[create_handle]);
	ImGuiTextFilter filter;
	const char* lines[] = { "aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world" };
	if (ImGui::BeginPopup("Create..._popup"))
	{
		for (int i = 0; i < IM_ARRAYSIZE(attr); i++)
		{
			if (ImGui::Selectable(attr[i]))
			{
				ImGui::SameLine();
				filter.Draw();
				for (int j = 0; j < IM_ARRAYSIZE(lines); j++)
				{
					if (filter.PassFilter(lines[j]))
					{

					}
				}

				if (ImGui::IsItemClicked(ImGui::Selectable(attr[i])))
				{
					create_handle = i;
				}
			}

		}

		ImGui::EndPopup();
	}

	if (ImGui::TreeNode("Scene Hierachy"))
	{
		for (auto nt : m_ActiveScene->m_Characters)
		{
			int t = static_cast<int>(nt);
			std::string snt{ std::to_string(t) };
			if (ImGui::TreeNode(snt.c_str()))
			{
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}

	ImGui::End();
}

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

	if (ImGui::BeginChild("FIrst Window", ImVec2(x * a, b * y), true))
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


inline void RenderPanel::Run()
{
	ImGui::Begin(m_PanelName.c_str());
	int width = 1920;
	int height = 1080;

	//glReadPixels(0, 0, width, height, GL_RGB, GL_FLOAT, pixels);

	if (m_Pixels)
	{
		glDrawBuffer(GL_COLOR_ATTACHMENT1);

		//glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (uint8_t*)m_Pixels);
	}

	//delete[] m_Pixels;

	ImGui::End();

}

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
