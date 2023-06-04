#include "Scene.h"

void Scene::AddToScene(Application* draw_data)
{
	//draw_data->OnInit();
	m_CurrentIndexCount += draw_data->m_IndexCount;

	// some more working
	m_Objects.push_back(draw_data);
}

void Scene::AttachCamera(std::shared_ptr<PerspectiveCamera> cam)
{

	//m_Cameras.push_back(*cam);
	
	//m_ActiveCamera = std::shared_ptr<PerspectiveCamera>(cam);
	m_ActiveCamera = std::make_shared<PerspectiveCamera>(*cam);

	m_ActiveCamera->SetWidth(1920);
	m_ActiveCamera->SetHeight(1080);
	m_ActiveCamera->OnInit();
}

void Scene::OnUpdate(float ts)
{
	m_ActiveCamera->OnUpdate(ts);
	for (int i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->OnUpdate(ts);
		unsigned int model_location = m_ModelLocations[i];
		unsigned int normal_matrix_location = m_NormalMatrixLocations[i];

		glm::mat4 transform = m_Objects[i]->m_Transform;
		glm::mat4 model_view = transform * m_ActiveCamera->GetV();

		glm::mat4 normal_matrix = glm::transpose(glm::inverse(model_view));

		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(transform));
		glUniformMatrix4fv(normal_matrix_location, 1, GL_FALSE, glm::value_ptr(normal_matrix));
	}
}


void Scene::Process()
{	
	for (int i = 0; i < m_Objects.size(); i++)
	{
		glGetIntegerv(GL_CURRENT_PROGRAM, &m_ActiveMaterial); // m_CurrentProgram shoubd be the currently bound Material ID
		unsigned int model_location = glGetUniformLocation(m_ActiveMaterial, "Model");
		unsigned int normal_matrix_location = glGetUniformLocation(m_ActiveMaterial, "NormalMatrix");

		m_ModelLocations.push_back(model_location);
		m_NormalMatrixLocations.push_back(normal_matrix_location);
	}

}

void Scene::MoveObjectBackward()
{
	m_Objects[0]->MoveBackward();
}

void Scene::MoveObjectForward()
{
	m_Objects[0]->MoveForward();
}

void Scene::MoveObjectLeft()
{
	m_Objects[0]->MoveLeft();
}

void Scene::MoveObjectRight()
{
	m_Objects[0]->MoveRight();
}

void Scene::MoveObjectUp()
{
	m_Objects[0]->MoveUp();
}

void Scene::MoveObjectDown()
{
	m_Objects[0]->MoveDown();
}
