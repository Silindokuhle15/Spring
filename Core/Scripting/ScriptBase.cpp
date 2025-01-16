#include "ScriptBase.h"

void scripting::ScriptBase::Run()
{

}

void scripting::ScriptBase::SetScriptPath(const std::string& path)
{
	this->ScriptPath = path;
}

void scripting::ScriptBase::SetKeys(const std::vector<std::string> keys_t)
{
	this->Keys = keys_t;
}

lua_Number scripting::ScriptBase::GetNamedField(lua_State* L, const char * field)
{
	lua_Number result{ 0 };
	int is_number {0};
	switch (lua_getfield(L, -1, field))
	{
	case LUA_TNUMBER:
		result = (lua_Number)lua_tonumberx(L, -1, &is_number);
		if (!is_number)
		{
			int error = 1;
		}
		lua_pop(L, 1);
		return result;
	case LUA_TSTRING:
		return -1;
	case LUA_TTABLE:
		lua_len(L, -1);
		return lua_tonumber(L, -1);
	default:
		result = static_cast<lua_Number>(-3);
	}
	return result;
}

void scripting::ScriptBase::GetField3fv(lua_State* L, std::vector<const char*>& fields, Vector3* data_out)
{
	for (auto& field : fields)
	{
		 if (field == "x")
		 {
			 data_out->x = GetNamedField(L, field);
		 }
		 else if (field == "y")
		 {
			 data_out->y = GetNamedField(L, field);
		 }
		 else if (field == "z")
		 {
			 data_out->z = GetNamedField(L, field);
		 }
	}
	return;
}

void scripting::ScriptBase::GetField4fv(lua_State* L, std::vector<const char*>& fields, Vector4* data_out)
{
	for (auto& field : fields)
	{
		if (field == "x")
		{
			data_out->x = GetNamedField(L, field);
		}
		else if (field == "y")
		{
			data_out->y = GetNamedField(L, field);
		}
		else if (field == "z")
		{
			data_out->z = GetNamedField(L, field);
		}
		else if (field == "w")
		{
			data_out->w = GetNamedField(L, field);
		}
	}
	return;
}
