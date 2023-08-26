#include "OrthographicCamera.h"

void OrthographicCamera::OnInit()
{
    m_AspectRatio = float(m_Width) / m_Height;
    glm::mat4 p;
    p = glm::ortho(-m_AspectRatio, m_AspectRatio, -1.0f, 1.0f, .10f, 1000.0f);

    m_View = glm::lookAt(m_eye, m_center, m_up);

    V = m_View;
    m_Proj = p;
    VP = p * m_View;
}

void OrthographicCamera::Present()
{
    m_AspectRatio = float(m_Width) / m_Height;
    glm::mat4  p;
    p = glm::ortho(-m_AspectRatio, m_AspectRatio, -1.0f, 1.0f, .10f, 1000.0f);
    m_View = glm::lookAt(m_eye, m_center, m_up);

    V = m_View;
    m_Proj = p;
    VP = p * m_View;
}

void OrthographicCamera::MoveForward()
{
}

void OrthographicCamera::MoveBackward()
{
}

void OrthographicCamera::MoveRight()
{
}

void OrthographicCamera::MoveLeft()
{
}


void OrthographicCamera::MoveUp()
{
}

void OrthographicCamera::MoveDown()
{
}

void OrthographicCamera::Rotate(glm::vec3 rot_dir)
{
}

void OrthographicCamera::Focus(glm::vec3 rot_dir)
{
}

void OrthographicCamera::OnUpdate(float ts)
{
    m_Delta= ts;
}