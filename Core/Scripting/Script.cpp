#include "Script.h"

void scripting::ControlScript::OnInit()
{
}

void scripting::ControlScript::OnUpdate(float ts)
{
	//int result = luaL_dofile(m_pLuaState, m_ScriptPath.c_str());
	//if ( result != LUA_OK)
	{
		//printf("Unable to execute script %s\n", m_ScriptPath.c_str());
	}
}


void scripting::ConfigScript::OnInit()
{
	m_pLuaState = luaL_newstate();
	luaL_openlibs(m_pLuaState);
}

void scripting::ConfigScript::OnUpdate(float ts)
{
}

void scripting::ConfigScript::Run() const
{
	int r = luaL_dofile(m_pLuaState, ScriptPath.c_str());
	if (r != LUA_OK)
	{
		std::cerr << "Failed to Execute Lua Scripts %s\n"<<  ScriptPath << std::endl;
	}
}
