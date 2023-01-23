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
}


UILayer::~UILayer()
{
    // ImGui Clean Up Code
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void UILayer::Enable()
{
    ImGui::BeginMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        ImGui::MenuItem("New");
        ImGui::MenuItem("Open");
        ImGui::MenuItem("Save As");
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
    ImGui::Text("Number of Primitives : %d", m_NumPrimitives);
    ImGui::End();

    ImGui::Begin("Animation panel");
    ImGui::End();

    ImGui::Begin("Utilities");
    ImGui::Separator();
    ImGui::Text("Viewing and Transformation");
    ImGui::Separator();

    ImGui::SliderFloat3("Camera Position", m_CameraPosition, -50.0, 50.0);
    ImGui::Separator();

    ImGui::Text("Lights and Shadow");

    int enable_lighting_locatin = glGetUniformLocation(m_ActiveMaterial, "EnableTexture");
    glProgramUniform1i(m_ActiveMaterial, enable_lighting_locatin, m_EnableLighting);

    ImGui::Checkbox("Enable Lighting", &m_EnableLighting);
    ImGui::Separator();

    //light_pos[0] = BaseApplication::m_PointLight->GetPosition().x;
    //light_pos[1] = BaseApplication::m_PointLight->GetPosition().y;
    //light_pos[2] = BaseApplication::m_PointLight->GetPosition().z;
    //ImGui::SliderFloat3("Light Position", light_pos, 0.0, 100.);

    ImGui::Text("Materials");
    ImGui::Separator();

    glGetIntegerv(GL_CURRENT_PROGRAM, &m_ActiveMaterial); // m_CurrentProgram shoubd be the currently bound Material ID
    glGetProgramiv(m_ActiveMaterial, GL_ACTIVE_UNIFORMS, &m_ActiveUniforms);

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


}

void UILayer::BeginFrame()
{
    // ImGui BeginFrame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UILayer::EndFrame()
{
    // ImGui EndFrame
    // Render ImGui stuff
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
