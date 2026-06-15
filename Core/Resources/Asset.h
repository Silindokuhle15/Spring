#ifndef _ASSET_H_
#define _ASSET_H_
#include "UUID.h"
#include <string>

enum class AssetType
{
	None = 0,
	ComputeShaderResource,
	GraphicsShaderResource,
	Texture2D,
	CubeMap,
	Material,
	MeshResource,
	ScriptResource,
	SoundClipResource
};

struct AssetResource
{
public:
	AssetType m_Type;
	std::string m_Filepath;
	bool operator<(const AssetResource& other) const
	{
		return (m_Filepath < other.m_Filepath);
	}
};

#endif