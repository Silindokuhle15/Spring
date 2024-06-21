#pragma
#include <stdint.h>
#include <string>

enum class _TextureFormat
{
	RGB8
};
enum class _TextureSource
{
	FILE,
	GL_ATTACHMENT
};

enum class _TextureTarget
{
	TEXTURE_1D,
	TEXTURE_2D,
	TEXTURE_3D
};

class _TextureView
{
public:
	uint64_t m_Width;
	uint64_t m_Height;
	uint64_t m_NumMipMaps;
	uint64_t m_MipMapLevel;
};

class _TextureDescription
{
public:
	_TextureFormat m_TextureFormat;
	_TextureSource m_TextureSource;
	_TextureTarget m_TextureTarget;
};

template<typename PLATFORM>
class TextureBase : public _TextureView, public _TextureDescription
{
public:

	std::string m_Source;
	uint64_t m_ID;
	PLATFORM m_Texture;

	virtual void OnInit()
	{
		m_Texture.OnInit();
	}
	virtual void Bind()
	{
		m_Texture.Bind(0);
	}
	virtual void OnUpdate()
	{
		m_Texture.OnUpdate();
	}

	TextureBase(std::string path="", uint64_t id = 0xFFFFFFFF) :
		m_Source{ path },
		m_ID{id},
		m_Texture{path}
	{

	}

};