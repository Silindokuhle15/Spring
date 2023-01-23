#include "Material.h"

void Material::OnInit()
{
	m_MaterialID = glCreateProgram();
	m_Shader.m_Info[0] = this->m_Info[0];
	m_Shader.m_Info[1] = this->m_Info[1];

	m_Shader.OnInit(m_MaterialID);
    LoadMTLFile(mtl_path);
	m_Texture.OnInit();

	glProgramParameteri(m_MaterialID, GL_PROGRAM_SEPARABLE, GL_TRUE);
	glLinkProgram(m_MaterialID);
    //glUseProgram(m_MaterialID);
}

void Material::Bind()
{
	glUseProgram(m_MaterialID);

    KaLocation = glGetUniformLocation(m_MaterialID, "AmbientColor");
    KdLocation = glGetUniformLocation(m_MaterialID, "DiffuseColor");
    KsLocation = glGetUniformLocation(m_MaterialID, "SpecularColor");

	//m_Shader.Bind();
	m_Texture.Bind();
}

void Material::OnUpdate()
{
    glProgramUniform3fv(m_MaterialID, KaLocation, 1, glm::value_ptr(m_Ka));
    glProgramUniform3fv(m_MaterialID, KdLocation, 1, glm::value_ptr(m_Kd));
    glProgramUniform3fv(m_MaterialID, KsLocation, 1, glm::value_ptr(m_Ks));

	m_Texture.OnUpdate();	
}

void Material::LoadMTLFile(std::string path)
{
    // Read the file
    std::ifstream is(path.c_str(), std::ios::binary | std::ios::in);

    if (!is.is_open())
    {
        std::cerr << "Failed to open the file\n" << path << "us not a vaild file" << std::endl;
        std::flush(std::cerr);
    }

    // Now Extract the data and do the thingies
    //is >> std::ws; // skip whitespaces

    std::string description;
    float x, y, z;
    const char* descriptors[] = { "Ka", "Kd", "Ks", "Ni"};

    for (std::string str; std::getline(is, str);)
    {
        is >> description;
        if (description == descriptors[0])
        {
            // Ambient Color
            is >> std::skipws;
            is >> x >> y >> z;
            m_Ka = glm::vec3(x, y, z);
        }
        if (description == descriptors[1])
        {
            // Diffuse Color
            is >> std::skipws;
            is >> x >> y >> z;
            m_Kd = glm::vec3(x, y, z);
        }
        if (description == descriptors[2])
        {
            // Specular Color
            is >> std::skipws;
            is >> x >> y >> z;
            m_Ks = glm::vec3(x, y, z);
        }
        if (description == descriptors[3])
        {
            // Refractive Index
            is >> std::skipws;
            is >> x;
            m_Ni = x;
        }
        else continue;
    }
    // Now Upload the data to OpenGL
    // or Not
}
