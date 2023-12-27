#pragma once

#include "lua.hpp"
#include "luaconf.h"
#include <vector>
#include <string>

typedef struct
{
	double x;
	double y;
	double z;
}Vector3;

namespace scripting
{
	class ScriptingEngine
	{
	public:
		std::string cmd = "";
		std::string str = "";
		std::string ScriptPath;
		std::vector<std::string> Keys;
		lua_State * m_pLuaState;

		void Run();
		void SetScriptPath(const std::string& path);
		void SetKeys(const std::vector<std::string> keys_t);

		lua_Number static getNamedfield(lua_State* L, const char* field);
		void GetField(lua_State* L, std::vector<const char*>& field, Vector3* data_out);

		ScriptingEngine() : m_pLuaState{ nullptr } {

			m_pLuaState = luaL_newstate();
			luaL_openlibs(m_pLuaState);			
		}

		~ScriptingEngine() {
			lua_close(m_pLuaState);				// CLOSE LUA AND FREE MEMORY MAYBE?
		}


	};
}