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

    std::vector<const char*> descriptorSet = { "v", "vt", "vn","f", "#", "o"};

    for (std::string str=""; std::getline(is, str);)
    {
        auto index = str.find(" ");
        description = str.substr(0, index);

        if (CheckDescription(description, "o"))
        {
            // CREATE NEW MESH AND THEN BASH THEM TOGETHER
            //m_Positions.clear();
            //m_TexCoords.clear();
            //m_Normals.clear();
            //m_VertexIDs.clear();
            //m_TextureIndices.clear();
            //m_NormalIndices.clear();
            //is.clear();
        }
        else if (CheckDescription(description, "v"))
        {
            // Vertex Position
            auto words = getWords(str," ");
            x = std::stof(words[1]);
            y = std::stof(words[2]);
            z = std::stof(words[3]);
            m_Positions.push_back(glm::vec3(x, y, z));
            m_VertexIDs.push_back(id++);
        }
        else if (CheckDescription(description, "vt"))
        {
            // Texture Coordinate
            auto words = getWords(str, " ");
            u = std::stof(words[1]);
            v = std::stof(words[2]);
            m_TexCoords.push_back(glm::vec2(u, v));
        }
        else if (CheckDescription(description, "vn"))
        {
            // Vertex Normal
            auto words = getWords(str, " ");
            x = std::stof(words[1]);
            y = std::stof(words[2]);
            z = std::stof(words[3]);
            m_Normals.push_back(glm::vec3(x, y, z));
        }
        else if (CheckDescription(description, "f"))
        {
            ExtractDump(str);            
        }
    }
    return 0;
}

void ObjectLoader::LoadObject(const char* file_path)
{ 
    int ret = LoadObjectFromFile(file_path);
}

void ObjectLoader::ExtractDump(std::string dump)
{
    /*
    std::istringstream iss(dump);
    unsigned int vindex;
    unsigned int tindex;
    unsigned int nindex;

    iss >> vindex;
    m_VertexIndices.push_back(--vindex);
    
    char delim;
    iss.read(&delim, 1);
    
    if (iss >> tindex) {
        m_TextureIndices.push_back(--tindex);
    }
    else {
        m_TextureIndices.push_back(0xffffffff);
    }
    iss.read(&delim, 1);
   
    iss >> nindex;
    m_NormalIndices.push_back(--nindex);
    //iss.read(&delim, 1);

    */
    dump = dump.substr(2, dump.length());
    auto string_words = getWords(dump, " ");
    char* endptr;
    for (auto word_temp : string_words)
    {
        auto digits = getWords(word_temp, "/");
        switch (digits.size())
        {
        case 2:
            m_VertexIndices.push_back((strtoul(digits[0].c_str(), &endptr, 10)) - 1);
            m_TextureIndices.push_back(0xFFFFFFFF);
            m_NormalIndices.push_back((strtoul(digits[1].c_str(), &endptr, 10)) - 1);
            break;
        case 3:
            m_VertexIndices.push_back((strtoul(digits[0].c_str(), &endptr, 10)) - 1);
            m_TextureIndices.push_back((strtoul(digits[1].c_str(), &endptr, 10)) - 1);
            m_NormalIndices.push_back((strtoul(digits[2].c_str(), &endptr, 10)) - 1);
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