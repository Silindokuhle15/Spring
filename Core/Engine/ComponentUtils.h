#ifndef _COMPONENT_UTILS_H_
#define _COMPONENT_UTILS_H_
#include <iostream>
#include <string>
#include "PhysicsState.h"
#include "Mesh.h"
#include "Script.h"

std::ostream& operator<<(std::ostream& out, const AssetHandle& assetHandle);

std::ostream& operator<<(std::ostream& os, const glm::vec3& v);
std::ostream& operator<<(std::ostream& os, const glm::quat& q);

std::ostream& operator<<(std::ostream& out, const physics::PhysicsState& s);

std::ostream& operator<<(std::ostream& out, const primitives::MeshInstance& mesh);

std::ostream& operator<<(std::ostream& out, const scripting::ControlScript& script);

std::string ReadLuaScriptFromDisk(const std::string& path);

#endif