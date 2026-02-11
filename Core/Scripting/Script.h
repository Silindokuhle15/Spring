#pragma once
#include "ScriptBase.h"
#include <string>
#include <fstream>
#include "UUID.h"
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
		AssetHandle m_Handle;
		std::string m_Data;
		ControlScript(const AssetHandle& asset_handle, const std::string& data = "")
			: m_Handle{asset_handle},
			m_Data{data}
		{

		}
	private:
	};
}