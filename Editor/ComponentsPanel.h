#ifndef _COMPONENTS_PANEH_H_
#define _COMPONENTS_PANEH_H_
#include "Panel.h"
#include "ComponentUtils.h"
#include <entt.hpp>


template<class T>
class ComponentPanel : public Panel
{
public:
	int32_t create_handle = 0;
	int selectedCharacter = -1;
	bool showAddCharacterMenu = false;
	bool showCharacterEditor = false;
	bool showEditCharacterMenu = false;

	bool characterAddComponent = false;
	bool characterRemoveComponent = false;
	std::string createNewCharacterMeshPath = "";
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
		auto assetManager = m_ActiveScene->GetAssetManager();
		auto title = GetFilenameWithoutExtension(activeScene->GetTitle());
		title += "_asset.pak";
		assetManager->Serialize(title);
	}

	if (ImGui::Button("Create Character Registry"))
	{
		auto& activeScene = m_ActiveScene;
		activeScene->Serialize();
	}
	// ---------- Character list ----------
	if (m_ActiveScene != nullptr)
	{
		auto& activeScene = m_ActiveScene;
		auto view = activeScene->GetView<entt::entity>();
		ImGui::TextUnformatted("Scene Characters");
		for (auto [entity] : view.each())
		{
			int i = static_cast<int>(entity);
			ImGui::PushID(i);
			char identifier[16] = "";
			sprintf(identifier, "Character %d", i);
			if (ImGui::Selectable(identifier, selectedCharacter == i))
			{
				selectedCharacter = i;
				showEditCharacterMenu = true;
				showCharacterEditor = true;
			}
			ImGui::PopID();
		}
	}


	// ---------- Right-click trigger (no popup body here) ----------
	/*/
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
		ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("AddCharacter");
	}
	if (ImGui::BeginPopup("AddCharacter"))
	{
		char buffer[512] = "";
		if (ImGui::InputText("pathToFile", buffer, 512))
		{
			createNewCharacterMeshPath = std::string(buffer);
			std::ifstream ifs(createNewCharacterMeshPath);
			if (!ifs.is_open())
			{
				std::cerr << "Unable to import " << createNewCharacterMeshPath  << std::endl;
				return;
			}
			ifs.close();
			showAddCharacterMenu = true;
		}

		ImGui::SameLine();
		if (ImGui::SmallButton("+"))
		{

		}
		ImGui::EndPopup();
	}
	/**/

	if (showAddCharacterMenu && ! createNewCharacterMeshPath.empty())
	{
		auto& activeScene = m_ActiveScene;
		auto assetManager = activeScene->GetAssetManager();
		ImGui::Begin("Add Character");

		static float rotBuffer[4] = {0, 0, 0, -1};
		static float posBuffer[4] = {0, 0, 0, 1};
		static char tagBuffer[32] = "";
		if (ImGui::InputFloat4("Position", posBuffer));
		if (ImGui::InputFloat4("Orientation", rotBuffer));
		if (ImGui::InputText("character tag", tagBuffer, 32));
		ImGui::Separator();
		ImGui::TextUnformatted(createNewCharacterMeshPath.c_str());
		ImGui::SameLine();
		if (ImGui::SmallButton("x"))
		{
			createNewCharacterMeshPath.clear();
		}
		ImGui::Separator();
		if (ImGui::Button("Add to Scene"))
		{
			AssetResource meshResource{ AssetType::MeshResource, createNewCharacterMeshPath };
			auto meshHandle = assetManager->GetResourceHandle(meshResource);
			auto character = activeScene->CreateSceneObject();
			character->AddComponent<primitives::MeshInstance>(meshHandle);
			physics::PhysicsState ps{};
			ps.orientation = glm::quat(rotBuffer[0], rotBuffer[1], rotBuffer[2], rotBuffer[3]);
			ps.position = glm::vec3(posBuffer[0], posBuffer[1], posBuffer[2]);
			character->AddComponent<physics::PhysicsState>(ps);
			character->AddComponent<primitives::RenderComponent>(primitives::RenderComponent{});
		}

		ImGui::End();
	}

	if (showCharacterEditor && selectedCharacter >= 0)
	{
		auto& activeScene = m_ActiveScene;
		auto entity = entt::entity((uint32_t)selectedCharacter);
		auto character = activeScene->GetSceneCharacter(entity);
		ImGui::Begin("Character Editor");
		if (character.HasComponent<physics::PhysicsState>())
		{
			auto& physicsState = character.GetComponent<physics::PhysicsState>();
			ImGui::TextUnformatted("Physics Component");
			float orientation[] = { physicsState.orientation.w, physicsState.orientation.x , physicsState.orientation.y , physicsState.orientation.z };
			float position[] = { physicsState.position.x , physicsState.position.y , physicsState.position.z , 1.0f };
			if (ImGui::InputFloat4("orientation", orientation))
			{
				
				physicsState.orientation = glm::quat(orientation[0], orientation[1], orientation[2], orientation[3]);

				//physicsState.orientation.w = orientation[0];
				//physicsState.orientation.x = orientation[1];
				//physicsState.orientation.y = orientation[2];
				//physicsState.orientation.z = orientation[3];
			}
			if (ImGui::InputFloat4("position", position))
			{
				physicsState.position.x = position[0];
				physicsState.position.y = position[1];
				physicsState.position.z = position[2];
			}
			if (ImGui::InputFloat("mass", &physicsState.mass));
		}

		if (character.HasComponent<primitives::MeshInstance>())
		{
			char buffer[256] = "";
			auto& meshInstance = character.GetComponent<primitives::MeshInstance>();
			ImGui::TextUnformatted("Mesh Instance");
			ImGui::Separator();
			sprintf(buffer, "HWORD: %d LWORD %d", (int)meshInstance.m_Handle.m_HWORD, (int)meshInstance.m_Handle.m_LWORD);
			ImGui::TextUnformatted(buffer);
		}

		if (character.HasComponent<scripting::ControlScript>())
		{
			char buffer[256] = "";
			auto& scriptInstance = character.GetComponent<scripting::ControlScript>();
			ImGui::TextUnformatted("Script Instance");
			ImGui::Separator();
			sprintf(buffer, "HWORD: %d LWORD %d", (int)scriptInstance.m_Handle.m_HWORD, (int)scriptInstance.m_Handle.m_LWORD);
			ImGui::TextUnformatted(buffer);
		}

		if (character.HasComponent<Character*>())
		{
			ImGui::TextUnformatted("Children");
		}

		if (character.HasComponent<primitives::Parent>())
		{
			ImGui::TextUnformatted("Parent");
			auto& parent = character.GetComponent<primitives::Parent>();
			char buffer[256] = "";
			sprintf(buffer, "Character %d", (int)parent.ParentEntity);
			ImGui::TextUnformatted(buffer);
			if (character.HasComponent<primitives::ParticleSystem>())
			{
				bool showParticleEditor = false;
				ImGui::TextUnformatted("Particle System");
				ImGui::Checkbox("Open Editor", &showParticleEditor);
				//if (showParticleEditor)
				{
					auto& ps = character.GetComponent<primitives::ParticleSystem>();
					int numParticles = ps.m_NumParticles;
					int maxNumParticles = ps.m_MaxNumParticles;
					int particleRate = ps.m_ParticleRate;
					float emitterPosition[] = { ps.m_EmitterInfo.m_VectorOne.x, ps.m_EmitterInfo.m_VectorOne.y , ps.m_EmitterInfo.m_VectorOne.z };
					float emitterOrientation[] = { ps.m_EmitterInfo.m_VectorTwo.x, ps.m_EmitterInfo.m_VectorTwo.y, ps.m_EmitterInfo.m_VectorTwo.z };
					float w1 = ps.m_EmitterInfo.m_VectorOne.w;
					float w2 = ps.m_EmitterInfo.m_VectorTwo.w;

					ImGui::Begin("Particle System Editor", &showParticleEditor);
					ImGui::Checkbox("Enabled", &ps.m_EmitterInfo.m_Flags.m_IsEnabled);

					ImGui::Separator();
					ImGui::TextUnformatted("Compute Handle");
					ImGui::DragScalar("HWORD", ImGuiDataType_U64, &ps.m_ShaderHandle.m_HWORD);
					ImGui::DragScalar("LWORD", ImGuiDataType_U64, &ps.m_ShaderHandle.m_LWORD);

					static std::vector<std::string> emitterShapes = { "Point", "Circle",  "Cylinder", "Cone", "Sphere", "Prism" };
					static int selectedShape = 0;

					ImGui::Separator();
					if (ImGui::Button("Emitter"));
					ImGui::SameLine();
					if (ImGui::Button(emitterShapes[selectedShape].c_str()))
					{
						ImGui::OpenPopup("Emitter Shape");
					}
					if (ImGui::BeginPopup("Emitter Shape"))
					{
						for (auto index = 0; index < emitterShapes.size(); index++)
						{
							if (ImGui::Selectable(emitterShapes[index].c_str()))
							{
								//selectedShape = index;
								ps.m_EmitterInfo.m_Shape = (primitives::EmitterShape)index;

							}
						}
						ImGui::EndPopup();
					}

					selectedShape = (int)ps.m_EmitterInfo.m_Shape;

					if (selectedShape == 0)														// Point Emitter
					{
						ImGui::InputFloat3("Position", emitterPosition);
						ImGui::Checkbox("Random Orientation", &ps.m_EmitterInfo.m_Flags.m_Unused1);
						if (ps.m_EmitterInfo.m_Flags.m_Unused1)
						{
							ImGui::SliderFloat3("Ranges", emitterOrientation, 0.0f, 1.0f);
						}
						else
						{
							ImGui::InputFloat3("Orientation", emitterOrientation);
						}
						ps.m_EmitterInfo.m_Shape = primitives::EmitterShape::POINT;

						ps.m_EmitterInfo.m_VectorOne.x = emitterPosition[0];
						ps.m_EmitterInfo.m_VectorOne.y = emitterPosition[1];
						ps.m_EmitterInfo.m_VectorOne.z = emitterPosition[2];
						ps.m_EmitterInfo.m_VectorOne.w = w1;

						ps.m_EmitterInfo.m_VectorTwo.x = emitterOrientation[0];
						ps.m_EmitterInfo.m_VectorTwo.y = emitterOrientation[1];
						ps.m_EmitterInfo.m_VectorTwo.z = emitterOrientation[2];
						ps.m_EmitterInfo.m_VectorTwo.w = w2;
					}

					if (selectedShape == 1)														// Circle Emitter
					{
						ImGui::InputFloat3("Center", emitterPosition);
						ImGui::InputFloat3("Normal", emitterOrientation);
						ImGui::InputFloat("Radius", &w1);
						ImGui::Checkbox("Fill", &ps.m_EmitterInfo.m_Flags.m_Fill);
						ps.m_EmitterInfo.m_Shape = primitives::EmitterShape::CIRCLE;

						ps.m_EmitterInfo.m_VectorOne.x = emitterPosition[0];
						ps.m_EmitterInfo.m_VectorOne.y = emitterPosition[1];
						ps.m_EmitterInfo.m_VectorOne.z = emitterPosition[2];
						ps.m_EmitterInfo.m_VectorOne.w = w1;

						ps.m_EmitterInfo.m_VectorTwo.x = emitterOrientation[0];
						ps.m_EmitterInfo.m_VectorTwo.y = emitterOrientation[1];
						ps.m_EmitterInfo.m_VectorTwo.z = emitterOrientation[2];
						ps.m_EmitterInfo.m_VectorTwo.w = w2;
					}

					if (selectedShape == 2)														// Cylinder Emitter
					{

						ImGui::InputFloat3("Center", emitterPosition);
						ImGui::InputFloat3("Normal", emitterOrientation);
						ImGui::InputFloat("Radius", &w1);
						ImGui::InputFloat("Height", &w2);
						ImGui::Checkbox("Fill", &ps.m_EmitterInfo.m_Flags.m_Fill);
						ps.m_EmitterInfo.m_Shape = primitives::EmitterShape::CYLINDER;

						ps.m_EmitterInfo.m_VectorOne.x = emitterPosition[0];
						ps.m_EmitterInfo.m_VectorOne.y = emitterPosition[1];
						ps.m_EmitterInfo.m_VectorOne.z = emitterPosition[2];
						ps.m_EmitterInfo.m_VectorOne.w = w1;

						ps.m_EmitterInfo.m_VectorTwo.x = emitterOrientation[0];
						ps.m_EmitterInfo.m_VectorTwo.y = emitterOrientation[1];
						ps.m_EmitterInfo.m_VectorTwo.z = emitterOrientation[2];
						ps.m_EmitterInfo.m_VectorTwo.w = w2;
					}

					if (selectedShape == 3)														// Cone Emitter
					{
						ImGui::InputFloat3("Apex", emitterPosition);
						ImGui::InputFloat3("Direction", emitterOrientation);
						ImGui::InputFloat("Half Angle", &w1);
						ImGui::InputFloat("Height", &w2);
						ImGui::Checkbox("Fill", &ps.m_EmitterInfo.m_Flags.m_Fill);
						ps.m_EmitterInfo.m_Shape = primitives::EmitterShape::CONE;

						ps.m_EmitterInfo.m_VectorOne.x = emitterPosition[0];
						ps.m_EmitterInfo.m_VectorOne.y = emitterPosition[1];
						ps.m_EmitterInfo.m_VectorOne.z = emitterPosition[2];
						ps.m_EmitterInfo.m_VectorOne.w = w1;

						ps.m_EmitterInfo.m_VectorTwo.x = emitterOrientation[0];
						ps.m_EmitterInfo.m_VectorTwo.y = emitterOrientation[1];
						ps.m_EmitterInfo.m_VectorTwo.z = emitterOrientation[2];
						ps.m_EmitterInfo.m_VectorTwo.w = w2;
					}

					if (selectedShape == 4)														// Sphere Emitter
					{
						ImGui::InputFloat3("Center", emitterPosition);
						ImGui::InputFloat("Radius", &w1);
						ImGui::Checkbox("Fill", &ps.m_EmitterInfo.m_Flags.m_Fill);
						ps.m_EmitterInfo.m_Shape = primitives::EmitterShape::SPHERE;

						ps.m_EmitterInfo.m_VectorOne.x = emitterPosition[0];
						ps.m_EmitterInfo.m_VectorOne.y = emitterPosition[1];
						ps.m_EmitterInfo.m_VectorOne.z = emitterPosition[2];
						ps.m_EmitterInfo.m_VectorOne.w = w1;

						ps.m_EmitterInfo.m_VectorTwo.x = emitterOrientation[0];
						ps.m_EmitterInfo.m_VectorTwo.y = emitterOrientation[1];
						ps.m_EmitterInfo.m_VectorTwo.z = emitterOrientation[2];
						ps.m_EmitterInfo.m_VectorTwo.w = w2;
					}

					float prismLength = 10.0f;
					float prismWidth = 10.0f;
					float prismHeight = 10.0f;
					if (selectedShape == 5)														// Prism Emitter
					{
						ImGui::InputFloat4("Center", emitterPosition);
						ImGui::InputFloat("L", &prismLength);
						ImGui::InputFloat("W", &prismWidth);
						ImGui::InputFloat("H", &prismHeight);

						ps.m_EmitterInfo.m_VectorOne.x = emitterPosition[0];
						ps.m_EmitterInfo.m_VectorOne.y = emitterPosition[1];
						ps.m_EmitterInfo.m_VectorOne.z = emitterPosition[2];
						ps.m_EmitterInfo.m_VectorOne.w = emitterPosition[3];

						ps.m_EmitterInfo.m_VectorTwo.x = emitterOrientation[0];
						ps.m_EmitterInfo.m_VectorTwo.y = emitterOrientation[1];
						ps.m_EmitterInfo.m_VectorTwo.z = emitterOrientation[2];
						ps.m_EmitterInfo.m_VectorTwo.w = emitterOrientation[3];
					}

					float initialSpeed = 1.0f;
					float colorOne[] = { 0,0,0, 1 };
					float colorTwo[] = { 0.45f, 0.55f, 0.45f, 1.0f };
					float colorThree[] = { 1.0f, 1.0f, 1.0f, 1.0f };
					static bool editColorOverLifetime = false;
					ImGui::Separator();
					ImGui::TextUnformatted("Particle Lifecycle");
					ImGui::InputInt("Max Particles", &maxNumParticles);
					ImGui::InputInt("Particles per second", &particleRate);
					ImGui::InputFloat("Duration", &ps.m_Duration);
					ImGui::InputFloat("Initial Speed", &initialSpeed);
					if (ImGui::Button("Size over lifetime"));
					if (ImGui::Button("Speed over lifetime"));
					if (ImGui::Checkbox("Color over lifetime", &editColorOverLifetime));
					if (editColorOverLifetime)
					{
						ImGui::ColorEdit4("Color 1", colorOne);
						ImGui::ColorEdit4("Color 2", colorTwo);
						ImGui::ColorEdit4("Color 3", colorThree);
					}
					ImGui::Separator();
					ImGui::TextUnformatted("Stats");
					ImGui::InputInt("Number of Particles", &numParticles);
					ImGui::InputFloat("TimeStep", &ps.m_Ts);
					ImGui::InputFloat("Accumulated Time", &ps.m_AccumulatedTime);

					if (numParticles >= 0)
					{
						ps.m_NumParticles = numParticles;
					}

					if (maxNumParticles >= 0)
					{
						ps.m_MaxNumParticles = maxNumParticles;
					}

					if (particleRate >= 0)
					{
						ps.m_ParticleRate = particleRate;
					}
					ImGui::End();
				}
			}
		}

		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
			ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("Edit Character");
		}
		if (ImGui::BeginPopup("Edit Character"))
		{
			if (ImGui::Button("Add Component"))
			{
				characterRemoveComponent = false;
				characterAddComponent = true;
			}

			if (ImGui::Button("Remove Component"))
			{
				characterAddComponent = false;
				characterRemoveComponent = true;
			}

			if (characterAddComponent)
			{
				ImGui::Separator();
				ImGui::Text("Adding Components");
				ImGui::Separator();
				{
					if (!(character.HasComponent<physics::PhysicsState>()))
					{
						if (ImGui::Button("PhysicsState  "))
						{
							float orientation[4] = {};
							float position[3] = {};
							float mass = 0;
							float velocity[3] = {};
							float restitution = 0;
							float linear_acceleration[3] = {};
							float unused1 = 0;
							float angular_acceleration[3] = {};
							float unused2 = 0;
							float inertia[3] = {};
							float unused3 = 0;

							ImGui::InputFloat4("orientation", orientation);
							ImGui::InputFloat3("position", position);
							ImGui::InputFloat("mass", &mass);
							ImGui::InputFloat3("velocity", velocity);
							ImGui::InputFloat("restitution", &restitution);
							ImGui::InputFloat3("linear_acceleration", linear_acceleration);
							ImGui::InputFloat3("angular_acceleration", angular_acceleration);
							ImGui::InputFloat3("inertia", inertia);

							physics::PhysicsState ps{};
							ps.orientation = glm::quat(orientation[0], orientation[1], orientation[2], orientation[3]);
							ps.position = glm::vec3(position[0], position[1], position[2]);
							ps.mass = mass;
							ps.velocity = glm::vec3(velocity[0], velocity[1], velocity[2]);
							ps.restitution = restitution;
							ps.linear_acceleration = glm::vec3(linear_acceleration[0], linear_acceleration[1], linear_acceleration[2]);
							ps.angular_acceleration = glm::vec3(angular_acceleration[0], angular_acceleration[1], linear_acceleration[2]);
							ps.inertia = glm::vec3(inertia[0], inertia[1], inertia[2]);
							character.AddComponent<physics::PhysicsState>(ps);
						}
					}
					if (!character.HasComponent<primitives::MeshInstance>())
					{
						if (ImGui::Button("Mesh  Instance"))
						{

						}
					}
					if (!character.HasComponent<scripting::ControlScript>())
					{
						if (ImGui::Button("Script Instance"))
						{

						}
					}
				}
			}

			if (characterRemoveComponent)
			{
				ImGui::Separator();
				ImGui::Text("Removing Components");
				ImGui::Separator();
				{
					if (character.HasComponent<physics::PhysicsState>())
					{
						if (ImGui::Button("PhysicsState  "))
						{
							character.RemoveComponent<physics::PhysicsState>();
						}
					}
					if (character.HasComponent<primitives::MeshInstance>())
					{
						if (ImGui::Button("Mesh  Instance"))
						{
							character.RemoveComponent<primitives::MeshInstance>();
						}
					}
					if (character.HasComponent<scripting::ControlScript>())
					{
						if (ImGui::Button("Script Instance"))
						{
							character.RemoveComponent<scripting::ControlScript>();
						}
					}
				}
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}

	ImGui::End();
}

#endif