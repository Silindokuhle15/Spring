#include "Shader.h"

void Shader::OnInit()
{
    m_ShaderProgram = glCreateProgram();

    //Tryna Refactor this entire Functuion
    std::ifstream is;
    std::string file_data;

    for (int i = 0; i < 2; i++)   // Must Update this not to use the hard-coded value for the number of shaders
    {
        const char* file_path = m_Info[i].FilePath;

        is = std::ifstream(file_path);
        is.seekg(0, std::ios::end);
        file_data.reserve(is.tellg());
        is.seekg(0, std::ios::beg);
        file_data.assign(std::istreambuf_iterator<char>(is),
            std::istreambuf_iterator<char>());

        const char* file_data_text = file_data.c_str();
        //file_data.clear();

        GLenum shader_type = m_Info[i].ShaderType;
        m_Shader[i] = glCreateShader(shader_type);
        glShaderSource(m_Shader[i], 1, &file_data_text, NULL);
        glCompileShader(m_Shader[i]);

        glAttachShader(m_ShaderProgram, m_Shader[i]);
        //glDeleteShader(m_Shaders[i]);
        //glDeleteProgram(m_ShaderPrograms[i]);
    }

    glProgramParameteri(m_ShaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
    glLinkProgram(m_ShaderProgram);
}


void Shader::Bind()
{
    glUseProgram(m_ShaderProgram);
}

void Shader::Link()
{
    if (!glIsProgram(m_ShaderProgram)) {
        m_ShaderProgram = glCreateProgram();
        glProgramParameteri(m_ShaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
    }
    glAttachShader(m_ShaderProgram, m_Shader[0]);
    glAttachShader(m_ShaderProgram, m_Shader[1]);

    glLinkProgram(m_ShaderProgram);
}
