#include "ComponentUtils.h"

std::ostream& operator<<(std::ostream& out, const AssetHandle& assetHandle)
{
	out << "AssetHandle = { " << assetHandle.m_HWORD << ", " << assetHandle.m_LWORD << " }\n";
	return out;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& v)
{
    return os << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
}

std::ostream& operator<<(std::ostream& os, const glm::quat& q)
{
    return os << "{ " << q.w << ", " << q.x << ", "
        << q.y << ", " << q.z << " }";
}
std::ostream& operator<<(std::ostream& out, const physics::PhysicsState& s)
{
    out << "physics::PhysicsState = {\n"
        << "\torientation = " << s.orientation << '\n'
        << "\tposition = " << s.position << '\n'
        << "\tmass = " << s.mass << '\n'
        << "\tvelocity = " << s.velocity << '\n'
        << "\trestitution = " << s.restitution << '\n'
        << "\tlinear_acceleration = " << s.linear_acceleration << '\n'
        << "\tunused_1 = " << s.unused_1 << '\n'
        << "\tangular_acceleration = " << s.angular_acceleration << '\n'
        << "\tunused_2 = " << s.unused_2 << '\n'
        << "\tinertia = " << s.inertia << '\n'
        << "\tunused_3 = " << s.unused_3 << '\n'
        << "}\n";

    return out;
}

std::ostream& operator<<(std::ostream& out, const primitives::MeshInstance& mesh)
{
    out << "primitives::MeshInstance = { " << mesh.m_Handle.m_HWORD << ", " << mesh.m_Handle.m_LWORD << " }\n";
    return out;
}

std::ostream& operator<<(std::ostream& out, const scripting::ControlScript& script)
{
    out << "scripting::ControlScript = {\n"
		<< script.m_Handle << '\n'
        << "}\n";
    return out;
}

std::string ReadLuaScriptFromDisk(const std::string& path)
{
	std::string data;
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
    return data;
}
