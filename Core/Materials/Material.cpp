#include "Material.h"

void Material::OnInit()
{
	m_Shader.OnInit(m_MaterialID);
    LoadMTLFile(mtl_path);
	m_Texture.OnInit();
}

void Material::Bind()
{
	//m_Shader.Bind();
	m_Texture.Bind();
}

void Material::OnUpdate()
{

}

void Material::LoadMTLFile(std::string path)
{
    // Read the file
    std::ifstream is(path.c_str(), std::ios::binary | std::ios::in);

    if (!is.is_open())
    {
        std::cerr << "Failed to open the file\n" << path << " is not a vaild file" << std::endl;
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
