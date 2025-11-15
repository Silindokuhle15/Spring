#include "Scene.h"
#include "Character.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
Scene::Scene(const std::string& path)
	:
	m_Ts{0.0f},
	m_AccumulatedTime{0.0},
	m_Title{ path },
	m_Script{ path },
	m_pLuaState{ nullptr },
	m_MousePosition{0.0f, 0.0f}
{
	m_pLuaState = luaL_newstate();
	luaL_openlibs(m_pLuaState);
}

Character* Scene::CreateSceneObject()
{
	static BVHArenaAllocator<Character> allocator;
	auto newCharacter = allocator.allocate(Character(m_Registry.create(), this));
	return newCharacter;
}

void Scene::DestroySceneObject(entt::entity id)
{
	auto isValidEntity = m_Registry.valid(id);
	if ((isValidEntity))
	{
		m_Registry.emplace<DestructComponent>(id);
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

	BoundingVolume bbox = BoundingVolume::CreateBoundingVolume(static_cast<uint64_t>(entity), pos, 1.0f);
	auto& min = bbox.GetMin();
	auto& max = bbox.GetMax();
	auto bound{ Bound3D{ min.x, min.y, min.z, max.x, max.y, max.z }};

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
	scripting::ScriptMgr::register_input(m_pLuaState);
	scripting::ScriptMgr::register_scene(m_pLuaState);
	scripting::ScriptMgr::register_scene_camera(m_pLuaState);
	scripting::ScriptMgr::register_character(m_pLuaState);
	scripting::ScriptMgr::register_vector3(m_pLuaState);
	scripting::ScriptMgr::register_physicsstate(m_pLuaState);
	scripting::ScriptMgr::expose_scene(m_pLuaState, this, "Scene");
}

void Scene::OnUpdate(TimeStep ts)
{
	m_Ts = ts;
	m_BVEntries.clear();
	collisionPairs.clear();

	auto scriptView = m_Registry.view<scripting::ControlScript>();
	scriptView.each([&](const auto& script) 
	{
		scripting::ScriptMgr::ExecuteScriptFunction(m_pLuaState, script.data.c_str(), "onUpdate", m_Ts);
	});

	auto physicsView = m_Registry.view<physics::PhysicsState>();
	for (auto [entity, physics_state] : physicsView.each())
	{
		physics_state.position += physics_state.orientation * physics_state.linear_acceleration * (float)m_Ts * 5.0f;
		AddBVBoundEntry(entity, physics_state);
	}

	auto node = create_tree<Bound3D>(m_BVEntries);
	for (auto& bound : m_BVEntries)
	{
		auto collisions = detect_overlapping_bounds<Bound3D>(bound, node);
		if (collisions.size() == 2)
		{
			uint64_t first = collisions[0];
			uint64_t second = collisions[1];
			if (first == second) continue;
			if (m_Registry.valid(static_cast<entt::entity>(first)) && (m_Registry.valid(static_cast<entt::entity>(second))))
			{
				std::pair<entt::entity, entt::entity> pair(static_cast<entt::entity>(first), static_cast<entt::entity>(second));
				collisionPairs.push_back(pair);
			}
		}
	}
	m_AccumulatedTime += (float)m_Ts;
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

