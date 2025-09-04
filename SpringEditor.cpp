#include "Core/Engine/BaseApplication.h"

int main(void)
{
    std::string path = "C:/dev/Silindokuhle15/Spring/Assets/Projects/Lobby.lua";
    Scene * pscene = new Scene{ path };
    pscene->LoadSceneFromFile();
    pscene->OnInit();

    scripting::ScriptMgr::register_scene(pscene->GetLuaState());
    scripting::ScriptMgr::register_character(pscene->GetLuaState());
    scripting::ScriptMgr::register_vector3(pscene->GetLuaState());
    scripting::ScriptMgr::register_physicsstate(pscene->GetLuaState());
    scripting::ScriptMgr::expose_scene(pscene->GetLuaState(), pscene, "myScene");
    pscene->OnCreateSceneObjects();

    BaseApplication::m_Scene = pscene;
    BaseApplication::Run();
    BaseApplication::ShutDown();
}
