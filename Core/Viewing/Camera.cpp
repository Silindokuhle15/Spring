#include "Camera.h"

const glm::vec3 Camera::UnProjectMouse(const glm::vec2& mouse_position) const
{
    glm::vec4 clipSpacePos{ mouse_position.x, mouse_position.y, 1.0, 1.0 };
    auto invProj = glm::inverse(m_ProjectionMatrix);
    auto viewSpacePos = invProj * clipSpacePos;

    viewSpacePos.x /= viewSpacePos.w;
    viewSpacePos.y /= viewSpacePos.w;
    viewSpacePos.z /= viewSpacePos.w;
    viewSpacePos.w /= viewSpacePos.w;

    auto invView = glm::inverse(m_View);
    auto worldSpacePos = invView * viewSpacePos;

    return glm::vec3(worldSpacePos);
}
