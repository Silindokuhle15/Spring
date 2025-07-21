#pragma once
#include "LightAndShadow/PointLight.h"
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "PhysicsEngine.h"
#include "Script.h"
#include <entt.hpp>
#include "ScriptMgr.h"

typedef enum
{
	LOADING, RUNNING, PAUSED, STOPPED, END
} SceneState;

class Character;
class Renderer;



class Scene
{
public:
	friend class Character;
	bool OnEvent;
	TimeStep ts;
	std::string m_Title;
	std::vector<std::string> shader_paths;
	std::vector<std::string> static_mesh_paths;
	std::vector<std::string> dynamic_mesh_paths;
	std::vector<Material> m_Materials;
	std::vector<Shader> m_Shaders;
	std::vector<PointLight> m_Lights;
	std::vector<Camera> m_Cameras;
	std::vector<entt::entity> m_Characters;

	physics::PhysicsEngine m_PhysicsEngine;

	std::vector<physics::PhysicsState> m_TempPhysicsStates;
	std::vector<std::string> m_TempControlScripts;
	virtual void OnCreateSceneObjects();
	virtual void OnInit();
	virtual void OnUpdate(TimeStep ts);

	void Run();
	void OnEnd();
	void OnPause();
	void OnStop();
	void OnReload();

	template<typename T>
	int LoadCharacterBase(T ch);
	virtual int LoadSceneFromFile();
	virtual int LoadSceneFromFile(const std::string& path);

	std::string GetTitle() const { return m_Title; }

	lua_State* GetLuaState() const { return m_pLuaState; }

	Scene(const std::string& path);
	~Scene()
	{
	}
	friend class Character;
	friend class Renderer;

	Character* CreateSceneObject();
private:
	SceneState m_State;
	entt::registry m_Registry;
	scripting::ControlScript m_Script;
	lua_State* m_pLuaState = nullptr;
	scripting::ScriptMgr scriptEngine;
};

template<typename T>
inline int Scene::LoadCharacterBase(T ch)
{
	return 0;
}