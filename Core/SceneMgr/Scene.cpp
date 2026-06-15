#include "Scene.h"
#include "Character.h"

Scene::Scene(const std::string& path)
	:
	m_Ts{0.0f},
	m_AccumulatedTime{0.0f},
	m_Title{ path },
	m_pLuaState{ nullptr },
	m_AssetManager{nullptr},
	m_BVHTreeRoot{nullptr},
	m_NodeBuffer{},
	m_CollisionVolumeSize{1024.0f}
{
	m_pLuaState = luaL_newstate();
	luaL_openlibs(m_pLuaState);
}

bool Scene::Serialize()
{
	auto title = GetFilenameWithoutExtension(m_Title);
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
	auto parentView = GetView<primitives::Parent, primitives::ParticleSystem, std::string>();
	for (auto [entity, parent, particleSystem, tag] : parentView.each())
	{
		json jCharacter;
		jCharacter["id"] = (int)entity;
		jCharacter["tag"] = tag;
		jCharacter["parent"] = (int)parent.ParentEntity;
		jCharacter["particle_system"] = particleSystem;

		jScene["children"].push_back(jCharacter);
	}
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
		scripting::ScriptMgr::InitScript(m_pLuaState, ch, script);
		//scripting::ScriptMgr::CallOnInit(m_pLuaState, ch->GetCharacterID(), script);
		ch->AddComponent<scripting::ControlScript>(script);
	}

	for (auto& jCharacter : jScene["children"])
	{
		auto id = jCharacter["id"].get<int>();
		auto tag = jCharacter["tag"].get<std::string>();
		auto parent = jCharacter["parent"].get<int>();
		auto particleSystem = jCharacter["particle_system"].get<primitives::ParticleSystem>();

		auto child = CreateSceneObject((uint32_t)id);
		child->AddComponent<std::string>(tag);
		child->AddComponent<primitives::Parent>(primitives::Parent{ entt::entity(parent) });
		child->AddComponent<primitives::ParticleSystem>(particleSystem);
	}
	ifs.close();
	return true;
}

Character* Scene::CreateSceneObject(uint32_t hint)
{
	static ArenaAllocator<Character> allocator;
	auto desired = entt::entity(hint);
	auto newCharacter = allocator.allocate(Character(m_Registry.create(desired), this));
	return newCharacter;
}

void Scene::DestroySceneObject(entt::entity id)
{
	auto isValidEntity = m_Registry.valid(id);
	if (isValidEntity && !m_Registry.any_of<primitives::DestructComponent>(id))
	{
		m_Registry.emplace<primitives::DestructComponent>(id);
	}
}

Character Scene::GetSceneCharacter(entt::entity& id)
{
	return Character::GetCharacterPtr(id, this);
}

Character* Scene::GetSceneCharacterPtr(entt::entity& id)
{
	return nullptr;
}

void Scene::OnCreateSceneObjects()
{
	auto lua_state = GetLuaState();
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
	for (auto [entity, script] : scriptView.each())
	{
		scripting::ScriptMgr::CallOnUpdate(m_pLuaState, entity, script, m_Ts);
	}

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
			// Fix this later, the collision size should be allowed to be greater than 2
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
			"texturePack"
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
		}
	}
	return 0;
}
