#include "Scene.h"
#include "Character.h"

Scene::Scene(const std::string& path)
	:
	OnEvent{ true },
	m_Title{ path },
	m_State{ SceneState::LOADING },
	m_Script{ path },
	m_pLuaState{ nullptr }
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

void Scene::OnCreateSceneObjects()
{
	std::vector<ShaderInfo> m_shaderInfo;
	for (size_t index = 0; index < shader_paths.size(); index += 2)
	{
		m_shaderInfo = { ShaderInfo{ shader_paths[index], GL_VERTEX_SHADER },
			ShaderInfo{ shader_paths[index + 1], GL_FRAGMENT_SHADER } };
		Shader temp_shader(m_shaderInfo);
		m_Shaders.push_back(temp_shader);
		m_shaderInfo.clear();
	}

	/*
	for (size_t index = 0; index < dynamic_mesh_paths.size(); index++)
	{
		auto nt = CreateSceneObject();
		Character* ch = new Character{ nt, this };
		ch->AddComponent<Mesh>(Mesh{ dynamic_mesh_paths[index] });
		scriptEngine.expose_character(m_pLuaState, ch, "myCharacter");
		m_Characters.push_back(nt);
	}
	*/

	// Should be calling Create Scene Object?
	auto lambdaFunc = [](Scene * pscene) {
		for (size_t index = 0; index < pscene->shader_paths.size(); index += 2)
		{
			auto vShaderPath = pscene->shader_paths[index];
			auto fShaderPath = pscene->shader_paths[index + 1];
			ShaderInfo	vShaderInfo{ vShaderPath, GL_VERTEX_SHADER };
			ShaderInfo	fShaderInfo{ fShaderPath, GL_FRAGMENT_SHADER};
			std::vector<ShaderInfo> shaderInfo{ vShaderInfo, fShaderInfo };
			Shader tempShader{ shaderInfo };
			pscene->m_Shaders.push_back(tempShader);
			shaderInfo.clear();

			for (auto& path : pscene->dynamic_mesh_paths)
			{
				auto ch = pscene->m_Registry.create();
				pscene->m_Registry.emplace<Mesh>(ch, Mesh{ path });
				pscene->m_Characters.push_back(ch);

			}
		}
		
		return 0;
	};
}


void Scene::OnInit()
{
	m_State = SceneState::LOADING;

	m_PhysicsEngine.OnInit();

	m_State = SceneState::RUNNING;
}

void Scene::OnUpdate(TimeStep ts)
{
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
	for (auto& entity : scriptView)
	{
		auto& script = scriptView.get<scripting::ControlScript>(entity);

		auto& path = script.m_ScriptPath;
		auto result = luaL_dofile(m_pLuaState, path.c_str());

		int res = lua_getglobal(m_pLuaState, "onUpdate");
		lua_pushnumber(m_pLuaState, ts);

		if (lua_pcall(m_pLuaState, 1, 0, 0) != LUA_OK)
		{
			auto error = true;
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
				std::vector<std::string> dynamic_keys = { "Mesh", "script","orientation", "position", "mass" ,"velocity", "restitution","angular_acceleration" ,"inertia" };
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
						}
					}

					m_TempPhysicsStates.push_back(ps);
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

int Scene::LoadSceneFromFile(const std::string& path)
{
	return 0;
}


