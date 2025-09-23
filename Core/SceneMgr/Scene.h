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
#include "BVH.h"
#include "BoundingVolume.h"

typedef enum
{
	LOADING, RUNNING, PAUSED, STOPPED, END
} SceneState;

class Character;
class Renderer;

class Scene
{
private:
	glm::vec2 m_MousePosition;
public:
	friend class Character;
	bool OnEvent;
	TimeStep m_Ts;
	double m_AccumulatedTime;
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
	const glm::vec2 GetMousePosition() const;
	void SetMousePosition(const glm::vec2& mouse_position);

	std::vector<physics::PhysicsState> m_TempPhysicsStates;
	std::vector<std::string> m_TempControlScripts;
	std::vector<std::string> m_TempNames;
	virtual void OnCreateSceneObjects();
	virtual bool CreateBoundingVolumeHierarchy(std::vector<physics::PhysicsState>& states);
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

	lua_State* GetLuaState() const { return m_pLuaState; }

	Scene(const std::string& path = "");
	friend class Character;
	friend class Renderer;

	Character* CreateSceneObject();
	Character GetSceneCharacter(entt::entity& id);
private:
	SceneState m_State;
	entt::registry m_Registry;
	scripting::ControlScript m_Script;
	lua_State* m_pLuaState = nullptr;
	scripting::ScriptMgr scriptEngine;
};
