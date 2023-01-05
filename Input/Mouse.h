#pragma once
#include "InputDevice.h"
#include "Engine/BaseApplication.h"

class Mouse :
    public InputDevice
{
private:
    unsigned int m_X;
    unsigned int m_Y;

    bool m_IsRawMotionSupported;

    int CheckForRawMotionSupport();
public:
    Mouse() : InputDevice(), m_X{ 0 }, m_Y{ 0 }, m_IsRawMotionSupported { false } {}

    unsigned int GetX() const { return m_X; }
    unsigned int GetY() const { return m_Y; }

    int CheckForRawInputSupport();

    static void cursor_position_callback(GLFWwindow* window, double x_pos, double y_pos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffest);

    void EnableRawMotion(GLFWwindow* window);
};

