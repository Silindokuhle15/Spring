#include "BaseApplication.h"
#include <format>
#include <filesystem>

void BaseApplication::DrawMenuBarPanel()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "..."))
            {
                if (!showCreateNewSceneWindow)
                {
                    showCreateNewSceneWindow = true;
                }
            }
            if (ImGui::MenuItem("Open", "..."))
            {
                const std::string sceneName = GetFileName(FILTER::LUA_FILTER);
				if (!sceneName.empty())
				{
					if (m_Scene)
					{
						m_Scene.reset();
					}
					m_Scene = std::make_shared<Scene>(sceneName);
					m_Scene->SetAssetManager(&m_AssetManager);
					m_Scene->LoadSceneFromFile();
					if (!m_Scene->asset_pack.empty())
					{
						m_AssetManager.Deserialize(m_Scene->asset_pack);
					}
					if (!m_Scene->meshPack.empty())
					{
						m_AssetManager.DeserializeMeshPack(m_Scene->meshPack);
					}
					if (!m_Scene->materialPack.empty())
					{
						m_AssetManager.DeserializeMaterialPack(m_Scene->materialPack);
					}
					if (!m_Scene->texturePack.empty())
					{
						m_AssetManager.DeserializeTexturePack(m_Scene->texturePack);
					}
					if (!m_Scene->shaderPack.empty())
					{
						m_AssetManager.DeserializeShaderPack(m_Scene->shaderPack);
					}
					m_Scene->OnInit();
					m_Scene->OnCreateSceneObjects();
					m_pUILayer.LoadScene(m_Scene);
				}
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
                    const std::string path = GetFileName(FILTER::PNG_FILTER);
					if (!path.empty())
					{
						const auto dirWords = stringUtils::getWords(path, "\\");
						if (!dirWords.empty())
						{
							const auto newPath = stringUtils::combineWords(dirWords, "/");
							if (!newPath.empty())
							{
								AssetResource textureResource{ AssetType::Texture2D, newPath };
								auto assetHandle = m_AssetManager.GetResourceHandle(textureResource);
							}
						}
					}
                }
				if (ImGui::MenuItem("Image (.DDS)"))
				{
					const std::string path = GetFileName(FILTER::DDS_FILTER);
					if (!path.empty())
					{
						const auto dirWords = stringUtils::getWords(path, "\\");
						if (!dirWords.empty())
						{
							const auto newPath = stringUtils::combineWords(dirWords, "/");
							if (!newPath.empty())
							{
								AssetResource textureResource{ AssetType::Texture2D, newPath };
								auto assetHandle = m_AssetManager.GetResourceHandle(textureResource);
							}
						}
					}
				}
                if (ImGui::MenuItem("Wavefront (.obj)"))
                {
                    const std::string path = GetFileName(FILTER::OBJ_FILTER);
					if (!path.empty())
					{
						const auto dirWords = stringUtils::getWords(path, "\\");
						if (!dirWords.empty())
						{
							const auto newPath = stringUtils::combineWords(dirWords, "/");
							if (!newPath.empty())
							{
								AssetResource meshResource{ AssetType::MeshResource, newPath };
								physics::PhysicsState ps{};
								ps.orientation = glm::quat(0, 0, 0, 1);
								ps.position = glm::vec3(0, 0, -2);
								ps.mass = 10;

								auto assetHandle = m_AssetManager.GetResourceHandle(meshResource);
								auto& mesh = m_AssetManager.GetAsset<primitives::Mesh>(assetHandle);
								if (m_Scene)
								{
									auto character = m_Scene->CreateSceneObject();
									character->AddComponent<primitives::MeshInstance>(primitives::MeshInstance{ assetHandle });
									character->AddComponent<primitives::RenderComponent>(primitives::RenderComponent{ 0,0 });
									character->AddComponent<physics::PhysicsState>(ps);
								}
							}
						}
					}
                }

                if (ImGui::MenuItem("MeshPack"))
                {
                    const std::string path = GetFileName(FILTER::PAK_FILTER);
                    auto result = m_AssetManager.DeserializeMeshPack(path);
                }
                if (ImGui::MenuItem("MaterialPack"))
                {

                }
                if (ImGui::MenuItem("TexturePack"))
                {
                    const std::string path = GetFileName(FILTER::PAK_FILTER);

                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Export"))
            {
                if (ImGui::MenuItem("MeshPack"))
                {

                }
                if (ImGui::MenuItem("MaterialPack"))
                {

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

        if (ImGui::BeginMenu("Asset Manager"))
        {
            showAssetManagerWindow = true;
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (showCreateNewSceneWindow)
    {
        ImGui::Begin("New Scene");
        ImGui::InputText("File Name", newSceneNameBuffer, sizeof(char) * 512);
        static bool importAssets = false;
        meshPackBuffer[511] = '\0';
        materialPackBuffer[511] = '\0';
        texturePackBuffer[511] = '\0';
        if (ImGui::Checkbox("Import Assets", &importAssets));
        if (importAssets)
        {
            ImGui::Button("MeshPack", ImVec2(100, 19));
            ImGui::SameLine();
            ImGui::InputText("Mesh Pack Path", meshPackBuffer, sizeof(char) * 512);
            ImGui::SameLine();
            if (ImGui::Button("Add"))
            {

            }
            ImGui::Button("MaterialPack", ImVec2(100, 19));
            ImGui::SameLine();
            ImGui::InputText("Material Pack Path", materialPackBuffer, sizeof(char) * 512);
            ImGui::SameLine();
            if (ImGui::Button("Add"))
            {

            }
            ImGui::Button("TexturePack", ImVec2(100, 19));
            ImGui::SameLine();
            ImGui::InputText("Texture Pack Path", texturePackBuffer, sizeof(char) * 512);
            ImGui::SameLine();
            if (ImGui::Button("Add"))
            {

            }
        }
        if (ImGui::Button("Create"))
        {
            std::string fileName(newSceneNameBuffer);
            if (m_Scene)
            {
                m_Scene.reset();
            }
            if (!m_Scene)
            {
                m_Scene = std::make_shared<Scene>(fileName);
                std::string meshPack{ meshPackBuffer };
                std::string materialPack{ materialPackBuffer };
                std::string texturePack{ texturePackBuffer };
                if (!meshPack.empty())
                {
                    m_Scene->meshPack = meshPack;
                }
                if (!materialPack.empty())
                {
                    m_Scene->materialPack = materialPack;
                }
                if (!texturePack.empty())
                {
                    m_Scene->texturePack = texturePack;
                }
                showCreateNewSceneWindow = false;
            }
        }
        ImGui::End();
    }

    if (showAssetManagerWindow)
    {
        ImGui::Begin("Asset Manager");
        {
            if (ImGui::BeginTabBar("Asset Manager"))
            {
                if (ImGui::BeginTabItem("Shaders"))
                {
                    auto& shaderMap = m_AssetManager.m_ShaderMap;
                    static uint32_t selectedShader = -1;
                    for (auto& p : shaderMap)
                    {
                        auto& handle = p.first;
                        auto& shader = p.second;
                        auto platformHandle = shader.GetHandle();
                        ImGui::PushID(platformHandle);
                        if (ImGui::Selectable(std::format("{}{}", handle.m_HWORD, handle.m_LWORD).c_str()), selectedShader == platformHandle)
                        {
                        }
                        ImGui::PopID();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Materials"))
                {
                    
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Textures"))
                {
                    auto& textureMap = m_AssetManager.m_TextureMap;
                    static uint32_t selectedTexture = -1;
                    for (auto& t : textureMap)
                    {
                        auto& handle = t.first;
                        auto& texture = t.second;
                        auto& platformHandle = texture.m_Texture;
                        ImGui::PushID(platformHandle);
                        if (ImGui::Selectable(std::format("{}{}", handle.m_HWORD, handle.m_LWORD).c_str()), selectedTexture == platformHandle)
                        {

                        }
                        ImGui::PopID();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Meshes"))
                {
                    auto& meshMap = m_AssetManager.m_MeshMap;
                    static uint32_t selectedMesh = -1;
                    uint32_t index = 0;
                    for (auto& m : meshMap)
                    {
                        auto& handle = m.first;
                        auto& mesh = m.second;
                        ImGui::PushID(index);
                        if (ImGui::Selectable(std::format("{}{}", handle.m_HWORD, handle.m_LWORD).c_str()), selectedMesh == index)
                        {

                        }
                        ImGui::PopID();
						index++;
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Scripts"))
                {
                    auto& scriptMap = m_AssetManager.m_ScriptMap;
                    static uint32_t selectedScript = -1;
                    uint32_t index = 0;
                    for (auto& s : scriptMap)
                    {
                        auto& handle = s.first;
                        auto& scriptData = s.second;
                        ImGui::PushID(index);
                        if (ImGui::Selectable(std::format("{}{}", handle.m_HWORD, handle.m_LWORD).c_str()), selectedScript == index)
                        {

                        }
                        ImGui::PopID();
						index++;
                    }
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }
        ImGui::End();
    }
}

void BaseApplication::DrawComponentsPanel()
{
	ImGui::Begin("Components");
	// ---------- Character list ----------
	if (m_Scene != nullptr)
	{
		auto& activeScene = m_Scene;
		auto view = activeScene->GetView<entt::entity>();
		ImGui::TextUnformatted("Scene Characters");
		for (auto [entity] : view.each())
		{
			int i = static_cast<int>(entity);
			ImGui::PushID(i);
			if (ImGui::Selectable(std::format("Character {}", i).c_str(), selectedCharacter == i))
			{
				selectedCharacter = i;
				showEditCharacterMenu = true;
				showCharacterEditor = true;
				selectedEntity = entity;
			}
			ImGui::PopID();
		}
	}

	// ---------- Right-click trigger (no popup body here) ----------
	/**/
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
		ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("AddCharacter");
	}
	if (ImGui::BeginPopup("AddCharacter"))
	{
		//char buffer[512] = "";
		//if (ImGui::InputText("pathToFile", buffer, 512))
		//{
		//	createNewCharacterMeshPath = std::string(buffer);
		//	std::ifstream ifs(createNewCharacterMeshPath);
		//	if (!ifs.is_open())
		//	{
		//		std::cerr << "Unable to import " << createNewCharacterMeshPath  << std::endl;
		//		return;
		//	}
		//	ifs.close();
		//	showAddCharacterMenu = true;
		//}
		//
		//ImGui::SameLine();
		//if (ImGui::SmallButton("+"))
		//{
		//
		//}
		if (ImGui::Button("Add Character"))
		{
			m_Scene->CreateSceneObject();
			showAddCharacterMenu = true;
		}
		ImGui::EndPopup();
	}
	/**/

	if (showAddCharacterMenu && !createNewCharacterMeshPath.empty())
	{
		auto& activeScene = m_Scene;
		auto assetManager = activeScene->GetAssetManager();
		ImGui::Begin("Add Character");

		static float rotBuffer[4] = { 0, 0, 0, -1 };
		static float posBuffer[4] = { 0, 0, 0, 1 };
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
		static bool changeMeshInstance = false;
		auto& activeScene = m_Scene;
		auto entity = entt::entity((uint32_t)selectedCharacter);
		auto character = activeScene->GetSceneCharacter(entity);
		ImGui::Begin("Character Editor");
		if (character->HasComponent<physics::PhysicsState>())
		{
			auto& physicsState = character->GetComponent<physics::PhysicsState>();
			ImGui::TextUnformatted("Physics Component");
			float orientation[] = { physicsState.orientation.w, physicsState.orientation.x , physicsState.orientation.y , physicsState.orientation.z };
			float position[] = { physicsState.position.x , physicsState.position.y , physicsState.position.z , 1.0f };
			if (ImGui::InputFloat4("orientation", orientation))
			{

				physicsState.orientation = glm::quat(orientation[0], orientation[1], orientation[2], orientation[3]);
			}
			if (ImGui::InputFloat4("position", position))
			{
				physicsState.position.x = position[0];
				physicsState.position.y = position[1];
				physicsState.position.z = position[2];
			}
			if (ImGui::InputFloat("mass", &physicsState.mass));
		}

		if (character->HasComponent<primitives::MeshInstance>())
		{
			auto& meshInstance = character->GetComponent<primitives::MeshInstance>();
			const auto& handle = meshInstance.m_Handle;
			ImGui::TextUnformatted("Mesh Instance");
			ImGui::Separator();
			if (ImGui::Button(std::format("{}{}", handle.m_HWORD, handle.m_LWORD).c_str()))
			{
				ImGui::OpenPopup("##Available Meshes");
			}
			if (ImGui::BeginPopup("##Available Meshes"))
			{
				const auto& meshMap = m_AssetManager.m_MeshMap;
				uint32_t index = 0;
				for (auto& m : meshMap)
				{
					const auto& handle = m.first;
					ImGui::PushID(index);
					if (ImGui::Selectable(std::format("{}{}", handle.m_HWORD, handle.m_LWORD).c_str()))
					{
						meshInstance.m_Handle = handle;
					}
					ImGui::PopID();
					index++;
				}
				ImGui::EndPopup();
			}
		}

		if (character->HasComponent<scripting::ControlScript>())
		{
			auto& scriptInstance = character->GetComponent<scripting::ControlScript>();
			const auto& handle = scriptInstance.m_Handle;
			ImGui::TextUnformatted("Script Instance");
			ImGui::Separator();
			if (ImGui::Button(std::format("{}{}", handle.m_HWORD, handle.m_LWORD).c_str()))
			{
				ImGui::OpenPopup("##Available Scripts");
			}
			if (ImGui::BeginPopup("##Available Scripts"))
			{
				const auto& scriptMap = m_AssetManager.m_ScriptMap;
				uint32_t index = 0;
				for (auto& s : scriptMap)
				{
					auto& handle = s.first;
					ImGui::PushID(index);
					if (ImGui::Selectable(std::format("{}{}", handle.m_HWORD, handle.m_LWORD).c_str()))
					{
						scriptInstance.m_Handle = handle;
					}
					ImGui::PopID();
					index++;
				}
				ImGui::EndPopup();
			}
		}

		if (character->HasComponent<Character*>())
		{
			ImGui::TextUnformatted("Children");
		}

		if (character->HasComponent<primitives::Parent>())
		{
			const auto& parent = character->GetComponent<primitives::Parent>();
			ImGui::Button(std::format("Parent : Character {}", (uint32_t)parent.ParentEntity).c_str());
		}
		if (character->HasComponent<primitives::ParticleSystemInstance>())
		{
			ImGui::Button("Particle Instance");
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
					if (!(character->HasComponent<physics::PhysicsState>()))
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
							character->AddComponent<physics::PhysicsState>(ps);
						}
					}
					if (!character->HasComponent<primitives::MeshInstance>())
					{
						if (ImGui::Button("Mesh  Instance"))
						{

						}
					}
					if (!character->HasComponent<scripting::ControlScript>())
					{
						if (ImGui::Button("Script Instance"))
						{

						}
					}
					if (!character->HasComponent<primitives::ParticleSystemInstance>())
					{
						if (ImGui::Button("Particle Instance"))
						{
							ImGui::OpenPopup("Add Particle Instance");
						}
					}
				}
				if (ImGui::BeginPopup("Add Particle Instance"))
				{
					ImGui::Text("Select Particle System Configuration");
					for (int index = 0; index < m_AssetManager.m_ParticleSystems.size(); index++)
					{
						auto& psConfig = m_AssetManager.m_ParticleSystems[index];
						ImGui::PushID(index);
						if (ImGui::Selectable(std::format("Config {}", index).c_str(), false))
						{
							primitives::ParticleSystemInstance psInstance{};
							psInstance.m_AccumulatedTime = 0.0f;
							psInstance.m_Allocation.Offset = -1;
							psInstance.m_Allocation.Size = psConfig.m_MaxNumParticles;
							psInstance.m_ConfigIndex = static_cast<uint32_t>(index);
							psInstance.m_Duration = psConfig.m_Duration;
							psInstance.m_IsEnabled = true;
							psInstance.m_NumParticles = 0;
							psInstance.m_Restart = true;
							character->AddComponent<primitives::ParticleSystemInstance>(psInstance);
						}
						ImGui::PopID();
					}
					ImGui::EndPopup();
				}
			}

			if (characterRemoveComponent)
			{
				ImGui::Separator();
				ImGui::Text("Removing Components");
				ImGui::Separator();
				{
					if (character->HasComponent<physics::PhysicsState>())
					{
						if (ImGui::Button("PhysicsState  "))
						{
							character->RemoveComponent<physics::PhysicsState>();
						}
					}
					if (character->HasComponent<primitives::MeshInstance>())
					{
						if (ImGui::Button("Mesh  Instance"))
						{
							character->RemoveComponent<primitives::MeshInstance>();
						}
					}
					if (character->HasComponent<scripting::ControlScript>())
					{
						if (ImGui::Button("Script Instance"))
						{
							character->RemoveComponent<scripting::ControlScript>();
						}
					}
					if (character->HasComponent<primitives::ParticleSystemInstance>())
					{
						if (ImGui::Button("Particle Instance"))
						{
							character->RemoveComponent<primitives::ParticleSystemInstance>();
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

void BaseApplication::DrawParticleSystemPanel()
{
	auto& activeScene = m_Scene;
	if (activeScene != nullptr)
	{
		ImGui::Begin("Particle System Panel");
		ImGui::TextUnformatted("Particle Systems");
		ImGui::SameLine();
		if (ImGui::SmallButton("+"))
		{
			ImGui::OpenPopup("AddSystemPopup");
		}
		ImGui::Separator();
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
			ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("AddSystemPopup");
		}
		if (ImGui::BeginPopup("AddSystemPopup"))
		{
			newParticleConfigName[511] = '\0';
			if (ImGui::InputText("Name", newParticleConfigName, 512));
			ImGui::SameLine();
			if(ImGui::Button("Add"))
			{
				auto currentDirectory = std::filesystem::current_path();
				std::string fmtPath = std::format("{}/{}.psconfig", currentDirectory.generic_string(), newParticleConfigName);
				const auto assetHandle = CreateAssetHandleFromPath(fmtPath.c_str());

				primitives::ParticleSystemConfig psConfig{};
				psConfig.m_EmitterInfo.m_Flags.m_IsEnabled = false;
				psConfig.m_EmitterInfo.m_Shape = primitives::EmitterShape::POINT;
				psConfig.m_MaxNumParticles = 2048;
				psConfig.m_ParticleRate = 1;
				psConfig.m_EmitterInfo.m_VectorOne = glm::vec4(0, 0, 0, 0);
				psConfig.m_EmitterInfo.m_VectorTwo = glm::vec4(0, 0, 0, 0);
				psConfig.m_Duration = 60.0f;

				std::string combinedShaderPaths = "C:/dev/Spring/Assets/Shaders/ParticleCompute.glsl";
				AssetResource shaderResource{ AssetType::ComputeShaderResource, combinedShaderPaths };
				psConfig.m_ShaderHandle = m_AssetManager.GetResourceHandle(shaderResource);
				m_AssetManager.m_ParticleSystems.push_back(psConfig);
				selectedParticleSystem = m_AssetManager.m_ParticleSystems.size() - 1;
				showParticleEditor = true;
			}
			ImGui::EndPopup();
		}
		
		for (int index = 0; index < m_AssetManager.m_ParticleSystems.size(); index++)
		{
			ImGui::PushID(index);
			if (ImGui::Selectable(std::format("Config {}", index).c_str(), index == selectedParticleSystem))
			{
				selectedParticleSystem = index;
				showParticleEditor = true;
			}
			ImGui::PopID();
		}
		ImGui::End();
	}
}

void BaseApplication::DrawParticleSystemEditor()
{
	// ---------- Editor window ----------
	if (showParticleEditor && (selectedParticleSystem > -1))
	{		
		auto& ps = m_AssetManager.m_ParticleSystems[selectedParticleSystem];
		int maxNumParticles = ps.m_MaxNumParticles;
		int particleRate = ps.m_ParticleRate;
		float emitterPosition[] = { ps.m_EmitterInfo.m_VectorOne.x, ps.m_EmitterInfo.m_VectorOne.y , ps.m_EmitterInfo.m_VectorOne.z };
		float emitterOrientation[] = { ps.m_EmitterInfo.m_VectorTwo.x, ps.m_EmitterInfo.m_VectorTwo.y, ps.m_EmitterInfo.m_VectorTwo.z };
		float w1 = ps.m_EmitterInfo.m_VectorOne.w;
		float w2 = ps.m_EmitterInfo.m_VectorTwo.w;

		ImGui::Begin("Particle System Editor", &showParticleEditor);
		//ImGui::Checkbox("Enabled", &ps.m_EmitterInfo.m_Flags.m_IsEnabled);
		//ImGui::Separator();
		ImGui::TextUnformatted("Texture Handle");
		if (ImGui::Button(std::format("{}{}", ps.m_TextureHandle.m_HWORD, ps.m_TextureHandle.m_LWORD).c_str())) 
		{
		}
		ImGui::SameLine();
		if(ImGui::Button("Change"))
		{
			ImGui::OpenPopup("##Available Textures");
		}
		if (ImGui::BeginPopup("##Available Textures"))
		{
			const auto& textureMap = m_AssetManager.m_TextureMap;
			int id = 0;
			for (const auto& t : textureMap)
			{
				const auto& handle = t.first;
				ImGui::PushID(id);
				if (ImGui::Selectable(std::format("{}{}", handle.m_HWORD, handle.m_LWORD).c_str()))
				{
					ps.m_TextureHandle = handle;
				}
				ImGui::PopID();
				id++;
			}
			ImGui::EndPopup();
		}

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
					ps.m_EmitterInfo.m_Shape = (primitives::EmitterShape)index;
				}
			}
			ImGui::EndPopup();
		}

		selectedShape = (int)ps.m_EmitterInfo.m_Shape;

		if (selectedShape == 0)														// Point Emitter
		{
			ImGui::InputFloat3("Position", emitterPosition);
			ImGui::Checkbox("Random Orientation", &ps.m_EmitterInfo.m_Flags.m_RandomOrientation);
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
			ps.m_EmitterInfo.m_VectorOne.w = w1;

			ps.m_EmitterInfo.m_VectorTwo.x = emitterOrientation[0];
			ps.m_EmitterInfo.m_VectorTwo.y = emitterOrientation[1];
			ps.m_EmitterInfo.m_VectorTwo.z = emitterOrientation[2];
			ps.m_EmitterInfo.m_VectorTwo.w = w2;
		}

		float initialSpeed = ps.m_InitialSpeed;
		float colorOne[] = { 0,0,0, 1 };
		float colorTwo[] = { 0.45f, 0.55f, 0.45f, 1.0f };
		float colorThree[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		static bool editSizeOverLifetime = false;
		static bool editColorOverLifetime = false;
		ImGui::Separator();
		ImGui::TextUnformatted("Particle Lifecycle");
		ImGui::InputInt("Max Particles", &maxNumParticles);
		ImGui::InputInt("Particles per second", &particleRate);
		ImGui::InputFloat("Duration", &ps.m_Duration);
		ImGui::InputFloat("Initial Speed", &ps.m_InitialSpeed);
		if (ImGui::Button("SizeOverLifetime"))
		{
			if (editSizeOverLifetime)
			{
				editSizeOverLifetime = false;
			}
			else if (!editSizeOverLifetime)
			{
				editSizeOverLifetime = true;
			}
		}
		if (editSizeOverLifetime)
		{
			static float initialSize = 1.0f;
			static float finalSize = 1.0f;
			if (ImGui::InputFloat("Initial Size", &initialSize));
			if (ImGui::InputFloat("Final Size", &finalSize));

			ImGui::PlotLines("Size over lifetime", colorTwo, 3, 0);
		}
		if (ImGui::Button("Speed over lifetime"));
		if (ImGui::Checkbox("Color over lifetime", &editColorOverLifetime));

		if (editColorOverLifetime)
		{
			ImGui::ColorEdit4("Color 1", colorOne);
			ImGui::ColorEdit4("Color 2", colorTwo);
			ImGui::ColorEdit4("Color 3", colorThree);
		}
		ImGui::Separator();
		if (particleRate >= 0)
		{
			ps.m_ParticleRate = particleRate;
		}
		if (maxNumParticles >= 0)
		{
			ps.m_MaxNumParticles = maxNumParticles;
		}
		ImGui::End();
	}
}

void BaseApplication::Run()
{	
    m_pActiveRenderer = std::make_shared<Renderer>();
    m_pUILayer.BindRenderer(m_pActiveRenderer);
    m_pUILayer.m_pActiveCamera->SetEye(glm::vec3(0, 0, 20));
    m_pUILayer.m_pActiveCamera->SetCenter(glm::vec3(0, 0, 0));
    m_pUILayer.m_pActiveCamera->SetOrientation(glm::quat(1, 0, 0, 0));
    m_pUILayer.m_pActiveCamera->OnInit();

    //scripting::ScriptMgr::expose_scene_camera(m_Scene->GetLuaState(), m_pUILayer.m_pActiveCamera.get(), "camera");
    m_LobbyGraphicsShaderHandle = AssetHandle{ 3531024263087085773, 10156511931093447336 }; //LevelOne
    m_ParticleGraphicsShaderHandle = AssetHandle{ 6016858150360079293, 10861247056247163883 };
	m_AssetManager.DeserializeScriptPack("C:/dev/Astron Battles/Assets/LevelOne_asset_SCRP.pak");
	m_AssetManager.DeserializeParticlePack("C:/dev/Astron Battles/Assets/LevelOne_asset_PSCP.pak");
    if (m_Scene)
    {
        m_Scene->SetAssetManager(&m_AssetManager);
        m_Scene->LoadSceneFromFile();
        if (!m_Scene->asset_pack.empty())
        {
            m_AssetManager.Deserialize(m_Scene->asset_pack);
        }
		if (!m_Scene->meshPack.empty())
		{
			//m_AssetManager.DeserializeMeshPack(m_Scene->meshPack);
		}
		if (!m_Scene->materialPack.empty())
		{
			m_AssetManager.DeserializeMaterialPack(m_Scene->materialPack);
		}
		if (!m_Scene->texturePack.empty())
		{
			//m_AssetManager.DeserializeTexturePack(m_Scene->texturePack);
		}
		if (!m_Scene->shaderPack.empty())
		{
			//m_AssetManager.DeserializeShaderPack(m_Scene->shaderPack);
		}
        m_Scene->OnInit();
        m_Scene->OnCreateSceneObjects();
        m_pUILayer.LoadScene(m_Scene);
    }
    
    while (!m_ExitWindow)
    {
        m_pUILayer.BeginFrame();
        m_pActiveRenderer->BeginFrame();

        m_pUILayer.Enable();

        DrawMenuBarPanel();
		DrawComponentsPanel();
		DrawParticleSystemPanel();
		DrawParticleSystemEditor();
        OnUpdate();
        if (m_Scene)
        {
            DrawSceneCharacters(m_AssetManager);
            DrawParticleSystems(m_AssetManager);
        }

        m_pActiveRenderer->OnUpdate(updateInterval);
        m_pUILayer.OnUpdate(updateInterval);

        m_pActiveRenderer->EndFrame();
        m_pUILayer.EndFrame();

        m_AppWindow.SwapBuffer();
    }
    ShutDown();
}

void BaseApplication::OnUpdate()
{
    m_AppWindow.OnUpdate();
    if(m_Scene)
    {
        m_pActiveRenderer->ParticleSystemSync(m_Scene.get());
    }
}

void BaseApplication::ShutDown()
{

}

void BaseApplication::OnMouseMove(event::MouseMoveEvent& event)
{
}

void BaseApplication::OnKeyPress(event::KeyPressEvent& key_press)
{
}

void BaseApplication::OnPlaySound(primitives::PlaySoundRequest& sound_event)
{
    auto& assetHandle = sound_event.SoundID;
    auto& sound = m_AssetManager.GetSound(assetHandle);
    sound.playing = true;
}

void BaseApplication::DrawSceneCharacters(AssetManager& asset_manager)
{
    std::vector<RenderCommand> commandQueue;
    VertexBuffer geometryBuffer;
    auto meshView = m_Scene->GetView<physics::PhysicsState, primitives::MeshInstance>();
    for (auto [entity, physicsComponent, meshInstance] : meshView.each())
    {
        RenderCommand cmd;
        auto& mesh = m_AssetManager.GetMesh(meshInstance.m_Handle);
        auto& material = m_AssetManager.GetMaterial(meshInstance.m_Handle);
        geometryBuffer.UpdateBuffer(mesh.m_V);
        uint64_t enableBits{ 0 };
        enableBits |= (1ULL << 0); // Enable CULL_FACE
        enableBits |= (1ULL << 4); // Enable DEPTH_TEST
        cmd.m_EnableBits = enableBits;
        cmd.m_EntityID = static_cast<uint64_t>(entity);
        cmd.m_ShaderHandle = m_LobbyGraphicsShaderHandle;
        cmd.m_MaterialHandle = meshInstance.m_Handle;
        cmd.m_VertexBufferOffset = geometryBuffer.m_BufferOffset;
        cmd.m_IndexBufferOffset = geometryBuffer.m_BufferElementCount;
        cmd.m_CommandSize = static_cast<uint32_t>(sizeof(primitives::Vertex) * mesh.m_V.size());
        cmd.m_IndexCount = static_cast<uint32_t>(mesh.m_V.size());
        cmd.m_PrimitiveType = GL_TRIANGLES;
        cmd.m_Viewport[0] = 0;
        cmd.m_Viewport[1] = 0;
        cmd.m_Viewport[2] = 1920;
        cmd.m_Viewport[3] = 1080;
        cmd.m_UniformBuffer.m_Mat4Map["Model"] = glm::translate(glm::mat4(1.0f), physicsComponent.position) * glm::mat4_cast(physicsComponent.orientation);
        cmd.m_UniformBuffer.m_Mat4Map["View"] = m_pUILayer.m_pActiveCamera->GetV();
        cmd.m_UniformBuffer.m_Mat4Map["Projection"] = m_pUILayer.m_pActiveCamera->GetP();
        commandQueue.push_back(cmd);
    }

    m_pActiveRenderer->DrawBuffer(commandQueue, geometryBuffer, asset_manager);
}

void BaseApplication::DrawParticleSystems(AssetManager& asset_manager)
{
    std::vector<ParticleCommand> commandQueue;
    auto view = m_pUILayer.m_pActiveCamera->GetV();
    auto proj = m_pUILayer.m_pActiveCamera->GetP();
    auto particleView = m_Scene->GetView<primitives::ParticleSystemInstance>();
    for (auto [entity, particleSystemInstance] : particleView.each())
    {
        ParticleCommand cmd;
		auto& psConfig = m_AssetManager.m_ParticleSystems[particleSystemInstance.m_ConfigIndex];
        if (particleSystemInstance.m_IsEnabled)
        {
            cmd.m_BufferOffset = particleSystemInstance.m_Allocation.Offset;
            cmd.m_NumParticles = particleSystemInstance.m_NumParticles;
            cmd.m_ShaderHandle = m_ParticleGraphicsShaderHandle;
			//cmd.m_ShaderHandle = psConfig.m_ShaderHandle;
			cmd.m_TextureHandle = psConfig.m_TextureHandle;
            cmd.m_UniformBuffer.m_FloatMap["bufferOffset"] = static_cast<float>(particleSystemInstance.m_Allocation.Offset);
            cmd.m_UniformBuffer.m_Mat4Map["View"] = view;
            cmd.m_UniformBuffer.m_Mat4Map["Proj"] = proj;
            commandQueue.push_back(cmd);
        }
    }
    m_pActiveRenderer->DrawParticles(commandQueue, asset_manager);
}

BaseApplication::BaseApplication(uint32_t width, uint32_t height, const char* title)
    :
    m_AppWindow{width, height, title},
    m_ExitWindow{false},
    //m_Scene{},
    m_Scene{std::make_shared<Scene>("C:/dev/Astron Battles/Assets/Scripts/LevelOne.lua")},
    m_pUILayer{std::shared_ptr<Win32Window>(&m_AppWindow)},
    m_AssetManager{}
{
    m_AppWindow.OnEvent = &event::Dispatcher::Dispatch;
    event::Dispatcher::RegisterListener(*this);
    event::Dispatcher::RegisterListener(m_pUILayer);

    newSceneNameBuffer = new char[512];
    memset(newSceneNameBuffer, 0, sizeof(char) * 512);
    meshPackBuffer = new char[512];
    memset(meshPackBuffer, 0, sizeof(char) * 512);
    materialPackBuffer = new char[512];
    memset(materialPackBuffer, 0, sizeof(char) * 512);
    texturePackBuffer = new char[512];
    memset(texturePackBuffer, 0, sizeof(char) * 512);
	newParticleConfigName = new char[512];
	memset(newParticleConfigName, 0, sizeof(char) * 512);
}

BaseApplication::~BaseApplication()
{
    delete[] newSceneNameBuffer;
    delete[] meshPackBuffer;
    delete[] materialPackBuffer;
    delete[] texturePackBuffer;
}
