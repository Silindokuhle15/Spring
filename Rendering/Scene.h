#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Engine/Application.h"

class Scene
{
private:

public:
	std::vector<Application*> m_SceneData;

	Scene* GetSceneData() { return reinterpret_cast<Scene*>(& m_SceneData[0]); }
	void AddToScence(Application* draw_data);
};

