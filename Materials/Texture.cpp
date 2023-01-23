#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Texture::OnInit()
{
    //int res = LoadTextureFromFile("Textures/brinkwall.png");
    int res = LoadTextureFromFile("Textures/crate_1.jpg");

    glGenSamplers(1, &m_Sampler);
    //glBindSampler(4, GL_SAMPLER_2D);
    glBindSampler(0, m_Sampler);
    //glSamplerParameteri(m_Sampler, GL_UNPACK_ALIGNMENT, 1);

    glSamplerParameteri(m_Sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(m_Sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Texture::Bind()
{
    glBindTextureUnit(0, m_Tex1); 
}
void Texture::OnUpdate()
{
    glUniform1i(m_Sampler, 0);
}
int Texture::LoadTextureFromFile(const char* file_path)
{
    int width, height, channels;

    if (!fopen(file_path, "r"))
    {
        std::cerr << "Texture File Path not valid " << std::endl;
        return -1;
    }

    stbi_uc * data = stbi_load(file_path, &width, &height, &channels, 0);

    m_Width = width;
    m_Height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_Tex1);
    //glBindTexture(GL_TEXTURE_2D, m_Tex1);
    //glBindTextureUnit(0, m_Tex1);
    GLenum format = GL_RGB8;

    glTextureStorage2D(m_Tex1, 1, format, m_Width, m_Height);

    glTextureParameteri(m_Tex1, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_Tex1, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureSubImage2D(m_Tex1, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    //glTexParameteri(GL_TEXTURE_2D, GL_UNPACK_ALIGNMENT, 1);

    stbi_image_free(data);
    return 0;
}
