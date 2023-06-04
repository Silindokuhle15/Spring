#include "Serializer.h"

void Serializer::Serialize(Scene* scene)
{
}

Scene* Serializer::Deserilize(const char* file_path)
{
    
    Application* temp_app;

    // Read the file
    std::string whole_file;
    std::ifstream is(file_path, std::ios::binary | std::ios::in);

    if (!is.is_open())
    {
        std::cerr << "Failed to open the file\n" << file_path << "us not a vaild file" << std::endl;
        std::flush(std::cerr);
    }

    // Now try to read the entire 
    const char* descriptors[] = { "Object", "IndexCount", "Speed",
        "transform", "Positions", "TexCoords", 
        "Normals", "VertexIndices","TextureIndices", "NormalIndices"
    };
    
    std::string header;
    for (std::string str; std::getline(is, str);)
    {
        is >> header;
        
        if (header == descriptors[0]) 
        {
            // Object description is the header used to define the start of a new object
            // Actually nothing to do but pass to the next stage

        }
        if (header == descriptors[1])
        {
            unsigned int index_count = 0;
            is >> index_count;
            temp_app->m_IndexCount = index_count;
            
            // This should be it, hope it works
        }  
        if (header == descriptors[2])
        {
            float speed;
            is >> speed;
            temp_app->m_Speed = speed;

            // Hope again
        }  
        if (header == descriptors[3])
        {
            // Now read the transform, gonna be a tough one this one

        }        
        if (header == descriptors[4])
        {
            //
        }        
        if (header == descriptors[5])
        {

        }        
        if (header == descriptors[6])
        {

        }
        if (header == descriptors[7])
        {

        }
        if (header == descriptors[8])
        {

        }
        if (header == descriptors[9])
        {

        }
    }

}
