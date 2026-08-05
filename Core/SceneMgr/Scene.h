#ifndef _SCENE_
#define _SCENE_
#include <entt.hpp>
#include "LightAndShadow/PointLight.h"
#include "Camera.h"
#include "BVH.h"
#include "BoundingVolume.h"
#include "ScriptMgr.h"
#include "AssetManager.h"
#include "SceneState.h"
#include "ParticleSystem.h"
#include "StringUtils.h"

class Character;
class Renderer;

class Scene
{
private:
	std::string m_Title;
public:
	friend class Character;
	friend class Renderer;
	friend class scripting::ScriptMgr;
	float m_Ts;
	float m_AccumulatedTime;
	std::string asset_pack;
	std::string scene_blob;
	std::string meshPack;
	std::string materialPack;
	std::string texturePack;
	std::string shaderPack;
	std::string soundPack;
	std::string scriptPack;

	std::vector<std::string> shader_paths;
	std::vector<std::string> static_mesh_paths;
	std::vector<std::string> dynamic_mesh_paths;
	std::vector<std::string> m_TempControlScripts;
	std::vector<std::string> m_TempNames;
	std::vector<PointLight> m_Lights;
	std::vector<Camera> m_Cameras;

	std::vector<BVNode<primitives::Bound3D>> m_BVEntries;
	std::vector<uint32_t> m_Collisions;
	std::vector<physics::CollisionPairDescription> m_CollisionPairs;

	virtual void OnCreateSceneObjects();
	virtual void AddBVBoundEntry(const entt::entity& entity, const physics::PhysicsState& physics_state, const primitives::Bound3D& bound);
	virtual void OnInit();
	virtual void OnUpdate(float ts);
	virtual int LoadSceneFromFile();

	const std::string GetTitle() const { return m_Title; }

	lua_State* GetLuaState() const { return m_pLuaState; }

	Character* CreateSceneObject(uint32_t hint = 0);
	Character* GetSceneCharacter(entt::entity& id);
	void DestroySceneObject(entt::entity id);

	template<typename... T>
	inline auto GetView() { return m_Registry.view<T...>(); }

	AssetManager* GetAssetManager() const { return m_AssetManager; }
	void SetAssetManager(AssetManager* pAsset_manager) { m_AssetManager = pAsset_manager; }

	const BVNode<primitives::Bound3D>* GetBVHRoot() const { return m_BVHTreeRoot; }

	Scene(const std::string& path = "");

	virtual bool Serialize();
	virtual bool Deserialize();

protected:
	entt::registry m_Registry;
	lua_State* m_pLuaState = nullptr;
	AssetManager* m_AssetManager;
	BVNode<primitives::Bound3D>* m_BVHTreeRoot;
	ArenaAllocator<Character> m_CharacterAllocator;
	std::map<entt::entity, Character*> m_EntityCharacterMap;
private:
	std::vector<const BVNode<primitives::Bound3D>*> m_NodeBuffer;
	float m_CollisionVolumeSize;
};

#endif