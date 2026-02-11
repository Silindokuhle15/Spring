#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Scene.h"
#include "Character.h"

Scene::Scene(const std::string& path)
	:
	m_Ts{0.0f},
	m_AccumulatedTime{0.0f},
	m_Title{ path },
	m_pLuaState{ nullptr },
	m_MousePosition{0.0f, 0.0f},
	m_AssetManager{nullptr},
	m_BVHTreeRoot{nullptr},
	m_NodeBuffer{},
	m_CollisionVolumeSize{1024.0f}
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
		m_Registry.emplace<primitives::DestructComponent>(id);
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
		primitives::MeshInstance meshInstance{ meshHandle };

		auto* ch = CreateSceneObject();
		ch->AddComponent<primitives::MeshInstance>(meshInstance);
		AssetResource scriptResource{ AssetType::ScriptResource, m_TempControlScripts[index] };
		auto scriptHandle = m_AssetManager->GetResourceHandle(scriptResource);
		auto scriptData = ReadLuaScriptFromDisk(m_TempControlScripts[index]);
		scripting::ControlScript script{ scriptHandle, scriptData};
		ch->AddComponent<scripting::ControlScript>(script);
		ch->AddComponent<primitives::RenderComponent>(primitives::RenderComponent{ 0, 0 });
		auto name = m_TempNames[index].c_str();
		scripting::ScriptMgr::expose_character(lua_state, ch, name);
		scripting::ScriptMgr::ExecuteScript(lua_state, scriptData.data(), scriptData.size(), name);
	}
}

void Scene::AddBVBoundEntry(const entt::entity& entity, const physics::PhysicsState& physics_state, const primitives::Bound3D& bound)
{
	uint32_t bits = 21;
	auto& pos = physics_state.position;
	auto& orientation = physics_state.orientation;
	
	auto localMin =  orientation * ( glm::vec3(bound.xMin, bound.yMin, bound.zMin));
	auto localMax =  orientation * ( glm::vec3(bound.xMax, bound.yMax, bound.zMax));

	float xMin = localMin.x < localMax.x ? localMin.x : localMax.x;
	float yMin = localMin.y < localMax.y ? localMin.y : localMax.y;
	float zMin = localMin.z < localMax.z ? localMin.z : localMax.z;

	float xMax = localMin.x > localMax.x ? localMin.x : localMax.x;
	float yMax = localMin.y > localMax.y ? localMin.y : localMax.y;
	float zMax = localMin.z > localMax.z ? localMin.z : localMax.z;

	float absoluteMin = xMin < yMin ? xMin : yMin;
	absoluteMin = absoluteMin < zMin ? absoluteMin : zMin;
	float absoluteMax = xMax > yMax ? xMax : yMax;
	absoluteMax = absoluteMax > zMax ? absoluteMax : zMax;

	auto globalMin = pos + glm::vec3(absoluteMin);
	auto globalMax = pos + glm::vec3(absoluteMax);

	primitives::Bound3D worldBound
	{
		globalMin.x, globalMin.y, globalMin.z, 
		globalMax.x, globalMax.y, globalMax.z,
	};

	auto y = static_cast<uint64_t>(glm::floor((globalMax.x - m_CollisionVolumeSize) / (2.0f * m_CollisionVolumeSize) * glm::pow(2, bits)));
	auto z = static_cast<uint64_t>(glm::floor((globalMax.y - m_CollisionVolumeSize) / (2.0f * m_CollisionVolumeSize) * glm::pow(2, bits)));
	auto x = static_cast<uint64_t>(glm::floor((globalMax.z - m_CollisionVolumeSize) / (2.0f * m_CollisionVolumeSize) * glm::pow(2, bits)));

	auto morton_code = morton_encode_3d32(x, y, z);
	m_BVEntries.push_back(BVNode<primitives::Bound3D>{static_cast<uint64_t>(entity), morton_code, worldBound, nullptr, nullptr});
}

void Scene::CreateShaders()
{
	for (size_t index = 0; index < shader_paths.size(); index += 2)
	{
		auto combinedPaths = shader_paths[index] + "\n" + shader_paths[index + 1];
		AssetResource shaderResource{ AssetType::GraphicsShaderResource, combinedPaths };
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

void Scene::OnUpdate(float ts)
{
	m_Ts = ts;
	m_BVEntries.clear();
	m_CollisionPairs.clear();
	m_NodeBuffer.clear();

	auto scriptView = m_Registry.view<scripting::ControlScript>();
	scriptView.each([&](const auto& script) 
	{
		scripting::ScriptMgr::ExecuteScriptFunction(m_pLuaState, script.m_Data.c_str(), "onUpdate", m_Ts);
	});

	auto boundView = m_Registry.view<physics::PhysicsState, primitives::Bound3D>();
	for (auto [entity, physicsState, localBound] : boundView.each())
	{
		physicsState.position += physicsState.orientation * physicsState.linear_acceleration * (float)m_Ts * 5.0f;
		AddBVBoundEntry(entity, physicsState, localBound);
	}

	m_NodeBuffer.reserve(131072);
	m_BVHTreeRoot = create_tree<primitives::Bound3D>(m_BVEntries);
	for (auto& bound : m_BVEntries)
	{
		m_Collisions.clear();
		m_Collisions.reserve(m_BVEntries.size());
		detect_overlapping_bounds<primitives::Bound3D>(bound, m_BVHTreeRoot, m_Collisions, m_NodeBuffer);
		if (m_Collisions.size() == 2)
		{
			// Fix this later, the collision size should be aloud to be greater than 2
			uint64_t first = m_Collisions[0];
			uint64_t second = m_Collisions[1];
			if (first == second) continue;
			if (m_Registry.valid(static_cast<entt::entity>(first)) && (m_Registry.valid(static_cast<entt::entity>(second))))
			{
				AssetHandle a{ 0, first };
				AssetHandle b{ 0, second };
				physics::CollisionPairDescription collisionDesc{ a, b };
				m_CollisionPairs.push_back(collisionDesc);
			}
		}
	}
	m_AccumulatedTime += m_Ts;
}

int Scene::LoadSceneFromFile()
{	
	std::string str{ "" };
	scripting::ConfigScript m_LuaEngine;
	m_LuaEngine.SetScriptPath(m_Title);
	auto pLuaState = m_LuaEngine.GetLuaState();
	std::vector<std::string> dynamic_keys{ "Mesh", "script", "name" };
	m_LuaEngine.SetKeys(
		std::vector<std::string>({
			"dynamic_geometry",
			"static_geometry",
			"shader"
			}
		)
	);
	m_LuaEngine.Run();
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
				break;
		}
	}
	return 0;
}
