#pragma once

#include "lua.hpp"
#include "luaconf.h"

namespace scripting
{
	class ScriptingEngine
	{
	public:

		lua_State * m_pLuaState;

		void Run();

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