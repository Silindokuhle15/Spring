#pragma once
#include "ScriptBase.h"
#include <string>
#include <fstream>
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
		std::string data;
		ControlScript(const std::string& path = "")
			:
			m_ScriptPath{path},
			data{}
		{
			std::string pt{ path };
			auto extr = pt.substr(pt.find("."));

			if (extr == ".lua")
			{
				// READ THE FILE FROM DISK
				std::ifstream is;				
				is = std::ifstream(path);
				is.seekg(0, std::ios::end);
				data.reserve(is.tellg());
				is.seekg(0, std::ios::beg);
				data.assign(std::istreambuf_iterator<char>(is),
					std::istreambuf_iterator<char>());
			}
			else if (extr == ".luac")
			{
				std::string file_data;
				std::ifstream is(path, std::ios::binary);

				if (!is) { //Handle error here
				}
				is.seekg(0, std::ios::end);
				std::streamsize size = is.tellg();
				is.seekg(0, std::ios::beg);
				data.resize(size);
				if (!is.read(&data[0], size))
				{
					//Handle error here
				}
			}
		}
	private:
	};
}