#pragma once
#include <map>
#include <string>
#include "UUID.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Script.h"

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

	AssetHandle GetResourceHandle(const AssetResource& resource)
	{
		auto& assetHandle = m_AssetResourceAndHandleMap[resource];
		if ((assetHandle.m_HWORD == 0) && (assetHandle.m_LWORD == 0))
		{
			assetHandle.m_LWORD = m_CurrentAssetHandle;
			++m_CurrentAssetHandle;
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
