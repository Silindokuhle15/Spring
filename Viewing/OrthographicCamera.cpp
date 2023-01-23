#include "OrthographicCamera.h"

void OrthographicCamera::OnResize(int new_width, int new_height)
{
    glViewport(0, 0, new_width, new_height);
}

void OrthographicCamera::OnCreate()
{
    m_AspectRatio = float(m_Width) / m_Height;
    glm::mat4 p;
    p = glm::ortho(-m_AspectRatio, m_AspectRatio, -1.0f, 1.0f, .10f, 1000.0f);

    m_View = glm::lookAt(m_eye, m_center, m_up);

    V = m_View;
    VP = p * m_View;
}

void OrthographicCamera::Present()
{
    m_AspectRatio = float(m_Width) / m_Height;
    glm::mat4  p;
    p = glm::ortho(-m_AspectRatio, m_AspectRatio, -1.0f, 1.0f, .10f, 1000.0f);
    m_View = glm::lookAt(m_eye, m_center, m_up);

    V = m_View;
    VP = p * m_View;
}


void OrthographicCamera::Reset()
{
    VP = glm::mat4(1.0);
}

glm::vec3 OrthographicCamera::GetPosition()
{
    return m_eye;
}

void OrthographicCamera::OnUpdate(float ts)
{
    m_Delta= ts;
}
