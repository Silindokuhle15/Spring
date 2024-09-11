#include "Scene.h"

void Scene::AddNewMesh(Mesh& mesh)
{
	mesh.OnInit();
	m_StaticGeometry.push_back(mesh);
}

void Scene::OnCreateSceneObjects()
{
	// CREATE LIGHTS

	PointLight pointLight;
	Vector3 dummy_pos = {};
	Vector3 dummy_color = {};

	// CREATE THE SCENE CAMERA(s)
	std::vector<const char*> vec3_keys =
	{
		"x",
		"y",
		"z"
	};
	Vector3 vec3 = {};
	std::string str = "";
	std::vector<std::string> OBJ_paths;
	std::vector<std::string> shader_paths;
	std::vector<std::string> static_mesh_paths;
	std::vector<std::string> dynamic_mesh_paths;

	lua_Number number = 0;

	for (auto& var : m_LuaEngine.Keys)
	{
		lua_getglobal(m_LuaEngine.m_pLuaState, var.c_str());
		switch (lua_type(m_LuaEngine.m_pLuaState, -1))
		{
		case LUA_TTABLE:

			if (var == "static_geometry")
			{
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "OBJ_path"))
				{
					str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
					static_mesh_paths.push_back(str);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				break;
			}
			else if (var == "dynamic_geometry")
			{
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "OBJ_path"))
				{
					str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
					dynamic_mesh_paths.push_back(str);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				break;
			}
			else if (var == "shader")
			{
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "VShaderPath"))
				{
					str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
					shader_paths.push_back(str);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "FShaderPath"))
				{
					str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
					shader_paths.push_back(str);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				/*
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "material_texture"))
				{
					str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
					shader_paths.push_back(str);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "mtl_path"))
				{
					str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
					shader_paths.push_back(str);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				*/
				break;
			}
			else if (var == "scene_camera")
			{

				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "eye"))
				{
					m_LuaEngine.GetField(m_LuaEngine.m_pLuaState, vec3_keys, &vec3);
					m_ActiveCamera.m_eye = glm::vec3(vec3.x, vec3.y, vec3.z);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "center"))
				{
					m_LuaEngine.GetField(m_LuaEngine.m_pLuaState, vec3_keys, &vec3);
					m_ActiveCamera.m_center = glm::vec3(vec3.x, vec3.y, vec3.z);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				if (lua_getfield(m_LuaEngine.m_pLuaState, -1, "up"))
				{
					m_LuaEngine.GetField(m_LuaEngine.m_pLuaState, vec3_keys, &vec3);
					m_ActiveCamera.m_up = glm::vec3(vec3.x, vec3.y, vec3.z);
					lua_pop(m_LuaEngine.m_pLuaState, 1);
				}
				break;
			}

			if (var == "light_position")
			{
				dummy_pos = vec3;
			}
			else if (var == "light_color")
			{
				dummy_color = vec3;
			}
			else if (var == "sky_color")
			{
				auto var = glm::vec3(vec3.x, vec3.y, vec3.z);
				m_SkyLights.push_back(SkyLight(var));
			}
			else if (var == "ground_color")
			{
				auto var = glm::vec3(vec3.x, vec3.y, vec3.z);
				m_GroundLights.push_back(GroundLight(var));
			}
			break;
		case LUA_TSTRING:
			
			if (var == "obj_path")
			{
				str = lua_tostring(m_LuaEngine.m_pLuaState, -1);
				OBJ_paths.push_back(str);
				lua_pop(m_LuaEngine.m_pLuaState, 1);
			}
			break;

		case LUA_TNUMBER:
			number = lua_tonumber(m_LuaEngine.m_pLuaState, -1);
			var == "WIDTH" ? m_ActiveCamera.SetWidth(static_cast<int>(number)) : void();
			var == "HEIGHT" ? m_ActiveCamera.SetHeight(static_cast<int>(number)) : void();
			lua_pop(m_LuaEngine.m_pLuaState, 1);
 			break;
		}

	}

	/*	
	std::vector<ShaderInfo> m_shaderInfo;
	for (size_t index = 0; index < shader_paths.size(); index += 4)
	{
		m_shaderInfo = { ShaderInfo{ shader_paths[index], GL_VERTEX_SHADER },
			ShaderInfo{ shader_paths[index+1], GL_FRAGMENT_SHADER } };
		Shader temp_shader(m_shaderInfo);

		std::string& texture_path = shader_paths[index + 2];
		std::string& mtl_path = shader_paths[index + 3];
		m_Materials.push_back(
			Material(texture_path, mtl_path, m_shaderInfo));

		m_shaderInfo.clear();
	}
	for (auto& mat : m_Materials)
	{
		mat.OnInit();
	}
	m_Lights.push_back(
		PointLight(
			glm::vec3(dummy_pos.x, dummy_pos.y, dummy_pos.z),
			glm::vec3(dummy_color.x, dummy_color.y, dummy_color.z)
			)
		);

	for (auto& s_mesh : static_mesh_paths)
	{
		LoadMeshData(s_mesh.c_str(), 0);
	}

	BatchStaticGeometry();
	for (auto& d_mesh : dynamic_mesh_paths)
	{
		LoadMeshData(d_mesh.c_str(), 1);
	}

	m_SelectedMesh = 0;
	m_SelectedBuffer = (m_StaticGeometry.size() > 0 ? 0 : (m_DynamicGeometry.size() > 0 ? 1 : 2));
	switch (m_SelectedBuffer)
	{
	case 0:
		m_pActiveTransform = std::shared_ptr<glm::mat4>(&(m_StaticGeometry[m_SelectedMesh].m_Transform));
		break;
	case 1:
		m_pActiveTransform = std::shared_ptr<glm::mat4>(&(m_DynamicGeometry[m_SelectedMesh].m_Transform));
		break;
	case 2:
		m_pActiveTransform = std::shared_ptr<glm::mat4>(&(m_MeshData[m_SelectedMesh].m_Transform));
		break;
	}
	*/

	m_pActiveCamera = std::shared_ptr<Camera>(&m_ActiveCamera);
	m_pActiveCamera->OnInit();
}

void Scene::OnInit()
{
	m_State = SceneState::LOADING;
	/*
	m_LuaEngine.SetScriptPath(m_Title);

	m_LuaEngine.SetKeys(
		std::vector<std::string>({ 
			"WIDTH",
			"HEIGHT",
			"dynamic_geometry",
			"static_geometry",
			//LIGHTS
			"light_position",
			"light_color",
			"sky_color",
			"ground_color",
			//CAMERA
			"scene_camera",
			// TEXTUTES
			"material"
			}
		)
	);
	m_LuaEngine.Run();
	OnCreateSceneObjects();
	*/

	m_PhysicsEngine.OnInit();

	m_State = SceneState::RUNNING;
}

void Scene::OnUpdate(TimeStep ts)
{

	/*
	auto new_transform = *(m_pActiveTransform.get());
	switch (m_SelectedBuffer)
	{
	case 0:
		m_StaticGeometry[m_SelectedMesh].SetTransform(new_transform);
		break;
	case 1:
		m_DynamicGeometry[m_SelectedMesh].SetTransform(new_transform);
		break;
	case 2:
		m_MeshData[m_SelectedMesh].SetTransform(new_transform);
		break;
	}
	*/
	switch (m_State)
	{
	case SceneState::LOADING:

		break;

	case SceneState::RUNNING:
		//m_PhysicsEngine.OnUpdate(ts);
		break;

	case SceneState::PAUSED:
		break;

	case SceneState::END:
		break;

	case SceneState::STOPPED:
		break;
	}
}

void Scene::Run()
{
	//m_State = SceneState::RUNNING;
}

void Scene::OnEnd()
{
	//m_State = SceneState::END;
}

void Scene::OnPause()
{
	//m_State = SceneState::PAUSED;
}

void Scene::OnStop()
{
	//m_State = SceneState::STOPPED;
}

void Scene::OnReload()
{
	//m_State = SceneState::LOADING;
}


void Scene::Process()
{	

}

void Scene::LoadFbxScene(const std::string& path)
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

	auto filename = path.c_str();

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
	///for (auto node_index = 0; node_index < 1; node_index++)
	for (auto node_index = 1; node_index < m_pScene->GetNodeCount(); node_index++)
	{
		auto child_node = m_pScene->GetNode(node_index);
		auto node_attrib = child_node->GetNodeAttribute();
		auto sc = node_attrib->GetSrcObjectCount<FbxAnimStack>();
		auto node_attrib_type = node_attrib->GetAttributeType();

		auto _scl = child_node->LclScaling.Get();
		auto _rot = child_node->LclRotation.Get();
		auto _pos = child_node->LclTranslation.Get();

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
			m_MeshData.push_back(Mesh(ConstructMesh(mesh), transform));
			break;
		case FbxNodeAttribute::EType::eSkeleton:
			numSkeleton++;
			skeleton = reinterpret_cast<FbxSkeleton*>(child_node->GetSkeleton());
			break;
		}
	}
	pFbxImporter->Destroy();

}

void Scene::LoadMeshData(const char* path, int buffer)
{
	Mesh new_mesh;

	if (std::string(path) == "Grid")
	{		
		Grid d = Grid(250,250);
		d.OnInit();
		new_mesh = Mesh(d);
		new_mesh.SetTransform(glm::mat4(1.0f));
	}
	switch (buffer)
	{
	case 1: // DYANAMIC GEOMETRY
		//LoadDynamicGeometry(new_mesh);
		LoadFbxScene(path);
		break;
	default:
		new_mesh = Mesh(path);
		//new_mesh.OnInit();
		//new_mesh.SetTransform(glm::mat4(1.0f));
		LoadStaticGeometry(new_mesh);
	}

}

void Scene::LoadDynamicGeometry(Mesh& other)
{
	m_DynamicGeometry.push_back(other);
	NumMeshes++;
}

void Scene::BatchStaticGeometry()
{
	Mesh BatchedMesh = Mesh::Batch(m_StaticGeometry);
	m_StaticGeometry.clear();
	m_StaticGeometry.push_back(BatchedMesh);
}

void Scene::LoadStaticGeometry(Mesh& other)
{
	m_MeshData.push_back(other);
	NumMeshes++;
}
