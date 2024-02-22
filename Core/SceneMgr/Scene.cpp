#include "Scene.h"

ObjectLoader Scene::m_ObjectLoader = ObjectLoader();

void Scene::AttachCamera(std::shared_ptr<Camera> cam)
{
	//m_ActiveCamera = std::make_shared<PerspectiveCamera>(*cam);
	m_ActiveCamera = cam;
}

void Scene::OnCreateSceneObjects()
{
	// CREATE LIGHTS

	PointLight pointLight;
	Vector3 dummy_pos = {};
	Vector3 dummy_color = {};

	// CREATE THE SCENE CAMERA(s)
	Camera pCam;
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
	std::vector<std::string> static_mesh_paths;
	std::vector<std::string> dynamic_mesh_paths;

	lua_Number number = 0;

	for (auto& var : m_LuaEngine.Keys)
	{
		lua_getglobal(m_LuaEngine.m_pLuaState, var.c_str());
		switch (lua_type(m_LuaEngine.m_pLuaState, -1))
		{
		case LUA_TTABLE:

			if (var == "static_geometry")
			{
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "OBJ_path"))
				{
					str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
					static_mesh_paths.push_back(str);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				break;
			}
			else if (var == "dynamic_geometry")
			{
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "OBJ_path"))
				{
					str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
					dynamic_mesh_paths.push_back(str);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				break;
			}

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
				//m_SkyColor[0] = vec3.x;
				//m_SkyColor[1] = vec3.y;
				//m_SkyColor[2] = vec3.z;
				auto var = glm::vec3(vec3.x, vec3.y, vec3.z);
				m_SkyLights.push_back(SkyLight(var));
			}
			else if (var == "ground_color")
			{
				//m_GroundColor[0] = vec3.x;
				//m_GroundColor[1] = vec3.y;
				//m_GroundColor[2] = vec3.z;
				auto var = glm::vec3(vec3.x, vec3.y, vec3.z);
				m_GroundLights.push_back(GroundLight(var));
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
			lua_pop(m_LuaEngine.m_pLuaState, 1);
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
	m_ActiveCamera = std::make_shared<Camera>(pCam);
	for (auto& s_mesh : static_mesh_paths)
	{
		LoadMeshData(s_mesh.c_str(), 0);
	}

	BatchStaticGeometry();
	for (auto& d_mesh : dynamic_mesh_paths)
	{
		LoadMeshData(d_mesh.c_str(), 1);
	}

	//Collider<SphereCollider> s1 = {0.5f, +1.0f, 0.00f, 1.0f};
	//{ 1.0f, {-0.5f, 0.5f, 0.5f} };
	//Collider<SphereCollider> s2 = {0.5f, +1.0f, 0.00f, 0.49f};
	//{ 1.0f, {+0.5f, 0.5f, 0.5f} };

	Collider<GenericCollider> g1 = {glm::vec3(-5.4, 1.0, 0.5), m_StaticGeometry[0].m_Positions };
	Collider<GenericCollider> g2 = {glm::vec3(+5.6, 1.0, 0.0), m_DynamicGeometry[0].m_Positions };
	
	//bool intersect = s1.Intersect(s2);

	bool intersect = g1.Intersect(g2);
}

void Scene::OnInit()
{
	m_State = SceneState::LOADING;

	NumMeshes = 0;
	m_LuaEngine.SetScriptPath(
		"C:/dev/Silindokuhle15/Spring/Assets/Projects/Lobby.lua"
	);

	m_LuaEngine.SetKeys(
		std::vector<std::string>({ 
			"WIDTH",
			"HEIGHT",
			//"obj_path" ,
			"dynamic_geometry",
			"static_geometry",
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

	m_PhysicsEngine.OnInit();

	m_State = SceneState::RUNNING;
}

void Scene::OnUpdate(float ts)
{
	auto func = [this, ts](std::vector<Mesh> buffer)
	{
		m_ActiveCamera->OnUpdate(ts);
		for (int i = 0; i < buffer.size(); i++)
		{
			buffer[i].OnUpdate(ts);
			unsigned int model_location = m_ModelLocations[i];
			unsigned int normal_matrix_location = m_NormalMatrixLocations[i];

			glm::mat4 transform = buffer[i].m_Transform;
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
	};

	switch (m_State)
	{
	case SceneState::LOADING:

		break;

	case SceneState::RUNNING:

		func(m_StaticGeometry);
		func(m_DynamicGeometry);
		m_PhysicsEngine.OnUpdate(ts);
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


void Scene::Process()
{	
	auto func = [this](std::vector<Mesh> buffer)
	{
		for (int i = 0; i < buffer.size(); i++)
		{
			glGetIntegerv(GL_CURRENT_PROGRAM, &m_ActiveMaterial); // m_CurrentProgram shoubd be the currently bound Material ID
			uint32_t model_location = glGetUniformLocation(m_ActiveMaterial, "Model");
			uint32_t normal_matrix_location = glGetUniformLocation(m_ActiveMaterial, "NormalMatrix");

			uint32_t light_location = glGetUniformLocation(m_ActiveMaterial, "LightPosition");
			//int light_color_location = glGetUniformLocation(m_ActiveMaterial, "LightColor");
			//int sky_color_location = glGetUniformLocation(m_ActiveMaterial, "SkyColor");
			//int ground_color_location = glGetUniformLocation(m_ActiveMaterial, "GroundColor");
			//int factor_location = glGetUniformLocation(m_ActiveMaterial, "factor");

			m_ModelLocations.push_back(model_location);
			m_NormalMatrixLocations.push_back(normal_matrix_location);
		}
	};

	func(m_StaticGeometry);
	func(m_DynamicGeometry);

}

void Scene::LoadMeshData(const char* path, int buffer)
{
	Mesh new_mesh;

	if (std::string(path) == "Grid")
	{		
		Grid d = Grid(250,250);
		d.OnInit();
		new_mesh = Mesh(d);
		new_mesh.SetTransform(glm::mat4(1.0f));
	}
	else
	{
		new_mesh = Mesh(path);
		new_mesh.OnInit();
		new_mesh.SetTransform(glm::mat4(1.0f));
		//LoadMeshData(new_mesh);
	}
	switch (buffer)
	{
	case 1: // DYANAMIC GEOMETRY
		LoadDynamicGeometry(new_mesh);
		break;
	default:
		LoadStaticGeometry(new_mesh);
	}

}

void Scene::LoadMeshData(Mesh& other)
{
	m_MeshData.push_back(other);
	NumMeshes++;
}

void Scene::LoadDynamicGeometry(Mesh& other)
{
	m_DynamicGeometry.push_back(other);
	NumMeshes++;
}

void Scene::BatchStaticGeometry()
{
	Mesh BatchedMesh = Mesh::Batch(m_StaticGeometry);
	m_StaticGeometry.clear();
	m_StaticGeometry.push_back(BatchedMesh);
}

void Scene::LoadStaticGeometry(Mesh& other)
{
	m_StaticGeometry.push_back(other);
	NumMeshes++;
}
