#include "ShaderResource.h"
#include <fstream>

ShaderResource::ShaderResource(const std::initializer_list<ShaderInfo> shader_infos) :
    m_Info{ shader_infos }
{
    std::ifstream is;
    std::string file_data;
    for (int i = 0; i < m_Info.size(); i++)
    {
        const char* file_path = m_Info[i].filePath.c_str();

        is = std::ifstream(file_path);
        is.seekg(0, std::ios::end);
        file_data.reserve(is.tellg());
        is.seekg(0, std::ios::beg);
        file_data.assign(std::istreambuf_iterator<char>(is),
            std::istreambuf_iterator<char>());

        m_ShaderSource.push_back(file_data);
    }
}

const std::vector<ShaderInfo>& ShaderResource::GetShaderInfo() const
{
    return m_Info;
}

const std::vector<std::string>& ShaderResource::GetShaderSources() const
{
    return m_ShaderSource;
}
