#include "ScriptingEngine.h"
#include <string>


void scripting::ScriptingEngine::Run()
{
	// TO DO : ADD MORE CODE HERE
	// EXECUTE SOME LUA TEST CODE
	
	std::string cmd = "a = 7 + 11";

	float in = 0.0;
	/*
	int r = luaL_dostring(m_pLuaState, cmd.c_str());
	if (r == LUA_OK)
	{
		lua_getglobal(m_pLuaState, "a");
		if (lua_isnumber(m_pLuaState, -1))
		{
			in = (float)lua_tonumber(m_pLuaState, -1);
		}
		else
		{
			// LUA FAILED
		}
	}
	*/

	cmd = "C:/dev/Silindokuhle15/Spring/Assets/test.lua";
	int r = luaL_dofile(m_pLuaState, cmd.c_str());
	if (r == LUA_OK)
	{
		lua_getglobal(m_pLuaState, "a");
		if (lua_isnumber(m_pLuaState, -1))
		{
			in = (float)lua_tonumber(m_pLuaState, -1);
		}
		else
		{
			// LUA FAILED
		}
	}
}