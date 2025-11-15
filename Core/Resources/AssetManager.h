#pragma once
#include <map>
#include <string>
#include "UUID.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Script.h"
#include "FrameBuffer.h"

enum class AssetType
{
	None = 0,
	ShaderResource,
	Texture2D,
	Material,
	MeshResource,
	ScriptResource
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
class AssetManager
{
private:
	std::map<AssetResource, AssetHandle> m_AssetResourceAndHandleMap;
	//std::map<AssetHandle, FrameBuffer> m_FrameBufferMap;
	std::map<AssetHandle, unsigned int> m_FrameBufferMap;
	std::map<AssetHandle, Shader> m_ShaderMap;
	std::map<AssetHandle, Material> m_MaterialMap;
	std::map<AssetHandle, TextureBase<GL_Texture>> m_TextureMap;
	std::map<AssetHandle, Mesh> m_MeshMap;
	std::map<AssetHandle, scripting::ControlScript> m_ScriptMap;
	uint64_t m_CurrentAssetHandle;
public:
	AssetManager() :
		m_AssetResourceAndHandleMap{},
		m_CurrentAssetHandle{ 0 }
	{

	}

	void CreateOpenGLTexture(TextureBase<GL_Texture>& tex_base);
	AssetHandle CreateOpenGLCubeMap(const std::vector<std::string>& image_file_paths);
	AssetHandle CreateOpenGLFrameBuffer(TextureBase<GL_Texture>& tex_base);

	template<typename T>
	const T& GetAsset(const AssetHandle& asset_handle);

	TextureBase<GL_Texture> LoadTextureFromFile(const std::string& image_path, bool flip_vertically = false);

	AssetHandle GetResourceHandle(const AssetResource& resource)
	{
		auto& assetHandle = m_AssetResourceAndHandleMap[resource];
		if ((assetHandle.m_HWORD == 0) && (assetHandle.m_LWORD == 0))
		{
			assetHandle.m_LWORD = ++m_CurrentAssetHandle;
	
			if (resource.m_Type == AssetType::MeshResource)
			{
				Mesh mesh{ resource.m_Filepath.c_str() };
				auto& material = mesh.m_Materials;
				m_MaterialMap[assetHandle] = material.front();
				m_MeshMap[assetHandle] = mesh;
				// Next is get the correct material handle and assign it
				// auto materialHandle = GetMaterialHandle(what parameter??)   -\0 - 0/-
			}
			if (resource.m_Type == AssetType::ShaderResource)
			{
				auto& combinedPaths = resource.m_Filepath;
				auto path1 = combinedPaths.substr(0, combinedPaths.find('\n'));
				auto path2 = combinedPaths.substr(combinedPaths.find('\n') + 1);

				ShaderResource r = { ShaderInfo{ path1, ShaderType::VERTEX },
					ShaderInfo{ path2, ShaderType::PIXEL } };
				Shader shader{ r };
				m_ShaderMap[assetHandle] = shader;
			}
			if (resource.m_Type == AssetType::Texture2D)
			{
				auto& filepath = resource.m_Filepath;
				auto glTexture = LoadTextureFromFile(filepath);
				m_TextureMap[assetHandle] = glTexture;
			}
		}
		return assetHandle;
	}

	Shader& GetShader(const AssetHandle& handle)
	{
		return m_ShaderMap[handle];
	}

	Material& GetMaterial(const AssetHandle& handle)
	{
		return m_MaterialMap[handle];
	}
	
	Mesh& GetMesh(const AssetHandle& handle)
	{
		auto& mesh = m_MeshMap[handle];
		return mesh;
	}


};

/*
template<typename T>
inline const T& AssetManager::GetAsset(const AssetHandle& asset_handle)
{
	// TODO: insert return statement here
}*/

template<>
inline const unsigned int& AssetManager::GetAsset(const AssetHandle& asset_handle)
{
	return m_FrameBufferMap[asset_handle];
}

template<>
inline const Mesh& AssetManager::GetAsset(const AssetHandle& asset_handle)
{
	return m_MeshMap[asset_handle];
}

template<>
inline const Shader& AssetManager::GetAsset(const AssetHandle& asset_handle)
{
	return m_ShaderMap[asset_handle];
}

template<>
inline const Material& AssetManager::GetAsset(const AssetHandle& asset_handle)
{
	return m_MaterialMap[asset_handle];
}

template<>
inline const TextureBase<GL_Texture>& AssetManager::GetAsset(const AssetHandle& asset_handle)
{
	return m_TextureMap[asset_handle];
}