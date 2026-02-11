#include "AssetManager.h"
#include "stb_image.h"

void AssetManager::CreateOpenGLTexture(TextureBase<GL_Texture>& tex_base)
{
    GLuint target = 0;
    GLenum format = 0;
    auto data = tex_base.m_TextureData.data();

    GLuint width = static_cast<GLuint>(tex_base.m_Width);
    GLuint height = static_cast<GLuint>(tex_base.m_Height);

    switch (tex_base.m_TextureTarget)
    {
    case _TextureTarget::TEXTURE_1D:
        glCreateTextures(GL_TEXTURE_1D, 1, &tex_base.m_Texture);
        glTextureStorage1D(tex_base.m_Texture, 1, format, width);
        break;
    case _TextureTarget::TEXTURE_2D:
        glCreateTextures(GL_TEXTURE_2D, 1, &tex_base.m_Texture);
        glTextureParameteri(tex_base.m_Texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(tex_base.m_Texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        switch (tex_base.m_TextureFormat)
        {
        case _TextureFormat::RGB8:
            glTextureStorage2D(tex_base.m_Texture, 1, GL_RGB8, width, height);
            break;
        case _TextureFormat::RGB32F:
            glTextureStorage2D(tex_base.m_Texture, 1, GL_RGB32F, width, height);
            break;
        case _TextureFormat::RGBA8:
            glTextureStorage2D(tex_base.m_Texture, 1, GL_RGBA8, width, height);
            break;
        case _TextureFormat::RGBA32F:
            glTextureStorage2D(tex_base.m_Texture, 1, GL_RGBA32F, width, height);
            break;
        }
        break;
    case _TextureTarget::TEXTURE_3D:
        glCreateTextures(GL_TEXTURE_3D, 1, &tex_base.m_Texture);
        glTextureStorage3D(tex_base.m_Texture, 1, format, width, height, 0);
        break;

    case _TextureTarget::TEXTURE_CUBE:
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &tex_base.m_Texture);

        switch (tex_base.m_TextureFormat)
        {
        case _TextureFormat::RGB8:
            glBindTexture(GL_TEXTURE_CUBE_MAP, tex_base.m_Texture);
            glTextureStorage2D(tex_base.m_Texture, 1, GL_RGB8, width, height);
            break;
        case _TextureFormat::RGB32F:
            glBindTexture(GL_TEXTURE_CUBE_MAP, tex_base.m_Texture);
            glTextureStorage2D(tex_base.m_Texture, 1, GL_RGB32F, width, height);
            break;
        case _TextureFormat::RGBA8:
            glBindTexture(GL_TEXTURE_CUBE_MAP, tex_base.m_Texture);
            glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, width, height);
            //glTextureStorage2D(tex_base.m_Texture, 1, GL_RGBA8, width, height);
            break;
        case _TextureFormat::RGBA32F:
            glBindTexture(GL_TEXTURE_CUBE_MAP, tex_base.m_Texture);
            glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_RGBA32F, width, height);
            //glTextureStorage2D(tex_base.m_Texture, 1, GL_RGBA32F, width, height);
            break;
        }
        break;
    }
}

AssetHandle AssetManager::CreateOpenGLCubeMap(const std::vector<std::string>& image_file_paths)
{
    int width = 0;
    int height = 0;
    int channels = 0;
    int desired_channels = 4;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* right = stbi_load(image_file_paths[0].c_str(), &width, &height, &channels, desired_channels);
    stbi_uc* left = stbi_load(image_file_paths[1].c_str(), &width, &height, &channels, desired_channels);
    stbi_uc* top = stbi_load(image_file_paths[2].c_str(), &width, &height, &channels, desired_channels);
    stbi_uc* bottom = stbi_load(image_file_paths[3].c_str(), &width, &height, &channels, desired_channels);
    stbi_uc* front = stbi_load(image_file_paths[4].c_str(), &width, &height, &channels, desired_channels);
    stbi_uc* behind = stbi_load(image_file_paths[5].c_str(), &width, &height, &channels, desired_channels);

    TextureBase<GL_Texture> cubeMap;

    cubeMap.m_Height = static_cast<uint64_t>(width);
    cubeMap.m_Width = static_cast<uint64_t>(height);

    switch (channels)
    {
    case 3:
        cubeMap.m_TextureFormat = _TextureFormat::RGB8;        
        break;
    case 4:
        cubeMap.m_TextureFormat = _TextureFormat::RGBA8;
        break;
    }

    cubeMap.m_TextureSource = _TextureSource::GL_ATTACHMENT;
    cubeMap.m_TextureTarget = _TextureTarget::TEXTURE_CUBE;

    CreateOpenGLTexture(cubeMap);

    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, cubeMap.m_Width, cubeMap.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, right);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, cubeMap.m_Width, cubeMap.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, left);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, cubeMap.m_Width, cubeMap.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, top);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, cubeMap.m_Width, cubeMap.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, bottom);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, cubeMap.m_Width, cubeMap.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, front);
    glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, cubeMap.m_Width, cubeMap.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, behind);

    stbi_image_free(right);
    stbi_image_free(left);
    stbi_image_free(top);
    stbi_image_free(bottom);
    stbi_image_free(front);
    stbi_image_free(behind);

    std::string flatPath{ "" };
    for (auto& string : image_file_paths)
    {
        flatPath += string + "\n";
    }
    AssetHandle assetHandle = CreateAssetHandleFromPath(flatPath.c_str());
    m_TextureMap[assetHandle] = cubeMap;
    return assetHandle;
}

AssetHandle AssetManager::CreateOpenGLFrameBuffer(TextureBase<GL_Texture>& tex_base)
{
    GLuint platformHandle{ 0 };
    glCreateFramebuffers(1, &platformHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, platformHandle);
    CreateOpenGLTexture(tex_base);
    glNamedFramebufferTexture(platformHandle, GL_COLOR_ATTACHMENT0, tex_base.m_Texture, 0);
    std::string frameBufferString = "OpenGL_FrameBuffer" + platformHandle + tex_base.m_Texture;
    AssetHandle assetHandle = CreateAssetHandleFromPath(frameBufferString.c_str());
    m_TextureMap[assetHandle] = tex_base;
    m_FrameBufferMap[assetHandle] = platformHandle;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return assetHandle;
}

TextureBase<GL_Texture> AssetManager::LoadTextureFromFile(const std::string& image_path, bool flip_vertically)
{
    int width{ 0 }, height{ 0 }, channels{ 0 }, desiredChannels{ 4 };
    stbi_set_flip_vertically_on_load(flip_vertically);
    stbi_uc* imageData = stbi_load(image_path.c_str(), &width, &height, &channels, desiredChannels);
    TextureBase<GL_Texture> glTexture;
    glTexture.m_Height = static_cast<uint64_t>(height);
    glTexture.m_Width = static_cast<uint64_t>(width);
    glTexture.m_TextureFormat = _TextureFormat::RGBA8;
    glTexture.m_TextureSource = _TextureSource::FILE;
    glTexture.m_TextureTarget = _TextureTarget::TEXTURE_2D;
    CreateOpenGLTexture(glTexture);
    glTextureSubImage2D(glTexture.m_Texture, 0,  0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    stbi_image_free(imageData);
    return glTexture;
}
