#pragma once
#include <vector>
#include <string>

enum ShaderType {
	None = 0, VERTEX, TESSELATION, GEOMETRY, PIXEL, COMPUTE 
};
struct ShaderInfo
{
	ShaderType shaderType;
	std::string filePath;
};
class ShaderResource
{
private:
	std::vector<ShaderInfo> m_Info;
	std::vector<std::string> m_ShaderSource;
public:
	ShaderResource() :
		m_Info{},
		m_ShaderSource{}
	{

	}
	ShaderResource(const std::vector<ShaderInfo> shader_infos);
	ShaderResource& operator= (const ShaderResource& other) = default;
	const std::vector<ShaderInfo>& GetShaderInfo() const;
	const std::vector<std::string>& GetShaderSources() const;
};