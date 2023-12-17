#pragma once

#include "lua.hpp"
#include "luaconf.h"
#include <vector>
#include <string>

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

		int static getNamedfield(lua_State* L, const char* field);

		ScriptingEngine() : m_pLuaState{ nullptr } {

			m_pLuaState = luaL_newstate();
			
			/*
			m_pLuaState = lua_open();			// OPENS LUA
			luaopen_base(m_pLuaState);			// LOADS AND OPENS THE BASIC LUA LIB
			luaopen_table(m_pLuaState);			// ***** AND ***** *** TABLE ****
			luaopen_io(m_pLuaState);			// ***** AND ***** *** I/O LI
			luaopen_string(m_pLuaState);		// ***** AND ***** *** STRING LIB
			luaopen_math(m_pLuaState);			// ***** AND ***** *** MATH LIB
			*/
			// ALTERNATIVELY
			luaL_openlibs(m_pLuaState);			// DOES EXACTLY AS ABOVE BUT IN ONE  LINE
		}

		~ScriptingEngine() {
			lua_close(m_pLuaState);				// CLOSE LUA AND FREE MEMORY MAYBE?
		}


	};

	
}