#include "Scene.h"

ObjectLoader Scene::m_ObjectLoader = ObjectLoader();

void Scene::AttachCamera(std::shared_ptr<PerspectiveCamera> cam)
{
	m_ActiveCamera = std::make_shared<PerspectiveCamera>(*cam);

	m_ActiveCamera->SetWidth(1920);
	m_ActiveCamera->SetHeight(1080);
	m_ActiveCamera->OnInit();
}

void Scene::OnCreateSceneObjects()
{

}

void Scene::OnInit()
{
	NumMeshes = 0;
	m_LuaEngine.SetScriptPath(
		"C:/dev/Silindokuhle15/Spring/Assets/test.lua"
	);

	m_LuaEngine.SetKeys(
		std::vector<std::string>({ 
			"obj_path" ,
			"scene_camera_m_eye",
			"scene_camera_m_center",
			"scene_camera_m_up"
			}
		)
	);
	m_LuaEngine.Run();
	std::string obj_path = m_LuaEngine.str;
	//Scene::LoadMeshData(obj_path.c_str());

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
	Mesh new_mesh(path);
	new_mesh.OnInit();
	new_mesh.SetTransform(glm::mat4());
	LoadMeshData(new_mesh);
}

void Scene::LoadMeshData(Mesh& other)
{
	m_MeshData.push_back(other);
	NumMeshes++;
}