#ifndef _COMPONENT_UTILS_H_
#define _COMPONENT_UTILS_H_
#include <iostream>
#include <string>
#include "Asset.h"
#include "PhysicsState.h"
#include "Mesh.h"
#include "Script.h"
#include "ParticleSystem.h"
#include "nlohmann/json.hpp"

std::ostream& operator<<(std::ostream& out, const AssetHandle& assetHandle);

std::ostream& operator<<(std::ostream& os, const glm::vec3& v);
std::ostream& operator<<(std::ostream& os, const glm::quat& q);

std::ostream& operator<<(std::ostream& out, const physics::PhysicsState& s);

std::ostream& operator<<(std::ostream& out, const primitives::MeshInstance& mesh);

std::ostream& operator<<(std::ostream& out, const scripting::ControlScript& script);

std::string ReadLuaScriptFromDisk(const std::string& path);
using json = nlohmann::json;

void to_json(json& j, const AssetHandle& h);
void from_json(const json& j, AssetHandle& h);

void to_json(json& j, const AssetResource& r);
void from_json(const json& j, AssetResource& r);

namespace glm
{
	void to_json(json& j, const glm::vec3& v);
	void from_json(const json& j, glm::vec3& v);

	void to_json(json& j, const glm::vec4& v);
	void from_json(const json& j, glm::vec4& v);

	void to_json(json& j, const glm::quat& q);
	void from_json(const json& j, glm::quat& q);
}
namespace physics
{
	void to_json(json & j, const physics::PhysicsState & state);
	void from_json(const json & j, physics::PhysicsState & state);
}

namespace primitives
{
	void to_json(json& j, const primitives::MeshInstance& mesh);
	void from_json(const json& j, primitives::MeshInstance& mesh);

	void to_json(json& j, const primitives::EmitterShape& emitter_shape);
	void from_json(const json& j, primitives::EmitterShape& emitter_shape);

	void to_json(json& j, const primitives::EmitterFlags& emitter_flags);
	void from_json(const json& j, primitives::EmitterFlags& emitter_flags);

	void to_json(json& j, const primitives::EmitterInfo& emitter_info);
	void from_json(const json& j, primitives::EmitterInfo& emitter_info);

	//void to_json(json& j, const primitives::ParticleSystem& particle_system);
	//void from_json(const json& j, primitives::ParticleSystem& particle_system);
}

namespace scripting
{
	void to_json(json& j, const scripting::ControlScript& script);
	void from_json(const json& j,  scripting::ControlScript& script);
}
#endif