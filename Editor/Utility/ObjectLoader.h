#ifndef _OBJECT_LOADER_H_
#define _OBJECT_LOADER_H_
#include "Mesh.h"
#include "Sound.h"
#include "Allocator.h"
#include "StringUtils.h"

class ObjectLoader
{
private:
public:
	std::vector<std::vector<glm::vec3>> m_Positions;
	std::vector<std::vector<glm::vec3>> m_Normals;
	std::vector<std::vector<glm::vec2>> m_TexCoords;
	std::vector<std::vector<unsigned int>> m_VertexIndices;
	std::vector<std::vector<unsigned int>> m_VertexIDs;
	std::vector<std::vector<unsigned int>> m_TextureIndices;
	std::vector<std::vector<unsigned int>> m_NormalIndices;
	std::vector<Material> m_Materials;
	std::vector<std::string> m_MaterialPaths;
	std::vector<std::string> m_MaterialNames;
	std::vector<std::string> m_ObjectNames;
	std::vector<std::string> m_Surfaces;
	virtual int LoadObjectFromFile(const char* file_path);
	virtual int LoadMaterialFromFile(const char* file_path);

	static void ExtractDump(
		const std::string& dump, 
		uint64_t object_index, 
		std::vector<std::vector<unsigned int>>& vertex_indices,
		std::vector<std::vector<unsigned int>>& texture_indices,
		std::vector<std::vector<unsigned int>>& normal_indices
	);

public:
	virtual void LoadObject(const char* file_path);
};

class OBJObjectLoader : public ObjectLoader
{
public:
	//int LoadObjectFromFile(const char* file_path) override;
	static primitives::Mesh LoadObjectFromFile(const char* file_path, bool flag);
	//int LoadMaterialFromFile(const char* file_path) override;
	static int LoadMaterialFromFile(const char* file_path, std::vector<std::string>& material_names, std::vector<Material>& materials);

};

struct DDS_PIXELFORMAT
{
	uint32_t dwSize;
	uint32_t dwFlags;
	uint32_t dwFourCC;
	uint32_t dwRGBBitCount;
	uint32_t dwRBitMask;
	uint32_t dwGBitMask;
	uint32_t dwBBitMask;
	uint32_t dwABitMask;
};

struct DDS_HEADER
{
	uint32_t dwSize;
	uint32_t dwFlags;
	uint32_t dwHeight;
	uint32_t dwWidth;
	uint32_t dwPitchOrLinearSize;
	uint32_t dwDepth;
	uint32_t dwMipMapCount;
	uint32_t dwReserved1[11];
	DDS_PIXELFORMAT ddspf;
	uint32_t dwCaps;
	uint32_t dwCaps2;
	uint32_t dwCaps3;
	uint32_t dwCaps4;
	uint32_t dwReserved2;
};

struct DDS_HEADER_DX10
{
	uint32_t dxgiFormat;
	uint32_t resourceDimension;
	uint32_t miscFlag;
	uint32_t arraySize;
	uint32_t miscFlags2;
};

struct PakHeader
{
public:
	uint32_t magic = 0x4B434150; //"PACK"
	uint32_t version = 1;
	uint32_t offset = 0;
	uint32_t size = 0;
	uint32_t itemCount = 0;
};

struct MaterialGroupHeader
{
public:
	uint32_t magic = 0x5047544D; //"MTGP"
	uint32_t version = 1;
	uint32_t offset = 0;
	uint32_t size = 0;
	uint32_t materialCount = 0;
};

struct MaterialFileHeader
{
public:
	uint32_t magic = 0x4C52544D; //"MTRL"
	uint32_t version = 1;
	uint32_t offset = 0;
	uint32_t size = 0;
};

struct MeshFileHeader
{
public:
	uint32_t magic = 0x4853454D; //"MESH"
	uint32_t version = 1;
	uint32_t offset = 0;
	uint32_t size = 0;
	uint32_t subMeshCount = 0;
	uint32_t vertexCount = 0;
	uint32_t indexCount = 0;
};

struct ShaderFileHeader
{
public:
	uint32_t magic = 0x52444853; // SHDR
	uint32_t version = 1;
	uint32_t offset = 0;
	uint32_t size = 0;
	uint32_t stageCount = 0;
	uint32_t stageMask = 0;
};

class MaterialWriter
{
public:
	static int WriteMaterialToFile(const char* file_path, const MTLMaterial& material);
	static int WriteMaterialToFile(std::ofstream& ofs, const MTLMaterial& material);
};

class MaterialReader
{
public:
	static MTLMaterial ReadMaterialFromFile(const char* file_path);
	static MTLMaterial ReadMaterialFromFile(std::ifstream& ifs);
};

class MeshWriter
{
public:
	static int WriteMeshToFile(const char* file_path, const primitives::Mesh& mesh);
	static int WriteMeshToFile(std::ofstream& ofs, const primitives::Mesh& mesh);
};

class MeshReader
{
public:
	static primitives::Mesh ReadMeshFromFile(const char* file_path);
	static primitives::Mesh ReadMeshFromFile(std::ifstream& ifs);
};

class SoundWriter
{
public:
	static int WriteSoundToFile(std::ofstream& ofs, const char* path_to_source);
};

class SoundReader
{
public:
	static Sound ReadSoundClipFromFile(const char* file_path, BlockAllocator<BYTE>& sound_allocator);
	static Sound ReadSoundClipFromFile(std::ifstream& ifs, BlockAllocator<BYTE>& sound_allocator);

private:
	static WAVHEADER ReadWAVHeaderFromFile(std::ifstream& ifs);
};

class TextureWriter
{
public:
	static int WriteDDSTextureToFile(const char* pak_name, const char* dds_file_path);
	static int WriteDDSTextureToFile(std::ofstream& ofs, const char* dds_file_path);
};

class TextureReader 
{
public:
	static DDS_HEADER ReadDDSHeader(std::ifstream& ifs);
	static void LoadDDSTextureIntoMemory(std::ifstream& ifs, uint32_t size, byte* buffer);
};

class ShaderWriter
{
public:
	static int WriteShaderProgramToFile(const char* pak_name, AssetHandle& asset_handle, uint32_t num_stages, uint32_t* stages);
	static int WriteShaderProgramToFile(std::ofstream& ofs, AssetHandle& asset_handle, uint32_t num_stages, uint32_t* stages);
	static int WriteShaderProgramHeaderToFile(std::ofstream& ofs, const ShaderFileHeader& shader_file_header);
};

class ShaderReader
{
public:
	static std::string LoadShaderSourceIntoMemory(std::ifstream& ifs, uint32_t size);
	static ShaderFileHeader ReadShaderProgramHeader(std::ifstream& ifs);
};
#endif