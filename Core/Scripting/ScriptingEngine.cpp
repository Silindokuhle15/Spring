#include "ScriptingEngine.h"

void scripting::ScriptingEngine::Run()
{
	// TO DO : ADD MORE CODE HERE
	// EXECUTE SOME LUA TEST CODE
	cmd = ScriptPath;
	int r = luaL_dofile(m_pLuaState, cmd.c_str());

	if (r != LUA_OK)
	{ 
		printf("Failed to Execute Lua Scripts %s", cmd.c_str());
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

lua_Number scripting::ScriptingEngine::getNamedfield(lua_State* L, const char * field)
{
	lua_Number result{ 0 };
	int isnum;
	//lua_pushstring(L, field);
	switch (lua_getfield(L, -1, field))
	{
	case LUA_TNUMBER:
		result = (lua_Number)lua_tonumberx(L, -1, &isnum);
		if (!isnum)
		{
			int error = 1;
		}
		lua_pop(L, 1);
		return result;
	case LUA_TSTRING:
		return -1;
	case LUA_TTABLE:
		lua_len(L, -1);
		return lua_tonumber(L, -1);
	default:
		result = static_cast<lua_Number>(-3);
	}
	return result;
}

void scripting::ScriptingEngine::GetField(lua_State* L, std::vector<const char*>& fields, Vector3* data_out)
{
	for (auto& field : fields)
	{
		 if (field == "x")
		 {
			 data_out->x = getNamedfield(L, field);
		 }
		 else if (field == "y")
		 {
			 data_out->y = getNamedfield(L, field);
		 }
		 else if (field == "z")
		 {
			 data_out->z = getNamedfield(L, field);
		 }
	}
	return;
}
