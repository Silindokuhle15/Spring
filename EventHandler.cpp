#include "EventHandler.h"

ReturnStatus EventHandler::EventHandlerInit()
{
    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!(glfwRawMouseMotionSupported() == GLFW_TRUE))
    {
        std::cout << "RawMouseMotionSupported is not True\n";
    }
    else
    {
        glfwSetInputMode(*window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    return ReturnStatus::EVENT_HANDLER_INIT_SUCCESS;
}
