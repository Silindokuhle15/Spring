#include "Scene.h"


void Scene::OnCreateSceneObjects()
{
	m_pActiveCamera->OnInit();

	std::vector<ShaderInfo> m_shaderInfo;
	for (size_t index = 0; index < shader_paths.size(); index += 2)
	{
		m_shaderInfo = { ShaderInfo{ shader_paths[index], GL_VERTEX_SHADER },
			ShaderInfo{ shader_paths[index + 1], GL_FRAGMENT_SHADER } };
		Shader temp_shader(m_shaderInfo);
		m_Shaders.push_back(temp_shader);
		m_shaderInfo.clear();
	}

	for (auto& s_mesh : static_mesh_paths)
	{
		CharacterPtr pch = std::make_shared<Character>();
		pch->AddComponent<Mesh>(new Mesh(s_mesh));
		LoadCharacterBase<CharacterPtr>(pch);
	}

	for (size_t index = 0; index < dynamic_mesh_paths.size(); index++)
	{
		CharacterPtr pch = std::make_shared<Character>();
		pch->AddComponent<Mesh>(new Mesh(dynamic_mesh_paths[index]));
		auto& script_path = m_TempControlScripts[index];
		pch->AddComponent<scripting::ControlScript>(new scripting::ControlScript(script_path));
		LoadCharacterBase<CharacterPtr>(pch);
	}
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
		for (auto& ch : m_Characters)
		{
			ch->OnUpdate(ts);
		}
		break;

	case SceneState::PAUSED:
		break;

	case SceneState::END:
		break;

	case SceneState::STOPPED:
		break;
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
					m_pActiveCamera->m_eye = glm::vec3(vec3.x, vec3.y, vec3.z);
					lua_pop(pLuaState, 1);
				}
				if (lua_getfield(pLuaState, -1, "center"))
				{
					m_LuaEngine.GetField3fv(pLuaState, vec3_keys, &vec3);
					m_pActiveCamera->m_center = glm::vec3(vec3.x, vec3.y, vec3.z);
					lua_pop(pLuaState, 1);
				}
				if (lua_getfield(pLuaState, -1, "up"))
				{
					m_LuaEngine.GetField3fv(pLuaState, vec3_keys, &vec3);
					m_pActiveCamera->m_up = glm::vec3(vec3.x, vec3.y, vec3.z);
					lua_pop(pLuaState, 1);
				}
			}
			break;
		case LUA_TNUMBER:
			number = lua_tonumber(pLuaState, -1);
			var == "WIDTH" ? m_pActiveCamera->SetWidth(static_cast<int>(number)) : void();
			var == "HEIGHT" ? m_pActiveCamera->SetHeight(static_cast<int>(number)) : void();
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
