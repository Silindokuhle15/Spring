#include "BaseApplication.h"

void BaseApplication::DrawMenuBarPanel()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "..."))
            {
                if (ImGui::IsItemClicked())
                {
                    char buffer[512];
                    ImGui::InputText("File Name", buffer, sizeof(char) * 512);
                    std::string file_name(buffer);
                    Project NewProject(file_name);

                }
            }
            if (ImGui::MenuItem("Open", "..."))
            {
                const char* filter = ".lua";
                std::string sceneName = GetFileName(filter);
                if (m_Scene)
                {
                    m_Scene.reset();
                }
                m_Scene = std::make_shared<Scene>(sceneName);
                m_Scene->SetAssetManager(&m_AssetManager);
                m_Scene->LoadSceneFromFile();
                if (!m_Scene->asset_pack.empty())
                {
                    m_AssetManager.Deserialize(m_Scene->asset_pack);
                }
                m_Scene->OnInit();
                m_Scene->OnCreateSceneObjects();
                m_pUILayer.LoadScene(m_Scene);
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Close", "..."))
            {
            }
            if (ImGui::MenuItem("Close Project", "..."))
            {
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Save Selected", "crtl+s"))
            {
            }
            if (ImGui::MenuItem("Save All", "crtl+shift+s"))
            {
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Recent Files", "..."))
            {
            }
            if (ImGui::MenuItem("Recent Projects", "..."))
            {
            }
            ImGui::Separator();
            if (ImGui::BeginMenu("Import"))
            {
                if (ImGui::MenuItem("Image (.png)"))
                {
                    const char* filter = ".png";
                    std::string path = GetFileName(filter);
                    //parent->LoadImageFromFile(path, parent->m_ActiveRenderer->m_pTextureBuffer);
                    //parent->m_ActiveRenderer->CreateOpenGLTexture(_TextureView & view, _TextureDescription & desc, GL_Texture & tex)

                }
                if (ImGui::MenuItem("Wavefront (.obj)"))
                {
                    const char* filter = ".obj";
                    std::string obj_path = GetFileName(filter);
                    AssetResource meshResource{ AssetType::MeshResource, obj_path };
                    physics::PhysicsState ps{};
                    ps.orientation = glm::quat(0, 0, 0, 1);
                    ps.position = glm::vec3(0, 0, -2);
                    ps.mass = 10;

                    auto assetHandle = m_AssetManager.GetResourceHandle(meshResource);
                    auto& mesh = m_AssetManager.GetAsset<primitives::Mesh>(assetHandle);
                    auto character = m_Scene->CreateSceneObject();
                    character->AddComponent<primitives::MeshInstance>(primitives::MeshInstance{ assetHandle });
                    character->AddComponent<primitives::RenderComponent>(primitives::RenderComponent{0,0});
                    character->AddComponent<physics::PhysicsState>(ps);
                }

                if (ImGui::MenuItem("MeshPack"))
                {
                    const char* filter = ".pak";
                    std::string path = GetFileName(filter);
                    auto result = m_AssetManager.DeserializeMeshPack(path);


                }
                if (ImGui::MenuItem("MaterialPack"))
                {

                }
                if (ImGui::MenuItem("TexturePack"))
                {
                    const char* filter = ".pak";
                    std::string path = GetFileName(filter);

                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Export"))
            {
                if (ImGui::MenuItem("MeshPack"))
                {

                }
                if (ImGui::MenuItem("MaterialPack"))
                {

                }
                ImGui::EndMenu();
            }
            ImGui::Separator();

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Project"))
        {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Asset Manager"))
        {
            showAssetManagerWindow = true;
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    if (showAssetManagerWindow)
    {
        ImGui::Begin("Asset Manager");
        {
            if (ImGui::BeginTabBar("Asset Manager"))
            {
                if (ImGui::BeginTabItem("Shaders"))
                {
                    auto& shaderMap = m_AssetManager.m_ShaderMap;
                    static uint32_t selectedShader = 0;
                    for (auto& p : shaderMap)
                    {
                        auto& handle = p.first;
                        auto& shader = p.second;
                        auto platformHandle = shader.GetHandle();
                        ImGui::PushID(platformHandle);
                        char buffer[128] = "";
                        sprintf(buffer, "%llu%llu", handle.m_HWORD, handle.m_LWORD);
                        if (ImGui::Selectable(buffer), selectedShader == platformHandle)
                        {

                        }
                        ImGui::PopID();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Materials"))
                {
                    
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Textures"))
                {
                    auto& textureMap = m_AssetManager.m_TextureMap;
                    static uint32_t selectedTexture = 0;
                    for (auto& t : textureMap)
                    {
                        auto& handle = t.first;
                        auto& texture = t.second;
                        auto platformHandle = texture.m_Texture;
                        ImGui::PushID(platformHandle);
                        char buffer[128] = "";
                        sprintf(buffer, "%llu%llu", handle.m_HWORD, handle.m_LWORD);
                        if (ImGui::Selectable(buffer), selectedTexture == platformHandle)
                        {

                        }
                        ImGui::PopID();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Meshes"))
                {
                    auto& meshMap = m_AssetManager.m_MeshMap;
                    static uint32_t selectedMesh = 0;
                    uint32_t index = 0;
                    for (auto& m : meshMap)
                    {
                        auto& handle = m.first;
                        auto& mesh = m.second;
                        ImGui::PushID(index);
                        char buffer[128] = "";
                        sprintf(buffer, "%llu%llu", handle.m_HWORD, handle.m_LWORD);
                        if (ImGui::Selectable(buffer), selectedMesh == index)
                        {

                        }
                        ImGui::PopID();
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Scripts"))
                {
                    auto& scriptMap = m_AssetManager.m_ScriptMap;
                    static uint32_t selectedScript = 0;
                    uint32_t index = 0;
                    for (auto& s : scriptMap)
                    {
                        auto& handle = s.first;
                        auto& scriptData = s.second;
                        ImGui::PushID(index);
                        char buffer[128] = "";
                        sprintf(buffer, "%llu%llu", handle.m_HWORD, handle.m_LWORD);
                        if (ImGui::Selectable(buffer), selectedScript == index)
                        {

                        }
                        ImGui::PopID();
                    }
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }
        ImGui::End();
    }
}

void BaseApplication::Run()
{
    m_AssetManager.DeserializeTexturePack("C:/dev/Astron Battles/Assets/LevelOne_asset_TXTR.pak");
    m_pActiveRenderer = std::make_shared<Renderer>();

    m_pUILayer.BindRenderer(m_pActiveRenderer);
    m_pUILayer.m_ActiveRenderer->SetUpForRendering();
    m_pUILayer.m_pActiveCamera->SetEye(glm::vec3(0, 0, 20));
    m_pUILayer.m_pActiveCamera->SetCenter(glm::vec3(0, 0, 0));
    m_pUILayer.m_pActiveCamera->SetOrientation(glm::quat(1, 0, 0, 0));
    m_pUILayer.m_pActiveCamera->OnInit();

    //scripting::ScriptMgr::expose_scene_camera(m_Scene->GetLuaState(), m_pUILayer.m_pActiveCamera.get(), "camera");
    m_LobbyGraphicsShaderHandle = AssetHandle{ 3531024263087085773, 10156511931093447336 }; //LevelOne
    m_ParticleGraphicsShaderHandle = AssetHandle{ 10104892876724591594, 13049350730236798254 };
    
    while (!m_ExitWindow)
    {
        m_pUILayer.BeginFrame();
        m_pActiveRenderer->BeginFrame();

        m_pUILayer.Enable();

        DrawMenuBarPanel();
        OnUpdate();
        if (m_Scene)
        {
            DrawSceneCharacters(m_AssetManager);
            DrawParticleSystems(m_AssetManager);
        }

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
    if(m_Scene)
    {
        m_pActiveRenderer->ParticleSystemSync(m_Scene.get());
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

void BaseApplication::OnPlaySound(primitives::PlaySoundRequest& sound_event)
{
    auto& assetHandle = sound_event.SoundID;
    auto& sound = m_AssetManager.GetSound(assetHandle);
    sound.playing = true;
    RenderAudioData(sound);
}

void BaseApplication::DrawSceneCharacters(AssetManager& asset_manager)
{
    std::vector<RenderCommand> commandQueue;
    VertexBuffer geometryBuffer;
    auto meshView = m_Scene->GetView<physics::PhysicsState, primitives::MeshInstance>();
    for (auto [entity, physicsComponent, meshInstance] : meshView.each())
    {
        RenderCommand cmd;
        auto& mesh = m_AssetManager.GetMesh(meshInstance.m_Handle);
        auto& material = m_AssetManager.GetMaterial(meshInstance.m_Handle);
        geometryBuffer.UpdateBuffer(mesh.m_V);
        uint64_t enableBits{ 0 };
        enableBits |= (1ULL << 0); // Enable CULL_FACE
        enableBits |= (1ULL << 4); // Enable DEPTH_TEST
        cmd.m_EnableBits = enableBits;
        cmd.m_EntityID = static_cast<uint64_t>(entity);
        cmd.m_ShaderHandle = m_LobbyGraphicsShaderHandle;
        cmd.m_MaterialHandle = meshInstance.m_Handle;
        cmd.m_VertexBufferOffset = geometryBuffer.m_BufferOffset;
        cmd.m_IndexBufferOffset = geometryBuffer.m_BufferElementCount;
        cmd.m_CommandSize = static_cast<uint64_t>(sizeof(primitives::Vertex) * mesh.m_V.size());
        cmd.m_IndexCount = static_cast<uint64_t>(mesh.m_V.size());
        cmd.m_PrimitiveType = GL_TRIANGLES;
        cmd.m_Viewport[0] = 0;
        cmd.m_Viewport[1] = 0;
        cmd.m_Viewport[2] = 1920;
        cmd.m_Viewport[3] = 1080;
        cmd.m_UniformBuffer.m_Mat4Map["Model"] = glm::translate(glm::mat4(1.0f), physicsComponent.position) * glm::mat4_cast(physicsComponent.orientation);
        cmd.m_UniformBuffer.m_Mat4Map["View"] = m_pUILayer.m_pActiveCamera->GetV();
        cmd.m_UniformBuffer.m_Mat4Map["Projection"] = m_pUILayer.m_pActiveCamera->GetP();
        commandQueue.push_back(cmd);
    }

    m_pActiveRenderer->DrawBuffer(commandQueue, geometryBuffer, asset_manager);
}

void BaseApplication::DrawParticleSystems(AssetManager& asset_manager)
{
    std::vector<ParticleCommand> commandQueue;
    auto view = m_pUILayer.m_pActiveCamera->GetV();
    auto proj = m_pUILayer.m_pActiveCamera->GetP();
    auto particleView = m_Scene->GetView<primitives::ParticleSystem>();
    for (auto [entity, particleSystem] : particleView.each())
    {
        ParticleCommand cmd;
        if (particleSystem.m_EmitterInfo.m_Flags.m_IsEnabled)
        {
            cmd.m_BufferOffset = particleSystem.m_BufferOffset;
            cmd.m_NumParticles = particleSystem.m_NumParticles;
            cmd.m_ShaderHandle = m_ParticleGraphicsShaderHandle;
            cmd.m_UniformBuffer.m_FloatMap["bufferOffset"] = static_cast<float>(particleSystem.m_BufferOffset);
            cmd.m_UniformBuffer.m_Mat4Map["View"] = view;
            cmd.m_UniformBuffer.m_Mat4Map["Proj"] = proj;
            commandQueue.push_back(cmd);
        }
    }
    m_pActiveRenderer->DrawParticles(commandQueue, asset_manager);
}

BaseApplication::BaseApplication(uint64_t width, uint64_t height, const char* title)
    :
    m_AppWindow{width, height, title},
    m_ExitWindow{false},
    //m_Scene{std::make_shared<Scene>("C:/dev/Spring/Assets/Projects/Lobby.lua")},
    //m_Scene{std::make_shared<Scene>("C:/dev/Spring/Assets/Projects/LevelOne.lua")},
    //m_Scene{std::make_shared<Scene>("C:/dev/Astron Battles/Assets/Scripts/LevelOne.lua")},
    m_Scene{},
    m_pUILayer{std::shared_ptr<Win32Window>(&m_AppWindow)},
    m_AssetManager{}
{
    m_AppWindow.OnEvent = &event::Dispatcher::Dispatch;
    event::Dispatcher::RegisterListener(*this);
    event::Dispatcher::RegisterListener(m_pUILayer);

}
