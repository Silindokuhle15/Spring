#include "Scene.h"
#include "Character.h"

Scene::Scene(const std::string& path)
	:
	m_Ts{0.0f},
	m_AccumulatedTime{0.0},
	m_Title{ path },
	m_State{ SceneState::LOADING },
	m_Script{ path },
	m_pLuaState{ nullptr },
	m_MousePosition{0.5f, 0.5f}
{
	m_pLuaState = luaL_newstate();
	luaL_openlibs(m_pLuaState);
}

Character* Scene::CreateSceneObject()
{
	Character* newCharacter{ new Character(m_Registry.create(), this) };
	m_Characters.push_back(newCharacter->GetCharacterID());
	return newCharacter;
}

void Scene::DestroySceneObject(entt::entity id)
{
	auto isValidEntity = m_Registry.valid(id);
	auto isValidCharacter = std::find(m_Characters.begin(), m_Characters.end(), id);
	if ((isValidEntity && (isValidCharacter != m_Characters.end())))
	{
		m_Registry.destroy(id);
		m_Characters.erase(isValidCharacter);
	}
}

Character Scene::GetSceneCharacter(entt::entity& id)
{
	return Character::GetCharacterPtr(id, this);
}

void Scene::SetMousePosition(const glm::vec2& mouse_position)
{
	m_MousePosition = mouse_position;
}

const glm::vec2 Scene::GetMousePosition() const
{
	return m_MousePosition;
}

void Scene::OnCreateSceneObjects()
{
	CreateShaders();
	auto lua_state = GetLuaState();
	for (size_t index = 0; index < dynamic_mesh_paths.size(); index++)
	{
		AssetResource meshR{ AssetType::MeshResource, dynamic_mesh_paths[index] };
		auto meshHandle = m_AssetManager.GetResourceHandle(meshR);
		MeshInstance meshInstance{ meshHandle };

		auto* ch = CreateSceneObject();
		ch->AddComponent<MeshInstance>(meshInstance);
		scripting::ControlScript script{ m_TempControlScripts[index] };
		ch->AddComponent<scripting::ControlScript>(script);
		ch->AddComponent<RenderComponent>(RenderComponent{ 0, 0 });
		auto name = m_TempNames[index].c_str();
		scripting::ScriptMgr::expose_character(lua_state, ch, name);
		scripting::ScriptMgr::ExecuteScript(lua_state, script.data.data(), script.data.size(), name);
	}
}

bool Scene::CreateBoundingVolumeHierarchy(std::vector<physics::PhysicsState>& states)
{
	return false;
}

void Scene::CreateShaders()
{
	for (size_t index = 0; index < shader_paths.size(); index += 2)
	{
		auto combinedPaths = shader_paths[index] + "\n" + shader_paths[index + 1];
		AssetResource shaderResource{ AssetType::ShaderResource, combinedPaths };
		auto shaderHandle = m_AssetManager.GetResourceHandle(shaderResource);
	}
}

void Scene::OnInit()
{
	m_State = SceneState::LOADING;
	scripting::ScriptMgr::register_scene(m_pLuaState);
	scripting::ScriptMgr::register_character(m_pLuaState);
	scripting::ScriptMgr::register_vector3(m_pLuaState);
	scripting::ScriptMgr::register_physicsstate(m_pLuaState);

	auto filename = scripting::ScriptMgr::GetLuaFilenameWithoutExtension(m_Title);
	scripting::ScriptMgr::expose_scene(m_pLuaState, this, filename.c_str());

	m_State = SceneState::RUNNING;
}

void Scene::OnUpdate(TimeStep ts)
{
	entries.clear();
	bounds.clear();
	collisionPairs.clear();

	switch (m_State)
	{
	case SceneState::LOADING:
		break;

	case SceneState::RUNNING:
		break;

	case SceneState::PAUSED:
		break;

	case SceneState::END:
		break;

	case SceneState::STOPPED:
		break;
	}

	auto scriptView = m_Registry.view<scripting::ControlScript>();
	// use a callback
	scriptView.each([&](const auto& script) 
	{
		scripting::ScriptMgr::ExecuteScriptFunction(m_pLuaState, script.data.c_str(), "onUpdate", ts);
	});

	auto phzxView = m_Registry.view<physics::PhysicsState>();
	float minX{ -1.0f }, minY{ -1.0f }, minZ{ 0.0f };
	float maxX{ +1.0f }, maxY{ +1.0f }, maxZ{ 1.0f };
	float scale = 10.0f;
	uint32_t bits = 21;
	for (auto [entity, phzx_state] : phzxView.each())
	{
		phzx_state.velocity += phzx_state.linear_acceleration * (float)ts;
		phzx_state.position += phzx_state.velocity * (float)ts;

		auto& pos = phzx_state.position;
		auto y = static_cast<uint64_t>(glm::floor((pos.x - minX) / (maxX - minX) * glm::pow(2, bits)));
		auto z = static_cast<uint64_t>(glm::floor((pos.y - minY) / (maxY - minY) * glm::pow(2, bits)));
		auto x = static_cast<uint64_t>(glm::floor((pos.z - minZ) / (maxZ - minZ) * glm::pow(2, bits)));
		entries.push_back(BVEntry{ static_cast<uint64_t>(entity), morton_encode_3d32(x, y, z) });

		BoundingVolume bbox = BoundingVolume::CreateBoundingVolume(static_cast<uint64_t>(entity), pos, 1.0f);
		auto& min = bbox.GetMin();
		auto& max = bbox.GetMax();
		bounds.push_back(Bound3D{ min.x, min.y, min.z, max.x, max.y, max.z });
	}

	m_AccumulatedTime += (float)ts;
	auto node = create_tree<Bound3D>(entries, bounds);

	for (auto& bound : bounds)
	{
		auto collisions = detect_overlapping_bounds<Bound3D>(bound, node);
		if (collisions.size() == 2)
		{
			uint64_t first = collisions[0];
			uint64_t second = collisions[1];
			if ((first <= m_Characters.size() - 1) && (second <= m_Characters.size() - 1))
			{
				std::pair<entt::entity, entt::entity> pair(m_Characters[first], m_Characters[second]);
				collisionPairs.push_back(pair);
				//print_tree<Bound3D>(node);
			}
		}
	}
}

void Scene::Run()
{
	//m_State = SceneState::RUNNING;
}

void Scene::OnEnd()
{
	//m_State = SceneState::END;
}

void Scene::OnPause()
{
	//m_State = SceneState::PAUSED;
}

void Scene::OnStop()
{
	//m_State = SceneState::STOPPED;
}

void Scene::OnReload()
{
	//m_State = SceneState::LOADING;
}

int Scene::LoadSceneFromFile()
{	
	scripting::ConfigScript m_LuaEngine;

	m_LuaEngine.SetScriptPath(m_Title);

	m_LuaEngine.SetKeys(
		std::vector<std::string>({
			"WIDTH",
			"HEIGHT",
			"dynamic_geometry",
			"static_geometry",
			//CAMERA
			"scene_camera",
			// TEXTUTES
			"shader"
			}
		)
	);

	m_LuaEngine.Run();
	std::vector<const char*> vec3_keys =
	{
		"x",
		"y",
		"z"
	};
	std::vector<const char*> vec4_keys =
	{
		"x",
		"y",
		"z",
		"w"
	};
	Vector3 vec3 = {};
	Vector4 vec4 = {};
	std::string str = "";
	lua_Number number = 0;

	auto pLuaState = m_LuaEngine.GetLuaState();

	for (auto& var : m_LuaEngine.Keys)
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
			else if (var == "dynamic_geometry")
			{
				physics::PhysicsState ps = {};
				auto n = luaL_len(pLuaState, -1);
				std::vector<std::string> dynamic_keys = { "Mesh", "script", "name","orientation", "position", "mass" ,"velocity", "restitution","angular_acceleration" ,"inertia"};
				for (auto i = 1; i <= n; i++)
				{
					auto b = 1;
					auto member = lua_rawgeti(pLuaState, -i, i);
					//while (member = lua_rawgeti(m_LuaEngine.m_pLuaState, -i, b))
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
							/**/
							else if (!key.compare("position"))
							{
								m_LuaEngine.GetField3fv(pLuaState, vec3_keys, &vec3);
								ps.position = glm::vec3(vec3.x, vec3.y, vec3.z);
								lua_pop(pLuaState, 1);

							}
							else if (!key.compare("orientation"))
							{
								m_LuaEngine.GetField4fv(pLuaState, vec4_keys, &vec4);
								ps.orientation = glm::quat(vec4.w, vec4.x, vec4.y, vec4.z);
								lua_pop(pLuaState, 1);
							}
							else if (!key.compare("mass"))
							{
								number = lua_tonumber(pLuaState, -1);
								ps.mass = number;
								lua_pop(pLuaState, 1);
							}
							else if (!key.compare("velocity"))
							{
								m_LuaEngine.GetField3fv(pLuaState, vec3_keys, &vec3);
								ps.velocity = glm::vec3(vec3.x, vec3.y, vec3.z);
								lua_pop(pLuaState, 1);
							}
							else if (!key.compare("restitution"))
							{
								number = lua_tonumber(pLuaState, -1);
								ps.restitution = number;
								lua_pop(pLuaState, 1);
							}
							else if (!key.compare("angular_acceleration"))
							{
								m_LuaEngine.GetField3fv(pLuaState, vec3_keys, &vec3);
								ps.angular_acceleration = glm::vec3(vec3.x, vec3.y, vec3.z);
								lua_pop(pLuaState, 1);

							}
							else if (!key.compare("inertia"))
							{
								m_LuaEngine.GetField3fv(pLuaState, vec3_keys, &vec3);
								ps.inertia = glm::vec3(vec3.x, vec3.y, vec3.z);
								lua_pop(pLuaState, 1);
							}
							/**/
						}
					}
					//m_TempPhysicsStates.push_back(ps);
				}

			}
			else if (var == "shader")
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
			else if (var == "scene_camera")
			{

				if (lua_getfield(pLuaState, -1, "eye"))
				{
					m_LuaEngine.GetField3fv(pLuaState, vec3_keys, &vec3);
					lua_pop(pLuaState, 1);
				}
				if (lua_getfield(pLuaState, -1, "center"))
				{
					m_LuaEngine.GetField3fv(pLuaState, vec3_keys, &vec3);
					lua_pop(pLuaState, 1);
				}
				if (lua_getfield(pLuaState, -1, "up"))
				{
					m_LuaEngine.GetField3fv(pLuaState, vec3_keys, &vec3);
					lua_pop(pLuaState, 1);
				}
			}
			break;
		case LUA_TNUMBER:
			number = lua_tonumber(pLuaState, -1);
			lua_pop(pLuaState, 1);
			break;
		}
	}
	return 0;
}

