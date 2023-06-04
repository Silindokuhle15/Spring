#include "KeyBoard.h"

void KeyBoard::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            //Move Forward
            BaseApplication::MoveCameraForward();

            break;
        case GLFW_KEY_A:
            //Move Left
            BaseApplication::MoveCameraLeft();
            break;
        case GLFW_KEY_S:
            //Move Backward
            BaseApplication::MoveCameraBackward();
            break;
        case GLFW_KEY_D:
            //Move Right
            BaseApplication::MoveCameraRight();
            break;
        case GLFW_KEY_Q:
            BaseApplication::MoveCameraUp();
            break;

        case GLFW_KEY_Z:
            BaseApplication::MoveCameraDown();
            break;
        case GLFW_KEY_ESCAPE:

            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;

        case GLFW_KEY_UP:
            BaseApplication::m_pActiveRenderer->m_ActiveScene->MoveObjectUp();
            break;

        case GLFW_KEY_DOWN:
            BaseApplication::m_pActiveRenderer->m_ActiveScene->MoveObjectDown();
            break;

        case GLFW_KEY_LEFT:
            BaseApplication::m_pActiveRenderer->m_ActiveScene->MoveObjectLeft();
            break;

        case GLFW_KEY_RIGHT:
            BaseApplication::m_pActiveRenderer->m_ActiveScene->MoveObjectRight();
            break;          
        }
    }
    
}
