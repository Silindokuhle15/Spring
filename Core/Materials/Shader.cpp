#include "Shader.h"

void Shader::OnInit()
{
    //Tryna Refactor this entire Functuion
    std::ifstream is;
    std::string file_data;

    for (int i = 0; i < m_Info.size(); i++)   // Must Update this not to use the hard-coded value for the number of shaders
    {
        const char* file_path = m_Info[i].FilePath.c_str();

        is = std::ifstream(file_path);
        is.seekg(0, std::ios::end);
        file_data.reserve(is.tellg());
        is.seekg(0, std::ios::beg);
        file_data.assign(std::istreambuf_iterator<char>(is),
            std::istreambuf_iterator<char>());

        m_ShaderSource.push_back(file_data);
    }
}

void Shader::Bind()
{
}