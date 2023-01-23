#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Application/Application.h"
#include "Viewing/Camera.h"
#include <memory>

class Scene
{
public:

	std::vector<unsigned int> m_IndexData;
	std::vector<Application*> m_SceneData;

	void AddToScene(Application* draw_data);

	unsigned int m_CurrentIndexCount;

	void Process();
};

