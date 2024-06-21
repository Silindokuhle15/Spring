//#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"

int GL_Texture::LoadTextureFromFile(const char* file_path, GLuint* m_Tex)
{

    int width, height, channels;

    if (!fopen(file_path, "r"))
    {
        std::cerr << "Texture File Path not valid " << std::endl;
        return -1;
    }

    stbi_uc* data = stbi_load(file_path, &width, &height, &channels, 0);

    //m_Width = width;
    //m_Height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, m_Tex);
    //glBindTexture(GL_TEXTURE_2D, m_Tex1);
    //glBindTextureUnit(0, m_Tex1);
    GLenum format = GL_RGB8;

    glTextureStorage2D(*m_Tex, 1, format, width, height);

    glTextureParameteri(*m_Tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(*m_Tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureSubImage2D(*m_Tex, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //glTexParameteri(GL_TEXTURE_2D, GL_UNPACK_ALIGNMENT, 1);

    stbi_image_free(data);
    return 0;
}