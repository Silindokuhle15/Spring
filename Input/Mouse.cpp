#include "Mouse.h"

int Mouse::CheckForRawMotionSupport()
{
    return CheckForRawInputSupport();
}

int Mouse::CheckForRawInputSupport()
{
    int ret = glfwRawMouseMotionSupported();
    return ret;
}
void Mouse::cursor_position_callback(GLFWwindow* window, double x_pos, double y_pos)
{

}
void Mouse::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            break;
        }
    }
}
void Mouse::mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    glm::vec3 camera_pos = BaseApplication::m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->GetPosition();

    glm::vec3 move_dir = glm::vec3(xoffset, -yoffset, 0.0);

    glm::vec3 focus_dir = glm::vec3(xoffset, -yoffset, -1.0);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        BaseApplication::RotateCamera(move_dir);
    }
    else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
        BaseApplication::FocusCamera(focus_dir);
    }
}

void Mouse::EnableRawMotion(GLFWwindow * window)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED ); // disable the cursor 

    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE); // Enable Raw Mouse Motion
}