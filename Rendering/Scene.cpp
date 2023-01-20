#include "Scene.h"

void Scene::AddToScene(Application* draw_data)
{
	//draw_data->OnInit();
	m_SceneData.push_back(draw_data);
}

void Scene::Process()
{	
}