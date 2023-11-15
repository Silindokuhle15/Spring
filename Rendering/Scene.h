#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Application.h"
#include "Camera.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "Material.h"
#include "TimeStep.h"
//#include "entt.hpp"


class Scene
{
public:

	Scene() = default;
	~Scene()
	{
		m_Materials.clear();
		m_Cameras.clear();
		m_Objects.clear();

		m_IndexData.clear();
		m_ModelLocations.clear();
		m_NormalMatrixLocations.clear();
	}
	std::shared_ptr<PerspectiveCamera> m_ActiveCamera = nullptr;

	void AddToScene(Application* draw_data);

	void AttachCamera(std::shared_ptr<PerspectiveCamera> cam);

	void OnUpdate(float ts);
	void Process();

	// Re Write and Re structuring this whole class

	std::vector<Material> m_Materials;
	std::vector<PerspectiveCamera> m_Cameras;
	std::vector<Application *> m_Objects;

	std::vector<unsigned int> m_IndexData;
	std::vector<unsigned int> m_ModelLocations;
	std::vector<unsigned int> m_NormalMatrixLocations;

	std::vector<glm::vec3> m_positions;
	std::vector<glm::vec2> m_texcoords;
	std::vector<glm::vec3> m_normals;
	std::vector<unsigned int> m_indices;
	

	int m_ActiveMaterial;
	unsigned int m_CurrentIndexCount;

	TimeStep ts;

	void MoveObjectBackward();
	void MoveObjectForward();

	void MoveObjectLeft();
	void MoveObjectRight();

	void MoveObjectUp();
	void MoveObjectDown();
};

