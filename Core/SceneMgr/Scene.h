#ifndef _SCENE_
#define _SCENE_
#include <entt.hpp>
#include "LightAndShadow/PointLight.h"
#include "Camera.h"
#include "PhysicsSystem.h"
#include "ScriptSystem.h"
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
	std::string particlePack;

	std::vector<std::string> shader_paths;
	std::vector<std::string> static_mesh_paths;
	std::vector<std::string> dynamic_mesh_paths;
	std::vector<std::string> m_TempControlScripts;
	std::vector<std::string> m_TempNames;
	std::vector<PointLight> m_Lights;
	std::vector<Camera> m_Cameras;
	physics::PhysicsSystem m_PhysicsSystem;
	scripting::ScriptSystem m_ScriptSystem;

	virtual void OnCreateSceneObjects();
	virtual void OnInit();
	virtual void OnUpdate(float ts);
	virtual int LoadSceneFromFile();

	const std::string GetTitle() const { return m_Title; }

	bool IsValidCharacter(entt::entity& id);
	Character* CreateSceneObject(uint32_t hint = 0);
	Character* GetSceneCharacter(entt::entity& id);
	void DestroySceneObject(entt::entity id);

	template<typename... T>
	inline auto GetView() { return m_Registry.view<T...>(); }

	AssetManager* GetAssetManager() const { return m_AssetManager; }
	void SetAssetManager(AssetManager* pAsset_manager) { m_AssetManager = pAsset_manager; }

	Scene(const std::string& path = "");

	virtual bool Serialize();
	virtual bool Deserialize();

protected:
	entt::registry m_Registry;
	AssetManager* m_AssetManager;
	ArenaAllocator<Character> m_CharacterAllocator;
	std::map<entt::entity, Character*> m_EntityCharacterMap;
};

#endif