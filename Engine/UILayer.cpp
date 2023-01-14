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
    ImGui::Begin("File | Edit");
    ImGui::MenuItem("(demo menu)", NULL, false, false);
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    if (ImGui::BeginMenu("Open Recent"))
    {
        ImGui::MenuItem("fish_hat.c");
        ImGui::MenuItem("fish_hat.inl");
        ImGui::MenuItem("fish_hat.h");

        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
    if (ImGui::MenuItem("Save As..")) {}

    ImGui::End();


    ImGui::Begin("Stats And Performance");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Begin("Animation panel");
    ImGui::End();

    ImGui::Begin("Utilities");
    ImGui::Text("Viewing and Transformation");

    //cam_pos[0] = BaseApplication::cam_ptr->GetPosition().x;
    //cam_pos[1] = BaseApplication::cam_ptr->GetPosition().y;
    //cam_pos[2] = BaseApplication::cam_ptr->GetPosition().z;

    //ImGui::SliderFloat3("Camera Position", cam_pos, 0.0, 100.0);
    //ImGui::Separator();

    ImGui::Text("Lights and Shadow");
    //light_pos[0] = BaseApplication::m_PointLight->GetPosition().x;
    //light_pos[1] = BaseApplication::m_PointLight->GetPosition().y;
    //light_pos[2] = BaseApplication::m_PointLight->GetPosition().z;
    //ImGui::SliderFloat3("Light Position", light_pos, 0.0, 100.);
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
