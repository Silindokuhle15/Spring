#include "NGLFWwindow.h"

double NGLFWwindow::m_MouseX = 0;
double NGLFWwindow::m_MouseY = 0;
double NGLFWwindow::m_MouseScrollX = 0;
double NGLFWwindow::m_MouseScrollY = 0;

SceneEventQueue NGLFWwindow::m_SceneEventQueue = {};


void NGLFWwindow::SetUpForRendering()
{
    int a = 1;
    a++;
}

void NGLFWwindow::SwapBuffer()
{
	glfwSwapBuffers(m_pWindow);
}

void NGLFWwindow::OnUpdate()
{
    glfwPollEvents();
    m_SceneEventQueue.OnUpdate();
    ts = m_EndTime;
}

LRESULT NGLFWwindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

NGLFWwindow::~NGLFWwindow()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void NGLFWwindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            m_SceneEventQueue.Push(new MouseButtonDown(static_cast<uint64_t>(m_MouseX), static_cast<uint64_t>(m_MouseY), MouseButton::LEFT));
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            m_SceneEventQueue.Push(new MouseButtonDown(static_cast<uint64_t>(m_MouseX), static_cast<uint64_t>(m_MouseY), MouseButton::MIDDLE));
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            m_SceneEventQueue.Push(new MouseButtonDown(static_cast<uint64_t>(m_MouseX), static_cast<uint64_t>(m_MouseY), MouseButton::RIGHT));
            break;
        }
    }
}


void NGLFWwindow::error_callback(int error, const char* description)
{
    //std::cerr << "Error : " << description << std::endl;
    //exit(EXIT_FAILURE);
}

void NGLFWwindow::window_size_callback(GLFWwindow* window, int new_width, int new_height) { }

void NGLFWwindow::window_close_call(GLFWwindow* window)
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void NGLFWwindow::cursor_position_callback(GLFWwindow* window, double x, double y)
{
    glfwGetCursorPos(window, &x, &y);
    m_MouseX = x;
    m_MouseY = y;
    m_SceneEventQueue.Push(new MouseMove(static_cast<uint64_t>(m_MouseX), static_cast<uint64_t>(m_MouseY)));
}

void NGLFWwindow::cursor_scroll_callback(GLFWwindow* window, double x_scroll, double y_scroll)
{
    m_MouseScrollX = x_scroll;
    m_MouseScrollY = y_scroll;
    m_SceneEventQueue.Push(new MouseWheel(static_cast<int64_t>(m_MouseScrollY)));
}

void NGLFWwindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (action)
    {
    case GLFW_PRESS:
        switch (key) 
        {
        case GLFW_KEY_A:
            m_SceneEventQueue.Push(new SceneEvent(EventID::A));
            break;

        case GLFW_KEY_S:
            m_SceneEventQueue.Push(new SceneEvent(EventID::S));
            break;

        case GLFW_KEY_W:
            m_SceneEventQueue.Push(new SceneEvent(EventID::W));
            break;

        case GLFW_KEY_D:
            m_SceneEventQueue.Push(new SceneEvent(EventID::D));
            break;

        case GLFW_KEY_Q:
            m_SceneEventQueue.Push(new SceneEvent(EventID::Q));
            break;

        case GLFW_KEY_Z:
            m_SceneEventQueue.Push(new SceneEvent(EventID::Z));
            break;
        }
        break;
    case GLFW_RELEASE:
        break;
    }
    
}

/*
void NGFLWwindow::MoveCameraForward()
{
    m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->MoveForward();
}

void NGFLWwindow::MoveCameraBackward()
{
    m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->MoveBackward();
}

void NGFLWwindow::MoveCameraUp()
{
    m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->MoveUp();
}

void NGFLWwindow::MoveCameraDown()
{
    m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->MoveDown();
}

void NGFLWwindow::MoveCameraLeft()
{
    m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->MoveLeft();
}

void NGFLWwindow::MoveCameraRight()
{
    m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->MoveRight();
}

void NGFLWwindow::RotateCamera(glm::vec3 rot_dir)
{
    //m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->Rotate(rot_dir);
}

void BaseApplication::FocusCamera(glm::vec3 rot_dir)
{
    //m_pActiveRenderer->m_ActiveScene->m_ActiveCamera->Focus(rot_dir);
}

void BaseApplication::CreateNewScene()
{
    // Create A new Scene at set it as the active Scene??
}

*/