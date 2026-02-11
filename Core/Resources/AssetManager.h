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
	ComputeShaderResource,
	GraphicsShaderResource,
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
	std::map<AssetHandle, unsigned int> m_FrameBufferMap;
	std::map<AssetHandle, Shader> m_ShaderMap;
	std::map<AssetHandle, std::vector<Material>> m_MaterialMap;
	std::map<AssetHandle, TextureBase<GL_Texture>> m_TextureMap;
	std::map<AssetHandle, primitives::Mesh> m_MeshMap;
	std::map<AssetHandle, scripting::ControlScript> m_ScriptMap;
public:
	std::map<AssetResource, AssetHandle> m_AssetResourceAndHandleMap;
	AssetManager() :
		m_AssetResourceAndHandleMap{}
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
			if (resource.m_Type == AssetType::ComputeShaderResource)
			{
				assetHandle = CreateAssetHandleFromPath(resource.m_Filepath.c_str());
				ShaderResource r{ ShaderInfo{resource.m_Filepath, ShaderType::COMPUTE} };
				Shader shader{ r };
				m_ShaderMap[assetHandle] = shader;
				m_AssetResourceAndHandleMap[resource] = assetHandle;
			}
			if (resource.m_Type == AssetType::GraphicsShaderResource)
			{
				assetHandle = CreateAssetHandleFromPath(resource.m_Filepath.c_str());
				auto& combinedPaths = resource.m_Filepath;
				auto path1 = combinedPaths.substr(0, combinedPaths.find('\n'));
				auto path2 = combinedPaths.substr(combinedPaths.find('\n') + 1);

				ShaderResource r = { ShaderInfo{ path1, ShaderType::VERTEX },
					ShaderInfo{ path2, ShaderType::PIXEL } };
				Shader shader{ r };
				m_ShaderMap[assetHandle] = shader;
				m_AssetResourceAndHandleMap[resource] = assetHandle;
			}
			if (resource.m_Type == AssetType::MeshResource)
			{
				assetHandle = CreateAssetHandleFromPath(resource.m_Filepath.c_str());
				primitives::Mesh mesh{ resource.m_Filepath.c_str() };
				auto& materialGroup = mesh.m_Materials;
				m_MaterialMap[assetHandle] = materialGroup;
				m_MeshMap[assetHandle] = mesh;
				m_AssetResourceAndHandleMap[resource] = assetHandle;
			}
			if (resource.m_Type == AssetType::ScriptResource)
			{
				assetHandle = CreateAssetHandleFromPath(resource.m_Filepath.c_str());
				//m_ScriptMap[assetHandle] = scripting::ControlScript{ resource.m_Filepath.c_str() };
				m_AssetResourceAndHandleMap[resource] = assetHandle;
			}
			if (resource.m_Type == AssetType::Texture2D)
			{
				assetHandle = CreateAssetHandleFromPath(resource.m_Filepath.c_str());
				auto& filepath = resource.m_Filepath;
				auto glTexture = LoadTextureFromFile(filepath);
				m_TextureMap[assetHandle] = glTexture;
				m_AssetResourceAndHandleMap[resource] = assetHandle;
			}
		}
		return assetHandle;
	}

	Shader& GetShader(const AssetHandle& handle)
	{
		return m_ShaderMap[handle];
	}

	std::vector<Material>& GetMaterial(const AssetHandle& handle)
	{
		return m_MaterialMap[handle];
	}
	
	primitives::Mesh& GetMesh(const AssetHandle& handle)
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
inline const primitives::Mesh& AssetManager::GetAsset(const AssetHandle& asset_handle)
{
	return m_MeshMap[asset_handle];
}

template<>
inline const Shader& AssetManager::GetAsset(const AssetHandle& asset_handle)
{
	return m_ShaderMap[asset_handle];
}

template<>
inline const std::vector<Material>& AssetManager::GetAsset(const AssetHandle& asset_handle)
{
	return m_MaterialMap[asset_handle];
}

template<>
inline const TextureBase<GL_Texture>& AssetManager::GetAsset(const AssetHandle& asset_handle)
{
	return m_TextureMap[asset_handle];
}