#pragma once
#include <vector>
#include <tuple>
#include "glm/glm.hpp"
#include "Application.h"
#include "LightAndShadow/PointLight.h"
#include "Camera.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "Material.h"
#include "TimeStep.h"
//#include "entt.hpp"
#include "Utility/ObjectLoader.h"
#include "ScriptingEngine.h"

#include "Mesh.h"
#include "Square.h"
#include "Grid.h"

#include "PhysicsEngine.h"

typedef enum
{
	LOADING, RUNNING, PAUSED, STOPPED, END
} SceneState;


class Scene
{
public:

	float m_SkyColor[3];
	float m_GroundColor[3];

	std::vector<Material> m_Materials;
	std::vector<PointLight> m_Lights;
	std::vector<PerspectiveCamera> m_Cameras;

	std::vector<unsigned int> m_IndexData;
	std::vector<unsigned int> m_ModelLocations;
	std::vector<unsigned int> m_NormalMatrixLocations;

	std::vector<unsigned int> m_IDs;
	std::vector<Mesh>  m_MeshData;
	unsigned int NumMeshes;

	// STORE STATIC AND DYNAMIC MESHES IN SEPARATE VECTORS
	std::vector<Mesh> m_StaticGeometry;
	std::vector<Mesh> m_DynamicGeometry;

	int m_ActiveMaterial;
	unsigned int m_CurrentIndexCount;

	TimeStep ts;

	std::shared_ptr<PerspectiveCamera> m_ActiveCamera = nullptr;

	void AttachCamera(std::shared_ptr<PerspectiveCamera> cam);
	void OnCreateSceneObjects();
	void OnInit();

	void OnUpdate(float ts);
	void Run();
	void OnEnd();
	void OnPause();
	void OnStop();
	void OnReload();
	void Process();

	// Re Write and Re structuring this whole class

	static ObjectLoader m_ObjectLoader;

	void LoadMeshData(const char* path, int buffer);
	void LoadMeshData(Mesh& other);
	void LoadDynamicGeometry(Mesh& other);
	void LoadStaticGeometry(Mesh& other);
	void BatchStaticGeometry();


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
	SceneState m_State;
	scripting::ScriptingEngine m_LuaEngine;
	physics::PhysicsEngine m_PhysicsEngine;
};