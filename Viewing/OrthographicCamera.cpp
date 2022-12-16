#include "OrthographicCamera.h"

void OrthographicCamera::OnResize(int new_width, int new_height)
{
    glViewport(0, 0, new_width, new_height);
}

void OrthographicCamera::OnCreate()
{
    m_AspectRatio = float(m_Width) / m_Height;
    glm::mat4 m, p;
    p = glm::ortho(-m_AspectRatio, m_AspectRatio, -1.0f, 1.0f, .10f, 1000.0f);

    m = glm::mat4(1.0f);
    m_ModelScale = glm::scale(m, glm::vec3(1.f));
    m_ModelTranslate = glm::translate(m, glm::vec3(.0f));
    m_ModelRotate = glm::rotate(m, glm::radians(0.f), glm::vec3(.0f, 0.f, 1.0f));
    m = m_ModelRotate * m_ModelTranslate * m_ModelScale;
    m_View = glm::lookAt(m_eye, m_center, m_up);

    MV = m * m_View;
    MVP = p * m * m_View;
}

void OrthographicCamera::Present()
{
    m_AspectRatio = float(m_Width) / m_Height;
    glm::mat4  p;
    p = glm::ortho(-m_AspectRatio, m_AspectRatio, -1.0f, 1.0f, .10f, 1000.0f);

    glm::mat4 m = glm::mat4(1.0f);
    m_ModelScale = glm::scale(m, glm::vec3(1.f));
    m_ModelTranslate = glm::translate(m, glm::vec3(.0f));
    m_ModelRotate = glm::rotate(m, glm::radians(0.f), glm::vec3(.0f, 0.f, 1.0f));
    m = m_ModelRotate * m_ModelTranslate * m_ModelScale;
    m_View = glm::lookAt(m_eye, m_center, m_up);

    MV = m * m_View;
    MVP = p * m * m_View;
}


void OrthographicCamera::Reset()
{
    MVP = glm::mat4(1.0);
}

glm::vec3 OrthographicCamera::GetPosition()
{
    return m_eye;
}