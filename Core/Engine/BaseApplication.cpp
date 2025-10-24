#include "BaseApplication.h"

void BaseApplication::Run()
{
    AssetManager assetManager;
    m_Scene->SetAssetManager(&assetManager);
    m_Scene->LoadSceneFromFile();
    m_Scene->OnInit();
    m_Scene->OnCreateSceneObjects();
    m_pUILayer.LoadScene(m_Scene);
    m_pUILayer.OnInit();
    
    m_pActiveRenderer = std::make_shared<Renderer>();
    m_pActiveRenderer->BindScene(m_Scene);

    m_pUILayer.BindRenderer(m_pActiveRenderer);
    m_pUILayer.m_ActiveRenderer->SetUpForRendering();
    m_pUILayer.m_pActiveCamera->SetEye(glm::vec3(0, 2, -2));
    m_pUILayer.m_pActiveCamera->SetCenter(glm::vec3(0, 0, 2));
    m_pUILayer.m_pActiveCamera->SetOrientation(glm::quat(0, -1, 0, 0));
    m_pUILayer.m_pActiveCamera->OnInit();

    scripting::ScriptMgr::expose_scene_camera(m_Scene->GetLuaState(), m_pUILayer.m_pActiveCamera.get(), "camera");

    while (!m_ExitWindow)
    {
        m_pUILayer.BeginFrame();
        m_pActiveRenderer->BeginFrame();

        m_pUILayer.Enable();
 
        m_pActiveRenderer->ClearVertexBuffer();
        m_pActiveRenderer->Clear();
        m_pActiveRenderer->LoadMeshRenderData();
        m_pActiveRenderer->UploadToOpenGL();
        DrawSceneCharacters();
        m_pActiveRenderer->Flush();
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

void BaseApplication::DrawSceneCharacters()
{
    AssetHandle fakeShaderHandle{ 0,0 };
    auto meshView = m_Scene->GetView<physics::PhysicsState, MeshInstance, RenderComponent>();
    for (auto [entity, phyzx, meshInstance, renderComponent] : meshView.each())
    {
        RenderCommand cmd;
        auto& mesh = m_Scene->GetAssetManager()->GetMesh(meshInstance.m_Handle);
        auto& material = m_Scene->GetAssetManager()->GetMaterial(meshInstance.m_Handle);
        cmd.m_EntityID = static_cast<uint64_t>(entity);
        cmd.m_ShaderHandle = fakeShaderHandle;
        cmd.m_MaterialHandle = meshInstance.m_Handle;
        cmd.m_VertexBufferOffset = renderComponent.m_VertexBufferOffset;
        cmd.m_IndexBufferOffset = renderComponent.m_IndexBufferOffset;
        cmd.m_CommandSize = renderComponent.m_Size;
        cmd.m_IndexCount = renderComponent.m_IndexCount;
        cmd.m_PrimitiveType = GL_TRIANGLES;
        cmd.m_UniformBuffer.m_Mat4Map["Model"] = glm::translate(glm::mat4(1.0f), phyzx.position) * glm::mat4_cast(phyzx.orientation);
        cmd.m_UniformBuffer.m_Mat4Map["View"] = m_pUILayer.m_pActiveCamera->GetV();
        cmd.m_UniformBuffer.m_Mat4Map["Projection"] = m_pUILayer.m_pActiveCamera->GetP();
        m_pActiveRenderer->Draw(cmd);
    }
}

BaseApplication::BaseApplication(uint64_t width, uint64_t height, const char* title)
    :
    m_AppWindow{width, height, title},
    m_ExitWindow{false},
    m_Scene{std::make_shared<Scene>("C:/dev/Spring/Assets/Projects/Lobby.lua")},
    m_pUILayer{std::shared_ptr<Win32Window>(&m_AppWindow)}
{
    m_AppWindow.OnEvent = &event::Dispatcher::Dispatch;
    event::Dispatcher::RegisterListener(*this);
    event::Dispatcher::RegisterListener(m_pUILayer);
}
