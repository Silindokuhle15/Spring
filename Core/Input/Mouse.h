#pragma once
#include "InputDevice.h"
class Mouse :
    public InputDevice
{
private:
    uint64_t m_X;
    uint64_t m_Y;

    bool m_IsRawMotionSupported;

    int64_t CheckForRawMotionSupport();
public:
    Mouse() : InputDevice(), m_X{ 0 }, m_Y{ 0 }, m_IsRawMotionSupported { false } {}

    uint64_t GetX() const { return m_X; }
    uint64_t GetY() const { return m_Y; }

    int64_t CheckForRawInputSupport();

    //static void cursor_position_callback(GLFWwindow* window, double x_pos, double y_pos);
    //static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffest);

    //void EnableRawMotion(GLFWwindow* window);
};

