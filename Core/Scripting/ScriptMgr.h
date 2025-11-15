#pragma once
#include "lua.hpp"
#include "luaconf.h"
#include <memory>
#include <vector>
#include <string>
#include "PhysicsState.h"
#include "Camera.h"

class Scene;
class Character;

namespace scripting
{
	namespace MT {
		constexpr const char* INPUT_MT = "Input";
		constexpr const char* SCENE_MT = "Scene";
		constexpr const char* CAMERA_MT = "Camera";
		constexpr const char* CHARACTER_MT = "Character";
		constexpr const char* PHYSICSSTATE_MT = "PhysicsState";
		constexpr const char* RENDERCOMPONENT_MT = "RenderComponent";
	}

	struct LuaVec3 {
		glm::vec3* vec;
	};

	class ScriptMgr
	{
	public:

		static void PrintLuaStack(lua_State* L);
		static int PrintStack(lua_State* L);
		static int IsKeyDown(lua_State* L);

		static std::string GetLuaFilenameWithoutExtension(const std::string& path);
		static void ExecuteScript(lua_State* L, const char* script, size_t size, const char* name);
		static void ExecuteScriptFunction(lua_State* L, const char* script, const char* function_name, float ts);

		// -------------------Input-------------------
		static void register_input(lua_State* L);

		// -------------------Scene-------------------
		static Scene* lua_checkScene(lua_State* L, int index);
		static Camera* lua_checkSceneCamera(lua_State* L, int index);
		static void expose_scene(lua_State* L, Scene* scene, const char* name);
		static void expose_scene_camera(lua_State* L, Camera* camera, const char* name);
		static void register_scene(lua_State* L);
		static void register_scene_camera(lua_State* L);
		static int lua_Scene_GetTitle(lua_State* L);
		static int lua_Scene_gc(lua_State* L);
		static int lua_pushScene(lua_State* L, Scene* pscene);
		static int lua_pushSceneCamera(lua_State* L, Camera* pcamera);
		static int lua_Scene_CreateCharacter(lua_State* L);
		static int lua_Scene_DestroyCharacter(lua_State* L);
		static int lua_Scene_CameraUnProject(lua_State* L);

		// ------------------Character-------------------
		static Character* lua_checkCharacter(lua_State* L, int index);
		static void expose_character(lua_State* L, Character* character, const char* name);
		static void register_character(lua_State* L);
		static int lua_pushCharacter(lua_State* L, Character* character);
		static int lua_Character_AddMesh(lua_State* L);
		static int lua_Character_GetMesh(lua_State* L);
		static int lua_Character_AddRenderComponent(lua_State* L);
		static int lua_Character_GetRenderComponent(lua_State* L);
		static int lua_Character_AddPhysicsState(lua_State* L);
		static int lua_Character_GetPhysicsState(lua_State* L);
		static int lua_Character_ApplyForce(lua_State* L);
		static int lua_Character_ApplyRotation(lua_State* L);
		static int lua_Character_GetLocalForward(lua_State* L);
		static int lua_Character_GetLocalRight(lua_State* L);

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