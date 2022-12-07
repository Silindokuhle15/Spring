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
    is >> std::ws; // skip whitespaces

    std::string description;
    float x, y, z;
    float u, v;
  
    std::vector<std::string> descriptors = { "v", "vt", "f" };
    while (std::getline(is, temp))
    {
        is >> description; // get the first word out and check it out
        
        if (description == descriptors[0])// Vertex Position
        {
            is >> x >> y >> z;
            m_Positions.push_back(glm::vec3(x, y, z));
        }
        if (description == descriptors[1])// Texture Coordinate
        {
            is >> u >> v;
            m_TexCoords.push_back(glm::vec2(u, v));
        }
        if (description == descriptors[2]) // Face information
        {
            struct PerVertex vertex;
            while (is >> temp)
            {
                if (temp == "f") continue;
                else
                {
                    int vindex = atoi(temp.substr(0, 1).c_str());
                    int tindex = atoi(temp.substr(2, 3).c_str());
                    if (vindex && tindex)
                    {
                        m_VertexIndices.push_back(vindex - 1);

                        memset(&vertex, 0, sizeof(struct PerVertex));
                        vertex.Position = m_Positions[vindex - 1];
                        vertex.TexCoord = m_TexCoords[tindex - 1];

                        m_Vertices.push_back(vertex);
                    }
                }
            }
        }
    }

    // Now Upload the data to OpenGL
    // or Not
    return 0;
}

void ObjectLoader::LoadObject(const char* file_path)
{ 
    int ret = LoadObjectFromFile(file_path);
}
