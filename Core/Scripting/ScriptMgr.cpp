#include "ScriptMgr.h"
#include "Character.h"
#include <iostream>
#include <windows.h>
#include <cstring>

namespace scripting {

    namespace {
        constexpr const char* SCENE_MT = "Scene";
        constexpr const char* CHARACTER_MT = "Character";
        constexpr const char* PHYSICSSTATE_MT = "PhysicsState";
    }

    // --------------------- Scene ---------------------

    void ScriptMgr::expose_scene(lua_State* L, Scene* scene, const char* name) {
        lua_pushScene(L, scene);
        lua_setglobal(L, name);
    }

    int ScriptMgr::lua_Scene_GetTitle(lua_State* L) {
        Scene* scene = *static_cast<Scene**>(luaL_checkudata(L, 1, SCENE_MT));
        lua_pushstring(L, scene->GetTitle().c_str());
        return 1;
    }

    int ScriptMgr::lua_Scene_gc(lua_State* L) {
        // Do not delete the scene unless you explicitly own it.
        return 0;
    }

    Scene* ScriptMgr::lua_checkScene(lua_State* L, int index) {
        return *static_cast<Scene**>(luaL_checkudata(L, index, SCENE_MT));
    }

    void ScriptMgr::register_scene(lua_State* L) {
        luaL_newmetatable(L, SCENE_MT);
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_Scene_gc);
        lua_setfield(L, -2, "__gc");

        lua_pushcfunction(L, lua_Scene_GetTitle);
        lua_setfield(L, -2, "GetTitle");

        lua_pushcfunction(L, lua_Scene_CreateCharacter);
        lua_setfield(L, -2, "CreateCharacter");

        lua_pushcfunction(L, lua_Scene_DestroyCharacter);
        lua_setfield(L, -2, "DestroyCharacter");

        lua_pushcfunction(L, PrintStack);
        lua_setfield(L, -2, "PrintStack");

        lua_pushcfunction(L, IsKeyDown);
        lua_setfield(L, -2, "IsKeyDown");

        lua_pop(L, 1);
    }

    int ScriptMgr::lua_pushScene(lua_State* L, Scene* scene) {
        auto** userdata = static_cast<Scene**>(lua_newuserdata(L, sizeof(Scene*)));
        *userdata = scene;
        luaL_getmetatable(L, SCENE_MT);
        lua_setmetatable(L, -2);
        return 1;
    }

    int ScriptMgr::lua_Scene_CreateCharacter(lua_State* L) {
        Scene* scene = lua_checkScene(L, 1);
        // Example:
        Character* character = scene->CreateSceneObject();
        return lua_pushCharacter(L, character);
        return 0;
    }

    int ScriptMgr::lua_Scene_DestroyCharacter(lua_State* L) {
        Scene* scene = lua_checkScene(L, 1);
        // TODO: Implement actual logic
        return 0;
    }

    // --------------------- Character ---------------------

    void ScriptMgr::expose_character(lua_State* L, Character* character, const char* name) {
        lua_pushCharacter(L, character);
        lua_setglobal(L, name);
    }



    int ScriptMgr::lua_pushCharacter(lua_State* L, Character* character) {
        auto** userdata = static_cast<Character**>(lua_newuserdata(L, sizeof(Character*)));
        *userdata = character;
        luaL_getmetatable(L, CHARACTER_MT);
        lua_setmetatable(L, -2);
        return 1;
    }



    Character* ScriptMgr::lua_checkCharacter(lua_State* L, int index) {
        return *static_cast<Character**>(luaL_checkudata(L, index, CHARACTER_MT));
    }

    int ScriptMgr::lua_Character_AddMesh(lua_State* L)
    {
        Character* charater = lua_checkCharacter(L, 1);
        const char* path = luaL_checkstring(L, 2);
        PrintLuaStack(L);
        charater->AddComponent<Mesh>(Mesh{ path });
        return 0;
    }

    int ScriptMgr::lua_Character_GetMesh(lua_State* L)
    {
        return 0;
    }
    int ScriptMgr::lua_Character_AddPhysicsState(lua_State* L) {
        Character* character = lua_checkCharacter(L, 1);
        character->AddComponent<physics::PhysicsState>();
        return 0;
    }

    int ScriptMgr::lua_Character_GetPhysicsState(lua_State* L) {
        Character* character = lua_checkCharacter(L, 1);

        if (!character->HasComponent<physics::PhysicsState>()) {
            return luaL_error(L, "Character has no PhysicsState");
        }

        physics::PhysicsState* state = &character->GetComponent<physics::PhysicsState>();
        auto** userdata = static_cast<physics::PhysicsState**>(lua_newuserdata(L, sizeof(physics::PhysicsState*)));
        *userdata = state;

        luaL_getmetatable(L, PHYSICSSTATE_MT);
        lua_setmetatable(L, -2);
        return 1;
    }

    int ScriptMgr::lua_Character_ApplyForce(lua_State* L)
    {
        Character* character = lua_checkCharacter(L, 1);
        float temp[] = { 0,0,0 };
        if (lua_istable(L, 2))
        {
            for (int i = 0; i < 3; ++i) {
                lua_rawgeti(L, 2, i + 1);
                if (lua_isnumber(L, -1)) {
                    temp[i] = static_cast<float>(lua_tonumber(L, -1));
                }
                lua_pop(L, 1);
            }
        }

        glm::vec3 vt{ temp[0], temp[1], temp[2] };
        physics::PhysicsState* state = &character->GetComponent<physics::PhysicsState>();
        state->linear_acceleration = glm::normalize(vt) * 0.0166f;
        state->velocity += state->linear_acceleration * 0.01667f;
        state->position += state->velocity * 0.01667f;
        //state->position += glm::normalize(vt) * 0.01667f;
        return 0;
    }

    void ScriptMgr::register_character(lua_State* L) {
        luaL_newmetatable(L, CHARACTER_MT);

        lua_newtable(L);
        lua_pushcfunction(L, lua_Character_AddMesh);
        lua_setfield(L, -2, "AddMesh");

        lua_pushcfunction(L, lua_Character_GetMesh);
        lua_setfield(L, -2, "GetMesh");

        lua_pushcfunction(L, lua_Character_AddPhysicsState);
        lua_setfield(L, -2, "AddPhysicsState");

        lua_pushcfunction(L, lua_Character_GetPhysicsState);
        lua_setfield(L, -2, "GetPhysicsState");

        lua_pushcfunction(L, lua_Character_ApplyForce);
        lua_setfield(L, -2, "ApplyForce");

        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);
    }

    int ScriptMgr::push_vec3(lua_State* L, float* v)
    {
        return 0;
    }

    // --------------------------vec3--------------------------

    int ScriptMgr::push_glmvec3(lua_State* L, glm::vec3* v) {
        auto** userdata = static_cast<LuaVec3**>(lua_newuserdata(L, sizeof(LuaVec3*)));
        *userdata = new LuaVec3{ v };
        luaL_getmetatable(L, "Vector3");
        lua_setmetatable(L, -2);
        return 1;
    }

    LuaVec3* ScriptMgr::lua_checkVector3(lua_State* L, int index) {
        return *static_cast<LuaVec3**>(luaL_checkudata(L, index, "Vector3"));
    }

    int ScriptMgr::lua_vec3_add(lua_State* L) {
        auto a = lua_checkVector3(L, 1)->vec;
        auto b = lua_checkVector3(L, 2)->vec;

        auto* result = new glm::vec3(*a + *b);
        auto** userdata = static_cast<glm::vec3**>(lua_newuserdata(L, sizeof(glm::vec3*)));
        *userdata = result;

        luaL_getmetatable(L, "Vector3");
        lua_setmetatable(L, -2);
        return 1;
    }

    int ScriptMgr::lua_vec3_index(lua_State* L) {
        glm::vec3* v = lua_checkVector3(L, 1)->vec;
        const char* key = luaL_checkstring(L, 2);

        if (strcmp(key, "x") == 0) lua_pushnumber(L, v->x);
        else if (strcmp(key, "y") == 0) lua_pushnumber(L, v->y);
        else if (strcmp(key, "z") == 0) lua_pushnumber(L, v->z);
        else lua_pushnil(L);
        return 1;
    }

    int ScriptMgr::lua_vec3_newindex(lua_State* L) {
        glm::vec3* v = lua_checkVector3(L, 1)->vec;
        const char* key = luaL_checkstring(L, 2);
        float val = static_cast<float>(luaL_checknumber(L, 3));

        if (strcmp(key, "x") == 0) v->x = val;
        else if (strcmp(key, "y") == 0) v->y = val;
        else if (strcmp(key, "z") == 0) v->z = val;

        return 0;
    }

    int ScriptMgr::lua_Vector3_constructor(lua_State* L)
    {
        int nargs = lua_gettop(L);  // Number of args passed

        float x = (nargs >= 1) ? static_cast<float>(luaL_checknumber(L, 1)) : 0.0f;
        float y = (nargs >= 2) ? static_cast<float>(luaL_checknumber(L, 2)) : 0.0f;
        float z = (nargs >= 3) ? static_cast<float>(luaL_checknumber(L, 3)) : 0.0f;

        auto* vec = new glm::vec3(x, y, z);
        auto** userdata = static_cast<glm::vec3**>(lua_newuserdata(L, sizeof(glm::vec3*)));
        *userdata = vec;

        luaL_getmetatable(L, "Vector3");
        lua_setmetatable(L, -2);
        return 1;
    }

    int ScriptMgr::push_vec3_ref(lua_State* L, glm::vec3* vec)
    {
        auto** userdata = static_cast<glm::vec3**>(lua_newuserdata(L, sizeof(glm::vec3*)));
        *userdata = vec;

        luaL_getmetatable(L, "Vector3");
        lua_setmetatable(L, -2);

        return 1;
    }

    int ScriptMgr::push_quat_ref(lua_State* L, glm::quat* vec)
    {
        auto** userdata = static_cast<glm::quat**>(lua_newuserdata(L, sizeof(glm::quat*)));
        *userdata = vec;

        luaL_getmetatable(L, "Quaternion");
        lua_setmetatable(L, -2);

        return 1;
    }

    void ScriptMgr::register_vector3(lua_State* L)
    {
        luaL_newmetatable(L, "Vector3");

        lua_pushcfunction(L, lua_vec3_index);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_vec3_newindex);
        lua_setfield(L, -2, "__newindex");

        lua_pushcfunction(L, lua_vec3_add);
        lua_setfield(L, -2, "__add");

        lua_pop(L, 1);
        lua_pushcfunction(L, lua_Vector3_constructor);
        lua_setglobal(L, "Vector3");
    }

    // --------------------- PhysicsState ---------------------

    physics::PhysicsState* ScriptMgr::lua_checkPhysicsState(lua_State* L, int index) {
        return *static_cast<physics::PhysicsState**>(luaL_checkudata(L, index, PHYSICSSTATE_MT));
    }

    int ScriptMgr::lua_PhysicsState_index(lua_State* L) {
        physics::PhysicsState* state = lua_checkPhysicsState(L, 1);
        const char* key = luaL_checkstring(L, 2);

        if (strcmp(key, "orientation") == 0)
        {
            lua_newtable(L);
            for (int i = 0; i < 4; ++i)
            {
                lua_pushnumber(L, state->orientation[i]);
                lua_rawseti(L, -2, i + 1);
            }
        }
        else if (strcmp(key, "position") == 0) {
            lua_newtable(L);
            for (int i = 0; i < 3; ++i) {
                lua_pushnumber(L, state->position[i]);
                lua_rawseti(L, -2, i + 1);
            }
        }
        else if (strcmp(key, "mass") == 0) {
            lua_pushnumber(L, state->mass);
        }
        else if (strcmp(key, "velocity") == 0) {
            lua_newtable(L);
            for (int i = 0; i < 3; ++i) {
                lua_pushnumber(L, state->velocity[i]);
                lua_rawseti(L, -2, i + 1);
            }
        }
        else if (strcmp(key, "restitution") == 0) {
            lua_pushnumber(L, state->restitution);
        }
        else if (strcmp(key, "linear_acceleration") == 0) {
            lua_newtable(L);
            for (int i = 0; i < 3; ++i) {
                lua_pushnumber(L, state->linear_acceleration[i]);
                lua_rawseti(L, -2, i + 1);
            }
        }
        else if (strcmp(key, "unused_1") == 0) {
            lua_pushnumber(L, state->unused_1);
        }
        else if (strcmp(key, "angular_acceleration") == 0) {
            lua_newtable(L);
            for (int i = 0; i < 3; ++i) {
                lua_pushnumber(L, state->angular_acceleration[i]);
                lua_rawseti(L, -2, i + 1);
            }
        }
        else if (strcmp(key, "unused_2") == 0) {
            lua_pushnumber(L, state->unused_2);
        }
        else if (strcmp(key, "inertia") == 0) {
            lua_newtable(L);
            for (int i = 0; i < 3; ++i) {
                lua_pushnumber(L, state->inertia[i]);
                lua_rawseti(L, -2, i + 1);
            }
        }
        else if (strcmp(key, "unused_3") == 0) {
            lua_pushnumber(L, state->unused_3);
        }
        else {
            lua_pushnil(L);
        }
        return 1;
    }

    int ScriptMgr::lua_PhysicsState_newindex(lua_State* L) {
        physics::PhysicsState* state = lua_checkPhysicsState(L, 1);
        const char* key = luaL_checkstring(L, 2);

        if (strcmp(key, "orientation") == 0 && lua_istable(L, 3))
        {   for (int i = 0; i < 4; ++i){
            lua_rawgeti(L, 3, i + 1);
                if (lua_isnumber(L, -1)){
                    state->orientation[i] = static_cast<float>(lua_tonumber(L, -1));
                }
                lua_pop(L, 1);
            }
        }
        else if (strcmp(key, "position") == 0 && lua_istable(L, 3))
        {   for (int i = 0; i < 3; ++i){
                lua_rawgeti(L, 3, i + 1);
                if (lua_isnumber(L, -1)){
                    state->position[i] = static_cast<float>(lua_tonumber(L, -1));
                }
                lua_pop(L, 1);
            }
        }
        else if (strcmp(key, "mass") == 0) {
            state->mass = static_cast<float>(luaL_checknumber(L, 3));
        }
        else if (strcmp(key, "velocity") == 0 && lua_istable(L, 3)) {
            for (int i = 0; i < 3; ++i) {
                lua_rawgeti(L, 3, i + 1);
                if (lua_isnumber(L, -1)) {
                    state->velocity[i] = static_cast<float>(lua_tonumber(L, -1));
                }
                lua_pop(L, 1);
            }
        }
        else if (strcmp(key, "restitution") == 0) {
            state->restitution = static_cast<float>(luaL_checknumber(L, 3));
        }
        else if (strcmp(key, "linear_acceleration") == 0 && lua_istable(L, 3)) {
            for (int i = 0; i < 3; ++i) {
                lua_rawgeti(L, 3, i + 1);
                if (lua_isnumber(L, -1)) {
                    state->linear_acceleration[i] = static_cast<float>(lua_tonumber(L, -1));
                }
                lua_pop(L, 1);
            }
        }
        else if (strcmp(key, "unused_1") == 0) {
            state->unused_1 = static_cast<float>(luaL_checknumber(L, 3));
        }
        else if (strcmp(key, "angular_acceleration") == 0 && lua_istable(L, 3)) {
            for (int i = 0; i < 3; ++i) {
                lua_rawgeti(L, 3, i + 1);
                if (lua_isnumber(L, -1)) {
                    state->angular_acceleration[i] = static_cast<float>(lua_tonumber(L, -1));
                }
                lua_pop(L, 1);
            }
        }
        else if (strcmp(key, "unused_2") == 0) {
            state->unused_2 = static_cast<float>(luaL_checknumber(L, 3));
        }
        else if (strcmp(key, "inertia") == 0 && lua_istable(L, 3)) {
            for (int i = 0; i < 3; ++i) {
                lua_rawgeti(L, 3, i + 1);
                if (lua_isnumber(L, -1)) {
                    state->inertia[i] = static_cast<float>(lua_tonumber(L, -1));
                }
                lua_pop(L, 1);
            }
        }
        else if (strcmp(key, "unused_3") == 0) {
            state->unused_3 = static_cast<float>(luaL_checknumber(L, 3));
        }

        return 0;
    }

    void ScriptMgr::register_physicsstate(lua_State* L) {
        luaL_newmetatable(L, PHYSICSSTATE_MT);

        lua_pushcfunction(L, lua_PhysicsState_index);
        lua_setfield(L, -2, "__index");

        lua_pushcfunction(L, lua_PhysicsState_newindex);
        lua_setfield(L, -2, "__newindex");

        lua_pop(L, 1);
    }

    // --------------------- Debug ---------------------

    void ScriptMgr::PrintLuaStack(lua_State* L) {
        int top = lua_gettop(L);
        std::cout << "====== Lua Stack (Top = " << top << ") ======" << std::endl;

        for (int i = 1; i <= top; ++i) {
            int t = lua_type(L, i);
            std::cout << i << ": ";

            switch (t) {
            case LUA_TSTRING:
                std::cout << "string\t'" << lua_tostring(L, i) << "'";
                break;
            case LUA_TBOOLEAN:
                std::cout << "boolean\t" << (lua_toboolean(L, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:
                std::cout << "number\t" << lua_tonumber(L, i);
                break;
            case LUA_TTABLE:
                std::cout << "table\t" << lua_topointer(L, i);
                break;
            case LUA_TFUNCTION:
                std::cout << "function\t" << lua_topointer(L, i);
                break;
            case LUA_TUSERDATA:
                std::cout << "userdata\t" << lua_topointer(L, i);
                break;
            case LUA_TLIGHTUSERDATA:
                std::cout << "light userdata\t" << lua_topointer(L, i);
                break;
            case LUA_TTHREAD:
                std::cout << "thread";
                break;
            case LUA_TNIL:
                std::cout << "nil";
                break;
            default:
                std::cout << "unknown";
                break;
            }

            std::cout << std::endl;
        }

        std::cout << "====================================" << std::endl;
    }

    int ScriptMgr::PrintStack(lua_State* L)
    {
        PrintLuaStack(L);
        return 0;
    }

    int ScriptMgr::IsKeyDown(lua_State* L)
    {
        Scene* scene = lua_checkScene(L, 1);
        auto keyString = lua_tostring(L, 2);
        std::string key{ keyString };
        if (key == "w" && (GetKeyState(0x57) & 0x8000))
        {
            lua_pushboolean(L, 1);
            return 1;
        }
        else if (key == "a" && (GetKeyState(0x41) & 0x8000))
        {
            lua_pushboolean(L, 1);
            return 1;
        }
        else if (key == "s" && (GetKeyState(0x53) & 0x8000))
        {
            lua_pushboolean(L, 1);
            return 1;
        }
        else if (key == "d" && (GetKeyState(0x44) & 0x8000))
        {
            lua_pushboolean(L, 1);
            return 1;
        }
        else if (key == "LMOUSE" && (GetKeyState(0x01) & 0x8000))
        {
            lua_pushboolean(L, 1);
            return 1;
        }
        else if (key == "RMOUSE" && (GetKeyState(0x02) & 0x8000))
        {
            lua_pushboolean(L, 1);
            return 1;
        }
        else if (key == "MMOUSE" && (GetKeyState(0x04) & 0x8000))
        {
            lua_pushboolean(L, 1);
            return 1;
        }
        return 0;
    }

    

} // namespace scripting
