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
using json = nlohmann::json;

void to_json(json& j, const AssetHandle& h)
{
	j = json{ h.m_HWORD, h.m_LWORD };
}

void from_json(const json& j, AssetHandle& h)
{
	h.m_HWORD = j.at(0).get<uint64_t>();
	h.m_LWORD = j.at(1).get<uint64_t>();
}

void to_json(json& j, const AssetResource& r)
{
	j = json{ r.m_Type, r.m_Filepath };
}

void from_json(const json& j, AssetResource& r)
{
	r.m_Type = j.at(0).get<AssetType>();
	r.m_Filepath = j.at(1).get<std::string>();
}

namespace glm
{
	void to_json(json& j, const glm::vec3& v)
	{
		j = json{ v.x, v.y, v.z };
	}

	void from_json(const json& j, glm::vec3& v)
	{
		v.x = j.at(0);
		v.y = j.at(1);
		v.z = j.at(2);
	}

	void to_json(json& j, const glm::vec4& v)
	{
		j = json{ v.x, v.y, v.z, v.w };
	}

	void from_json(const json& j, glm::vec4& v)
	{
		v.x = j.at(0);
		v.y = j.at(1);
		v.z = j.at(2);
		v.w = j.at(3);
	}

	void to_json(json& j, const glm::quat& q)
	{
		j = json{ q.w, q.x, q.y, q.z };
	}

	void from_json(const json& j, glm::quat& q)
	{
		q.w = j.at(0);
		q.x = j.at(1);
		q.y = j.at(2);
		q.z = j.at(3);
	}
}

namespace physics
{

	void to_json(json & j, const physics::PhysicsState & state)
	{
		j = json
		{
			{"orientation", state.orientation},
			{"position", state.position},
			{"mass", state.mass},
			{"velocity", state.velocity},
			{"restitution", state.restitution},
			{"linear_acceleration", state.linear_acceleration},
			{"unused_1", state.unused_1},
			{"angular_acceleration", state.angular_acceleration},
			{"unused_2", state.unused_2},
			{"inertia", state.inertia},
			{"unused_3", state.unused_3}
		};
	}

	void from_json(const json & j, physics::PhysicsState & state)
	{
		j.at("orientation").get_to(state.orientation);
		j.at("position").get_to(state.position);
		j.at("mass").get_to(state.mass);
		j.at("velocity").get_to(state.velocity);
		j.at("restitution").get_to(state.restitution);
		j.at("linear_acceleration").get_to(state.linear_acceleration);
		j.at("unused_1").get_to(state.unused_1);
		j.at("angular_acceleration").get_to(state.angular_acceleration);
		j.at("unused_2").get_to(state.unused_2);
		j.at("inertia").get_to(state.inertia);
		j.at("unused_3").get_to(state.unused_3);
	}
}

namespace primitives
{
	void to_json(json& j, const primitives::MeshInstance& mesh)
	{
		j = json{ "assetHandle", mesh.m_Handle };
	}
	void from_json(const json& j, primitives::MeshInstance& mesh)
	{
		j.at("assetHandle").get_to(mesh.m_Handle);
	}
	void to_json(json& j, const primitives::EmitterShape& emitter_shape)
	{
		int shape = static_cast<int>(emitter_shape);
		j = json{ shape };
	}
	void from_json(const json& j, primitives::EmitterShape& emitter_shape)
	{
		int shape = j.at(0).get<int>();
		emitter_shape = static_cast<primitives::EmitterShape>(shape);
	}
	void to_json(json& j, const primitives::EmitterFlags& emitter_flags)
	{
		j = json
		{
			{"is_enabled", emitter_flags.m_IsEnabled},
			{"fill", emitter_flags.m_Fill},
			{"unused1", emitter_flags.m_Unused1},
			{"unused2", emitter_flags.m_Unused2}
		};
	}
	void from_json(const json& j, primitives::EmitterFlags& emitter_flags)
	{
		j.at("is_enabled").get_to(emitter_flags.m_IsEnabled);
		j.at("fill").get_to(emitter_flags.m_Fill);
		j.at("unused1").get_to(emitter_flags.m_Unused1);
		j.at("unused2").get_to(emitter_flags.m_Unused2);
	}
	void to_json(json& j, const primitives::EmitterInfo& emitter_info)
	{
		j = json
		{
			{"emitter_shape", emitter_info.m_Shape},
			{"emitter_flags", emitter_info.m_Flags},
			{"vector_one", emitter_info.m_VectorOne},
			{"vector_two", emitter_info.m_VectorTwo}
		};
	}
	void from_json(const json& j, primitives::EmitterInfo& emitter_info)
	{
		j.at("emitter_shape").get_to(emitter_info.m_Shape);
		j.at("emitter_flags").get_to(emitter_info.m_Flags);
		j.at("vector_one").get_to(emitter_info.m_VectorOne);
		j.at("vector_two").get_to(emitter_info.m_VectorTwo);
	}
	/*/
	void to_json(json& j, const primitives::ParticleSystem& particle_system)
	{
		int shape = static_cast<int>(particle_system.m_EmitterInfo.m_Shape);
		j = json
		{
			{"emitter_info", particle_system.m_EmitterInfo},
			{"buffer_offset", particle_system.m_BufferOffset},
			{"num_particles", particle_system.m_NumParticles},
			{"max_particles", particle_system.m_MaxNumParticles},
			{"particle_rate", particle_system.m_ParticleRate},
			{"time_step", particle_system.m_Ts},
			{"accumulated_time", particle_system.m_AccumulatedTime},
			{"duration", particle_system.m_Duration},
			{"initialSpeed", particle_system.m_InitialSpeed},
			{"shader_handle", particle_system.m_ShaderHandle},
			{"texture_handle", particle_system.m_TextureHandle}
		};
	}
	void from_json(const json& j, primitives::ParticleSystem & particle_system)
	{
		j.at("emitter_info").get_to(particle_system.m_EmitterInfo);
		j.at("buffer_offset").get_to(particle_system.m_BufferOffset);
		j.at("num_particles").get_to(particle_system.m_NumParticles);
		j.at("max_particles").get_to(particle_system.m_MaxNumParticles);
		j.at("particle_rate").get_to(particle_system.m_ParticleRate);
		j.at("time_step").get_to(particle_system.m_Ts);
		j.at("accumulated_time").get_to(particle_system.m_AccumulatedTime);
		j.at("duration").get_to(particle_system.m_Duration);
		j.at("initialSpeed").get_to(particle_system.m_InitialSpeed);
		j.at("shader_handle").get_to(particle_system.m_ShaderHandle);
		j.at("texture_handle").get_to(particle_system.m_TextureHandle);
	}
	/**/
}

namespace scripting
{
	void to_json(json& j, const scripting::ControlScript& script)
	{
		j = json{ 
			{"assetHandle", script.m_Handle},
			{"scriptData", script.m_Data}
		};
	}
	void from_json(const json& j, scripting::ControlScript& script)
	{
		j.at("assetHandle").get_to(script.m_Handle);
		j.at("scriptData").get_to(script.m_Data);
	}
}

