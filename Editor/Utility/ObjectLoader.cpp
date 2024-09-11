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

    uint64_t object_index = 0;
    auto material_index = 0;

    std::vector<const char*> descriptorSet = { "v", "vt", "vn","f", "#", "o", "s", "usemtl", "mtllib"};

    for (std::string str=""; std::getline(is, str);)
    {
        //auto index = str.find(" ");
        //description = str.substr(0, index);
        auto words = getWords(str, " ");
        description = words[0];

        if (CheckDescription(description, "mtllib"))
        {
            m_MaterialPaths.push_back(words[1]);
            LoadMaterialFromFile(m_MaterialPaths[m_MaterialPaths.size() - 1].c_str());
        }
        if (CheckDescription(description, "usemtl"))
        {
            auto& mat_name = words[1];
            for (size_t index = 0; index < m_MaterialNames.size(); index++)
            {
                if (m_MaterialNames[index] == mat_name)
                {
                    material_index = index;
                }
            }

        }
        else  if (CheckDescription(description, "o"))
        {
            // CREATE NEW MESH AND THEN BASH THEM TOGETHER

            m_ObjectNames.push_back(words[1]);
            m_Positions.push_back({});
            m_TexCoords.push_back({});
            m_VertexIDs.push_back({});
            m_Normals.push_back({});

            m_VertexIndices.push_back({});
            m_TextureIndices.push_back({});
            m_NormalIndices.push_back({});
            object_index++;
        }
        else if (CheckDescription(description, "s"))
        {
            m_Surfaces.push_back(words[1]);
        }
        else if (CheckDescription(description, "v"))
        {
            // Vertex Position
            x = std::stof(words[1]);
            y = std::stof(words[2]);
            z = std::stof(words[3]);
            m_Positions[object_index - 1].push_back(glm::vec3(x, y, z));
            //m_VertexIDs[object_index - 1].push_back(material_index);
        }
        else if (CheckDescription(description, "vt"))
        {
            // Texture Coordinate
            u = std::stof(words[1]);
            v = std::stof(words[2]);
            m_TexCoords[object_index - 1].push_back(glm::vec2(u, v));
        }
        else if (CheckDescription(description, "vn"))
        {
            // Vertex Normal
            x = std::stof(words[1]);
            y = std::stof(words[2]);
            z = std::stof(words[3]);
            m_Normals[object_index - 1].push_back(glm::vec3(x, y, z));
        }
        else if (CheckDescription(description, "f"))
        {
            ExtractDump(str, object_index - 1);
            m_VertexIDs[object_index - 1].push_back(material_index);
            m_VertexIDs[object_index - 1].push_back(material_index);
            m_VertexIDs[object_index - 1].push_back(material_index);
        }

        is.clear();
    }
    return 0;
}

int ObjectLoader::LoadMaterialFromFile(const char* file_path)
{
    // Read the file
    std::ifstream is(file_path, std::ios::binary | std::ios::in);

    if (!is.is_open())
    {
        std::cerr << "Failed to open the file\n" << file_path << " is not a vaild file" << std::endl;
        std::flush(std::cerr);
    }

    // Now Extract the data and do the thingies
    //is >> std::ws; // skip whitespaces

    std::string description;
    float x, y, z;
    const char* descriptors[] = {"newmtl", "Ka", "Kd", "Ks", "Ni", "Ns", "d", "illum"};
    auto object_index = 0;

    for (std::string str; std::getline(is, str);)
    {
        auto words = getWords(str, " ");
        description = words[0];
        //is >> description;
        if (CheckDescription(description, "newmtl"))
        {
            m_Materials.push_back(Material(object_index));
            m_MaterialNames.push_back(words[1]);
            object_index++;
        }
        if (CheckDescription(description, "Ka"))
        {
            // Ambient Color
            x = std::stof(words[1]);
            y = std::stof(words[2]);
            z = std::stof(words[3]);
            m_Materials[object_index-1].m_Ka = glm::vec3(x, y, z);
        }
        if (CheckDescription(description, "Kd"))
        {
            // Diffuse Color
            x = std::stof(words[1]);
            y = std::stof(words[2]);
            z = std::stof(words[3]);
            m_Materials[object_index-1].m_Kd = glm::vec3(x, y, z);
        }
        if (CheckDescription(description, "Ks"))
        {
            // Specular Color
            x = std::stof(words[1]);
            y = std::stof(words[2]);
            z = std::stof(words[3]);
            m_Materials[object_index-1].m_Ks = glm::vec3(x, y, z);
        }
        if (CheckDescription(description, "Ni"))
        {
            // Refractive Index
            x = std::stof(words[1]);
            //y = std::stof(words[2]);
            //z = std::stof(words[3]);
            m_Materials[object_index-1].m_Ni = x;
        }
        if (CheckDescription(description, "Ns"))
        {
            // Refractive Index
            x = std::stof(words[1]);
            //y = std::stof(words[2]);
            //z = std::stof(words[3]);
            m_Materials[object_index-1].m_Ns= x;
        }
        if (CheckDescription(description, "d"))
        {
            x = std::stof(words[1]);
            //y = std::stof(words[2]);
            //z = std::stof(words[3]);
            m_Materials[object_index-1].m_d = x;
        }
        if (CheckDescription(description, "illum"))
        {
            x = std::stof(words[1]);
            //y = std::stof(words[2]);
            //z = std::stof(words[3]);
            //m_Materials[object_index-1].m_illum = x;
        }
        else continue;
    }
    return 0;
}

void ObjectLoader::LoadObject(const char* file_path)
{ 
    int ret = LoadObjectFromFile(file_path);
}

void ObjectLoader::ExtractDump(std::string dump, uint64_t object_index)
{
    /*
    std::istringstream iss(dump);
    unsigned int vindex;
    unsigned int tindex;
    unsigned int nindex;

    iss >> vindex;
    m_VertexIndices[object_index].push_back(--vindex);
    
    char delim;
    iss.read(&delim, 1);
    
    if (iss >> tindex) {
        m_TextureIndices[object_index].push_back(--tindex);
    }
    else {
        m_TextureIndices[object_index].push_back(0x00000000);
    }
    iss.read(&delim, 1);
   
    iss >> nindex;
    m_NormalIndices[object_index].push_back(--nindex);
    //iss.read(&delim, 1);

    */
    dump = dump.substr(2, dump.length());
    auto string_words = getWords(dump, " ");
    char* endptr;
    for (auto& word_temp : string_words)
    {
        auto digits = getWords(word_temp, "/");
        switch (digits.size())
        {
            /*
        case 2:
            m_VertexIndices.push_back((strtoul(digits[0].c_str(), &endptr, 10)) - 1);
            m_TextureIndices.push_back(0xFFFFFFFF);
            m_NormalIndices.push_back((strtoul(digits[1].c_str(), &endptr, 10)) - 1);
            break;
            */
        case 3:
            m_VertexIndices[object_index].push_back((strtoul(digits[0].c_str(), &endptr, 10)) - 1);
            m_TextureIndices[object_index].push_back((strtoul(digits[1].c_str(), &endptr, 10)) - 1);
            m_NormalIndices[object_index].push_back((strtoul(digits[2].c_str(), &endptr, 10)) - 1);
            break;
        }
    }
}

std::vector<std::string> ObjectLoader::getWords(std::string s, const char* delim) {
    std::vector<std::string> res;
    int pos = 0;
    while (pos < s.size()) {
        pos = s.find(delim);
        res.push_back(s.substr(0, pos));
        s.erase(0, pos + 1); // 3 is the length of the delimiter, "%20"
    }
    res.push_back(s);
    return res;
}