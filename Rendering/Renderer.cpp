#include "Renderer.h"

void Renderer::SetUpForRendering()
{
    unsigned int m_Width = 1920;
    unsigned int m_Height = 1080;

    glViewport(0, 0, m_Width, m_Height);
    glEnable(GL_DEPTH_BUFFER_BIT);
    glDepthRange(.010f, 1000.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
}

void Renderer::OnRender(Scene* scene)
{
    for (int n = 0; n < scene->m_SceneData.size(); n++)
    {
        Application* app = scene->m_SceneData[n];
        app->OnRender();
        app->OnUpdate();
    }
}

void Renderer::OnUpdate()
{

    glUseProgram(m_Program);
    m_MVPlocation = glGetUniformLocation(m_Program, "MVP");
    m_MVlocation = glGetUniformLocation(m_Program, "MV");

    //m_PointLightLocation = glGetUniformLocation(m_Program, "LightPosition");
    //m_AmbientColorLocation = glGetUniformLocation(m_Program, "AmbientColor");

    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(m_MVPlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetMVP()));
    glUniformMatrix4fv(m_MVlocation, 1, GL_FALSE, glm::value_ptr(BaseApplication::cam_ptr->GetMV()));
    //glUniform3fv(m_PointLightLocation, 1, glm::value_ptr(BaseApplication::m_PointLight->GetPosition()));
}