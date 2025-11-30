#include "BaseApplication.h"

void BaseApplication::Run()
{
    m_Scene->SetAssetManager(&m_AssetManager);
    m_Scene->LoadSceneFromFile();
    m_Scene->OnInit();
    m_Scene->OnCreateSceneObjects();
    m_pUILayer.LoadScene(m_Scene);
    m_pUILayer.OnInit();
    
    m_pActiveRenderer = std::make_shared<Renderer>();

    m_pUILayer.BindRenderer(m_pActiveRenderer);
    m_pUILayer.m_ActiveRenderer->SetUpForRendering();
    m_pUILayer.m_pActiveCamera->SetEye(glm::vec3(0, 2, -2));
    m_pUILayer.m_pActiveCamera->SetCenter(glm::vec3(0, 0, 2));
    m_pUILayer.m_pActiveCamera->SetOrientation(glm::quat(0, -1, 0, 0));
    m_pUILayer.m_pActiveCamera->OnInit();

    scripting::ScriptMgr::expose_scene_camera(m_Scene->GetLuaState(), m_pUILayer.m_pActiveCamera.get(), "camera");

    m_LobbyMeshHandle = m_AssetManager.GetResourceHandle({ AssetType::MeshResource,"C:/dev/Spring/Assets/Objects/House/Room.obj" });
    std::string sound{ "C:/dev/Spring/Assets/Sounds/pitch_sweep_wasapi.wav" };
    //RenderAudioData(sound);

    while (!m_ExitWindow)
    {
        m_pUILayer.BeginFrame();
        m_pActiveRenderer->BeginFrame();

        m_pUILayer.Enable();
 
        DrawLobby(m_AssetManager);
        DrawSceneCharacters(m_AssetManager);
        m_AppWindow.OnUpdate();
        m_Scene->OnUpdate(m_AppWindow.m_Ts);
        m_pActiveRenderer->OnUpdate(m_AppWindow.m_Ts);
        m_pUILayer.OnUpdate(m_AppWindow.m_Ts);

        m_pActiveRenderer->EndFrame();
        m_pUILayer.EndFrame();

        m_AppWindow.SwapBuffer();
    }
    ShutDown();
}

void BaseApplication::ShutDown()
{
}

void BaseApplication::OnMouseMove(event::MouseMoveEvent& event)
{
}

void BaseApplication::DrawLobby(AssetManager& asset_manager)
{
    std::vector<RenderCommand> m_CommandQueue;
    VertexBuffer m_GeometryBuffer;
    AssetHandle fakeShaderHandle{ 0,1 };
    auto& mesh = asset_manager.GetAsset<primitives::Mesh>(m_LobbyMeshHandle);
    {
        RenderCommand cmd;
        m_GeometryBuffer.UpdateBuffer(mesh.m_V);
        uint64_t enableBits{ 0 };
        enableBits |= (1ULL << 0); // Enable CULL_FACE
        enableBits |= (1ULL << 4); // Enable DEPTH_TEST
        cmd.m_EnableBits = enableBits;
        cmd.m_ShaderHandle = fakeShaderHandle;
        cmd.m_MaterialHandle = m_LobbyMeshHandle;
        cmd.m_VertexBufferOffset = m_GeometryBuffer.m_BufferOffset;
        cmd.m_IndexBufferOffset = m_GeometryBuffer.m_BufferElementCount;
        cmd.m_CommandSize = static_cast<uint64_t>(sizeof(Vertex) * mesh.m_V.size());
        cmd.m_IndexCount = static_cast<uint64_t>(mesh.m_V.size());
        cmd.m_PrimitiveType = GL_TRIANGLES;
        cmd.m_Viewport[0] = 0;
        cmd.m_Viewport[1] = 0;
        cmd.m_Viewport[2] = 1920;
        cmd.m_Viewport[3] = 1080;
        cmd.m_UniformBuffer.m_Mat4Map["Model"] = glm::mat4(1.0f);
        cmd.m_UniformBuffer.m_Mat4Map["View"] = m_pUILayer.m_pActiveCamera->GetV();
        cmd.m_UniformBuffer.m_Mat4Map["Projection"] = m_pUILayer.m_pActiveCamera->GetP();
        m_CommandQueue.push_back(cmd);
    }
    m_pActiveRenderer->DrawBuffer(m_CommandQueue, m_GeometryBuffer, asset_manager);
}

void BaseApplication::DrawSceneCharacters(AssetManager& asset_manager)
{
    std::vector<RenderCommand> m_CommandQueue;
    VertexBuffer m_GeometryBuffer;
    AssetHandle fakeShaderHandle{ 0,1 };
    auto meshView = m_Scene->GetView<physics::PhysicsState, primitives::MeshInstance, primitives::RenderComponent>();
    for (auto [entity, physicsComponent, meshInstance, renderComponent] : meshView.each())
    {

        RenderCommand cmd;
        auto& mesh = m_AssetManager.GetMesh(meshInstance.m_Handle);
        auto& material = m_AssetManager.GetMaterial(meshInstance.m_Handle);
        m_GeometryBuffer.UpdateBuffer(mesh.m_V);
        uint64_t enableBits{ 0 };
        enableBits |= (1ULL << 0); // Enable CULL_FACE
        enableBits |= (1ULL << 4); // Enable DEPTH_TEST
        cmd.m_EnableBits = enableBits;
        cmd.m_EntityID = static_cast<uint64_t>(entity);
        cmd.m_ShaderHandle = fakeShaderHandle;
        cmd.m_MaterialHandle = meshInstance.m_Handle;
        cmd.m_VertexBufferOffset = m_GeometryBuffer.m_BufferOffset;
        cmd.m_IndexBufferOffset = m_GeometryBuffer.m_BufferElementCount;
        cmd.m_CommandSize = static_cast<uint64_t>(sizeof(Vertex) * mesh.m_V.size());
        cmd.m_IndexCount = static_cast<uint64_t>(mesh.m_V.size());
        cmd.m_PrimitiveType = GL_TRIANGLES;
        cmd.m_Viewport[0] = 0;
        cmd.m_Viewport[1] = 0;
        cmd.m_Viewport[2] = 1920;
        cmd.m_Viewport[3] = 1080;
        cmd.m_UniformBuffer.m_Mat4Map["Model"] = glm::translate(glm::mat4(1.0f), physicsComponent.position) * glm::mat4_cast(physicsComponent.orientation);
        cmd.m_UniformBuffer.m_Mat4Map["View"] = m_pUILayer.m_pActiveCamera->GetV();
        cmd.m_UniformBuffer.m_Mat4Map["Projection"] = m_pUILayer.m_pActiveCamera->GetP();
        m_CommandQueue.push_back(cmd);
    }

    m_pActiveRenderer->DrawBuffer(m_CommandQueue, m_GeometryBuffer, asset_manager);
}

BaseApplication::BaseApplication(uint64_t width, uint64_t height, const char* title)
    :
    m_AppWindow{width, height, title},
    m_ExitWindow{false},
    m_Scene{std::make_shared<Scene>("C:/dev/Spring/Assets/Projects/Lobby.lua")},
    m_pUILayer{std::shared_ptr<Win32Window>(&m_AppWindow)},
    m_AssetManager{}
{
    m_AppWindow.OnEvent = &event::Dispatcher::Dispatch;
    event::Dispatcher::RegisterListener(*this);
    event::Dispatcher::RegisterListener(m_pUILayer);

}
