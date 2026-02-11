#ifndef _COMPONENTS_PANEH_H_
#define _COMPONENTS_PANEH_H_
#include "Panel.h"
#include "ComponentUtils.h"

template<class T>
class ComponentPanel : public Panel
{
public:
	int32_t create_handle = 0;
	int selectedCharacter = -1;
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

template<class T>
inline void ComponentPanel<T>::EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
{
	/*
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
	*/
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

	if (ImGui::Button("Create Asset Registry"))
	{
		auto& activeScene = m_ActiveScene;
		auto assetManager = activeScene->GetAssetManager();
		std::ofstream ofs("Lobby.pak", std::ios::out);
		if (!ofs.is_open())
		{
			std::cerr << "Failed to open Lobby.pak" << std::endl;
		}

		for (auto& arp : assetManager->m_AssetResourceAndHandleMap)
		{
			ofs << "{" << std::endl;
			ofs << "\tAssetHandle : (" << arp.second.m_HWORD << " , " << arp.second.m_LWORD << ") " << std::endl;
			ofs << "\tAssetResource: { " << std::endl;
			ofs << "\t\tType : " << (int)arp.first.m_Type << std::endl;
			ofs << "\t\tPaths : {" << std::endl;
			ofs << arp.first.m_Filepath << std::endl;
			ofs << "\t}" << std::endl;
			ofs << "}" << std::endl;
		}
		ofs.close();
		std::cout << "Lobby.pak created!!!" << std::endl;
	}

	if (ImGui::Button("Create Character Registry"))
	{
		auto& activeScene = m_ActiveScene;
		std::ofstream ofs("CharacterRegistry.pak", std::ios::out);
		if (!ofs.is_open())
		{
			std::cerr << "Failed to Open CharacterRegistry.pak" << std::endl;
			return;
		}

		auto physicsView = activeScene->GetView<primitives::MeshInstance, physics::PhysicsState, scripting::ControlScript>();
		for (auto [entity, meshInstance, physicsState, controlScript] : physicsView.each())
		{
			ofs << "Character {\n"
				<< "id : " << (int)entity << '\n'
				<< meshInstance << '\n'
				<< controlScript << '\n'
				<< physicsState << '\n'
				<< "}\n";
		}
		ofs.close();
		std::cout << "CharacterRegistry.pak creted!!!" << std::endl;
	}
	// ---------- Character list ----------
	if (m_ActiveScene != nullptr)
	{
		auto& activeScene = m_ActiveScene;
		auto view = activeScene->GetView<physics::PhysicsState>();
		ImGui::TextUnformatted("Scene Characters");
		for (auto [entity, physicsState] : view.each())
		{
			int i = static_cast<int>(entity);
			ImGui::PushID(i);
			char identifier[16] = "";
			sprintf(identifier, "Character %d", i);
			if (ImGui::Selectable(identifier, selectedCharacter == i))
			{
				selectedCharacter = i;
				//	showCharacterEditor = true;
			}
			ImGui::PopID();
		}
	}


	// ---------- Right-click trigger (no popup body here) ----------
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
		ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("AddCharacter");
	}
	if (ImGui::BeginPopup("AddCharacter"))
	{
		auto& activeScene = m_ActiveScene;
		auto assetManager = activeScene->GetAssetManager();
		char buffer[512] = "";
		std::string filePath{ "" };
		bool onCreateNewCharacter = false;
		if (ImGui::InputText("pathToFile", buffer, 512))
		{
			filePath = std::string(buffer);
			std::ifstream ifs(filePath);
			if (!ifs.is_open())
			{
				std::cerr << "Unable to import " << filePath << std::endl;
				return;
			}
			ifs.close();
			onCreateNewCharacter = true;
		}
		if (onCreateNewCharacter)
		{
			onCreateNewCharacter = false;
			AssetResource meshResource{ AssetType::MeshResource, filePath };
			auto meshHandle = assetManager->GetResourceHandle(meshResource);
			auto character = activeScene->CreateSceneObject();
			character->AddComponent<primitives::MeshInstance>(meshHandle);
			physics::PhysicsState ps{};
			ps.orientation = glm::quat(0.0, 0.0, 0.0, -1.0);
			ps.position = glm::vec3(0, 0, -10);
			character->AddComponent<physics::PhysicsState>(ps);
			character->AddComponent<primitives::RenderComponent>(primitives::RenderComponent{});
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("+"))
		{

		}

		ImGui::EndPopup();
	}
	ImGui::End();
}

#endif