#include "Scene.h"

void Scene::AddToScene(Application* draw_data)
{
	//draw_data->OnInit();
	m_SceneData.push_back(draw_data);
	m_CurrentIndexCount += draw_data->m_IndexCount;
}

void Scene::Process()
{	
}