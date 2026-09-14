#ifndef _SCRIPT_SYSTEM_H_
#define _SCRIPT_SYSTEM_H_
#include "Script.h"
#include "ScriptMgr.h"
class Scene;
namespace scripting
{
	class ScriptSystem
	{
	public:
		explicit ScriptSystem();
		~ScriptSystem();
		void Update(Scene& scene, float time_step);
		void ExposeScene(Scene* p_scene, const char* title);
		void InitScript(Character* p_character, scripting::ControlScript& script);
		lua_State* GetLuaState() const { return m_pLuaState; }
	private:
		lua_State* m_pLuaState = nullptr;
	};
}
#endif // !_SCRIPT_SYSTEM_H_
