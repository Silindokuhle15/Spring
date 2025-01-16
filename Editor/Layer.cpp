#include "Layer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int gizmoCount = 1;

void Layer::LoadImageFromFile(std::string& path, _TextureView& image_data)
{
	const char* file_path = path.c_str();
	// STBI STUFF HERE
	int width, height, channels;
	std::fstream is(file_path, std::ios::in);
	if(is.is_open())
	{
		stbi_uc* data = stbi_load(file_path, &width, &height, &channels, 0);
		image_data.m_TextureData.push_back(new float[width*height+ 1]);
		memcpy(image_data.m_TextureData.back(), data, sizeof(unsigned char) * width * height);
		image_data.m_Width = static_cast<uint64_t>(width);
		image_data.m_Height = static_cast<uint64_t>(height);
		stbi_image_free(data);
	}
	else
	{
		std::cerr << "Invalid file path " << file_path << std::endl;
	}
}

void Layer::LoadSceneFromFile(std::string& path)
{

}
