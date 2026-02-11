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
    m_pUILayer.m_pActiveCamera->SetEye(glm::vec3(0, 0, 20));
    m_pUILayer.m_pActiveCamera->SetCenter(glm::vec3(0, 0, 0));
    m_pUILayer.m_pActiveCamera->SetOrientation(glm::quat(0, 0, 0, -1));
    m_pUILayer.m_pActiveCamera->OnInit();

    scripting::ScriptMgr::expose_scene_camera(m_Scene->GetLuaState(), m_pUILayer.m_pActiveCamera.get(), "camera");

    std::string combinedShaderPaths{ m_Scene->shader_paths[0] + "\n" + m_Scene->shader_paths[1]};

    m_LobbyGraphicsShaderHandle = m_AssetManager.GetResourceHandle({ AssetType::GraphicsShaderResource, combinedShaderPaths.c_str() });
    m_LobbyMeshHandle = m_AssetManager.GetResourceHandle({ AssetType::MeshResource,"C:/dev/Spring/Assets/Objects/House/Room.obj" });
    std::ifstream sound{ "C:/dev/Spring/Assets/Sounds/pitch_sweep_wasapi.wav", std::ios::binary };
    WAVHEADER header{};    
    LoadWAVFromDisk(sound, header);
    //RenderAudioData(header);
    //ReleaseAudioBuffer();
    std::string combinedParticleGraphicsShaderPaths{ "C:/dev/Rocket Launch Simulator/assets/Shaders/VertexShader.glsl\nC:/dev/Rocket Launch Simulator/assets/Shaders/fragShader.glsl" };
    AssetResource particleShaderGraphicsResource{ AssetType::GraphicsShaderResource, combinedParticleGraphicsShaderPaths };
    m_ParticleGraphicsShaderHandle = m_AssetManager.GetResourceHandle(particleShaderGraphicsResource);

    while (!m_ExitWindow)
    {
        m_pUILayer.BeginFrame();
        m_pActiveRenderer->BeginFrame();

        m_pUILayer.Enable();
        OnUpdate();
        //DrawLobby(m_AssetManager);
        DrawSceneCharacters(m_AssetManager);
        DrawParticleSystems(m_AssetManager);

        m_pActiveRenderer->OnUpdate(1.0f / 6.0f);
        m_pUILayer.OnUpdate(0.0f / 6.0f);

        m_pActiveRenderer->EndFrame();
        m_pUILayer.EndFrame();

        m_AppWindow.SwapBuffer();
    }
    ShutDown();
}

void BaseApplication::OnUpdate()
{
    m_AppWindow.OnUpdate();

    if (m_Scene != nullptr)
    {
        m_Scene->OnUpdate(0.01667f);

        for (auto& particleSystem : m_Scene->m_ParticleSystems)
        {
            if (particleSystem.m_IsEnabled)
            {
                GLuint numPts = particleSystem.m_NumParticles;
                auto& shader = m_AssetManager.GetAsset<Shader>(particleSystem.m_ShaderHandle);
                shader.Bind();
                auto platformHandle = shader.GetHandle();

                glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSystem.PositionBuffer);
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, particleSystem.PositionBuffer);

                glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSystem.VelocityBuffer);
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, particleSystem.VelocityBuffer);

                GLuint deltaLoc = glGetUniformLocation(platformHandle, "delta");
                GLuint uTimeLoc = glGetUniformLocation(platformHandle, "uTime");
                GLuint numPtsLoc = glGetUniformLocation(platformHandle, "numParticles");
                glUniform1f(deltaLoc, particleSystem.m_Ts);
                glUniform1f(uTimeLoc, particleSystem.m_AccumulatedTime);
                glUniform1ui(numPtsLoc, numPts);
                GLuint nx = 32;
                GLuint ny = 32;
                GLuint nz = 16;
                glDispatchCompute(nx, ny, nz);
                glMemoryBarrier(GL_ALL_BARRIER_BITS);
                particleSystem.m_Ts = 1.0f / 60.0f;
                particleSystem.m_AccumulatedTime += particleSystem.m_Ts;
            }
        }
    }
}
void BaseApplication::ShutDown()
{
}

void BaseApplication::OnMouseMove(event::MouseMoveEvent& event)
{
}

void BaseApplication::OnKeyPress(event::KeyPressEvent& key_press)
{
}

void BaseApplication::DrawLobby(AssetManager& asset_manager)
{
    std::vector<RenderCommand> m_CommandQueue;
    VertexBuffer m_GeometryBuffer;
    auto& mesh = asset_manager.GetAsset<primitives::Mesh>(m_LobbyMeshHandle);
    {
        RenderCommand cmd;
        m_GeometryBuffer.UpdateBuffer(mesh.m_V);
        uint64_t enableBits{ 0 };
        enableBits |= (1ULL << 0); // Enable CULL_FACE
        enableBits |= (1ULL << 4); // Enable DEPTH_TEST
        cmd.m_EnableBits = enableBits;
        cmd.m_ShaderHandle = m_LobbyGraphicsShaderHandle;
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
    auto meshView = m_Scene->GetView<physics::PhysicsState, primitives::MeshInstance>();
    for (auto [entity, physicsComponent, meshInstance] : meshView.each())
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
        cmd.m_ShaderHandle = m_LobbyGraphicsShaderHandle;
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

void BaseApplication::DrawParticleSystems(AssetManager& asset_manager)
{
    glEnable(GL_PROGRAM_POINT_SIZE);
    for (auto& particleSystem : m_Scene->m_ParticleSystems)
    {
        if (particleSystem.m_IsEnabled)
        {
            auto numPts = particleSystem.m_NumParticles;
            auto& graphics_shader = asset_manager.GetAsset<Shader>(m_ParticleGraphicsShaderHandle);
            graphics_shader.Bind();
            auto graphicsHandle = graphics_shader.GetHandle();
            GLuint viewLoc = glGetUniformLocation(graphicsHandle, "View");
            GLuint projLoc = glGetUniformLocation(graphicsHandle, "Proj");

            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_pUILayer.m_pActiveCamera->GetV()));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_pUILayer.m_pActiveCamera->GetP()));

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSystem.PositionBuffer);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, particleSystem.PositionBuffer);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSystem.VelocityBuffer);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, particleSystem.VelocityBuffer);

            //glBindTextureUnit(0, texture.m_Texture);
            glDrawArraysInstanced(GL_POINTS, 0, 1, numPts);
        }
    }
    glDisable(GL_PROGRAM_POINT_SIZE);
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
