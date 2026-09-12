#include "AssetManager.h"
#include "stb_image.h"

bool AssetManager::Serialize(const std::string& filename)
{
    std::ofstream ofs(filename, std::ios::out);
    if (!ofs.is_open())
    {
        throw std::runtime_error("Failed to open file for writing !!!");
    }
    using json = nlohmann::json;
    json jassetRegistry;
    jassetRegistry["assetResources"] = json::array();
    for (auto& arp : m_AssetResourceAndHandleMap)
    {
        json jasset;
        jasset["assetHandle"] = arp.second;
        jasset["assetResource"] = arp.first;
        jassetRegistry["assetResources"].push_back(jasset);
    }
    ofs << jassetRegistry.dump(4);
    ofs.close();
    std::cout << filename << " created!!!" << std::endl;
    std::string tempPath{ filename.substr(0,filename.find('.'))};
    std::string meshPath = tempPath + "_MESH.pak";
    std::string materialPath = tempPath + "_MTRL.pak";
    std::string texturePath = tempPath + "_TXTR.pak";
    std::string shaderPath = tempPath + "_SHDR.pak";
    std::string soundPath = tempPath + "_SWND.pak";
    std::string scriptPath = tempPath + "_SCRP.pak";
    std::string particlePath = tempPath + "_PSCP.pak";
    SerializeMaterialPack(materialPath);
    SerializeMeshPack(meshPath);
    SerializeTexturePack(texturePath);
    SerializeShaderPack(shaderPath);
    SerializeSoundPack(soundPath);
    SerializeScriptPack(scriptPath);
    SerializeParticleSystemPack(particlePath);
    return true;
}

bool AssetManager::SerializeMaterialPack(const std::string& filename)
{
    std::ofstream ofs(filename, std::ios::binary);
    auto startOffset = ofs.tellp();
    PakHeader header;
    PakWriter::WritePakHeader(ofs, header);
    uint32_t itemCount = 0;
    for (auto& group : m_MaterialGroupMap)
    {
        auto& assetHandle = group.first;
        auto& matGroup = group.second;
        if (!((assetHandle.m_HWORD == 0 && assetHandle.m_LWORD == 0) || matGroup.empty()))
        {
            auto dataStartOffset = ofs.tellp();
            PakHeader itemHeader;
            PakWriter::WritePakHeader(ofs, itemHeader);
            PakWriter::WriteAssetHandle(ofs, assetHandle);
            auto groupStartOffset = ofs.tellp();
            MaterialGroupHeader groupHeader;
            MaterialWriter::WriteMaterialGroupHeader(ofs, groupHeader);
            uint32_t materialCount = 0;
            for (auto& material : matGroup)
            {
                glm::vec4 KaNs{ 0 };
                glm::vec4 KdNi{ 0 };
                glm::vec4 KsD{ 0 };
                glm::vec4 KeIllum{ 0 };

                for (auto& materialComponent : material.m_Uniforms1f)
                {
                    auto& name = materialComponent.first;
                    auto& data = materialComponent.second;
                    if (name == "Ns")
                    {
                        KaNs[3] = data.x;
                    }
                    if (name == "Ni")
                    {
                        KdNi[3] = data.x;
                    }
                    if (name == "d")
                    {
                        KsD[3] = data.x;
                    }
                    if (name == "illum")
                    {
                        KeIllum[3] = data.x;
                    }
                }
                for (auto& materialComponent : material.m_Uniforms3f)
                {
                    auto& name = materialComponent.first;
                    auto& data = materialComponent.second;
                    if (name == "Kd")
                    {
                        KaNs[0] = data.x;
                        KaNs[1] = data.y;
                        KaNs[2] = data.z;
                    }
                    if (name == "Ka")
                    {
                        KdNi[0] = data.x;
                        KdNi[1] = data.y;
                        KdNi[2] = data.z;
                    }
                    if (name == "Ks")
                    {
                        KsD[0] = data.x;
                        KsD[1] = data.y;
                        KsD[2] = data.z;;
                    }
                    if (name == "Ke")
                    {
                        KeIllum[0] = data.x;
                        KeIllum[1] = data.y;
                        KeIllum[2] = data.z;
                    }
                }
                MTLMaterial tempMaterial{ KaNs, KdNi, KsD, KeIllum };
                MaterialWriter::WriteMaterialToFile(ofs, tempMaterial);
                ++materialCount;
            }
            MaterialWriter::RewriteMaterialGroupHeader(ofs, groupHeader, groupStartOffset, materialCount);
            PakWriter::RewritePakHeader(ofs, itemHeader, dataStartOffset, 1);
            ++itemCount;
        }
    }
    PakWriter::RewritePakHeader(ofs, header, startOffset, itemCount);
    ofs.close();
    return true;
}

bool AssetManager::SerializeMeshPack(const std::string& filename)
{
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs.is_open())
    {
        throw std::runtime_error("Failed to open file for writing !!!");
    }
    auto startOffset = ofs.tellp();
    PakHeader pakHeader;
    PakWriter::WritePakHeader(ofs, pakHeader);
    uint32_t itemCount = 0;
    for (auto& m : m_MeshMap)
    {
        auto& assetHandle = m.first;
        auto& mesh = m.second;
        auto subDataStartOffset = ofs.tellp();
        PakHeader itemHeader{};
        PakWriter::WritePakHeader(ofs, itemHeader);
        PakWriter::WriteAssetHandle(ofs, assetHandle);
        MeshWriter::WriteMeshToFile(ofs, mesh);
        PakWriter::RewritePakHeader(ofs, itemHeader, subDataStartOffset, 1);
        ++itemCount;
    }
    PakWriter::RewritePakHeader(ofs, pakHeader, startOffset, itemCount);
    ofs.close();
    return true;
}

bool AssetManager::SerializeTexturePack(const std::string& filename)
{
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs.is_open())
    {
        throw std::runtime_error("Failed to open file for writing !!!");
    }
    auto startOffset = ofs.tellp();
    PakHeader header{};
    PakWriter::WritePakHeader(ofs, header);
    uint32_t itemCount = 0;
    for (auto& t : m_TextureMap)
    {
        auto& textureHandle = t.first;
        auto& texture = t.second;
        for (auto& r : m_AssetResourceAndHandleMap)
        {
            auto& resource = r.first;
            auto& assetHandle = r.second;
            if (textureHandle == assetHandle)
            {
                auto& filepath = resource.m_Filepath;
                auto words = stringUtils::getWords(filepath, "+");
                auto itemCount = words.size();
                auto extension = words.front().substr(filepath.find('.'));
                if (extension == ".DDS" || extension == ".dds")
                {
                    auto subDataStartOffset = ofs.tellp();
                    PakHeader subHeader{};
                    PakWriter::WritePakHeader(ofs, subHeader);
                    PakWriter::WriteAssetHandle(ofs, textureHandle);
                    uint32_t subItemCount = 0;
                    for (auto& word : words)
                    {
                        TextureWriter::WriteDDSTextureToFile(ofs, word.c_str());
                        ++subItemCount;
                    }
                    PakWriter::RewritePakHeader(ofs, subHeader, subDataStartOffset, subItemCount);
                }
            }
        }
        ++itemCount;
    }
    PakWriter::RewritePakHeader(ofs, header, startOffset, itemCount);
    ofs.close();
    return false;
}

bool AssetManager::SerializeShaderPack(const std::string& filename)
{
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs.is_open())
    {
        throw std::runtime_error("Failed to open file for writing !!!");
    }
    auto startOffset = ofs.tellp();
    PakHeader pakHeader{};
    PakWriter::WritePakHeader(ofs, pakHeader);
    uint32_t programCount = 0;
    for (auto& program : m_ShaderMap)
    {
        const auto& assetHandle = program.first;
        const auto& resource = m_AssetHandleAndResourceMap[assetHandle];
        const auto& resourceCombinedPath = resource.m_Filepath;
        auto programDataStartOffset = ofs.tellp();
        PakHeader programHeader{};
        PakWriter::WritePakHeader(ofs, programHeader);
        PakWriter::WriteAssetHandle(ofs, assetHandle);
        const auto& shaderResource = m_ShaderResourceMap[assetHandle];
        const auto& shaderInfo = shaderResource.GetShaderInfo();
        const auto& shaderSources = shaderResource.GetShaderSources();
        uint32_t stageCount{ 0 };
        assert(shaderInfo.size() == shaderSources.size());
        for (auto index = 0; index < shaderSources.size(); index++)
        {
            const auto& stageInfo = shaderInfo[index];
            const auto& shaderSource = shaderSources[index];
            auto stageStartOffset = ofs.tellp();
            ShaderFileHeader stageHeader{};
            ShaderWriter::WriteShaderProgramHeaderToFile(ofs, stageHeader);
            ofs.write(
                reinterpret_cast<const char*>(shaderSource.data()),
                shaderSource.size()
            );
            ShaderWriter::ReWriteShaderProgramHeaderToFile(
                ofs,
                stageHeader,
                stageStartOffset,
                1,
                stageInfo.shaderType
            );
            stageCount++;
        }
        programCount++;
        PakWriter::RewritePakHeader(ofs, programHeader, programDataStartOffset, stageCount);
    }
    PakWriter::RewritePakHeader(ofs, pakHeader, startOffset, programCount);
    ofs.close();
    return true;
}

bool AssetManager::SerializeSoundPack(const std::string& filename)
{
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs.is_open())
    {
        throw std::runtime_error("Failed to open file for writing !!!");
    }
    auto startOffset = ofs.tellp();
    PakHeader pakHeader{};
    PakWriter::WritePakHeader(ofs, pakHeader);
    uint32_t soundCount{ 0 };
    for (auto& sound : m_SoundMap)
    {
        const auto& assetHandle = sound.first;
        const auto& resource = m_AssetHandleAndResourceMap[assetHandle];
        const auto& filePath = resource.m_Filepath;
        auto subDataStartOffset = ofs.tellp();
        PakHeader itemHeader;
        PakWriter::WritePakHeader(ofs, itemHeader);
        PakWriter::WriteAssetHandle(ofs, assetHandle);
        SoundWriter::WriteSoundToFile(ofs, filePath.c_str());
        PakWriter::RewritePakHeader(ofs, itemHeader, subDataStartOffset, 1);
        soundCount++;
    }
    PakWriter::RewritePakHeader(ofs, pakHeader, startOffset, soundCount);
    ofs.close();
    return true;
}

bool AssetManager::SerializeScriptPack(const std::string& filename)
{
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs.is_open())
    {
        throw std::runtime_error("Failed to open file for writing !!!");
    }
    auto startOffset = ofs.tellp();
    PakHeader pakHeader{};
    PakWriter::WritePakHeader(ofs, pakHeader);
    uint32_t scriptCount{ 0 };
    for (auto& script : m_ScriptMap)
    {
        const auto& assetHandle = script.first;
        const auto& scriptData = script.second;
        const auto& resource = m_AssetHandleAndResourceMap[assetHandle];
        auto subDataStartOffset = ofs.tellp();
        PakHeader itemHeader;
        PakWriter::WritePakHeader(ofs, itemHeader);
        PakWriter::WriteAssetHandle(ofs, assetHandle);
        ScriptWriter::WriteScriptDataToFile(ofs, resource.m_Filepath.c_str());
        PakWriter::RewritePakHeader(ofs, itemHeader, subDataStartOffset, 1);
        scriptCount++;
    }
    PakWriter::RewritePakHeader(ofs, pakHeader, startOffset, scriptCount);
    ofs.close();
    return true;
}

bool AssetManager::SerializeParticleSystemPack(const std::string& filename)
{
    std::ofstream ofs(filename);
    if (!ofs.is_open())
    {
        throw std::runtime_error("Failed to open file for reading !!!");
    }
    using json = nlohmann::json;
    json jassetRegistery;
    jassetRegistery["particleSystemConfigs"] = json::array();
    for (auto& ps : m_ParticleSystems)
    {
        json jasset;
        jasset["assetHandle"] = AssetHandle(0, 0);
        jasset["config"] = ps;
        jassetRegistery["particleSystemConfigs"].push_back(jasset);
    }
    ofs << jassetRegistery.dump(4);
    ofs.close();
    return true;
}

bool AssetManager::Deserialize(const std::string& filepath)
{
    std::ifstream ifs(filepath);
    if (!ifs.is_open())
    {
        throw std::runtime_error("Failed to open for reading !!!");
    }
    using json = nlohmann::json;
    json jassetRegistry;
    ifs >> jassetRegistry;
    for (auto& jasset : jassetRegistry["assetResources"])
    {
        auto assetHandle = jasset["assetHandle"].get<AssetHandle>();
        auto assetResource = jasset["assetResource"].get<AssetResource>();

        auto validAssetHandle = GetResourceHandle(assetResource);
        if (assetHandle != validAssetHandle)
        {
            std::cerr << "Invalid AssetHandle Read from file" << std::endl;
            return false;
        }
    }
    ifs.close();
    return true;
}

bool AssetManager::DeserializeMaterialPack(const std::string filepath)
{
    std::ifstream ifs(filepath, std::ios::binary);
    if (!ifs.is_open())
    {
        throw std::runtime_error("Failed to open Mesh Pack !!!");
    }
    const PakHeader header = PakReader::ReadPakHeader(ifs);
    for (auto index = 0; index < header.itemCount; index++)
    {
        const PakHeader itemHeader = PakReader::ReadPakHeader(ifs);
        const AssetHandle assetHandle = PakReader::ReadAssetHandle(ifs);
        const MaterialGroupHeader groupHeader = MaterialReader::ReaderMaterialGroupHeader(ifs);
        std::vector<MTLMaterial> materialGroup(groupHeader.materialCount);
        for (auto index = 0; index < groupHeader.materialCount; index++)
        {
            const auto material = MaterialReader::ReadMaterialFromFile(ifs);
            materialGroup[index] = material;
        }
        m_NewMaterialGroupMap[assetHandle] = materialGroup;
    }
    ifs.close();
    return true;
}

bool AssetManager::DeserializeMeshPack(const std::string& filepath)
{
    std::ifstream ifs(filepath, std::ios::binary);
    if (!ifs.is_open())
    {
        throw std::runtime_error("Failed to open Mesh Pack !!!");
    }
    const PakHeader header = PakReader::ReadPakHeader(ifs);
    for (auto index = 0; index < header.itemCount; index++)
    {
        const PakHeader itemHeader = PakReader::ReadPakHeader(ifs);
        const AssetHandle readAssetHandle = PakReader::ReadAssetHandle(ifs);
        auto mesh = MeshReader::ReadMeshFromFile(ifs);
        auto& assetHandle = mesh.m_MaterialGroupHandle;
        mesh.m_Materials = m_MaterialGroupMap[assetHandle];
        m_MeshMap[assetHandle] = mesh;
    }
    ifs.close();
    return true;
}

bool AssetManager::DeserializeTexturePack(const std::string& filepath)
{
    std::ifstream ifs(filepath, std::ios::binary);
    if (!ifs.is_open())
    {
        throw std::runtime_error("Failed to open file for reading !!!");
    }
    const PakHeader pakHeader = PakReader::ReadPakHeader(ifs);
    for (auto index = 0; index < pakHeader.itemCount; index++)
    {
        const PakHeader subHeader = PakReader::ReadPakHeader(ifs);
        const AssetHandle assetHandle = PakReader::ReadAssetHandle(ifs);
        if (subHeader.itemCount == 1) // Texture2D
        {
            const PakHeader itemHeader = PakReader::ReadPakHeader(ifs);
            const auto ddsHeader = TextureReader::ReadDDSHeader(ifs);
            byte* buffer = new byte[itemHeader.size];
            TextureReader::LoadDDSTextureIntoMemory(ifs, itemHeader.size, buffer);
            TextureBase<GL_Texture> glTexture;
            glTexture.m_Height = static_cast<uint64_t>(ddsHeader.dwHeight);
            glTexture.m_Width = static_cast<uint64_t>(ddsHeader.dwWidth);
            glTexture.m_TextureFormat = _TextureFormat::RGBA_BPTC_UNORM;
            glTexture.m_TextureSource = _TextureSource::FILE;
            glTexture.m_TextureTarget = _TextureTarget::TEXTURE_2D;
            CreateOpenGLTexture(glTexture);
            glCompressedTextureSubImage2D(glTexture.m_Texture, 0, 0, 0, static_cast<GLsizei>(ddsHeader.dwWidth), static_cast<GLsizei>(ddsHeader.dwHeight), GL_COMPRESSED_RGBA_BPTC_UNORM, itemHeader.size, buffer);
            delete[] buffer;
            m_TextureMap[assetHandle] = glTexture;
            ifs.seekg(itemHeader.size + itemHeader.offset);
        }
        if (subHeader.itemCount == 6) // CubeMap
        {
            byte* buffers[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
            uint32_t bufferSizes[6] = { 0,0,0,0,0,0 };
            DDS_HEADER ddsHeader{};
            for (auto subIndex = 0; subIndex < subHeader.itemCount; subIndex++)
            {
                const PakHeader itemHeader = PakReader::ReadPakHeader(ifs);
                ddsHeader = TextureReader::ReadDDSHeader(ifs);
                bufferSizes[subIndex] = itemHeader.size;
                buffers[subIndex] = new byte[itemHeader.size];
                TextureReader::LoadDDSTextureIntoMemory(ifs, itemHeader.size, buffers[subIndex]);
                ifs.seekg(itemHeader.size + itemHeader.offset);
            }
            TextureBase<GL_Texture> map;
            map.m_Height = static_cast<uint64_t>(ddsHeader.dwHeight);
            map.m_Width = static_cast<uint64_t>(ddsHeader.dwWidth);
            map.m_TextureFormat = _TextureFormat::RGBA_BPTC_UNORM;
            map.m_TextureSource = _TextureSource::GL_ATTACHMENT;
            map.m_TextureTarget = _TextureTarget::TEXTURE_CUBE;
            CreateOpenGLTexture(map);
            glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, static_cast<GLsizei>(map.m_Width), static_cast<GLsizei>(map.m_Height), GL_COMPRESSED_RGBA_BPTC_UNORM, bufferSizes[0], buffers[0]);
            glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, static_cast<GLsizei>(map.m_Width), static_cast<GLsizei>(map.m_Height), GL_COMPRESSED_RGBA_BPTC_UNORM, bufferSizes[1], buffers[1]);
            glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, static_cast<GLsizei>(map.m_Width), static_cast<GLsizei>(map.m_Height), GL_COMPRESSED_RGBA_BPTC_UNORM, bufferSizes[2], buffers[2]);
            glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, static_cast<GLsizei>(map.m_Width), static_cast<GLsizei>(map.m_Height), GL_COMPRESSED_RGBA_BPTC_UNORM, bufferSizes[3], buffers[3]);
            glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, static_cast<GLsizei>(map.m_Width), static_cast<GLsizei>(map.m_Height), GL_COMPRESSED_RGBA_BPTC_UNORM, bufferSizes[4], buffers[4]);
            glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, static_cast<GLsizei>(map.m_Width), static_cast<GLsizei>(map.m_Height), GL_COMPRESSED_RGBA_BPTC_UNORM, bufferSizes[5], buffers[5]);
            m_TextureMap[assetHandle] = map;
            for (auto itemIndex = 0; itemIndex < 6; itemIndex++)
            {
                delete[] buffers[itemIndex];
            }
        }
    }
    ifs.close();
    return false;
}

bool AssetManager::DeserializeShaderPack(const std::string& filepath)
{
    std::ifstream ifs(filepath, std::ios::binary);
    if (!ifs.is_open())
    {
        throw std::runtime_error("Failed to open file for reading !!!");
    }
    const PakHeader pakHeader = PakReader::ReadPakHeader(ifs);
    for (size_t index = 0; index < pakHeader.itemCount; index++)
    {
        const PakHeader programHeader = PakReader::ReadPakHeader(ifs);
        const AssetHandle assetHandle = PakReader::ReadAssetHandle(ifs);
        std::vector<ShaderType> types(programHeader.itemCount);
        std::vector<std::string> sources(programHeader.itemCount);  
        for (auto stageIndex = 0; stageIndex < programHeader.itemCount; stageIndex++)
        {
            auto stageInfo = ShaderReader::ReadShaderProgramHeader(ifs);
            const auto readSize = stageInfo.size - sizeof(ShaderFileHeader);
            sources[stageIndex].resize(readSize);
            ifs.read(
                reinterpret_cast<char*>(sources[stageIndex].data()),
                readSize
            );
            ShaderType typeInfo{};
            switch (stageInfo.stageMask)
            {
            case 0:
            case 1:
                types[stageIndex] = ShaderType::VERTEX;
                break;
            case 2:
            case 3:
            case 4:
                types[stageIndex] = ShaderType::PIXEL;
                break;
            case 5:
                types[stageIndex] = ShaderType::COMPUTE;
                break;
            }
        }
        if (programHeader.itemCount > 0)
        {
            Shader shader{ types, sources };
            m_ShaderMap[assetHandle] = shader;
        }
    }
    ifs.close();
    return true;
}

bool AssetManager::DeserializeScriptPack(const std::string& filepath)
{
    std::ifstream ifs(filepath, std::ios::binary);
    if (!ifs.is_open())
    {
        throw std::runtime_error("Failed to open file for reading !!!");
    }
    const auto header = PakReader::ReadPakHeader(ifs);
    assert(header.magic == magic::PAK_MAGIC);
    for (auto index = 0; index < header.itemCount; index++)
    {
        const PakHeader itemHeader = PakReader::ReadPakHeader(ifs);
        const AssetHandle assetHandle = PakReader::ReadAssetHandle(ifs);
        std::string scriptData;
        const std::size_t readSize = itemHeader.size - (sizeof(PakHeader) + sizeof(AssetHandle));
        scriptData.resize(readSize);
        ifs.read(
            reinterpret_cast<char*>(scriptData.data()),
            readSize
        );
        m_ScriptMap[assetHandle] = scriptData;
    }
    ifs.close();
    return true;
}

bool AssetManager::DeserializeParticlePack(const std::string& filepath)
{
    std::ifstream ifs(filepath);
    if (!ifs.is_open())
    {
        throw std::runtime_error("Failed to open file for reading !!!");
    }
    using json = nlohmann::json;
    json jassetRegistry;
    ifs >> jassetRegistry;
    for (auto& jasset : jassetRegistry["particleSystemConfigs"])
    {
        auto assetHandle = jasset["assetHandle"].get<AssetHandle>();
        auto psConfig = jasset["config"].get<primitives::ParticleSystemConfig>();
        m_ParticleSystems.push_back(psConfig);
    }
    return true;
}

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
        case _TextureFormat::RGBA_BPTC_UNORM:
            glTextureStorage2D(tex_base.m_Texture, 1, GL_COMPRESSED_RGBA_BPTC_UNORM, width, height);
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
        case _TextureFormat::RGBA_BPTC_UNORM:
            glBindTexture(GL_TEXTURE_CUBE_MAP, tex_base.m_Texture);
            glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GL_COMPRESSED_RGBA_BPTC_UNORM, width, height);
            //glTextureStorage2D(tex_base.m_Texture, 1, GL_RGBA32F, width, height);
            break;
        }
        break;
    }
}

TextureBase<GL_Texture> AssetManager::CreateOpenGLCubeMap(const std::vector<std::string>& image_file_paths)
{
    auto& rightPath = image_file_paths.front();
    auto extension = rightPath.substr(rightPath.find('.'));
    TextureBase<GL_Texture> map;
    if (extension == ".DDS")
    {
        std::ifstream ifsRight(image_file_paths[0], std::ios::binary);
        auto rightDDSHeader = TextureReader::ReadDDSHeader(ifsRight);
        size_t rightBufferSize = 0;
        auto rightBuffer = LoadDDSIntoMemory(ifsRight, rightBufferSize);

        std::ifstream ifsLeft(image_file_paths[1], std::ios::binary);
        auto leftDDSHeader = TextureReader::ReadDDSHeader(ifsLeft);
        size_t leftBufferSize = 0;
        auto leftBuffer = LoadDDSIntoMemory(ifsLeft, leftBufferSize);

        std::ifstream ifsTop(image_file_paths[2], std::ios::binary);
        auto topDDSHeader = TextureReader::ReadDDSHeader(ifsTop);
        size_t topBufferSize = 0;
        auto topBuffer = LoadDDSIntoMemory(ifsTop, topBufferSize);

        std::ifstream ifsBottom(image_file_paths[3], std::ios::binary);
        auto bottomDDSHeader = TextureReader::ReadDDSHeader(ifsBottom);
        size_t bottomBufferSize = 0;
        auto bottomBuffer = LoadDDSIntoMemory(ifsBottom, bottomBufferSize);

        std::ifstream ifsFront(image_file_paths[4], std::ios::binary);
        auto frontDDSHeader = TextureReader::ReadDDSHeader(ifsFront);
        size_t frontBufferSize = 0;
        auto frontBuffer = LoadDDSIntoMemory(ifsFront, frontBufferSize);

        std::ifstream ifsBehind(image_file_paths[5], std::ios::binary);
        auto behindDDSHeader = TextureReader::ReadDDSHeader(ifsBehind);
        size_t behindBufferSize = 0;
        auto behindBuffer = LoadDDSIntoMemory(ifsBehind, behindBufferSize);

        map.m_Height = static_cast<uint64_t>(rightDDSHeader.dwHeight);
        map.m_Width = static_cast<uint64_t>(rightDDSHeader.dwWidth);
        map.m_TextureFormat = _TextureFormat::RGBA_BPTC_UNORM;
        map.m_TextureSource = _TextureSource::GL_ATTACHMENT;
        map.m_TextureTarget = _TextureTarget::TEXTURE_CUBE;
        CreateOpenGLTexture(map);

        glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, static_cast<GLsizei>(map.m_Width), static_cast<GLsizei>(map.m_Height), GL_COMPRESSED_RGBA_BPTC_UNORM, rightBufferSize, rightBuffer);
        glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, static_cast<GLsizei>(map.m_Width), static_cast<GLsizei>(map.m_Height), GL_COMPRESSED_RGBA_BPTC_UNORM, leftBufferSize, leftBuffer);
        glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, static_cast<GLsizei>(map.m_Width), static_cast<GLsizei>(map.m_Height), GL_COMPRESSED_RGBA_BPTC_UNORM, topBufferSize, topBuffer);
        glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, static_cast<GLsizei>(map.m_Width), static_cast<GLsizei>(map.m_Height), GL_COMPRESSED_RGBA_BPTC_UNORM, bottomBufferSize, bottomBuffer);
        glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, static_cast<GLsizei>(map.m_Width), static_cast<GLsizei>(map.m_Height), GL_COMPRESSED_RGBA_BPTC_UNORM, frontBufferSize, frontBuffer);
        glCompressedTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, static_cast<GLsizei>(map.m_Width), static_cast<GLsizei>(map.m_Height), GL_COMPRESSED_RGBA_BPTC_UNORM, behindBufferSize, behindBuffer);

        delete[] rightBuffer;
        delete[] leftBuffer;
        delete[] topBuffer;
        delete[] bottomBuffer;
        delete[] frontBuffer;
        delete[] behindBuffer;

        ifsRight.close();
        ifsLeft.close();
        ifsTop.close();
        ifsBottom.close();
        ifsFront.close();
        ifsBehind.close();
    }
    else
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

        map.m_Height = static_cast<uint64_t>(width);
        map.m_Width = static_cast<uint64_t>(height);

        switch (channels)
        {
        case 3:
            map.m_TextureFormat = _TextureFormat::RGB8;
            break;
        case 4:
            map.m_TextureFormat = _TextureFormat::RGBA8;
            break;
        }
        map.m_TextureSource = _TextureSource::GL_ATTACHMENT;
        map.m_TextureTarget = _TextureTarget::TEXTURE_CUBE;
        CreateOpenGLTexture(map);

        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, map.m_Width, map.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, right);
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, map.m_Width, map.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, left);
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, map.m_Width, map.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, top);
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, map.m_Width, map.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, bottom);
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, map.m_Width, map.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, front);
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, map.m_Width, map.m_Height, GL_RGBA, GL_UNSIGNED_BYTE, behind);

        stbi_image_free(right);
        stbi_image_free(left);
        stbi_image_free(top);
        stbi_image_free(bottom);
        stbi_image_free(front);
        stbi_image_free(behind);
    }
    return map;
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
    auto extension = image_path.substr(image_path.find('.'));
    if (extension == ".DDS")
    {
        auto texture = LoadDDSTextureFromFile(image_path);
        return texture;
    }
    else
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
        glTextureSubImage2D(glTexture.m_Texture, 0, 0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height), GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        stbi_image_free(imageData);
        return glTexture;
    }
}

TextureBase<GL_Texture> AssetManager::LoadDDSTextureFromFile(const std::string& file_path)
{
    std::ifstream ifs(file_path, std::ios::binary);
    if (!ifs.is_open())
    {
        throw std::runtime_error("Failed to open file for reading !!!");
    }
    DDS_HEADER  ddsHeader = TextureReader::ReadDDSHeader(ifs);
    size_t bufferSize = 0;
    byte* buffer = LoadDDSIntoMemory(ifs, bufferSize);
    TextureBase<GL_Texture> glTexture;
    glTexture.m_Height = static_cast<uint64_t>(ddsHeader.dwHeight);
    glTexture.m_Width = static_cast<uint64_t>(ddsHeader.dwWidth);
    glTexture.m_TextureFormat = _TextureFormat::RGBA_BPTC_UNORM;
    glTexture.m_TextureSource = _TextureSource::FILE;
    glTexture.m_TextureTarget = _TextureTarget::TEXTURE_2D;
    CreateOpenGLTexture(glTexture);
    glCompressedTextureSubImage2D(glTexture.m_Texture, 0, 0, 0, static_cast<GLsizei>(ddsHeader.dwWidth), static_cast<GLsizei>(ddsHeader.dwHeight), GL_COMPRESSED_RGBA_BPTC_UNORM, bufferSize, buffer);
    delete[] buffer;
    ifs.close();
    return glTexture;
}

byte* AssetManager::LoadDDSIntoMemory(std::ifstream& ifs, size_t & size)
{
    auto dataStartPos = ifs.tellg();
    ifs.seekg(0, std::ios::end);
    auto dataEndPos = ifs.tellg();
    size = static_cast<size_t>(dataEndPos - dataStartPos);
    byte* buffer = new byte[size];
    ifs.seekg(dataStartPos);
    ifs.read(
        reinterpret_cast<char*>(buffer),
        size
    );
    return buffer;
}
