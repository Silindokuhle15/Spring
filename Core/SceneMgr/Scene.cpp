#include "Scene.h"
#include "Character.h"

Scene::Scene(const std::string& path)
	:
	m_Ts{0.0f},
	m_AccumulatedTime{0.0f},
	m_Title{ path },
	m_AssetManager{nullptr}
{
}

bool Scene::Serialize()
{
	auto title = stringUtils::GetFilenameWithoutExtension(m_Title);
	title += "_scene.pak";
	std::ofstream ofs(title, std::ios::out);
	if (!ofs.is_open())
	{
		std::cerr << "Failed to Open " << title << std::endl;
		return false;
	}
	using json = nlohmann::json;
	json jScene;
	jScene["characters"] = json::array();
	jScene["children"] = json::array();
	
	auto physicsView = GetView<primitives::MeshInstance, physics::PhysicsState, scripting::ControlScript>();
	for (auto [entity, meshInstance, physicsState, controlScript] : physicsView.each())
	{
		json jCharacter;
		jCharacter["id"] = (int)entity;
		jCharacter["mesh"] = meshInstance.m_Handle;
		jCharacter["script"] = controlScript;
		jCharacter["physics"] = physicsState;
		jScene["characters"].push_back(jCharacter);
	}
	/**/
	auto parentView = GetView<primitives::Parent, primitives::ParticleSystemInstance, std::string>();
	for (auto [entity, parent, particleSystemInstance, tag] : parentView.each())
	{
		json jCharacter;
		jCharacter["id"] = (int)entity;
		jCharacter["tag"] = tag;
		jCharacter["parent"] = (int)parent.ParentEntity;
		//jCharacter["particle_system"] = particleSystem;
		jScene["children"].push_back(jCharacter);
	}
	/**/
	ofs << jScene.dump(4);
	ofs.close();
	std::cout << title << " created!!!" << std::endl;
	return true;
}

bool Scene::Deserialize()
{
	using json = nlohmann::json;
	json jScene;
	std::ifstream ifs(scene_blob);
	if (!ifs.is_open())
	{
		std::cerr << "Failed to open " << m_Title << std::endl;
		return false;
	}
	ifs >> jScene;
	for (auto& jCharacter : jScene["characters"])
	{
		auto id = jCharacter["id"].get<int>();
		auto meshHandle = jCharacter["mesh"].get<AssetHandle>();
		auto physicsState = jCharacter["physics"].get<physics::PhysicsState>();
		auto scriptResource = jCharacter["script"].get<scripting::ControlScript>();
		auto ch = CreateSceneObject((uint32_t)id);
		ch->AddComponent<primitives::MeshInstance>(primitives::MeshInstance{ meshHandle });
		ch->AddComponent<physics::PhysicsState>(physics::PhysicsState{ physicsState });
		scripting::ControlScript script{ scriptResource.m_Handle, scriptResource.m_Data };
		ch->AddComponent<primitives::RenderComponent>(primitives::RenderComponent{});
		m_ScriptSystem.InitScript(ch, script);
		ch->AddComponent<scripting::ControlScript>(script);
	}

	for (auto& jCharacter : jScene["children"])
	{
		auto id = jCharacter["id"].get<int>();
		auto tag = jCharacter["tag"].get<std::string>();
		auto parent = jCharacter["parent"].get<int>();
		//auto particleInstance = jCharacter["particle_system"].get<primitives::ParticleSystemInstance>();

		auto child = CreateSceneObject((uint32_t)id);
		child->AddComponent<std::string>(tag);
		child->AddComponent<primitives::Parent>(primitives::Parent{ entt::entity(parent) });
		//child->AddComponent<primitives::ParticleSystem>(particleSystem);
	}
	ifs.close();
	return true;
}

bool Scene::IsValidCharacter(entt::entity& id)
{
	return m_Registry.valid(id);
}

Character* Scene::CreateSceneObject(uint32_t hint)
{
	auto entity = m_Registry.create(entt::entity(hint));
	auto newCharacter = m_CharacterAllocator.allocate(entity, this);
	m_EntityCharacterMap[entity] = newCharacter;
	return newCharacter;
}

void Scene::DestroySceneObject(entt::entity id)
{
	auto isValidEntity = m_Registry.valid(id);
	if (isValidEntity && !m_Registry.any_of<primitives::DestructComponent>(id))
	{
		m_Registry.emplace<primitives::DestructComponent>(id);
		m_EntityCharacterMap[id] = nullptr;
	}
}

Character* Scene::GetSceneCharacter(entt::entity& id)
{
	auto isValidEntity = m_Registry.valid(id);
	if (isValidEntity)
	{
		return m_EntityCharacterMap[id];
	}
	return nullptr;
}

void Scene::OnCreateSceneObjects()
{
	/*
	for (size_t index = 0; index < dynamic_mesh_paths.size(); index++)
	{
		AssetResource meshR{ AssetType::MeshResource, dynamic_mesh_paths[index] };
		auto meshHandle = m_AssetManager->GetResourceHandle(meshR);
		auto* ch = CreateSceneObject();
		primitives::MeshInstance meshInstance{ meshHandle };
		ch->AddComponent<primitives::MeshInstance>(meshInstance);
		ch->AddComponent<primitives::RenderComponent>(primitives::RenderComponent{ 0, 0 });
		ch->AddComponent<std::string>(m_TempNames[index]);

		AssetResource scriptResource{ AssetType::ScriptResource, m_TempControlScripts[index] };
		auto scriptHandle = m_AssetManager->GetResourceHandle(scriptResource);
		auto scriptData = ReadLuaScriptFromDisk(m_TempControlScripts[index]);
		scripting::ControlScript script{ scriptHandle, scriptData};
		scripting::ScriptMgr::InitScript(lua_state, ch, script);
		scripting::ScriptMgr::CallOnInit(lua_state, ch->GetCharacterID(), script);
		ch->AddComponent<scripting::ControlScript>(script);

		auto parentEntity = ch->GetCharacterID();
		auto* child = CreateSceneObject();
		auto childEntity = child->GetCharacterID();
		child->AddComponent<std::string>(std::string{ "Child" });
		child->AddComponent<primitives::Parent>(primitives::Parent{ parentEntity });

		std::string combinedShaderPaths = "C:/dev/Spring/Assets/Shaders/ParticleCompute.glsl";
		AssetResource shaderResource{ AssetType::ComputeShaderResource, combinedShaderPaths };
		auto computeHandle = m_AssetManager->GetResourceHandle(shaderResource);
		primitives::ParticleSystem ps;
		ps.m_EmitterInfo.m_Shape = primitives::EmitterShape::CONE;
		ps.m_EmitterInfo.m_Flags.m_IsEnabled = true;
		ps.m_EmitterInfo.m_Flags.m_Fill = false;
		ps.m_EmitterInfo.m_VectorOne = glm::vec4(0, -5, 0, 15.0f);
		ps.m_EmitterInfo.m_VectorTwo = glm::vec4(0, 1, -1, 10.0f);
		ps.m_Ts = 0.0f;
		ps.m_AccumulatedTime = 0.0f;
		ps.m_Duration = 100.0f;
		ps.m_BufferOffset = -1;
		ps.m_NumParticles = 0;
		ps.m_MaxNumParticles = 1024;
		ps.m_ParticleRate = 2;
		ps.m_ShaderHandle = computeHandle;
		child->AddComponent<primitives::ParticleSystem>(ps);
	}
	//*/
	Deserialize();
}

void Scene::OnInit()
{
	m_ScriptSystem.ExposeScene(this, "Scene");
}

void Scene::OnUpdate(float ts)
{
	m_Ts = ts;
	m_ScriptSystem.Update(*this, m_Ts);
	m_PhysicsSystem.Update(*this, m_Ts);
	m_AccumulatedTime += m_Ts;
}

int Scene::LoadSceneFromFile()
{	
	std::string str{ "" };
	scripting::ConfigScript m_LuaEngine{m_Title};
	auto pLuaState = m_LuaEngine.GetLuaState();
	std::vector<std::string> dynamic_keys{ "Mesh", "script", "name" };
	std::vector<std::string> keys({
			"dynamic_geometry",
			"static_geometry",
			"shader",
			"asset_pack",
			"scene_blob",
			"meshPack",
			"materialPack",
			"texturePack",
			"shaderPack",
			"soundPack",
			"scriptPack",
			"particlePack"
		}
	);
	m_LuaEngine.Run();
	for (auto& var : keys)
	{
		lua_getglobal(pLuaState, var.c_str());
		switch (lua_type(pLuaState, -1))
		{
			case LUA_TTABLE:
				if (var == "static_geometry")
				{
					auto n = luaL_len(pLuaState, -1);
					for (auto i = 1; i <= n; i++)
					{
						auto index = lua_geti(pLuaState, -1, i);
						str = lua_tostring(pLuaState, -1);
						static_mesh_paths.push_back(str);
						lua_pop(pLuaState, 1);
					}
					break;
				}
				if (var == "dynamic_geometry")
				{
					auto n = luaL_len(pLuaState, -1);
					for (auto i = 1; i <= n; i++)
					{
						auto member = lua_rawgeti(pLuaState, -i, i);
						{
							for (auto& key : dynamic_keys)
							{
								lua_getfield(pLuaState, -1, key.c_str());
								if (!key.compare("Mesh"))
								{
									str = lua_tostring(pLuaState, -1);
									dynamic_mesh_paths.push_back(str);
									lua_pop(pLuaState, 1);
								}
								else if (!key.compare("script"))
								{
									str = lua_tostring(pLuaState, -1);
									m_TempControlScripts.push_back(str);
									lua_pop(pLuaState, 1);
								}
								else if (!key.compare("name"))
								{
									str = lua_tostring(pLuaState, -1);
									m_TempNames.push_back(str);
									lua_pop(pLuaState, 1);
								}
							}
						}
					}
				}
				if (var == "shader")
				{
					if (lua_getfield(pLuaState, -1, "VShaderPath"))
					{
						str = lua_tostring(pLuaState, -1);
						shader_paths.push_back(str);
						lua_pop(pLuaState, 1);
					}
					if (lua_getfield(pLuaState, -1, "FShaderPath"))
					{
						str = lua_tostring(pLuaState, -1);
						shader_paths.push_back(str);
						lua_pop(pLuaState, 1);
					}
					break;
				}
				break;

			case LUA_TSTRING:
				if (var == "asset_pack")
				{
					str = lua_tostring(pLuaState, -1);
					asset_pack = str;
					lua_pop(pLuaState, 1);
				}
				if (var == "scene_blob")
				{
					str = lua_tostring(pLuaState, -1);
					scene_blob = str;
					lua_pop(pLuaState, 1);
				}
				if (var == "meshPack")
				{
					str = lua_tostring(pLuaState, -1);
					meshPack = str;
					lua_pop(pLuaState, 1);
				}
				if (var == "materialPack")
				{
					str = lua_tostring(pLuaState, -1);
					materialPack = str;
					lua_pop(pLuaState, 1);
				}
				if (var == "texturePack")
				{
					str = lua_tostring(pLuaState, -1);
					texturePack = str;
					lua_pop(pLuaState, 1);
				}
				if (var == "shaderPack")
				{
					str = lua_tostring(pLuaState, -1);
					shaderPack = str;
					lua_pop(pLuaState, 1);
				}
				if (var == "soundPack")
				{
					str = lua_tostring(pLuaState, -1);
					soundPack = str;
					lua_pop(pLuaState, 1);
				}
				if (var == "scriptPack")
				{
					str = lua_tostring(pLuaState, -1);
					scriptPack = str;
					lua_pop(pLuaState, 1);
				}
				if (var == "particlePack")
				{
					str = lua_tostring(pLuaState, -1);
					particlePack = str;
					lua_pop(pLuaState, 1);
				}
				break;
		}
	}
	return 0;
}
