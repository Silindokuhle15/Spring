#include "Layer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int gizmoCount = 1;

void Layer::LoadImageFromFile(std::string& path, _TextureView& image_data)
{
	const char* file_path = path.c_str();
	// STBI STUFF HERE
	int width, height, channels;

	if (fopen(file_path, "r"))
	{
		stbi_uc* data = stbi_load(file_path, &width, &height, &channels, 0);
		std::string s(reinterpret_cast<char const*>(data), width * height);
		image_data.m_TextureData = s;
		image_data.m_Width = static_cast<uint64_t>(width);
		image_data.m_Height = static_cast<uint64_t>(height);
		stbi_image_free(data);
	}
	else
	{
		std::cerr << "Invalid file path " << file_path << std::endl;
	}
}

void Layer::LoadSceneFromFile(std::string& path)
{
	m_LuaEngine.SetScriptPath(path);

	m_LuaEngine.SetKeys(
		std::vector<std::string>({
			"WIDTH",
			"HEIGHT",
			"dynamic_geometry",
			"static_geometry",
			//LIGHTS
			"light_position",
			"light_color",
			//CAMERA
			"scene_camera",
			// TEXTUTES
			"material"
			}
		)
	);

	m_LuaEngine.Run();
	// CREATE LIGHTS

	PointLight pointLight;
	Vector3 dummy_pos = {};
	Vector3 dummy_color = {};

	// CREATE THE SCENE CAMERA(s)
	std::vector<const char*> vec3_keys =
	{
		"x",
		"y",
		"z"
	};
	Vector3 vec3 = {};
	std::string str = "";


	lua_Number number = 0;

	for (auto& var : m_LuaEngine.Keys)
	{
		lua_getglobal(m_LuaEngine.m_pLuaState, var.c_str());
		switch (lua_type(m_LuaEngine.m_pLuaState, -1))
		{
		case LUA_TTABLE:

			if (var == "static_geometry")
			{
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "OBJ_path"))
				{
					str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
					static_mesh_paths.push_back(str);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				break;
			}
			else if (var == "dynamic_geometry")
			{
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "OBJ_path"))
				{
					str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
					dynamic_mesh_paths.push_back(str);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				break;
			}
			else if (var == "material")
			{
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "VShaderPath"))
				{
					str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
					shader_paths.push_back(str);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "FShaderPath"))
				{
					str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
					shader_paths.push_back(str);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "material_texture"))
				{
					str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
					shader_paths.push_back(str);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "mtl_path"))
				{
					str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
					shader_paths.push_back(str);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				break;
			}
			else if (var == "scene_camera")
			{

				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "eye"))
				{
					m_LuaEngine.GetField(m_LuaEngine.m_pLuaState, vec3_keys, &vec3);
					m_ActiveCamera.m_eye = glm::vec3(vec3.x, vec3.y, vec3.z);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "center"))
				{
					m_LuaEngine.GetField(m_LuaEngine.m_pLuaState, vec3_keys, &vec3);
					m_ActiveCamera.m_center = glm::vec3(vec3.x, vec3.y, vec3.z);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "up"))
				{
					m_LuaEngine.GetField(m_LuaEngine.m_pLuaState, vec3_keys, &vec3);
					m_ActiveCamera.m_up = glm::vec3(vec3.x, vec3.y, vec3.z);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				break;
			}

			if (var == "light_position")
			{
				dummy_pos = vec3;
			}
			else if (var == "light_color")
			{
				dummy_color = vec3;
			}
			break;
		case LUA_TSTRING:

			if (var == "obj_path")
			{
				str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
				OBJ_paths.push_back(str);
				lua_pop(m_LuaEngine.m_pLuaState, 1);
			}
			break;

		case LUA_TNUMBER:
			number = lua_tonumber(m_LuaEngine.m_pLuaState, -1);
			var == "WIDTH" ? m_ActiveCamera.SetWidth(static_cast<int>(number)) : void();
			var == "HEIGHT" ? m_ActiveCamera.SetHeight(static_cast<int>(number)) : void();
			lua_pop(m_LuaEngine.m_pLuaState, 1);
			break;
		}

	}
}

void Layer::CreateSceneObjects()
{

}
