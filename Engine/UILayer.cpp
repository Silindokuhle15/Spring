#include "UILayer.h"


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
    m_ActiveScene = 2;
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
    ImGui::SliderInt("Scene Number ", &m_ActiveScene, 0, 4);
    ImGui::End();

    ImGui::Begin("Animation panel");
    ImGui::Button("Play", ImVec2(30.f, 30.0f));
    ImGui::End();

    ImGui::Begin("Utilities");
    ImGui::Separator();
    ImGui::Text("Viewing and Transformation");
    ImGui::Separator();

    ImGui::Text("Number Of Cameras : %d", m_NumberOfCamera);
    ImGui::SliderFloat3("Camera Position", m_CameraPosition, -50.0, 50.0);
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
    ImGui::End();
}

void UILayer::OnInit()
{
    // TO do Add some code here bro

    /*
    PerspectiveCamera pCam;
    pCam.m_center = glm::vec3(0.0f);
    pCam.m_eye = glm::vec3(0.0f, 0.0, 5.0);
    pCam.m_up = glm::vec3(0.0, 1.0, 0.0);

    m_EditorCamera = std::make_shared<PerspectiveCamera>(pCam);

    m_EditorCamera->SetHeight(1080);
    m_EditorCamera->SetWidth(1920);
    m_EditorCamera->OnInit();
    */


    m_ActiveScene = 1;

    glGenQueries(1, &m_Query);

}

void UILayer::BeginFrame()
{
    // ImGui BeginFrame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Begin the Query the Samples Renders
    glBeginQuery(GL_SAMPLES_PASSED, m_Query);

}

void UILayer::EndFrame()
{
    // ImGui EndFrame
    // Render ImGui stuff


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    int count = 1000;
    while (!m_ResultAvailable && count--)
    {
        glGetQueryObjectiv(m_Query, GL_QUERY_RESULT_AVAILABLE, &m_ResultAvailable);
    }
    if (m_ResultAvailable)
    {
        glGetQueryObjectiv(m_Query, GL_QUERY_RESULT, &m_Samples);
    }
}

void UILayer::OnUpdate(float ts)
{
    m_Delta = ts;

    glm::vec3 cam_pos = m_EditorCamera->GetPosition();
    m_CameraPosition[0] = cam_pos.x;
    m_CameraPosition[1] = cam_pos.y;
    m_CameraPosition[2] = cam_pos.z;

    //glEndQuery(GL_SAMPLES_PASSED);
 }
