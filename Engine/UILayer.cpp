#include "UILayer.h"

int gizmoCount = 1;
static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

UILayer::UILayer(GLFWwindow* win, const char* gl_ver)
{
    //start the Imgui code here
    //Initialization Code

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); // Enable ImGui IO
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init(gl_ver);
    // Dark Theme lemme see 
    ImGui::StyleColorsDark();
    //m_ActiveScene = 2;
}


UILayer::~UILayer()
{
    //ImGui Clean Up Code
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void UILayer::Enable()
{

    glGetIntegerv(GL_CURRENT_PROGRAM, &m_ActiveMaterial); // m_CurrentProgram shoubd be the currently bound Material ID
    glGetProgramiv(m_ActiveMaterial, GL_ACTIVE_UNIFORMS, &m_ActiveUniforms);

    ImGui::BeginMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        //ImGui::MenuItem("New");
        if (ImGui::MenuItem("New"))
        {
            BaseApplication::CreateNewScene();

        }
        if (ImGui::MenuItem("Open"))
        {

        }
        if (ImGui::MenuItem("Save As"))
        {

        }
        ImGui::EndMenu();

    }

    ImGui::Begin("Stats And Performance");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Separator();
    ImGui::Text("Active Render Mode");
    ImGui::Checkbox("Wire Frame", &m_RenderMode);
    ImGui::Separator();
    ImGui::Text("Draw Calls : %d", 1);

    ImGui::Text("Number of Indices : %d", m_NumIndices);
    ImGui::Text("Number of Samples : %d", m_Samples);
    ImGui::Separator();
    //ImGui::SliderInt("Scene Number ", &, 0, 4);
    ImGui::End();

    ImGui::Begin("Animation panel");
    ImGui::Button("Play", ImVec2(50.f, 30.0f));
    ImGui::End();

    ImGui::Begin("Utilities");
    ImGui::Separator();
    ImGui::Checkbox("Enable ImGuizmo", &m_EnableImGuizmo);
    ImGuizmo::Enable(m_EnableImGuizmo);
    ImGui::Text("Viewing and Transformation");
    ImGui::Separator();

    ImGui::Text("Number Of Cameras : %d", m_NumberOfCamera);
    //ImGui::SliderFloat3("Camera Position", m_CameraPosition, -50.0, 50.0);
    ImGui::SliderFloat3("Camera Position", (float*)glm::value_ptr(m_EditorCamera->GetPosition()), -50.0, 50.0);
    ImGui::Separator();

    ImGui::Text("Lights and Shadow");

    int enable_lighting_locatin = glGetUniformLocation(m_ActiveMaterial, "EnableTexture");
    glProgramUniform1i(m_ActiveMaterial, enable_lighting_locatin, m_EnableLighting);

    ImGui::Checkbox("Enable Lighting", &m_EnableLighting);
    ImGui::Text("HemiSphere Lighting Model");
    ImGui::SliderFloat3("Sky Color", m_SkyColor, 0.0, 1.0);
    ImGui::SliderFloat3("Grounr Color", m_GroundColor, 0.0, 1.0);
    int light_location = glGetUniformLocation(m_ActiveMaterial, "LightPosition");
    int light_color_location = glGetUniformLocation(m_ActiveMaterial, "LightColor");
    int sky_color_location = glGetUniformLocation(m_ActiveMaterial, "SkyColor");
    int ground_color_location = glGetUniformLocation(m_ActiveMaterial, "GroundColor");
    int factor_location = glGetUniformLocation(m_ActiveMaterial, "factor");

    glProgramUniform3fv(m_ActiveMaterial, light_location, 1, m_PointLightPosition);
    glProgramUniform3fv(m_ActiveMaterial, light_color_location, 1, m_LightColor);
    glProgramUniform3fv(m_ActiveMaterial, sky_color_location, 1, m_SkyColor);
    glProgramUniform3fv(m_ActiveMaterial, ground_color_location, 1, m_GroundColor);
    glProgramUniform3fv(m_ActiveMaterial, factor_location, 1, &m_Factor);

    ImGui::SliderFloat3("Light Position", m_PointLightPosition, -50.0, 50.0);
    ImGui::SliderFloat3("Light Color", m_LightColor, 0.0, 1.0);
    ImGui::SliderFloat("Factor [a]", &m_Factor, 0.0, 1.0);
    ImGui::Separator();

    //light_pos[0] = BaseApplication::m_PointLight->GetPosition().x;
    //light_pos[1] = BaseApplication::m_PointLight->GetPosition().y;
    //light_pos[2] = BaseApplication::m_PointLight->GetPosition().z;
    //ImGui::SliderFloat3("Light Position", light_pos, 0.0, 100.);

    ImGui::Text("Materials");
    ImGui::Separator();

    ImGui::Text("Active Material : %d", m_ActiveMaterial);

    int enable_texture_location = glGetUniformLocation(m_ActiveMaterial, "EnableTexture");

    glProgramUniform1i(m_ActiveMaterial, enable_texture_location, m_EnableTexture);

    ImGui::Checkbox("Apply Texture", &m_EnableTexture);
    int attached_shaders = 0;
    ImGui::Text("Attached Shaders : % d", attached_shaders);
    int tex_slot = 0;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &tex_slot);
    ImGui::Text("Active Texture Slot : %d", 0);
    ImGui::Text("Number of Active Uniforms : %d", m_ActiveUniforms);

    int width = 0, height = 0, size = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    //glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_BUFFER_SIZE, &size);
    ImGui::Text("Active Texture Dims : [%d , %d]", width, height);
    //ImGui::Text("Active Texture Size : [ %d ]", size);


    ImGui::Checkbox("use Gizmo Window", &m_UseGizmoWindow);

    ImGui::End();
}

void UILayer::OnInit()
{
    // TO do Add some code here bro
    //m_ActiveScene = 1;

    glGenQueries(1, &m_Query);

}

void UILayer::BeginFrame()
{
    // ImGui BeginFrame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    ImGuizmo::SetOrthographic(false);
    ImGuizmo::BeginFrame(); // ImGuizmo Begin Frame

    // Begin the Query the Samples Renders
    //glBeginQuery(GL_SAMPLES_PASSED, m_Query);

    EditTransform((float*)glm::value_ptr(m_EditorCamera->GetV()), (float*)glm::value_ptr(m_EditorCamera->GetP()), (float*)glm::value_ptr(*m_ActiveTransform), true);

}

void UILayer::EndFrame()
{
    // ImGui EndFrame
    // Render ImGui stuff


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    int count = 10;
    //while (!m_ResultAvailable)
    //{
        //glGetQueryObjectiv(m_Query, GL_QUERY_RESULT_AVAILABLE, &m_ResultAvailable);
    //}
    /*
    if (m_ResultAvailable)
    {
        glGetQueryObjectiv(m_Query, GL_QUERY_RESULT, &m_Samples);
    }
    */
}

void UILayer::OnUpdate(float ts)
{
    m_Delta = ts;
    //Update the Editor camera delta right here
    m_EditorCamera->OnUpdate(m_Delta);

    //glEndQuery(GL_SAMPLES_PASSED);
 }

void UILayer::EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
{
    static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
    static bool useSnap = false;
    static float snap[3] = { 1.f, 1.f, 1.f };
    static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
    static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
    static bool boundSizing = false;
    static bool boundSizingSnap = false;

    if (editTransformDecomposition)
    {
        if (ImGui::IsKeyPressed(ImGuiKey_T))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        if (ImGui::IsKeyPressed(ImGuiKey_E))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
            mCurrentGizmoOperation = ImGuizmo::SCALE;
        if (ImGui::RadioButton("Universal", mCurrentGizmoOperation == ImGuizmo::UNIVERSAL))
            mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;
        float matrixTranslation[3], matrixRotation[3], matrixScale[3];
        ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
        ImGui::InputFloat3("Tr", matrixTranslation);
        ImGui::InputFloat3("Rt", matrixRotation);
        ImGui::InputFloat3("Sc", matrixScale);
        ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

        if (mCurrentGizmoOperation != ImGuizmo::SCALE)
        {
            if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
                mCurrentGizmoMode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
                mCurrentGizmoMode = ImGuizmo::WORLD;
        }
        if (ImGui::IsKeyPressed(ImGuiKey_S))
            useSnap = !useSnap;
        ImGui::Checkbox("##UseSnap", &useSnap);
        ImGui::SameLine();

        switch (mCurrentGizmoOperation)
        {
        case ImGuizmo::TRANSLATE:
            ImGui::InputFloat3("Snap", &snap[0]);
            break;
        case ImGuizmo::ROTATE:
            ImGui::InputFloat("Angle Snap", &snap[0]);
            break;
        case ImGuizmo::SCALE:
            ImGui::InputFloat("Scale Snap", &snap[0]);
            break;
        }
        ImGui::Checkbox("Bound Sizing", &boundSizing);
        if (boundSizing)
        {
            ImGui::PushID(3);
            ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
            ImGui::SameLine();
            ImGui::InputFloat3("Snap", boundsSnap);
            ImGui::PopID();
        }
    }


    ImGuiIO& io = ImGui::GetIO();
    float viewManipulateRight = io.DisplaySize.x;
    float viewManipulateTop = 0;
    static ImGuiWindowFlags gizmoWindowFlags = 0;
    if (m_UseGizmoWindow)
    {
        ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Appearing);
        ImGui::SetNextWindowPos(ImVec2(400, 20), ImGuiCond_Appearing);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
        ImGui::Begin("Gizmo", 0, gizmoWindowFlags);
        ImGuizmo::SetDrawlist();
        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
        viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
        viewManipulateTop = ImGui::GetWindowPos().y;
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;
    }
    else
    {
        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    }

    //ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
    //ImGuizmo::DrawCubes(cameraView, cameraProjection, &objectMatrix[0][0], gizmoCount);
    //ImGuizmo::DrawCubes(cameraView, cameraProjection, (float*)glm::value_ptr(*m_ActiveTransform), gizmoCount);
    ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

    glm::vec3 displacement = m_EditorCamera->GetPosition() - glm::vec3(0);
    float distance = glm::dot(displacement, displacement);
    distance = glm::sqrt(distance);

    ImGuizmo::ViewManipulate(cameraView, distance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

    if (m_UseGizmoWindow)
    {
        ImGui::End();
        ImGui::PopStyleColor(1);
    }
}


void UILayer::LoadScene(std::shared_ptr<Scene> scene)
{
    m_ActiveScene = std::shared_ptr<Scene>(scene);
    m_ActiveTransform = std::shared_ptr<glm::mat4>(&scene->m_Objects[0]->m_Transform);
    m_EditorCamera = std::shared_ptr<PerspectiveCamera>(scene->m_ActiveCamera);
}
