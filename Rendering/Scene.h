#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Engine/Application.h"

class Scene
{
private:

public:
	std::vector<unsigned int> m_IndexData;
	std::vector<glm::vec2> m_VertexData;
	std::vector<Application*> m_SceneData;

	Scene* GetSceneData() { return reinterpret_cast<Scene*>(& m_SceneData[0]); }
	void AddToScene(Application* draw_data);
};

