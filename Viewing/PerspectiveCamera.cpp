#include "PerspectiveCamera.h"

void PerspectiveCamera::OnResize(int new_width, int new_height)
{
    glViewport(0, 0, new_width, new_height);
}

void PerspectiveCamera::OnCreate()
{
    m_AspectRatio = float(m_Width) / m_Height;

    m_View = glm::lookAt(m_eye, m_center, m_up);
    m_Proj = glm::perspective(glm::radians(60.f), m_AspectRatio, 0.010f, 1000.0f);

    V = m_View;
    VP = m_Proj * m_View;
}

void PerspectiveCamera::Present()
{
    m_AspectRatio = float(m_Width) / m_Height;

    m_View = glm::lookAt(m_eye, m_center, m_up);
    m_Proj = glm::perspective(glm::radians(60.f), m_AspectRatio, 0.010f, 1000.0f);

    V = m_View;
    VP = m_Proj * m_View;
}

void PerspectiveCamera::Reset()
{
    VP = glm::mat4(1.0);
    m_eye = glm::vec3(0.0, .0, 5.0f);
    m_center = glm::vec3(0.0f);
    m_up =glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 PerspectiveCamera::GetPosition()
{
    return m_eye;
}

void PerspectiveCamera::OnUpdate(float ts)
{
    m_Delta = ts;
}
