#include "ObjectLoader.h"


bool CheckDescription(std::string str1, std::string str2)
{
    return (bool)str1.compare(str2) == 0 ? true : false;
}

bool InDescriptorSet(const char* disc, std::vector<const char*> descriptors)
{
    for (auto& i : descriptors)
    {
        if (CheckDescription(i, disc))
        {
            return true;
        }
    }
    return false;
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

void ObjectLoader::ExtractDump(std::string dump, uint64_t object_index)
{
    /*
    std::istringstream iss(dump);
    unsigned int vindex;
    unsigned int tindex;
    unsigned int nindex;

    iss >> vindex;
    m_VertexIndices[object_index].push_back(--vindex);
    
    char delim;
    iss.read(&delim, 1);
    
    if (iss >> tindex) {
        m_TextureIndices[object_index].push_back(--tindex);
    }
    else {
        m_TextureIndices[object_index].push_back(0x00000000);
    }
    iss.read(&delim, 1);
   
    iss >> nindex;
    m_NormalIndices[object_index].push_back(--nindex);
    //iss.read(&delim, 1);

    */
    dump = dump.substr(2, dump.length());
    auto string_words = getWords(dump, " ");
    char* endptr;
    for (auto& word_temp : string_words)
    {
        auto digits = getWords(word_temp, "/");
        switch (digits.size())
        {
            /*
        case 2:
            m_VertexIndices.push_back((strtoul(digits[0].c_str(), &endptr, 10)) - 1);
            m_TextureIndices.push_back(0xFFFFFFFF);
            m_NormalIndices.push_back((strtoul(digits[1].c_str(), &endptr, 10)) - 1);
            break;
            */
        case 3:
            m_VertexIndices[object_index].push_back((strtoul(digits[0].c_str(), &endptr, 10)) - 1);
            m_TextureIndices[object_index].push_back((strtoul(digits[1].c_str(), &endptr, 10)) - 1);
            m_NormalIndices[object_index].push_back((strtoul(digits[2].c_str(), &endptr, 10)) - 1);
            break;
        }
    }
}

std::vector<std::string> ObjectLoader::getWords(std::string s, const char* delim) {
    std::vector<std::string> res;
    int pos = 0;
    while (pos < s.size()) {
        pos = s.find(delim);
        res.push_back(s.substr(0, pos));
        s.erase(0, pos + 1); // 3 is the length of the delimiter, "%20"
    }
    res.push_back(s);
    return res;
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

int OBJObjectLoader::LoadObjectFromFile(const char* file_path)
{
	unsigned int id = 1;
	// Read the file
	std::string whole_file;
	std::ifstream is(file_path, std::ios::binary | std::ios::in);

	if (!is.is_open())
	{
		std::cerr << "Failed to open the file\n" << file_path << "us not a vaild file" << std::endl;
		std::flush(std::cerr);
	}

	// Now all the data is stored in whole_file
	// Now Extract the data and do the thingies
	std::string temp = "";
	std::string c_temp = "";
	//is >> std::ws; // skip whitespaces

	std::string description;
	float x, y, z;
	float u, v;

	uint64_t object_index = 0;
	auto material_index = 0;

	std::vector<const char*> descriptorSet = { "v", "vt", "vn","f", "#", "o", "s", "usemtl", "mtllib" };

	for (std::string str = ""; std::getline(is, str);)
	{
		//auto index = str.find(" ");
		//description = str.substr(0, index);
		auto words = getWords(str, " ");
		description = words[0];

		if (CheckDescription(description, "mtllib"))
		{
			m_MaterialPaths.push_back(words[1]);
			LoadMaterialFromFile(m_MaterialPaths[m_MaterialPaths.size() - 1].c_str());
		}
		if (CheckDescription(description, "usemtl"))
		{
			auto& mat_name = words[1];
			for (size_t index = 0; index < m_MaterialNames.size(); index++)
			{
				if (m_MaterialNames[index] == mat_name)
				{
					material_index = index;
				}
			}

		}
		else  if (CheckDescription(description, "o"))
		{
			// CREATE NEW MESH AND THEN BASH THEM TOGETHER

			m_ObjectNames.push_back(words[1]);
			m_Positions.push_back({});
			m_TexCoords.push_back({});
			m_VertexIDs.push_back({});
			m_Normals.push_back({});

			m_VertexIndices.push_back({});
			m_TextureIndices.push_back({});
			m_NormalIndices.push_back({});
			object_index++;
		}
		else if (CheckDescription(description, "s"))
		{
			m_Surfaces.push_back(words[1]);
		}
		else if (CheckDescription(description, "v"))
		{
			// Vertex Position
			x = std::stof(words[1]);
			y = std::stof(words[2]);
			z = std::stof(words[3]);
			m_Positions[object_index - 1].push_back(glm::vec3(x, y, z));
			//m_VertexIDs[object_index - 1].push_back(material_index);
		}
		else if (CheckDescription(description, "vt"))
		{
			// Texture Coordinate
			u = std::stof(words[1]);
			v = std::stof(words[2]);
			m_TexCoords[object_index - 1].push_back(glm::vec2(u, v));
		}
		else if (CheckDescription(description, "vn"))
		{
			// Vertex Normal
			x = std::stof(words[1]);
			y = std::stof(words[2]);
			z = std::stof(words[3]);
			m_Normals[object_index - 1].push_back(glm::vec3(x, y, z));
		}
		else if (CheckDescription(description, "f"))
		{
			ExtractDump(str, object_index - 1);
			m_VertexIDs[object_index - 1].push_back(material_index);
			m_VertexIDs[object_index - 1].push_back(material_index);
			m_VertexIDs[object_index - 1].push_back(material_index);
		}

		is.clear();
	}
	return 0;
}

int OBJObjectLoader::LoadMaterialFromFile(const char* file_path)
{
	// Read the file
	std::ifstream is(file_path, std::ios::binary | std::ios::in);

	if (!is.is_open())
	{
		std::cerr << "Failed to open the file\n" << file_path << " is not a vaild file" << std::endl;
		std::flush(std::cerr);
	}

	// Now Extract the data and do the thingies
	//is >> std::ws; // skip whitespaces

	std::string description;
	float x, y, z;
	const char* descriptors[] = { "newmtl", "Ka", "Kd", "Ks", "Ni", "Ns", "d", "illum" };
	auto object_index = 0;

	for (std::string str; std::getline(is, str);)
	{
		auto words = getWords(str, " ");
		description = words[0];
		//is >> description;
		if (CheckDescription(description, "newmtl"))
		{
			m_Materials.push_back(Material(object_index));
			m_MaterialNames.push_back(words[1]);
			object_index++;
		}
		if (CheckDescription(description, "Ka"))
		{
			// Ambient Color
			x = std::stof(words[1]);
			y = std::stof(words[2]);
			z = std::stof(words[3]);
			m_Materials[object_index - 1].m_Ka = glm::vec3(x, y, z);
		}
		if (CheckDescription(description, "Kd"))
		{
			// Diffuse Color
			x = std::stof(words[1]);
			y = std::stof(words[2]);
			z = std::stof(words[3]);
			m_Materials[object_index - 1].m_Kd = glm::vec3(x, y, z);
		}
		if (CheckDescription(description, "Ks"))
		{
			// Specular Color
			x = std::stof(words[1]);
			y = std::stof(words[2]);
			z = std::stof(words[3]);
			m_Materials[object_index - 1].m_Ks = glm::vec3(x, y, z);
		}
		if (CheckDescription(description, "Ni"))
		{
			// Refractive Index
			x = std::stof(words[1]);
			//y = std::stof(words[2]);
			//z = std::stof(words[3]);
			m_Materials[object_index - 1].m_Ni = x;
		}
		if (CheckDescription(description, "Ns"))
		{
			// Refractive Index
			x = std::stof(words[1]);
			//y = std::stof(words[2]);
			//z = std::stof(words[3]);
			m_Materials[object_index - 1].m_Ns = x;
		}
		if (CheckDescription(description, "d"))
		{
			x = std::stof(words[1]);
			//y = std::stof(words[2]);
			//z = std::stof(words[3]);
			m_Materials[object_index - 1].m_d = x;
		}
		if (CheckDescription(description, "illum"))
		{
			x = std::stof(words[1]);
			//y = std::stof(words[2]);
			//z = std::stof(words[3]);
			//m_Materials[object_index-1].m_illum = x;
		}
		else continue;
	}
	return 0;
}
