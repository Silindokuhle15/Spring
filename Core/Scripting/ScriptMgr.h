#pragma once
#include "lua.hpp"
#include "luaconf.h"
#include <memory>
#include <vector>
#include <string>
#include "PhysicsState.h"

class Scene;
class Character;

namespace scripting
{
	struct LuaVec3 {
		glm::vec3* vec;
	};

	class ScriptMgr
	{
	public:

		static void PrintLuaStack(lua_State* L);
		static int PrintStack(lua_State* L);

		// -------------------Scene-------------------
		static Scene* lua_checkScene(lua_State* L, int index);
		static void expose_scene(lua_State* L, Scene* scene, const char* name);
		static void register_scene(lua_State* L);
		static int lua_Scene_GetTitle(lua_State* L);
		static int lua_Scene_gc(lua_State* L);
		static int lua_pushScene(lua_State* L, Scene* pscene);
		static int lua_Scene_CreateCharacter(lua_State* L);
		static int lua_Scene_DestroyCharacter(lua_State* L);

		// ------------------Character-------------------
		static Character* lua_checkCharacter(lua_State* L, int index);
		static void expose_character(lua_State* L, Character* character, const char* name);
		static void register_character(lua_State* L);
		static int lua_pushCharacter(lua_State* L, Character* character);
		static int lua_Character_AddPhysicsState(lua_State* L);
		static int lua_Character_GetPhysicsState(lua_State* L);
		static int lua_Character_ApplyForce(lua_State* L);

		// --------------------Vector3--------------------
		static LuaVec3* lua_checkVector3(lua_State* L, int index);
		static void register_vector3(lua_State* L);
		static int lua_Vector3_constructor(lua_State* L);
		static int push_vec3(lua_State* L, float* v);
		static int push_glmvec3(lua_State* L, glm::vec3* v);
		static int push_vec3_ref(lua_State* L, glm::vec3* vec);
		static int push_quat_ref(lua_State* L, glm::quat* vec);
		static int lua_vec3_add(lua_State* L);
		static int lua_vec3_index(lua_State* L);
		static int lua_vec3_newindex(lua_State* L);
		// ------------------PhysicsState-----------------
		static physics::PhysicsState* lua_checkPhysicsState(lua_State* L, int index);
		static void register_physicsstate(lua_State* L);
		static int lua_PhysicsState_index(lua_State* L);
		static int lua_PhysicsState_newindex(lua_State* L);
		
		// --------------------Forces---------------------


	};
}