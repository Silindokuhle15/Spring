#ifndef _SCENE_
#define _SCENE_
#include "LightAndShadow/PointLight.h"
#include "Camera.h"
#include "ComponentUtils.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include <entt.hpp>
#include "ScriptMgr.h"
#include "BVH.h"
#include "BoundingVolume.h"
#include "AssetManager.h"
#include "SceneState.h"
#include "ParticleSystem.h"

class Character;
class Renderer;

class Scene
{
private:
	glm::vec2 m_MousePosition;
	std::string m_Title;
public:
	friend class Character;
	friend class Renderer;
	friend class scripting::ScriptMgr;

	float m_Ts;
	float m_AccumulatedTime;
	std::vector<std::string> shader_paths;
	std::vector<std::string> static_mesh_paths;
	std::vector<std::string> dynamic_mesh_paths;
	std::vector<std::string> m_TempControlScripts;
	std::vector<std::string> m_TempNames;
	std::vector<PointLight> m_Lights;
	std::vector<Camera> m_Cameras;

	std::vector<BVNode<primitives::Bound3D>> m_BVEntries;
	std::vector<uint64_t> m_Collisions;
	std::vector<physics::CollisionPairDescription> m_CollisionPairs;

	std::vector<primitives::ParticleSystem> m_ParticleSystems;

	virtual void OnCreateSceneObjects();
	virtual void AddBVBoundEntry(const entt::entity& entity, const physics::PhysicsState& physics_state, const primitives::Bound3D& bound);
	virtual void CreateShaders();
	virtual void OnInit();
	virtual void OnUpdate(float ts);
	virtual int LoadSceneFromFile();

	const std::string GetTitle() const { return m_Title; }

	const glm::vec2 GetMousePosition() const;
	void SetMousePosition(const glm::vec2& mouse_position);

	lua_State* GetLuaState() const { return m_pLuaState; }

	Character* CreateSceneObject();
	void DestroySceneObject(entt::entity id);
	Character GetSceneCharacter(entt::entity& id);

	template<typename... T>
	inline auto GetView() { return m_Registry.view<T...>(); }

	AssetManager* GetAssetManager() const { return m_AssetManager; }
	void SetAssetManager(AssetManager* pAsset_manager) { m_AssetManager = pAsset_manager; }

	const BVNode<primitives::Bound3D>* GetBVHRoot() const { return m_BVHTreeRoot; }

	Scene(const std::string& path = "");

protected:
	entt::registry m_Registry;
	lua_State* m_pLuaState = nullptr;
	AssetManager* m_AssetManager;
	BVNode<primitives::Bound3D>* m_BVHTreeRoot;
private:
	std::vector<const BVNode<primitives::Bound3D>*> m_NodeBuffer;
	float m_CollisionVolumeSize;
};

#endif