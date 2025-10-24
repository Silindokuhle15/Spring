#include "Scene.h"
#include "Character.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
Scene::Scene(const std::string& path)
	:
	m_Ts{0.0f},
	m_AccumulatedTime{0.0},
	m_Title{ path },
	m_State{ SceneState::LOADING },
	m_Script{ path },
	m_pLuaState{ nullptr },
	m_MousePosition{0.0f, 0.0f}
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
		auto meshHandle = m_AssetManager->GetResourceHandle(meshR);
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

void Scene::AddBVBoundEntry(const entt::entity& entity, const physics::PhysicsState& physics_state)
{
	float minX{ -2.0f }, minY{ -2.0f }, minZ{ -2.0f };
	float maxX{ +2.0f }, maxY{ +2.0f }, maxZ{ +2.0f };
	uint32_t bits = 21;

	auto& pos = physics_state.position;
	auto y = static_cast<uint64_t>(glm::floor((pos.x - minX) / (maxX - minX) * glm::pow(2, bits)));
	auto z = static_cast<uint64_t>(glm::floor((pos.y - minY) / (maxY - minY) * glm::pow(2, bits)));
	auto x = static_cast<uint64_t>(glm::floor((pos.z - minZ) / (maxZ - minZ) * glm::pow(2, bits)));
	auto morton_code = morton_encode_3d32(x, y, z);
	entries.push_back(BVEntry{ static_cast<uint64_t>(entity), morton_code});

	BoundingVolume bbox = BoundingVolume::CreateBoundingVolume(static_cast<uint64_t>(entity), pos, 1.0f);
	auto& min = bbox.GetMin();
	auto& max = bbox.GetMax();
	auto bound{ Bound3D{ min.x, min.y, min.z, max.x, max.y, max.z }};
	bounds.push_back(bound);

	m_BVEntries.push_back(BVNode<Bound3D>{static_cast<uint64_t>(entity), morton_code, bound, nullptr, nullptr});

}

void Scene::CreateShaders()
{
	for (size_t index = 0; index < shader_paths.size(); index += 2)
	{
		auto combinedPaths = shader_paths[index] + "\n" + shader_paths[index + 1];
		AssetResource shaderResource{ AssetType::ShaderResource, combinedPaths };
		auto shaderHandle = m_AssetManager->GetResourceHandle(shaderResource);
	}
}

void Scene::OnInit()
{
	m_State = SceneState::LOADING;
	scripting::ScriptMgr::register_scene(m_pLuaState);
	scripting::ScriptMgr::register_scene_camera(m_pLuaState);
	scripting::ScriptMgr::register_character(m_pLuaState);
	scripting::ScriptMgr::register_vector3(m_pLuaState);
	scripting::ScriptMgr::register_physicsstate(m_pLuaState);

	auto filename = scripting::ScriptMgr::GetLuaFilenameWithoutExtension(m_Title);
	scripting::ScriptMgr::expose_scene(m_pLuaState, this, filename.c_str());

	m_State = SceneState::RUNNING;
}

void Scene::OnUpdate(TimeStep ts)
{
	m_BVEntries.clear();
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
	int scriptCounter{ 0 };
	scriptView.each([&](const auto& script) 
	{
		scripting::ScriptMgr::ExecuteScriptFunction(m_pLuaState, script.data.c_str(), "onUpdate", ts);
		scriptCounter++;
	});

	//std::cout << "Executed " << scriptCounter << " scripts" << std::endl;

	auto phzxView = m_Registry.view<physics::PhysicsState>();

	for (auto [entity, phzx_state] : phzxView.each())
	{
		phzx_state.position += phzx_state.orientation * phzx_state.linear_acceleration * (float)ts;
		AddBVBoundEntry(entity, phzx_state);
	}

	m_AccumulatedTime += (float)ts;
	auto node = create_tree<Bound3D>(m_BVEntries);
	for (auto& bound : m_BVEntries)
	{
		auto collisions = detect_overlapping_bounds<Bound3D>(bound, node);
		if (collisions.size() == 2)
		{
			uint64_t first = collisions[0];
			uint64_t second = collisions[1];
			if (first == second) continue;
			if ((first <= m_Characters.size() - 1) && (second <= m_Characters.size() - 1))
			{
				std::pair<entt::entity, entt::entity> pair(m_Characters[first], m_Characters[second]);
				collisionPairs.push_back(pair);
			}
		}
	}
}

void Scene::Run()
{
	m_State = SceneState::RUNNING;
}

void Scene::OnEnd()
{
	m_State = SceneState::END;
}

void Scene::OnPause()
{
	m_State = SceneState::PAUSED;
}

void Scene::OnStop()
{
	m_State = SceneState::STOPPED;
}

void Scene::OnReload()
{
	m_State = SceneState::LOADING;
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
				std::vector<std::string> dynamic_keys = { "Mesh", "script", "name"};
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

