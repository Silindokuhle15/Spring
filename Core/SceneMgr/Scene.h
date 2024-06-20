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

// FBX ADOPTION

#include <fbxsdk.h>
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
	uint32_t m_SelectedMesh, m_SelectedBuffer;
	PrimitiveMode m_CurrentRenderMode;
	TimeStep ts;

	Camera m_ActiveCamera;
	std::shared_ptr<Camera> m_pActiveCamera;
	std::shared_ptr<glm::mat4> m_pActiveTransform;

	template<typename U>
	void AddNewItem(U item);
	//void AddNewMesh(Mesh mesh);
	void AddNewMesh(Mesh& mesh);

	void OnCreateSceneObjects();
	void OnInit();

	void OnUpdate(TimeStep ts);
	void Run();
	void OnEnd();
	void OnPause();
	void OnStop();
	void OnReload();
	void Process();

	// Re Write and Re structuring this whole class
	void LoadFbxScene(const std::string& path);
	void LoadMeshData(const char* path, int buffer);
	void LoadDynamicGeometry(Mesh& other);
	void LoadStaticGeometry(Mesh& other);
	void BatchStaticGeometry();


	Scene() = default;

	Scene(std::string path)
		:
		m_Title{ path },
		m_State{ SceneState::LOADING },
		NumMeshes{ 0 },
		m_ActiveMaterial{ 0 },
		m_MeshData{},
		m_DynamicGeometry{},
		m_StaticGeometry{},
		m_pManager{ nullptr },
		m_pScene{ nullptr }
	{

	}
	~Scene()
	{
		m_Materials.clear();
		m_Cameras.clear();
		m_IndexData.clear();
		m_ModelLocations.clear();
		m_NormalMatrixLocations.clear();
		//m_LuaEngine.~ScriptingEngine();
	}


private:
	SceneState m_State;
	scripting::ScriptingEngine m_LuaEngine;
	physics::PhysicsEngine m_PhysicsEngine;

	FbxManager* m_pManager;
	FbxScene* m_pScene;
	std::string m_Title;

	const int TRIANGLE_VERTEX_COUNT = 3;
	// Four floats for every position.
	const int VERTEX_STRIDE = 4;
	// Three floats for every normal.
	const int NORMAL_STRIDE = 3;
	// Two floats for every UV.
	const int UV_STRIDE = 2;

	enum
	{
		VERTEX_VBO,
		NORMAL_VBO,
		UV_VBO,
		INDEX_VBO,
		VBO_COUNT,
	};

	// For every material, record the offsets in every VBO and triangle counts
	struct SubMesh
	{
		SubMesh() : IndexOffset(0), TriangleCount(0) {}

		int IndexOffset;
		int TriangleCount;
	};
};

template<typename U>
inline void Scene::AddNewItem(U item)
{
	if (std::is_same<U, Mesh>::value)
	{
		AddNewMesh(item);
	}
	return;
}
