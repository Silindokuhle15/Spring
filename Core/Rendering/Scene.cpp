#include "Scene.h"

ObjectLoader Scene::m_ObjectLoader = ObjectLoader();

void Scene::AttachCamera(std::shared_ptr<PerspectiveCamera> cam)
{
	m_ActiveCamera = std::make_shared<PerspectiveCamera>(*cam);
}

void Scene::OnCreateSceneObjects()
{
	// CREATE LIGHTS

	PointLight pointLight;
	Vector3 dummy_pos = {};
	Vector3 dummy_color = {};

	// CREATE THE SCENE CAMERA(s)
	PerspectiveCamera pCam;
	std::vector<const char*> vec3_keys =
	{
		"x",
		"y",
		"z"
	};
	Vector3 vec3 = {};
	std::string str = "";
	std::string texture_path = "", mtl_path = "";
	std::vector<std::string> OBJ_paths;

	lua_Number number = 0;

	for (auto& var : m_LuaEngine.Keys)
	{
		lua_getglobal(m_LuaEngine.m_pLuaState, var.c_str());
		switch (lua_type(m_LuaEngine.m_pLuaState, -1))
		{
		case LUA_TTABLE:
			//if(!m_LuaEngine.getNamedfield(m_LuaEngine.m_pLuaState, var.c_str())) break;
			/*
			lua_len(m_LuaEngine.m_pLuaState, -1);
			if (lua_type(m_LuaEngine.m_pLuaState, -1) == LUA_TNUMBER)
			{
				LUA_NUMBER dummy = lua_tonumber(m_LuaEngine.m_pLuaState, -1);
				switch ((int)dummy)
				{
				case 1:
					break;

				case 2:

					break;

				case 3:

					break;

				}
			}
			lua_pop(m_LuaEngine.m_pLuaState, 1);
			*/
			m_LuaEngine.GetField(m_LuaEngine.m_pLuaState, vec3_keys, &vec3);

			if (var == "scene_camera_m_eye")
			{
				pCam.m_eye = glm::vec3(vec3.x, vec3.y, vec3.z);
			}
			else if (var == "scene_camera_m_center")
			{
				pCam.m_center = glm::vec3(vec3.x, vec3.y, vec3.z);
			}
			else if (var == "scene_camera_m_up")
			{
				pCam.m_up = glm::vec3(vec3.x, vec3.y, vec3.z);
			}
			else if (var == "light_position")
			{
				dummy_pos = vec3;
			}
			else if (var == "light_color")
			{
				dummy_color = vec3;
			}
			else if (var == "sky_color")
			{
				m_SkyColor[0] = vec3.x;
				m_SkyColor[1] = vec3.y;
				m_SkyColor[2] = vec3.z;
			}
			else if (var == "ground_color")
			{
				m_GroundColor[0] = vec3.x;
				m_GroundColor[1] = vec3.y;
				m_GroundColor[2] = vec3.z;
			}
			/*
			else if (var == "obj_path")
			{
				str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
				OBJ_paths.push_back(str);
				lua_pop(m_LuaEngine.m_pLuaState, 1);
			}*/
			break;
		case LUA_TSTRING:
			
			if (var == "obj_path")
			{
				str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
				OBJ_paths.push_back(str);
				lua_pop(m_LuaEngine.m_pLuaState, 1);
			}
			
			else if (var == "material_texture")
			{
				texture_path = lua_tostring(m_LuaEngine.m_pLuaState, -1);
			}

			else if (var == "mtl_path")
			{
				mtl_path = lua_tostring(m_LuaEngine.m_pLuaState, -1);
			}
			break;

		case LUA_TNUMBER:
			number = lua_tonumber(m_LuaEngine.m_pLuaState, -1);
			var == "WIDTH" ? pCam.SetWidth(number) : void();
			var == "HEIGHT" ? pCam.SetHeight(number) : void();
 			break;
		}

	}
	
	m_Materials.push_back(
		Material(texture_path, mtl_path)
	);
	m_Lights.push_back(
		PointLight(
			glm::vec3(dummy_pos.x, dummy_pos.y, dummy_pos.z),
			glm::vec3(dummy_color.x, dummy_color.y, dummy_color.z)
			)
		);

	pCam.OnInit();
	m_ActiveCamera = std::make_shared<PerspectiveCamera>(pCam);

	for (auto& mesh : OBJ_paths)
	{
			LoadMeshData(mesh.c_str());
	}
}

void Scene::OnInit()
{
	NumMeshes = 0;
	m_LuaEngine.SetScriptPath(
		"C:/dev/Silindokuhle15/Spring/Assets/test.lua"
	);

	m_LuaEngine.SetKeys(
		std::vector<std::string>({ 
			"WIDTH",
			"HEIGHT",
			"obj_path" ,
			//LIGHTS
			"light_position",
			"light_color",
			"sky_color",
			"ground_color",
			//CAMERA
			"scene_camera_m_eye",
			"scene_camera_m_center",
			"scene_camera_m_up",
			// TEXTUTES
			"material_texture",
			"mtl_path"
			}
		)
	);
	m_LuaEngine.Run();
	OnCreateSceneObjects();
}

void Scene::OnUpdate(float ts)
{
	m_ActiveCamera->OnUpdate(ts);
	for (int i = 0; i < m_MeshData.size(); i++)
	{
		m_MeshData[i].OnUpdate(ts);
		unsigned int model_location = m_ModelLocations[i];
		unsigned int normal_matrix_location = m_NormalMatrixLocations[i];

		glm::mat4 transform = m_MeshData[i].m_Transform;
		glm::mat4 model_view = transform * m_ActiveCamera->GetV();

		glm::mat4 normal_matrix = glm::transpose(glm::inverse(model_view));

		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(transform));
		glUniformMatrix4fv(normal_matrix_location, 1, GL_FALSE, glm::value_ptr(normal_matrix));
	}

	// UPDATE THE MATERIALS

	for (auto& mtl : m_Materials)
	{
		mtl.OnUpdate();
	}
}


void Scene::Process()
{	
	for (int i = 0; i < m_MeshData.size(); i++)
	{
		glGetIntegerv(GL_CURRENT_PROGRAM, &m_ActiveMaterial); // m_CurrentProgram shoubd be the currently bound Material ID
		unsigned int model_location = glGetUniformLocation(m_ActiveMaterial, "Model");
		unsigned int normal_matrix_location = glGetUniformLocation(m_ActiveMaterial, "NormalMatrix");

		m_ModelLocations.push_back(model_location);
		m_NormalMatrixLocations.push_back(normal_matrix_location);
	}
}

void Scene::LoadMeshData(const char* path)
{
	Mesh new_mesh;

	if (std::string(path) == "Grid")
	{
		Grid dummy_grid = Grid(1, 1);
		dummy_grid.OnInit();
		Mesh dummy_mesh = Mesh::Batch(dummy_grid.m_Cells);
		
		LoadMeshData(dummy_mesh);
	}
	else
	{
		new_mesh = Mesh(path);
		new_mesh.OnInit();
		new_mesh.SetTransform(glm::mat4(1.0f));
		LoadMeshData(new_mesh);
	}
}

void Scene::LoadMeshData(Mesh& other)
{
	m_MeshData.push_back(other);
	NumMeshes++;
}