#include "ScriptSystem.h"
#include "Scene.h"

scripting::ScriptSystem::ScriptSystem()
{
	m_pLuaState = luaL_newstate();
	luaL_openlibs(m_pLuaState);
	scripting::ScriptMgr::register_input(m_pLuaState);
	scripting::ScriptMgr::register_scene(m_pLuaState);
	scripting::ScriptMgr::register_scene_camera(m_pLuaState);
	scripting::ScriptMgr::register_character(m_pLuaState);
	//scripting::ScriptMgr::register_vector3(m_pLuaState);
	scripting::ScriptMgr::register_physicsstate(m_pLuaState);
}

scripting::ScriptSystem::~ScriptSystem()
{
	if (m_pLuaState)
	{
		lua_close(m_pLuaState);
		m_pLuaState = nullptr;
	}
}

void scripting::ScriptSystem::Update(Scene& scene, float time_step)
{
	auto scriptView = scene.GetView<scripting::ControlScript>();
	for (auto [entity, script] : scriptView.each())
	{
		scripting::ScriptMgr::CallOnUpdate(m_pLuaState, script, time_step);
	}
}

void scripting::ScriptSystem::ExposeScene(Scene* p_scene, const char* title)
{
	scripting::ScriptMgr::expose_scene(m_pLuaState, p_scene, title);
}

void scripting::ScriptSystem::InitScript(Character* p_character, scripting::ControlScript& script)
{
	scripting::ScriptMgr::InitScript(m_pLuaState, p_character, script);
}
