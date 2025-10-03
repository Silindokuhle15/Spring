#pragma once
#include "ShaderResource.h"
#include "Common.h"

class UniformBuffer
{
public:
	std::vector<LayoutInfo> m_Uniforms;
	std::map<std::string, float>	 m_FloatMap;
	std::map<std::string, glm::vec2> m_Float2Map;
	std::map<std::string, glm::vec3> m_Float3Map;
	std::map<std::string, glm::vec4> m_Float4Map;
	std::map<std::string, glm::mat4> m_Mat4Map;
};