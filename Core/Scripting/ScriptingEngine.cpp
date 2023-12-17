#include "ScriptingEngine.h"
#include <string>
#include <vector>


typedef struct
{
	float var[3];
}Vector3;


void scripting::ScriptingEngine::Run()
{
	// TO DO : ADD MORE CODE HERE
	// EXECUTE SOME LUA TEST CODE
	
	float number = 0;
	int len = 0;

	//std::vector<int> buffer;

	const char* camera_keys[] =
	{
		"scene_camera_m_eye",
		"scene_camera_m_center",
		"scene_camera_m_up"
	};

	const char* vec3_keys[] =
	{
		"x",
		"y",
		"z"
	};

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
	//cmd = "C:/dev/Silindokuhle15/Spring/Assets/test.lua";
	cmd = ScriptPath;
	int r = luaL_dofile(m_pLuaState, cmd.c_str());

	if (r == LUA_OK)
	{ 
		for (auto& i : Keys)
		{
			lua_getglobal(m_pLuaState, i.c_str());
			switch (lua_type(m_pLuaState, -1))
			{
			case LUA_TTABLE:
				lua_settop(m_pLuaState, -1);
				lua_len(m_pLuaState, -1);
				len = (int)lua_tonumber(m_pLuaState, -1);
				lua_pop(m_pLuaState, 1);
				len = getNamedfield(m_pLuaState, i.c_str());
				break;

			case LUA_TNUMBER:
				number = lua_tonumber(m_pLuaState, -1);

				break;

			case LUA_TSTRING:
				str = lua_tostring(m_pLuaState, -1);
				lua_pop(m_pLuaState, 1);
				break;

			case LUA_TBOOLEAN:
				lua_pop(m_pLuaState, -1);
				break;
			}
		}
		//lua_pop(m_pLuaState, -1);
	}

}

void scripting::ScriptingEngine::SetScriptPath(const std::string& path)
{
	this->ScriptPath = path;
}

void scripting::ScriptingEngine::SetKeys(const std::vector<std::string> keys_t)
{
	this->Keys = keys_t;
}

int scripting::ScriptingEngine::getNamedfield(lua_State* L, const char* field)
{
	int result, isnum;
	//lua_pushstring(L, field);
	switch (lua_getfield(L, -1, field))
	{
	case LUA_TNUMBER:
		result = (int)lua_tonumberx(L, -1, &isnum);
		if (!isnum)
		{
			int error = 1;
		}
		lua_pop(L, 1);
		return result;
	case LUA_TSTRING:
		return -1;
	case LUA_TTABLE:
		return -2;

	defualt:
		return -3;
	}
}
