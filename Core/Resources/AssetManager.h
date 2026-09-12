#pragma once
#include <map>
#include "Shader.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "ComponentUtils.h" // asset, mesh , physicState, script
#include "ObjectLoader.h"
#include "Sound.h"
#include "Allocator.h"

class AssetManager
{
private:
	friend class BaseApplication;
	std::map<AssetHandle, unsigned int> m_FrameBufferMap;
	std::map<AssetHandle, Shader> m_ShaderMap;
	std::map<AssetHandle, std::vector<Material>> m_MaterialGroupMap;
	std::map<AssetHandle, std::vector<MTLMaterial>> m_NewMaterialGroupMap;
	std::map<AssetHandle, TextureBase<GL_Texture>> m_TextureMap;
	std::map<AssetHandle, primitives::Mesh> m_MeshMap;
	std::map<AssetHandle, std::string> m_ScriptMap;
	std::map<AssetHandle, Sound> m_SoundMap;
private:
	BlockAllocator<BYTE> soundBlockAllocator;
public:
	std::map<AssetResource, AssetHandle> m_AssetResourceAndHandleMap;
	std::map<AssetHandle, AssetResource> m_AssetHandleAndResourceMap;
	std::map<AssetHandle, ShaderResource> m_ShaderResourceMap;
	std::vector<primitives::ParticleSystemConfig> m_ParticleSystems;
	AssetManager() :
		m_AssetResourceAndHandleMap{},
		m_AssetHandleAndResourceMap{},
		m_ShaderResourceMap{},
		m_FrameBufferMap{},
		m_ShaderMap{},
		m_MaterialGroupMap{},
		m_NewMaterialGroupMap{},
		m_TextureMap{},
		m_MeshMap{},
		m_ScriptMap{},
		m_SoundMap{}
	{

	}
	bool Serialize(const std::string& filename);
	bool SerializeMaterialPack(const std::string& filename);
	bool SerializeMeshPack(const std::string& filename);
	bool SerializeTexturePack(const std::string& filename);
	bool SerializeShaderPack(const std::string& filename);
	bool SerializeSoundPack(const std::string& filename);
	bool SerializeScriptPack(const std::string& filename);
	bool SerializeParticleSystemPack(const std::string& filename);
	bool Deserialize(const std::string& filepath);
	bool DeserializeMaterialPack(const std::string filepath);
	bool DeserializeMeshPack(const std::string& filepath);
	bool DeserializeTexturePack(const std::string& filepath);
	bool DeserializeShaderPack(const std::string& filepath);
	bool DeserializeScriptPack(const std::string& filepath);
	bool DeserializeParticlePack(const std::string& filepath);
	
	void CreateOpenGLTexture(TextureBase<GL_Texture>& tex_base);
	TextureBase<GL_Texture> CreateOpenGLCubeMap(const std::vector<std::string>& image_file_paths);
	AssetHandle CreateOpenGLFrameBuffer(TextureBase<GL_Texture>& tex_base);

	template<typename T>
	const T& GetAsset(const AssetHandle& asset_handle);

	TextureBase<GL_Texture> LoadTextureFromFile(const std::string& image_path, bool flip_vertically = false);
	TextureBase<GL_Texture> LoadDDSTextureFromFile(const std::string& file_path);

	AssetHandle GetResourceHandle(const AssetResource& resource)
	{
		auto& assetHandle = m_AssetResourceAndHandleMap[resource];
		if ((assetHandle.m_HWORD == 0) && (assetHandle.m_LWORD == 0))
		{
			if (resource.m_Type == AssetType::ComputeShaderResource)
			{
				assetHandle = CreateAssetHandleFromPath(resource.m_Filepath.c_str());
				std::vector<ShaderInfo> infos(1);
				infos[0].filePath = resource.m_Filepath;
				infos[0].shaderType = ShaderType::COMPUTE;
				ShaderResource r{ infos};
				Shader shader{ r };
				m_ShaderResourceMap[assetHandle] = r;
				m_ShaderMap[assetHandle] = shader;
				m_AssetResourceAndHandleMap[resource] = assetHandle;
				m_AssetHandleAndResourceMap[assetHandle] = resource;
			}
			if (resource.m_Type == AssetType::GraphicsShaderResource)
			{
				assetHandle = CreateAssetHandleFromPath(resource.m_Filepath.c_str());
				auto& combinedPaths = resource.m_Filepath;
				auto path1 = combinedPaths.substr(0, combinedPaths.find('\n'));
				auto path2 = combinedPaths.substr(combinedPaths.find('\n') + 1);
				std::vector<ShaderInfo> infos(2);
				infos[0].filePath = path1;
				infos[0].shaderType = ShaderType::VERTEX;
				infos[1].filePath = path2;
				infos[1].shaderType = ShaderType::PIXEL;
				ShaderResource r { infos};
				Shader shader{ r };
				m_ShaderResourceMap[assetHandle] = r;
				m_ShaderMap[assetHandle] = shader;
				m_AssetResourceAndHandleMap[resource] = assetHandle;
				m_AssetHandleAndResourceMap[assetHandle] = resource;

			}
			if (resource.m_Type == AssetType::MeshResource)
			{
				auto& path = resource.m_Filepath;
				auto extension = path.substr(path.find('.'));
				if (extension == ".obj")
				{
					// load .obj 
					assetHandle = CreateAssetHandleFromPath(resource.m_Filepath.c_str());
					primitives::Mesh mesh = OBJObjectLoader::LoadObjectFromFile(resource.m_Filepath.c_str(), true);
					auto& materialGroup = mesh.m_Materials;
					mesh.m_MaterialGroupHandle = assetHandle;
					m_MaterialGroupMap[assetHandle] = materialGroup;
					m_MeshMap[assetHandle] = mesh;
					m_AssetResourceAndHandleMap[resource] = assetHandle;
					m_AssetHandleAndResourceMap[assetHandle] = resource;
				}
				if (extension == ".mesh")
				{
					// or load .mesh file
					auto mesh = MeshReader::ReadMeshFromFile(resource.m_Filepath.c_str());
					auto assetHandle = mesh.m_MaterialGroupHandle;
					mesh.m_Materials = m_MaterialGroupMap[assetHandle];
					m_MeshMap[assetHandle] = mesh;
					m_AssetResourceAndHandleMap[resource] = assetHandle;
					m_AssetHandleAndResourceMap[assetHandle] = resource;
				}
			}
			if (resource.m_Type == AssetType::ScriptResource)
			{
				assetHandle = CreateAssetHandleFromPath(resource.m_Filepath.c_str());
				auto scriptData = ReadLuaScriptFromDisk(resource.m_Filepath);
				m_ScriptMap[assetHandle] = scriptData;
				m_AssetResourceAndHandleMap[resource] = assetHandle;
				m_AssetHandleAndResourceMap[assetHandle] = resource;

			}
			if (resource.m_Type == AssetType::Texture2D)
			{
				assetHandle = CreateAssetHandleFromPath(resource.m_Filepath.c_str());
				auto& filepath = resource.m_Filepath;
				auto glTexture = LoadTextureFromFile(filepath);
				m_TextureMap[assetHandle] = glTexture;
				m_AssetResourceAndHandleMap[resource] = assetHandle;
				m_AssetHandleAndResourceMap[assetHandle] = resource;

			}
			if (resource.m_Type == AssetType::CubeMap)
			{
				assetHandle = CreateAssetHandleFromPath(resource.m_Filepath.c_str());
				auto imageFilePaths = stringUtils::getWords(resource.m_Filepath, "+");
				auto map = CreateOpenGLCubeMap(imageFilePaths);
				m_TextureMap[assetHandle] = map;
				m_AssetResourceAndHandleMap[resource] = assetHandle;
				m_AssetHandleAndResourceMap[assetHandle] = resource;
			}
			if (resource.m_Type == AssetType::SoundClipResource)
			{
				assetHandle = CreateAssetHandleFromPath(resource.m_Filepath.c_str());
				auto sound = SoundReader::ReadSoundClipFromFile(resource.m_Filepath.c_str(), soundBlockAllocator);
				m_SoundMap[assetHandle] = sound;
				m_AssetResourceAndHandleMap[resource] = assetHandle;
				m_AssetHandleAndResourceMap[assetHandle] = resource;
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
		return m_MaterialGroupMap[handle];
	}

	std::vector<MTLMaterial>& GetNewMaterial(const AssetHandle& handle)
	{
		return m_NewMaterialGroupMap[handle];
	}
	
	primitives::Mesh& GetMesh(const AssetHandle& handle)
	{
		return m_MeshMap[handle];
	}

	Sound& GetSound(const AssetHandle& handle)
	{
		return m_SoundMap[handle];
	}

private:
		byte* LoadDDSIntoMemory(std::ifstream& ifs, size_t& size);

};

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
	return m_MaterialGroupMap[asset_handle];
}

template<>
inline const TextureBase<GL_Texture>& AssetManager::GetAsset(const AssetHandle& asset_handle)
{
	return m_TextureMap[asset_handle];
}
template<>
inline const std::string& AssetManager::GetAsset(const AssetHandle& asset_handle)
{
	return m_ScriptMap[asset_handle];
}