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
#include "Utility/ObjectLoader.h"
#include "ScriptingEngine.h"

#include "Mesh.h"

class Scene
{
public:

	std::vector<Material> m_Materials;
	std::vector<PerspectiveCamera> m_Cameras;

	std::vector<unsigned int> m_IndexData;
	std::vector<unsigned int> m_ModelLocations;
	std::vector<unsigned int> m_NormalMatrixLocations;

	std::vector<unsigned int> m_IDs;
	std::vector<Mesh>  m_MeshData;
	unsigned int NumMeshes;

	int m_ActiveMaterial;
	unsigned int m_CurrentIndexCount;

	TimeStep ts;

	std::shared_ptr<PerspectiveCamera> m_ActiveCamera = nullptr;

	void AttachCamera(std::shared_ptr<PerspectiveCamera> cam);
	void OnCreateSceneObjects();
	void OnInit();

	void OnUpdate(float ts);
	void Process();

	// Re Write and Re structuring this whole class

	static ObjectLoader m_ObjectLoader;

	void LoadMeshData(const char* path);
	void LoadMeshData(Mesh& other);

	Scene() = default;
	~Scene()
	{
		m_Materials.clear();
		m_Cameras.clear();
		m_IndexData.clear();
		m_ModelLocations.clear();
		m_NormalMatrixLocations.clear();
	}


private:

	scripting::ScriptingEngine m_LuaEngine;
};

