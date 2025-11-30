#include "ObjectLoader.h"

std::string GetDirectoryFromPath(const std::string& filepath) {
	size_t found = filepath.find_last_of("/\\");
	return (found != std::string::npos) ? filepath.substr(0, found + 1) : "";
}


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

void ObjectLoader::ExtractDump(const std::string& dump_line, uint64_t object_index) {
	auto string_words = getWords(dump_line.substr(2), " ");
	for (const auto& word : string_words) {
		auto parts = getWords(word, "/");
		if (parts.size() >= 1 && !parts[0].empty())
			m_VertexIndices[object_index].push_back(std::stoul(parts[0]) - 1);
		if (parts.size() >= 2 && !parts[1].empty())
			m_TextureIndices[object_index].push_back(std::stoul(parts[1]) - 1);
		if (parts.size() >= 3 && !parts[2].empty())
			m_NormalIndices[object_index].push_back(std::stoul(parts[2]) - 1);
	}
}



int FBXObjectLoader::LoadObjectFromFile(const char* file_path)
{

	//GLuint mVBONames[VBO_COUNT];
	FbxArray<SubMesh*> mSubMeshes;
	bool mHasNormal{ false };
	bool mHasUV{ false };
	bool mAllByControlPoint{ false }; // Save data in VBO by control point or by polygon vertex.
	m_pManager = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create(m_pManager, IOSROOT);
	m_pManager->SetIOSettings(ios);

	FbxImporter* pFbxImporter = FbxImporter::Create(m_pManager, "");

	auto filename = file_path;

	bool import_status = pFbxImporter->Initialize(filename, -1, m_pManager->GetIOSettings());

	m_pScene = FbxScene::Create(m_pManager, "");

	if (pFbxImporter->IsFBX())
	{
		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		(*(m_pManager->GetIOSettings())).SetBoolProp(IMP_FBX_MATERIAL, true);
		(*(m_pManager->GetIOSettings())).SetBoolProp(IMP_FBX_TEXTURE, true);
		(*(m_pManager->GetIOSettings())).SetBoolProp(IMP_FBX_LINK, true);
		(*(m_pManager->GetIOSettings())).SetBoolProp(IMP_FBX_SHAPE, true);
		(*(m_pManager->GetIOSettings())).SetBoolProp(IMP_FBX_GOBO, true);
		(*(m_pManager->GetIOSettings())).SetBoolProp(IMP_FBX_ANIMATION, true);
		(*(m_pManager->GetIOSettings())).SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}
	// Import the scene
	import_status = pFbxImporter->Import(m_pScene);

	auto ConstructMesh = [&](FbxMesh* pMesh)
		{

			std::vector<glm::vec3> pos;
			std::vector<glm::vec2> tex;
			std::vector<float> id;
			std::vector<glm::vec3> norm;

			const int lPolygonCount = pMesh->GetPolygonCount();

			// Count the polygon count of each material
			FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
			FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
			if (pMesh->GetElementMaterial())
			{
				lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
				lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();
				if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
				{
					FBX_ASSERT(lMaterialIndice->GetCount() == lPolygonCount);
					if (lMaterialIndice->GetCount() == lPolygonCount)
					{
						// Count the faces of each material
						for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
						{
							const int lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
							if (mSubMeshes.GetCount() < lMaterialIndex + 1)
							{
								mSubMeshes.Resize(lMaterialIndex + 1);
							}
							if (mSubMeshes[lMaterialIndex] == NULL)
							{
								mSubMeshes[lMaterialIndex] = new SubMesh;
							}
							mSubMeshes[lMaterialIndex]->TriangleCount += 1;
						}

						// Make sure we have no "holes" (NULL) in the mSubMeshes table. This can happen
						// if, in the loop above, we resized the mSubMeshes by more than one slot.
						for (int i = 0; i < mSubMeshes.GetCount(); i++)
						{
							if (mSubMeshes[i] == NULL)
								mSubMeshes[i] = new SubMesh;
						}

						// Record the offset (how many vertex)
						const int lMaterialCount = mSubMeshes.GetCount();
						int lOffset = 0;
						for (int lIndex = 0; lIndex < lMaterialCount; ++lIndex)
						{
							mSubMeshes[lIndex]->IndexOffset = lOffset;
							lOffset += mSubMeshes[lIndex]->TriangleCount * 3;
							// This will be used as counter in the following procedures, reset to zero
							mSubMeshes[lIndex]->TriangleCount = 0;
						}
						FBX_ASSERT(lOffset == lPolygonCount * 3);
					}
				}
			}

			// All faces will use the same material.
			if (mSubMeshes.GetCount() == 0)
			{
				mSubMeshes.Resize(1);
				mSubMeshes[0] = new SubMesh();
			}

			// Congregate all the data of a mesh to be cached in VBOs.
			// If normal or UV is by polygon vertex, record all vertex attributes by polygon vertex.
			mHasNormal = pMesh->GetElementNormalCount() > 0;
			mHasUV = pMesh->GetElementUVCount() > 0;
			FbxGeometryElement::EMappingMode lNormalMappingMode = FbxGeometryElement::eNone;
			FbxGeometryElement::EMappingMode lUVMappingMode = FbxGeometryElement::eNone;
			if (mHasNormal)
			{
				lNormalMappingMode = pMesh->GetElementNormal(0)->GetMappingMode();
				if (lNormalMappingMode == FbxGeometryElement::eNone)
				{
					mHasNormal = false;
				}
				if (mHasNormal && lNormalMappingMode != FbxGeometryElement::eByControlPoint)
				{
					mAllByControlPoint = false;
				}
			}
			if (mHasUV)
			{
				lUVMappingMode = pMesh->GetElementUV(0)->GetMappingMode();
				if (lUVMappingMode == FbxGeometryElement::eNone)
				{
					mHasUV = false;
				}
				if (mHasUV && lUVMappingMode != FbxGeometryElement::eByControlPoint)
				{
					mAllByControlPoint = false;
				}
			}

			// Allocate the array memory, by control point or by polygon vertex.
			int lPolygonVertexCount = pMesh->GetControlPointsCount();
			if (!mAllByControlPoint)
			{
				lPolygonVertexCount = lPolygonCount * TRIANGLE_VERTEX_COUNT;
			}
			float* lVertices = new float[lPolygonVertexCount * VERTEX_STRIDE];
			unsigned int* lIndices = new unsigned int[lPolygonCount * TRIANGLE_VERTEX_COUNT];
			float* lNormals = NULL;
			if (mHasNormal)
			{
				lNormals = new float[lPolygonVertexCount * NORMAL_STRIDE];
			}
			float* lUVs = NULL;
			FbxStringList lUVNames;
			pMesh->GetUVSetNames(lUVNames);
			const char* lUVName = NULL;
			if (mHasUV && lUVNames.GetCount())
			{
				lUVs = new float[lPolygonVertexCount * UV_STRIDE];
				lUVName = lUVNames[0];
			}

			// Populate the array with vertex attribute, if by control point.
			const FbxVector4* lControlPoints = pMesh->GetControlPoints();
			FbxVector4 lCurrentVertex;
			FbxVector4 lCurrentNormal;
			FbxVector2 lCurrentUV;
			if (mAllByControlPoint)
			{
				const FbxGeometryElementNormal* lNormalElement = NULL;
				const FbxGeometryElementUV* lUVElement = NULL;
				if (mHasNormal)
				{
					lNormalElement = pMesh->GetElementNormal(0);
				}
				if (mHasUV)
				{
					lUVElement = pMesh->GetElementUV(0);
				}
				for (int lIndex = 0; lIndex < lPolygonVertexCount; ++lIndex)
				{
					// Save the vertex position.
					lCurrentVertex = lControlPoints[lIndex];
					lVertices[lIndex * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
					lVertices[lIndex * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
					lVertices[lIndex * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
					lVertices[lIndex * VERTEX_STRIDE + 3] = 1;

					// Save the normal.
					if (mHasNormal)
					{
						int lNormalIndex = lIndex;
						if (lNormalElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
						{
							lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndex);
						}
						lCurrentNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
						lNormals[lIndex * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
						lNormals[lIndex * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
						lNormals[lIndex * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);

					}

					// Save the UV.
					if (mHasUV)
					{
						int lUVIndex = lIndex;
						if (lUVElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
						{
							lUVIndex = lUVElement->GetIndexArray().GetAt(lIndex);
						}
						lCurrentUV = lUVElement->GetDirectArray().GetAt(lUVIndex);
						lUVs[lIndex * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
						lUVs[lIndex * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);


					}
				}

			}

			int lVertexCount = 0;
			for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
			{
				// The material for current face.
				int lMaterialIndex = 0;
				if (lMaterialIndice && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
				{
					lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
				}

				// Where should I save the vertex attribute index, according to the material
				const int lIndexOffset = mSubMeshes[lMaterialIndex]->IndexOffset +
					mSubMeshes[lMaterialIndex]->TriangleCount * 3;
				for (int lVerticeIndex = 0; lVerticeIndex < TRIANGLE_VERTEX_COUNT; ++lVerticeIndex)
				{
					const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);
					// If the lControlPointIndex is -1, we probably have a corrupted mesh data. At this point,
					// it is not guaranteed that the cache will work as expected.
					if (lControlPointIndex >= 0)
					{
						if (mAllByControlPoint)
						{
							lIndices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lControlPointIndex);
						}
						// Populate the array with vertex attribute, if by polygon vertex.
						else
						{
							lIndices[lIndexOffset + lVerticeIndex] = static_cast<unsigned int>(lVertexCount);

							lCurrentVertex = lControlPoints[lControlPointIndex];
							lVertices[lVertexCount * VERTEX_STRIDE] = static_cast<float>(lCurrentVertex[0]);
							lVertices[lVertexCount * VERTEX_STRIDE + 1] = static_cast<float>(lCurrentVertex[1]);
							lVertices[lVertexCount * VERTEX_STRIDE + 2] = static_cast<float>(lCurrentVertex[2]);
							lVertices[lVertexCount * VERTEX_STRIDE + 3] = 1;

							auto x = static_cast<float>(lCurrentVertex[0]);
							auto y = static_cast<float>(lCurrentVertex[1]);
							auto z = static_cast<float>(lCurrentVertex[2]);
							pos.push_back(glm::vec3(x, y, z));
							id.push_back(lMaterialIndex);

							if (mHasNormal)
							{
								pMesh->GetPolygonVertexNormal(lPolygonIndex, lVerticeIndex, lCurrentNormal);
								lNormals[lVertexCount * NORMAL_STRIDE] = static_cast<float>(lCurrentNormal[0]);
								lNormals[lVertexCount * NORMAL_STRIDE + 1] = static_cast<float>(lCurrentNormal[1]);
								lNormals[lVertexCount * NORMAL_STRIDE + 2] = static_cast<float>(lCurrentNormal[2]);

								auto x = static_cast<float>(lCurrentNormal[0]);
								auto y = static_cast<float>(lCurrentNormal[1]);
								auto z = static_cast<float>(lCurrentNormal[2]);
								norm.push_back(glm::vec3(x, y, z));
							}

							if (mHasUV)
							{
								bool lUnmappedUV;
								pMesh->GetPolygonVertexUV(lPolygonIndex, lVerticeIndex, lUVName, lCurrentUV, lUnmappedUV);
								lUVs[lVertexCount * UV_STRIDE] = static_cast<float>(lCurrentUV[0]);
								lUVs[lVertexCount * UV_STRIDE + 1] = static_cast<float>(lCurrentUV[1]);

								auto u = static_cast<float>(lCurrentUV[0]);
								auto v = static_cast<float>(lCurrentUV[1]);
								tex.push_back(glm::vec2(u, v));
							}
						}
					}
					++lVertexCount;
				}
				mSubMeshes[lMaterialIndex]->TriangleCount += 1;
			}

			std::vector<Vertex> v;


			for (auto index = 0; index < lPolygonVertexCount; index++)
			{
				auto uv = mHasUV ? tex[index] : glm::normalize(glm::vec2(index, lPolygonCount));
				v.push_back(Vertex{ pos[index], uv, id[index], norm[index] });
			}
			/*
			if (lVertices)
			{
				delete[] lVertices;
			}

			if (lIndices)
			{
				delete[] lIndices;
			}
			*/
			return v;
		};

	// RETRIEVE ANIMATION DATA
	int i = 0;
	int numObjects = m_pScene->GetSrcObjectCount<FbxGeometry>();
	for (int object_index = 0; object_index < numObjects; object_index++)
	{
		auto object = m_pScene->GetSrcObject<FbxGeometry>(object_index);
		int numStacks = object->GetSrcObjectCount<FbxAnimStack>();
		if (numStacks)
		{
			for (auto anim_index = 0; anim_index < numStacks; anim_index++)
			{
				auto anim_stack = object->GetSrcObject< FbxAnimStack>(anim_index);
				auto num_layers = anim_stack->GetMemberCount<FbxAnimLayer>();
				for (auto layer_index = 0; layer_index < num_layers; layer_index++)
				{
					auto anim_layer = anim_stack->GetMember<FbxAnimLayer>(layer_index);
				}
			}

		}
	}
	auto node_count = m_pScene->GetNodeCount();
	int numMeshes = 0, numSkeleton = 0, numLights = 0, numCameras = 0;
	for (auto node_index = 1; node_index < m_pScene->GetNodeCount(); node_index++)
	{
		auto child_node = m_pScene->GetNode(node_index);
		auto node_attrib = child_node->GetNodeAttribute();
		auto sc = node_attrib->GetSrcObjectCount<FbxAnimStack>();
		auto node_attrib_type = node_attrib->GetAttributeType();

		auto _scl = child_node->LclScaling.Get();
		auto _rot = child_node->LclRotation.Get();
		auto _pos = child_node->LclTranslation.Get();

		int numMaterials = child_node->GetMaterialCount();
		for (int material_index = 0; material_index < numMaterials; material_index++)
		{
			auto mat = child_node->GetMaterial(material_index);
			auto ambient = mat->sAmbientFactor;
			auto diffuse = mat->sDiffuseFactor;
			auto specular = mat->sSpecularFactor;
		}

		glm::vec3 scl = glm::vec3(_scl[0] * 0.001, _scl[1] * 0.001, _scl[2] * 0.001);
		glm::vec3 rot = glm::vec3(_rot[0] * 0.001, _rot[1] * 0.001, _rot[2] * 0.001);
		glm::vec3 pos = glm::vec3(_pos[0] * 0.001, _pos[1] * 0.001, _pos[2] * 0.001);

		auto transform = glm::scale(glm::mat4(1.0f), scl);
		//transform = glm::rotate(transform, glm::radians(90.0f), rot);
		transform = glm::translate(transform, pos);

		//auto _transform = child_node->EvaluateGlobalTransform();

		std::cout << "name:" << child_node->GetName() << std::endl;
		std::cout << "node type: " << node_attrib_type << std::endl;
		std::cout << "scale:" << _scl.Buffer() << std::endl;
		std::cout << "rotation:" << _rot.Buffer() << std::endl;
		std::cout << "translation:" << _pos.Buffer() << std::endl;
		//std::cout << "transform:" << _transform.Buffer() << std::endl;

		FbxLight* light = nullptr;
		FbxMesh* mesh = nullptr;
		FbxSkeleton* skeleton = nullptr;

		switch (node_attrib_type)
		{
		case FbxNodeAttribute::EType::eCamera:
			numCameras++;
			break;

		case FbxNodeAttribute::EType::eLight:
			numLights++;
			light = reinterpret_cast<FbxLight*>(child_node->GetLight());
			break;

		case FbxNodeAttribute::EType::eMesh:
			numMeshes++;
			mesh = reinterpret_cast<FbxMesh*>(child_node->GetGeometry());
			m_MeshData = ConstructMesh(mesh);
			break;
		case FbxNodeAttribute::EType::eSkeleton:
			numSkeleton++;
			skeleton = reinterpret_cast<FbxSkeleton*>(child_node->GetSkeleton());
			break;
		}
	}
	pFbxImporter->Destroy();

    return 0;
}

int OBJObjectLoader::LoadObjectFromFile(const char* file_path) {
	std::ifstream file(file_path, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		std::cerr << "Failed to open .obj file: " << file_path << "\n";
		return -1;
	}

	std::streamsize size = file.tellg();
	file.seekg(0);
	std::string buffer(size, '\0');
	if (!file.read(&buffer[0], size)) {
		std::cerr << "Failed to read .obj file into memory.\n";
		return -1;
	}

	std::istringstream in(buffer);
	std::string line;
	int64_t object_index = -1;
	size_t material_index = 0;
	std::unordered_map<std::string, size_t> materialLookup;
	std::string objDir = GetDirectoryFromPath(file_path);

	while (std::getline(in, line)) {
		auto words = getWords(line, " ");
		if (words.empty()) continue;
		const std::string& type = words[0];
		char* endptr;

		if (type == "mtllib" && words.size() >= 2) {
			std::string fullPath = objDir + words[1];
			m_MaterialPaths.push_back(fullPath);
			LoadMaterialFromFile(fullPath.c_str());

			for (size_t i = 0; i < m_MaterialNames.size(); ++i) {
				materialLookup[m_MaterialNames[i]] = i;
			}
		}
		else if (type == "usemtl" && words.size() >= 2) {
			auto it = materialLookup.find(words[1]);
			if (it != materialLookup.end()) {
				material_index = it->second;
			}
		}
		else if (type == "o" && words.size() >= 2) {
			m_ObjectNames.push_back(words[1]);
			m_Positions.emplace_back();
			m_TexCoords.emplace_back();
			m_VertexIDs.emplace_back();
			m_Normals.emplace_back();
			m_VertexIndices.emplace_back();
			m_TextureIndices.emplace_back();
			m_NormalIndices.emplace_back();
			++object_index;
		}
		else if (type == "s" && words.size() >= 2) {
			m_Surfaces.push_back(words[1]);
		}
		else if (type == "v" && words.size() >= 4) {
			float x = strtof(words[1].c_str(), &endptr);
			float y = strtof(words[2].c_str(), &endptr);
			float z = strtof(words[3].c_str(), &endptr);
			m_Positions[object_index].emplace_back(x, y, z);
		}
		else if (type == "vt" && words.size() >= 3) {
			float u = strtof(words[1].c_str(), &endptr);
			float v = strtof(words[2].c_str(), &endptr);
			m_TexCoords[object_index].emplace_back(u, v);
		}
		else if (type == "vn" && words.size() >= 4) {
			float x = strtof(words[1].c_str(), &endptr);
			float y = strtof(words[2].c_str(), &endptr);
			float z = strtof(words[3].c_str(), &endptr);
			m_Normals[object_index].emplace_back(x, y, z);
		}
		else if (type == "f") {
			ExtractDump(line, object_index);
			m_VertexIDs[object_index].insert(m_VertexIDs[object_index].end(), 3, material_index);
		}
	}

	return 0;
}


int OBJObjectLoader::LoadMaterialFromFile(const char* file_path) {
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
				m_MaterialNames.push_back(words[1]);
				firstMaterial = false;
				continue;
			}
			m_Materials.push_back(tempMaterial);
			m_MaterialNames.push_back(words[1]);
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
	m_Materials.push_back(tempMaterial);
	return 0;
}
