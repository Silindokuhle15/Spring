#pragma once
#include "Common.h"
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

template <class _ObjectType>
class SceneObject
{
public:
	uint32_t m_UUID;
	SceneObject() : m_UUID{ 0 } {}
};

class SkyLight : public SceneObject<SkyLight>
{
public:
	SkyLight(glm::vec3 col = glm::vec3(0.0)) : m_Color{col}
	{	
	}
	glm::vec3 m_Color;
};

using GroundLight = SkyLight;

class Scene
{
public:

	std::vector<SkyLight> m_SkyLights;
	std::vector<GroundLight> m_GroundLights;
	std::vector<Material> m_Materials;
	std::vector<PointLight> m_Lights;
	std::vector<Camera> m_Cameras;

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

	std::shared_ptr<Camera> m_ActiveCamera = nullptr;

	void AttachCamera(std::shared_ptr<Camera> cam);


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