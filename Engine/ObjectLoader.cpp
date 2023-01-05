#include "ObjectLoader.h"

int ObjectLoader::LoadObjectFromFile(const char* file_path)
{
    // Read the file
    std::string whole_file;
    std::ifstream is(file_path, std::ios::binary | std::ios::in);

    if (!is.is_open())
    {
        std::cerr << "Failed to open the file\n" << file_path << "us not a vaild file" << std::endl;
        std::flush(std::cerr);
    }

    // Now all the data is stored in whole_file
    // Now Extract the data and do the thingies
    std::string temp = "";
    std::string c_temp = "";
    //is >> std::ws; // skip whitespaces

    std::string description;
    float x, y, z;
    float u, v;

    const char* descriptors[] = { "v", "vn", "vt", "f" };
    unsigned int vindex, tindex;

    for (std::string str; std::getline(is, str);)
    {
        is >> description;
        if (description == descriptors[0])
        {
            // Vertex Position
            is >> std::skipws;
            is >> x >> y >> z;
            m_Positions.push_back(glm::vec3(x, y, z));
        }
        if (description == descriptors[1])
        {
            // Vertex Normal
            is >> std::skipws;
            is >> x >> y >> z;
            m_Normals.push_back(glm::vec3(x, y, z));
        }
        if (description == descriptors[2])
        {
            // Texture Coordinate
            is >> std::skipws;
            is >> u >> v;
            m_TexCoords.push_back(glm::vec2(u, v));
        }
        if (description == descriptors[3])
        {
            std::stringstream ss(str);
            while(ss >> c_temp)
            {
                ExtractDump(c_temp);
            }
        }
        else continue;
    }
    // Now Upload the data to OpenGL
    // or Not
    return 0;
}

void ObjectLoader::LoadObject(const char* file_path)
{ 
    int ret = LoadObjectFromFile(file_path);
}

void ObjectLoader::ExtractDump(std::string dump)
{
    std::istringstream iss(dump);
    unsigned int vindex;
    iss >> vindex;
    m_VertexIndices.push_back(--vindex);
    char delim;
    iss.read(&delim, 1);

    unsigned int nindex;
    unsigned int tindex;

    iss.read(&delim, 1);
    
    /*
    iss >> tindex;
    m_TextureIndices.push_back(--tindex);
    
    */
   
    iss >> nindex;
    m_NormalIndices.push_back(--nindex);
    //iss.read(&delim, 1);
}