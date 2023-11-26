#include "PerspectiveCamera.h"

void PerspectiveCamera::OnResize(int new_width, int new_height)
{
    glViewport(0, 0, new_width, new_height);
}

void PerspectiveCamera::OnInit()
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

void PerspectiveCamera::MoveForward()
{
    m_eye -= glm::vec3(0.0, 0.0, 1.0) * m_Speed * m_Delta;
}

void PerspectiveCamera::MoveBackward()
{
    m_eye += glm::vec3(0.0, 0.0, 1.0) * m_Speed * m_Delta;
}

void PerspectiveCamera::MoveRight()
{
    m_eye += glm::vec3(1.0, 0.0, 0.0) * m_Speed * m_Delta;
}

void PerspectiveCamera::MoveLeft()
{
    m_eye -= glm::vec3(1.0, 0.0, 0.0) * m_Speed * m_Delta;
}

void PerspectiveCamera::MoveUp()
{
    m_eye += glm::vec3(0.0, 1.0, 0.0) * m_Speed * m_Delta;
}

void PerspectiveCamera::MoveDown()
{
    m_eye -= glm::vec3(0.0, 1.0, 0.0) * m_Speed * m_Delta;
}

void PerspectiveCamera::Rotate(glm::vec3 rot_dir)
{
    // Acutally a Drag than a rotation
    m_eye += rot_dir * m_Speed * m_Delta;
}

void PerspectiveCamera::Focus(glm::vec3 rot_dir)
{
    m_eye += rot_dir * m_Speed * m_Delta;
}

void PerspectiveCamera::OnUpdate(float ts)
{
    m_Delta = ts;
}

glm::mat4 PerspectiveCamera::GetV()
{
    return V;
}

glm::mat4 PerspectiveCamera::GetVP()
{
    return VP;
}
