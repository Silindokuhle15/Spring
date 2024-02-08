#include "ObjectLoader.h"


bool CheckDescription(std::string str1, std::string str2)
{
    return (bool)str1.compare(str2) == 0 ? true : false;
}

bool InDescriptorSet(const char* disc, std::vector<const char*> descriptors)
{
    for (auto& i : descriptors)
    {
        if (CheckDescription(i, disc))
        {
            return true;
        }
    }
    return false;
}

int ObjectLoader::LoadObjectFromFile(const char* file_path)
{
    unsigned int id = 1;
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

    std::vector<const char*> descriptorSet = { "v", "vt", "vn","f", "#"};
    //std::string str;


    for (std::string str=""; std::getline(is, str);)
    {
        is >> description;
        if (InDescriptorSet(description.c_str(), descriptorSet))
        {
            if (CheckDescription(description, "v"))
            {
                // Vertex Position
                is >> std::skipws;
                is >> x >> y >> z;
                m_Positions.push_back(glm::vec3(x, y, z));
                m_VertexIDs.push_back(id++);

            }
            else if (CheckDescription(description, "vt"))
            {
                // Texture Coordinate
                is >> std::skipws;
                is >> u >> v;
                m_TexCoords.push_back(glm::vec2(u, v));
            }
            else if (CheckDescription(description, "vn"))
            {
                // Vertex Normal
                is >> std::skipws;
                is >> x >> y >> z;
                m_Normals.push_back(glm::vec3(x, y, z));
            }

            else if (CheckDescription(description, "f"))
            {
                is >> std::skipws;
                //is >> temp;
                //std::stringstream ss(temp);
                
                while (is >> c_temp)
                {
                    if (CheckDescription(c_temp.c_str(), "f"))
                    {
                        c_temp.clear();
                    }
                    else
                    {
                        ExtractDump(c_temp);
                    }
                }
            }
        }
        //str.clear();
    }
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
    unsigned int tindex;
    unsigned int nindex;

    iss >> vindex;
    m_VertexIndices.push_back(--vindex);
    
    char delim;
    iss.read(&delim, 1);
    
    iss >> tindex;
    m_TextureIndices.push_back(--tindex);

    iss.read(&delim, 1);
   
    iss >> nindex;
    m_NormalIndices.push_back(--nindex);
    //iss.read(&delim, 1);
}