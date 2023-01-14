#include "Scene.h"

void Scene::AddToScene(Application* draw_data)
{
	m_SceneData.push_back(draw_data);

	glm::vec2 pos[] = { {-1.0f, -1.0f}, { +1.0f, -1.0f }, {+1.0f, +1.0f}, { -1.0f, +1.0f} };
	glm::vec2 tex[] = { { 0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, { 0.0f, 1.0f} };
	//glm::vec2 tex[] = { { 1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f}, { 0.0f, 0.0f} };

	unsigned int indices[] = { 0,1,2, 2,3,0 };

	for (int i = 0; i < 4; i++)
	{
		m_VertexData.push_back(pos[i]);
		//m_TexCoords.push_back(tex[i]);
	}
	for (int n = 0; n < 6; n++)
	{
		m_IndexData.push_back(indices[n]);
	}
	m_VertexData.shrink_to_fit();
	m_IndexData.shrink_to_fit();
}