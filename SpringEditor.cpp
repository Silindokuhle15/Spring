#include "Core/Engine/BaseApplication.h"

int main(void)
{
    std::string path = "C:/dev/Silindokuhle15/Spring/out/build/Assets/Projects/Lobby.lua";
    Scene * pscene = new Scene{ path };
    pscene->LoadSceneFromFile();
    pscene->OnInit();
    pscene->OnCreateSceneObjects();

    scripting::ScriptMgr::register_scene(pscene->GetLuaState());
    scripting::ScriptMgr::register_character(pscene->GetLuaState());
    scripting::ScriptMgr::register_vector3(pscene->GetLuaState());
    scripting::ScriptMgr::register_physicsstate(pscene->GetLuaState());
    scripting::ScriptMgr::expose_scene(pscene->GetLuaState(), pscene, "myScene");

    auto nt = pscene->CreateSceneObject();
    nt->AddComponent<Mesh>(Mesh{ pscene->dynamic_mesh_paths[0] });
    nt->AddComponent<scripting::ControlScript>(scripting::ControlScript{ "C:/dev/Silindokuhle15/Spring/out/build/Assets/Projects/test.lua" });
    scripting::ScriptMgr::expose_character(pscene->GetLuaState(), nt, "myCharacter");
    
    //lua_getglobal(pscene->GetLuaState(), "myScene");
    //scripting::ScriptMgr::PrintLuaStack(pscene->GetLuaState());
    //lua_pop(pscene->GetLuaState(), 1); // clean it up after

    auto result = luaL_dofile(pscene->GetLuaState(), "C:/dev/Silindokuhle15/Spring/out/build/Assets/Projects/test.lua");
    //scripting::ScriptMgr::PrintLuaStack(pscene->GetLuaState());

    int res = lua_getglobal(pscene->GetLuaState(), "onInit");
    //res = lua_getglobal(pscene->GetLuaState(), "state");

    scripting::ScriptMgr::PrintLuaStack(pscene->GetLuaState());


    if (lua_pcall(pscene->GetLuaState(), 0, 0, 0) != LUA_OK)
    {
        auto error = true;
    }
    lua_pop(pscene->GetLuaState(), 1);
    BaseApplication::m_Scene = pscene;
    BaseApplication::Run();
    BaseApplication::ShutDown();
}
