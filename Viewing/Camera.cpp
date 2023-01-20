#include "Camera.h"

void Camera::OnCreate()
{   
    m_AspectRatio = float(m_Width) / m_Height;

    m_View = glm::lookAt(m_eye, m_center, m_up);
    m_Proj = glm::perspective(glm::radians(60.f), m_AspectRatio, 0.10f, 1000.0f);

    V = m_View;
    VP = m_Proj * m_View;
}

void Camera::Present()
{   
    m_AspectRatio = float(m_Width) / m_Height;
    m_View = glm::lookAt(m_eye, m_center, m_up);
    m_Proj = glm::perspective(glm::radians(60.f), m_AspectRatio, 0.10f, 1000.0f);

    V = m_View;
    VP = m_Proj *m_View;
}

void Camera::OnResize(int new_width, int new_height)
{
    glViewport(0, 0, new_width, new_height);
}
