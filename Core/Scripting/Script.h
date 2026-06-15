#pragma once
#include "lua.hpp"
#include "luaconf.h"
#include "UUID.h"
#include <string>

namespace scripting
{
	class ConfigScript
	{
	public:

		void OnInit();
		void OnUpdate(float ts);
		void Run() const;
		lua_State* GetLuaState() const { return m_pLuaState; }

		ConfigScript(const std::string& path) : 
			m_ScriptPath{path},
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
		std::string m_ScriptPath;
	};

	class ControlScript
	{
	public:
		int m_LuaTableRef;
		AssetHandle m_Handle;
		std::string m_Data;
		ControlScript() :
			m_LuaTableRef {-1},
			m_Handle{ 0, 0 },
			m_Data{}
		{

		}
		ControlScript(const AssetHandle& asset_handle, const std::string& data = "")
			: m_LuaTableRef{ -1 },
			m_Handle{asset_handle},
			m_Data{data}
		{

		}
	private:
	};
}