#include "ObjectLoader.h"

int ObjectLoader::LoadObjectFromFile(const char* file_path)
{
    return 0;
}

int ObjectLoader::LoadMaterialFromFile(const char* file_path)
{
    return 0;
}

void ObjectLoader::LoadObject(const char* file_path)
{ 
    int ret = LoadObjectFromFile(file_path);
}

void ObjectLoader::ExtractDump(
	const std::string& dump_line, 
	uint64_t object_index, 
	std::vector<std::vector<unsigned int>>& vertex_indices, 
	std::vector<std::vector<unsigned int>>& texture_indices, 
	std::vector<std::vector<unsigned int>>& normal_indices 
) 
{
	auto string_words = getWords(dump_line.substr(2), " ");
	for (const auto& word : string_words) {
		auto parts = getWords(word, "/");
		if (parts.size() >= 1 && !parts[0].empty())
			//m_VertexIndices[object_index].push_back(std::stoul(parts[0]) - 1);
			vertex_indices[object_index].push_back(std::stoul(parts[0]) - 1);
		if (parts.size() >= 2 && !parts[1].empty())
			//m_TextureIndices[object_index].push_back(std::stoul(parts[1]) - 1);
			texture_indices[object_index].push_back(std::stoul(parts[1]) - 1);
		if (parts.size() >= 3 && !parts[2].empty())
			//m_NormalIndices[object_index].push_back(std::stoul(parts[2]) - 1);
			normal_indices[object_index].push_back(std::stoul(parts[2]) - 1);
	}
}

primitives::Mesh OBJObjectLoader::LoadObjectFromFile(const char* file_path, bool flag) {

	std::vector<std::vector<glm::vec3>> positions;
	std::vector<std::vector<glm::vec3>> normals;
	std::vector<std::vector<glm::vec2>> texCoords;
	std::vector<std::vector<unsigned int>> vertexIndices;
	std::vector<std::vector<unsigned int>> vertexIDs;
	std::vector<std::vector<unsigned int>> textureIndices;
	std::vector<std::vector<unsigned int>> normalIndices;
	std::vector<Material> materials;
	std::vector<std::string> materialPaths;
	std::vector<std::string> materialNames;
	std::vector<std::string> objectNames;
	std::vector<std::string> surfaces;

	primitives::Mesh mesh;

	std::ifstream file(file_path, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		std::cerr << "Failed to open .obj file: " << file_path << "\n";
		return mesh;
	}

	std::streamsize size = file.tellg();
	file.seekg(0);
	std::string buffer(size, '\0');
	if (!file.read(&buffer[0], size)) {
		std::cerr << "Failed to read .obj file into memory.\n";
		return mesh;
	}

	std::istringstream in(buffer);
	std::string line;
	int64_t object_index = -1;
	size_t material_index = 0;
	std::unordered_map<std::string, size_t> materialLookup;
	std::string objDir = GetDirectoryFromPath(file_path);

	AssetHandle materialGroupHandle = CreateAssetHandleFromPath(file_path);
	while (std::getline(in, line)) {
		auto words = getWords(line, " ");
		if (words.empty()) continue;
		const std::string& type = words[0];
		char* endptr;

		if (type == "mtllib" && words.size() >= 2) {
			std::string fullPath = objDir + words[1];
			materialPaths.push_back(fullPath);
			//m_MaterialPaths.push_back(fullPath);
			LoadMaterialFromFile(fullPath.c_str(), materialNames, materials);

			//for (size_t i = 0; i < m_MaterialNames.size(); ++i) {
			for (size_t i = 0; i < materialNames.size(); ++i) {
				//materialLookup[m_MaterialNames[i]] = i;
				materialLookup[materialNames[i]] = i;
			}
		}
		else if (type == "usemtl" && words.size() >= 2) {
			auto it = materialLookup.find(words[1]);
			if (it != materialLookup.end()) {
				material_index = it->second;
			}
		}
		else if (type == "o" && words.size() >= 2) {
			//m_ObjectNames.push_back(words[1]);
			//m_Positions.emplace_back();
			//m_TexCoords.emplace_back();
			//m_VertexIDs.emplace_back();
			//m_Normals.emplace_back();
			//m_VertexIndices.emplace_back();
			//m_TextureIndices.emplace_back();
			//m_NormalIndices.emplace_back();
			// 
			objectNames.push_back(words[1]);
			positions.emplace_back();
			texCoords.emplace_back();
			vertexIDs.emplace_back();
			normals.emplace_back();
			vertexIndices.emplace_back();
			textureIndices.emplace_back();
			normalIndices.emplace_back();
			++object_index;
		}
		else if (type == "s" && words.size() >= 2) {
			//m_Surfaces.push_back(words[1]);
			surfaces.push_back(words[1]);
		}
		else if (type == "v" && words.size() >= 4) {
			float x = strtof(words[1].c_str(), &endptr);
			float y = strtof(words[2].c_str(), &endptr);
			float z = strtof(words[3].c_str(), &endptr);
			//m_Positions[object_index].emplace_back(x, y, z);
			positions[object_index].emplace_back(x, y, z);
		}
		else if (type == "vt" && words.size() >= 3) {
			float u = strtof(words[1].c_str(), &endptr);
			float v = strtof(words[2].c_str(), &endptr);
			//m_TexCoords[object_index].emplace_back(u, v);
			texCoords[object_index].emplace_back(u, v);
		}
		else if (type == "vn" && words.size() >= 4) {
			float x = strtof(words[1].c_str(), &endptr);
			float y = strtof(words[2].c_str(), &endptr);
			float z = strtof(words[3].c_str(), &endptr);
			//m_Normals[object_index].emplace_back(x, y, z);
			normals[object_index].emplace_back(x, y, z);
		}
		else if (type == "f") {
			//ExtractDump(line, object_index);
			ExtractDump(line, object_index, vertexIndices, textureIndices, normalIndices);
			//m_VertexIDs[object_index].insert(m_VertexIDs[object_index].end(), 3, material_index);
			vertexIDs[object_index].insert(vertexIDs[object_index].end(), 3, material_index);
		}
	}

	size_t object_count = objectNames.size();

	for (size_t i = 0; i < object_count; ++i) 
	{
		primitives::Mesh sub;
		const auto& Positions = positions[i];
		const auto& TexCoords = texCoords[i];
		const auto& Normals = normals[i];
		const auto& vIndices = vertexIndices[i];
		const auto& tIndices = textureIndices[i];
		const auto& nIndices = normalIndices[i];
		const auto& VertexIDs = vertexIDs[i];

		for (size_t j = 0; j < vIndices.size(); ++j) {
			primitives::Vertex vert;
			vert.pos = Positions[vIndices[j]];

			vert.tex = (j < tIndices.size() && !TexCoords.empty())
				? TexCoords[tIndices[j]]
				: glm::vec2(0.0f);

			vert.norm = (j < nIndices.size() && !Normals.empty())
				? Normals[nIndices[j]]
				: glm::vec3(0.0f);

			vert.ID = (j < VertexIDs.size())
				? static_cast<float>(VertexIDs[j])
				: 0.0f;

			sub.m_V.push_back(vert);
			sub.m_VertexIndices.push_back(static_cast<unsigned int>(j));
		}
		sub.m_MaterialGroupHandle = materialGroupHandle;
		sub.m_Materials = materials;
		mesh.m_SubMeshes.push_back(std::move(sub));
	}

	if (mesh.m_SubMeshes.size() == 1) {
		const primitives::Mesh& submesh = mesh.m_SubMeshes.front();

		// Clear existing mesh data to ensure we're overwriting the data
		mesh.m_VertexIndices.clear();
		mesh.m_V.clear();

		// Copy data from the submesh into the main mesh
		mesh.m_VertexIndices = submesh.m_VertexIndices;
		mesh.m_V = submesh.m_V;

		// Optionally copy materials if needed (if not already set)
		if (mesh.m_Materials.empty()) {
			mesh.m_Materials = submesh.m_Materials;
		}
		mesh.m_MaterialGroupHandle = materialGroupHandle;
		// Clear the submeshes vector
		mesh.m_SubMeshes.clear();
	}
	return mesh;
}

int OBJObjectLoader::LoadMaterialFromFile(const char* file_path, std::vector<std::string>& material_names, std::vector<Material>& materials) {
	std::ifstream file(file_path, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		std::cerr << "Failed to open .mtl file: " << file_path << "\n";
		return -1;
	}

	std::streamsize size = file.tellg();
	file.seekg(0);
	std::string buffer(size, '\0');
	if (!file.read(&buffer[0], size)) {
		std::cerr << "Failed to read .mtl file into memory.\n";
		return -1;
	}

	std::istringstream in(buffer);
	std::string line;
	uint64_t numMaterials = 0;
	char* endptr = nullptr;
	std::vector<LayoutInfo> materialInfos;
	Material tempMaterial{};
	bool firstMaterial = true;
	
	while (std::getline(in, line)) {
		auto words = getWords(line, " ");
		if (words.empty()) continue;

		const std::string& type = words[0];
		LayoutInfo materialInfo;
		bool materialComponent = false;

		if (type == "newmtl" && words.size() >= 2) {
			if (firstMaterial)
			{
				//m_MaterialNames.push_back(words[1]);
				material_names.push_back(words[1]);
				firstMaterial = false;
				continue;
			}
			//m_Materials.push_back(tempMaterial);
			//m_MaterialNames.push_back(words[1]);
			materials.push_back(tempMaterial);
			material_names.push_back(words[1]);
		}
		else if (type == "Ka" && words.size() >= 4) {
			auto Ka = glm::vec3(
				strtof(words[1].c_str(), &endptr),
				strtof(words[2].c_str(), &endptr),
				strtof(words[3].c_str(), &endptr)
			);
			//tempMaterial.m_Uniforms3f.insert({ "Ka", Ka });
			tempMaterial.m_Uniforms3f["Ka"] = Ka;
			materialInfo.Name = "Ka";
			materialInfo.Type = ShaderDataType::Float3;
			materialComponent = true;
		}
		else if (type == "Kd" && words.size() >= 4) {
			auto Kd = glm::vec3(
				strtof(words[1].c_str(), &endptr),
				strtof(words[2].c_str(), &endptr),
				strtof(words[3].c_str(), &endptr)
			);
			//tempMaterial.m_Uniforms3f.insert({ "Kd", Kd });
			tempMaterial.m_Uniforms3f["Kd"] = Kd;
			materialInfo.Name = "Kd";
			materialInfo.Type = ShaderDataType::Float3;
			materialComponent = true;
		}
		else if (type == "Ks" && words.size() >= 4) {
			auto Ks = glm::vec3(
				strtof(words[1].c_str(), &endptr),
				strtof(words[2].c_str(), &endptr),
				strtof(words[3].c_str(), &endptr)
			);
			//tempMaterial.m_Uniforms3f.insert({ "Ks", Ks });
			tempMaterial.m_Uniforms3f["Ks"] = Ks;
			materialInfo.Name = "Ks";
			materialInfo.Type = ShaderDataType::Float3;
			materialComponent = true;
		}
		else if (type == "Ns" && words.size() >= 2) {
			auto Ns = strtof(words[1].c_str(), &endptr);
			//tempMaterial.m_Uniforms1f.insert({ "Ns", glm::vec1(Ns) });
			tempMaterial.m_Uniforms1f["Ns"] = glm::vec1(Ns);
			materialInfo.Name = "Ns";
			materialInfo.Type = ShaderDataType::Float;
			materialComponent = true;
		}
		else if (type == "Ni" && words.size() >= 2) {
			auto Ni = strtof(words[1].c_str(), &endptr);
			//tempMaterial.m_Uniforms1f.insert({ "Ni", glm::vec1(Ni) });
			tempMaterial.m_Uniforms1f["Ni"] = glm::vec1(Ni);
			materialInfo.Name = "Ni";
			materialInfo.Type = ShaderDataType::Float;
			materialComponent = true;
		}
		else if (type == "d" && words.size() >= 2) {
			auto d = strtof(words[1].c_str(), &endptr);
			//tempMaterial.m_Uniforms1f.insert({ "d", glm::vec1(d) });
			tempMaterial.m_Uniforms1f["d"] = glm::vec1(d);
			materialInfo.Name = "d";
			materialInfo.Type = ShaderDataType::Float;
			materialComponent = true;
		}
		else if (type == "illum" && words.size() >= 2) {
			auto illum = strtof(words[1].c_str(), &endptr);
			//tempMaterial.m_Uniforms1f.insert({ "illum", glm::vec1(illum) });
			tempMaterial.m_Uniforms1f["illum"] = glm::vec1(illum);
			materialInfo.Name = "illum";
			materialInfo.Type = ShaderDataType::Float;
			materialComponent = true;
		}
		if (materialComponent)
		{
			materialInfos.push_back(materialInfo);
		}
	}
	//m_Materials.push_back(tempMaterial);
	materials.push_back(tempMaterial);
	return 0;
}

int MeshWriter::WriteMeshToFile(const char* file_path, const primitives::Mesh& mesh)
{
	std::ofstream ofs(file_path, std::ios::binary);
	if (!ofs.is_open())
	{
		throw std::runtime_error("Failed to open file");
	}
	WriteMeshToFile(ofs, mesh);
	ofs.close();
	return 0;
}

int MeshWriter::WriteMeshToFile(std::ofstream& ofs, const primitives::Mesh& mesh)
{
	MeshFileHeader header;
	header.subMeshCount = mesh.m_SubMeshes.size();
	header.vertexCount = mesh.m_V.size();
	header.indexCount = mesh.m_VertexIndices.size();

	if (!ofs.is_open())
	{
		throw std::runtime_error("Failed to open file");
	}
	auto startOffset = ofs.tellp();
	ofs.write(
		reinterpret_cast<const char*>(&header),
		sizeof(header)
	);

	for (auto index = 0; index < mesh.m_SubMeshes.size(); index++)
	{
		auto& subMesh = mesh.m_SubMeshes[index];
		MeshFileHeader subMeshHeader;
		subMeshHeader.subMeshCount = subMesh.m_SubMeshes.size();
		subMeshHeader.vertexCount = subMesh.m_V.size();
		subMeshHeader.indexCount = subMesh.m_VertexIndices.size();

		auto subMeshStartOffset = ofs.tellp();
		ofs.write(
			reinterpret_cast<const char*>(&subMeshHeader),
			sizeof(subMeshHeader)
		);
		auto subMeshDataOffset = ofs.tellp();
		ofs.write(
			reinterpret_cast<const char*>(&subMesh.m_MaterialGroupHandle),
			sizeof(AssetHandle)
		);
		ofs.write(
			reinterpret_cast<const char*>(subMesh.m_V.data()),
			sizeof(primitives::Vertex) * subMesh.m_V.size()
		);
		ofs.write(
			reinterpret_cast<const char*>(subMesh.m_VertexIndices.data()),
			sizeof(uint32_t) * subMesh.m_VertexIndices.size()
		);
		auto subMeshEndOffset = ofs.tellp();
		uint32_t size = subMeshEndOffset - subMeshDataOffset;
		uint32_t offset = static_cast<uint32_t>(subMeshDataOffset);
		subMeshHeader.offset = offset;
		subMeshHeader.size = size;

		ofs.seekp(subMeshStartOffset);
		ofs.write(
			reinterpret_cast<const char*>(&subMeshHeader),
			sizeof(subMeshHeader)
		);
		ofs.seekp(subMeshEndOffset);
	}

	auto dataOffset = ofs.tellp();
	ofs.write(
		reinterpret_cast<const char*>(&mesh.m_MaterialGroupHandle),
		sizeof(AssetHandle)
	);
	ofs.write(
		reinterpret_cast<const char*>(mesh.m_V.data()),
		sizeof(primitives::Vertex) * mesh.m_V.size()
	);

	ofs.write(
		reinterpret_cast<const char*>(mesh.m_VertexIndices.data()),
		sizeof(uint32_t) * mesh.m_VertexIndices.size()
	);

	auto endOffset = ofs.tellp();
	uint32_t size = endOffset - dataOffset;
	uint32_t offset = static_cast<uint32_t>(dataOffset);
	header.offset = offset;
	header.size = size;

	ofs.seekp(startOffset);
	ofs.write(
		reinterpret_cast<const char*>(&header),
		sizeof(header)
	);
	ofs.seekp(endOffset);
	return 0;
}

primitives::Mesh MeshReader::ReadMeshFromFile(const char* file_path)
{
	std::ifstream ifs(file_path, std::ios::binary);
	if (!ifs.is_open())
	{
		throw std::runtime_error("Failed to open file");
	}
	auto mesh = ReadMeshFromFile(ifs);
	ifs.close();
	return mesh;
}

primitives::Mesh MeshReader::ReadMeshFromFile(std::ifstream& ifs)
{
	MeshFileHeader header;
	if (!ifs.is_open())
	{
		throw std::runtime_error("Failed to open file");
	}
	ifs.read(
		reinterpret_cast<char*>(&header),
		sizeof(header)
	);

	assert(header.magic == 0x4853454D);
	primitives::Mesh mesh;

	mesh.m_V.resize(header.vertexCount);
	mesh.m_VertexIndices.resize(header.indexCount);
	for (auto index = 0; index < header.subMeshCount; index++)
	{
		primitives::Mesh subMesh;
		MeshFileHeader subMeshHeader;
		ifs.read(
			reinterpret_cast<char*>(&subMeshHeader),
			sizeof(subMeshHeader)
		);
		std::streampos dataOffset = subMeshHeader.offset;
		size_t dataSize = subMeshHeader.size;
		ifs.seekg(dataOffset);
		ifs.read(
			reinterpret_cast<char*>(
				&subMesh.m_MaterialGroupHandle),
			sizeof(AssetHandle)
		);
		ifs.read(
			reinterpret_cast<char*>(
				subMesh.m_V.data()),
			sizeof(primitives::Vertex) * subMeshHeader.vertexCount
		);
		ifs.read(
			reinterpret_cast<char*>(
				subMesh.m_VertexIndices.data()),
			sizeof(uint32_t) * subMeshHeader.indexCount
		);

		mesh.m_SubMeshes.push_back(std::move(subMesh));
	}
	std::streampos dataOffset = header.offset;
	size_t dataSize = header.size;
	ifs.seekg(dataOffset);
	ifs.read(
		reinterpret_cast<char*>(
			&mesh.m_MaterialGroupHandle),
		sizeof(AssetHandle)
	);
	ifs.read(
		reinterpret_cast<char*>(
			mesh.m_V.data()),
		sizeof(primitives::Vertex) * header.vertexCount
	);
	ifs.read(
		reinterpret_cast<char*>(
			mesh.m_VertexIndices.data()),
		sizeof(uint32_t) * header.indexCount
	);
	return mesh;
}

int MaterialWriter::WriteMaterialToFile(const char* file_path, const MTLMaterial& material)
{
	std::ofstream ofs(file_path, std::ios::binary);
	if (!ofs.is_open())
	{
		throw std::runtime_error("Failed to open file for writing !!!");
	}
	WriteMaterialToFile(ofs, material);
	ofs.close();
	return 0;
}

int MaterialWriter::WriteMaterialToFile(std::ofstream& ofs, const MTLMaterial& material)
{
	MaterialFileHeader header;
	if (!ofs.is_open())
	{
		throw std::runtime_error("Failed to open file !!!");
	}
	auto startOffset = ofs.tellp();
	ofs.write(
		reinterpret_cast<const char*>(&header),
		sizeof(MaterialFileHeader)
	);
	auto dataOffset = ofs.tellp();
	ofs.write(
		reinterpret_cast<const char*>(&material.KaNs),
		sizeof(glm::vec4)
	);
	ofs.write(
		reinterpret_cast<const char*>(&material.KdNi),
		sizeof(glm::vec4)
	);
	ofs.write(
		reinterpret_cast<const char*>(&material.KsD),
		sizeof(glm::vec4)
	);
	ofs.write(
		reinterpret_cast<const char*>(&material.KeIllum),
		sizeof(glm::vec4)
	);
	auto endOffset = ofs.tellp();
	uint32_t size = endOffset - dataOffset;
	uint32_t offset = static_cast<uint32_t>(dataOffset);
	header.offset = offset;
	header.size = size;
	ofs.seekp(startOffset);
	ofs.write(
		reinterpret_cast<const char*>(&header),
		sizeof(MaterialFileHeader)
	);
	ofs.seekp(endOffset);
	return 0;
}

MTLMaterial MaterialReader::ReadMaterialFromFile(const char* file_path)
{
	std::ifstream ifs(file_path, std::ios::binary);
	if (!ifs.is_open())
	{
		throw std::runtime_error("Failed to open file for reading !!!");
	}
	auto material = ReadMaterialFromFile(ifs);
	return material;
}

MTLMaterial MaterialReader::ReadMaterialFromFile(std::ifstream& ifs)
{
	MaterialFileHeader materialHeader;
	if (!ifs.is_open())
	{
		throw std::runtime_error("Failed to open file for reading !!!");
	}
	ifs.read(
		reinterpret_cast<char*>(&materialHeader),
		sizeof(MaterialFileHeader)
	);
	assert(materialHeader.magic = 0x4C52544D);
	auto tempVec = glm::vec4(0);
	MTLMaterial material{ tempVec, tempVec, tempVec, tempVec };
	ifs.read(
		reinterpret_cast<char*>(&material.KaNs),
		sizeof(glm::vec4)
	);
	ifs.read(
		reinterpret_cast<char*>(&material.KdNi),
		sizeof(glm::vec4)
	);
	ifs.read(
		reinterpret_cast<char*>(&material.KsD),
		sizeof(glm::vec4)
	);
	ifs.read(
		reinterpret_cast<char*>(&material.KeIllum),
		sizeof(glm::vec4)
	);
	return material;
}

Sound SoundReader::ReadSoundClipFromFile(const char* file_path, BlockAllocator<BYTE>& sound_allocator)
{
	std::ifstream ifs(file_path, std::ios::binary);
	if (!ifs.is_open())
	{
		throw std::runtime_error("Failed to open file for reading !!!");
	}
	auto sound = ReadSoundClipFromFile(ifs, sound_allocator);
	ifs.close();
	return sound;
}

Sound SoundReader::ReadSoundClipFromFile(std::ifstream& ifs, BlockAllocator<BYTE>& sound_allocator)
{
	if (!ifs.is_open())
	{
		throw std::runtime_error("Failed to open file for reading !!!");
	}
	auto wavHeader = ReadWAVHeaderFromFile(ifs);
	Sound sound{};
	sound.bitsPerSample = wavHeader.bitsPerSample;
	sound.blockAlign = wavHeader.blockAlign;
	sound.byteRate = wavHeader.byteRate;
	sound.currentFrame = 0;
	sound.frameCount = wavHeader.dataSize / wavHeader.blockAlign;
	sound.numChannels = wavHeader.numChannels;
	sound.PCMData = sound_allocator.allocate(wavHeader.dataSize);
	ifs.read(
		reinterpret_cast<char*>(sound.PCMData),
		wavHeader.dataSize
	);
	return sound;
}

WAVHEADER SoundReader::ReadWAVHeaderFromFile(std::ifstream& ifs)
{
	if (!ifs.is_open())
	{
		throw std::runtime_error("Failed to open file for reading !!!");
	}
	WAVHEADER wavHeader{};
	ifs.read(
		reinterpret_cast<char*>(&wavHeader),
		sizeof(WAVHEADER)
	);
	if (strncmp(wavHeader.RIFF, "RIFF", 4) != 0 ||
		strncmp(wavHeader.WAVE, "WAVE", 4) != 0 ||
		wavHeader.audioFormat != 1)
	{
		throw std::runtime_error("Reading Invalid WAVHEADER !!!");
	}
	return wavHeader;
}

int TextureWriter::WriteDDSTextureToFile(const char* pak_name, const char* dds_file_path)
{
	std::ofstream ofs(pak_name, std::ios::binary);
	if (!ofs.is_open())
	{
		throw std::runtime_error("Failed to open file for writing !!!");
	}
	WriteDDSTextureToFile(ofs, dds_file_path);
	ofs.close();
	return 0;
}

int TextureWriter::WriteDDSTextureToFile(std::ofstream& ofs, const char* dds_file_path)
{
	if (!ofs.is_open())
	{
		throw std::runtime_error("Failed to open file for writing !!!");
	}
	std::ifstream ifs(dds_file_path, std::ios::binary |  std::ios::ate);
	if (!ifs.is_open())
	{
		throw std::runtime_error("Failed to open file for reading !!!");
	}
	std::streamsize size = ifs.tellg();
	ifs.seekg(0);
	std::string ddsFileBuffer(size, '\0');
	if (!ifs.read(
		&ddsFileBuffer[0],
		size)) 
	{
		throw std::runtime_error("Failed to load DDS file into memory !!!");
	}

	PakHeader header{};
	header.itemCount = 1;
	auto startOffset = ofs.tellp();
	ofs.write(
		reinterpret_cast<const char*>(&header),
		sizeof(PakHeader)
	);
	auto dataStartOffset = ofs.tellp();
	ofs.write(
		reinterpret_cast<const char*>(ddsFileBuffer.data()),
		size
	);
	auto dataEndOffset = ofs.tellp();
	header.offset = static_cast<uint32_t>(dataStartOffset);
	header.size = static_cast<uint32_t>(size);
	ofs.seekp(startOffset);
	ofs.write(
		reinterpret_cast<const char*>(&header),
		sizeof(PakHeader)
	);
	ofs.seekp(dataEndOffset);
	ifs.close();
	return 0;
}

DDS_HEADER TextureReader::ReadDDSHeader(std::ifstream& ifs)
{
	DDS_HEADER  ddsHeader{};
	uint32_t magic = 0;
	if (!ifs.is_open())
	{
		throw std::runtime_error("Failed to open file for reading !!!");
	}
	ifs.read(
		reinterpret_cast<char*>(&magic),
		sizeof(uint32_t)
	);
	assert(magic == 0x20534444);
	ifs.read(
		reinterpret_cast<char*>(&ddsHeader),
		sizeof(DDS_HEADER)
	);
	if (ddsHeader.ddspf.dwFourCC = 0x30315844) // DX10
	{
		DDS_HEADER_DX10 dx10Header{};
		ifs.read(
			reinterpret_cast<char*>(&dx10Header),
			sizeof(DDS_HEADER_DX10)
		);
	}
	return ddsHeader;
}

void TextureReader::LoadDDSTextureIntoMemory(std::ifstream& ifs, uint32_t size, byte* buffer)
{
	if (!ifs.is_open())
	{
		throw std::runtime_error("Failed to open file for reading !!!");
	}
	ifs.read(
		reinterpret_cast<char*>(buffer),
		size
	);
}
