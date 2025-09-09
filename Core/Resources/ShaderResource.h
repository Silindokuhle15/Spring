#pragma once
#include <vector>
#include <string>

enum class ShaderType {
	None = 0, VERTEX, TESSELATION, GEOMETRY, PIXEL, COMPUTE 
};
struct ShaderInfo
{
	std::string filePath;
	const ShaderType shaderType;
};
class ShaderResource
{
private:
	std::vector<ShaderInfo> m_Info;
	std::vector<std::string> m_ShaderSource;
public:
	ShaderResource(const std::initializer_list<ShaderInfo> shader_infos);
	const std::vector<ShaderInfo>& GetShaderInfo() const;
	const std::vector<std::string>& GetShaderSources() const;
};