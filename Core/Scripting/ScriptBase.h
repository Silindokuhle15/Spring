#pragma once
#include "lua.hpp"
#include "luaconf.h"
#include "Common.h"

namespace scripting
{
	class ScriptBase
	{
	public:
		std::string ScriptPath;
		std::vector<std::string> Keys;

		void Run();
		void SetScriptPath(const std::string& path);
		void SetKeys(const std::vector<std::string> keys_t);

		lua_Number GetNamedField(lua_State* L, const char* field);
		void GetField3fv(lua_State* L, std::vector<const char*>& field, Vector3* data_out);
		void GetField4fv(lua_State* L, std::vector<const char*>& field, Vector4* data_out);

		ScriptBase() 
		{
		}
		~ScriptBase() 
		{
		}

	private:
	};
}