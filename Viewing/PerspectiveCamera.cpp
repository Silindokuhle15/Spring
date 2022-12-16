#include "PerspectiveCamera.h"

void PerspectiveCamera::OnResize(int new_width, int new_height)
{
    glViewport(0, 0, new_width, new_height);
}

void PerspectiveCamera::OnCreate()
{
    m_AspectRatio = float(m_Width) / m_Height;
    glm::mat4 m;

    m = glm::mat4(1.0f);
    m_ModelScale = glm::scale(m, glm::vec3(2.f));
    m_ModelTranslate = glm::translate(m, glm::vec3(.0f));
    m_ModelRotate = glm::rotate(m, glm::radians(0.f), glm::vec3(.0f, 0.f, 1.0f));
    m = m_ModelRotate * m_ModelTranslate * m_ModelScale;
    m_View = glm::lookAt(m_eye, m_center, m_up);
    m_Proj = glm::perspective(glm::radians(60.f), m_AspectRatio, 0.010f, 1000.0f);

    MV = m * m_View;
    MVP = m_Proj * m * m_View;
}

void PerspectiveCamera::Present()
{
    m_AspectRatio = float(m_Width) / m_Height;

    glm::mat4 m = glm::mat4(1.0f);
    m_ModelScale = glm::scale(m, glm::vec3(2.f));
    m_ModelTranslate = glm::translate(m, glm::vec3(.0f));
    m_ModelRotate = glm::rotate(m, glm::radians(0.f), glm::vec3(.0f, 0.0f, 1.0f));
    m = m_ModelRotate * m_ModelTranslate * m_ModelScale;
    m_View = glm::lookAt(m_eye, m_center, m_up);
    m_Proj = glm::perspective(glm::radians(60.f), m_AspectRatio, 0.010f, 1000.0f);

    MV = m * m_View;
    MVP = m_Proj * m * m_View;
}

void PerspectiveCamera::Reset()
{
    MVP = glm::mat4(1.0);
    m_eye = glm::vec3(0.0, .0, 5.0f);
    m_center = glm::vec3(0.0f);
    m_up =glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 PerspectiveCamera::GetPosition()
{
    return m_eye;
}