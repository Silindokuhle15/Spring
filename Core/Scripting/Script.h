#pragma once
#include "ScriptBase.h"

namespace scripting
{
	class ConfigScript : public ScriptBase
	{
	public:

		void OnInit();
		void OnUpdate(float ts);
		void Run() const;
		lua_State* GetLuaState() const { return m_pLuaState; }

		ConfigScript() : 
			m_pLuaState{nullptr}
		{
			m_pLuaState = luaL_newstate();
			luaL_openlibs(m_pLuaState);
		}
		~ConfigScript()
		{
			lua_close(m_pLuaState);
		}
	private:
		lua_State* m_pLuaState;
	};

	class ControlScript
	{
	public:
		std::string m_ScriptPath;
		void OnInit();
		void OnUpdate(float ts);

		lua_State* GetLuaState() const { return m_pLuaState; }

		ControlScript(std::string path = "")
			: 
			m_pLuaState{nullptr},
			m_ScriptPath{path}
		{
			m_pLuaState = luaL_newstate();
			luaL_openlibs(m_pLuaState);
		}
		~ControlScript()
		{
			lua_close(m_pLuaState);				// CLOSE LUA AND FREE MEMORY MAYBE?
		}

	private:
		lua_State* m_pLuaState;
	};
}