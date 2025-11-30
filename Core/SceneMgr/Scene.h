#pragma once
#include "LightAndShadow/PointLight.h"
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "Script.h"
#include <entt.hpp>
#include "ScriptMgr.h"
#include "BVH.h"
#include "BoundingVolume.h"
#include "AssetManager.h"
#include "SceneState.h"


class Character;
class Renderer;

class Scene
{
private:
	glm::vec2 m_MousePosition;
public:
	friend class Character;
	friend class Renderer;
	friend class scripting::ScriptMgr;

	TimeStep m_Ts;
	double m_AccumulatedTime;
	std::string m_Title;
	std::vector<std::string> shader_paths;
	std::vector<std::string> static_mesh_paths;
	std::vector<std::string> dynamic_mesh_paths;
	std::vector<std::string> m_TempControlScripts;
	std::vector<std::string> m_TempNames;
	std::vector<PointLight> m_Lights;
	std::vector<Camera> m_Cameras;

	std::vector<BVNode<Bound3D>> m_BVEntries;
	std::vector<uint64_t> m_Collisions;
	std::vector<std::pair<entt::entity, entt::entity>> m_CollisionPairs;

	virtual void OnCreateSceneObjects();
	virtual void AddBVBoundEntry(const entt::entity& entity, const physics::PhysicsState& physics_state);
	virtual void CreateShaders();
	virtual void OnInit();
	virtual void OnUpdate(TimeStep ts);

	void Run();
	void OnEnd();
	void OnPause();
	void OnStop();
	void OnReload();

	virtual int LoadSceneFromFile();
	std::string GetTitle() const { return m_Title; }

	const glm::vec2 GetMousePosition() const;
	void SetMousePosition(const glm::vec2& mouse_position);

	lua_State* GetLuaState() const { return m_pLuaState; }

	Scene(const std::string& path = "");
	friend class Character;
	friend class Renderer;

	Character* CreateSceneObject();
	void DestroySceneObject(entt::entity id);
	Character GetSceneCharacter(entt::entity& id);

	template<typename... T>
	inline auto GetView() { return m_Registry.view<T...>(); }

	AssetManager* GetAssetManager() const { return m_AssetManager; }
	void SetAssetManager(AssetManager* pAsset_manager) { m_AssetManager = pAsset_manager; }
protected:
	entt::registry m_Registry;
	lua_State* m_pLuaState = nullptr;
	AssetManager* m_AssetManager;
};

