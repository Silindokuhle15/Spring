#pragma once
#include "Window.h"
#include <GLFW/glfw3.h>

class NGLFWwindow : public BaseWindowClass<NGLFWwindow>
{
public:

    static double m_MouseX, m_MouseY;
    static double m_MouseScrollX, m_MouseScrollY;
    static SceneEventQueue m_SceneEventQueue;

    GLFWwindow* m_pWindow;
    TimeStep ts;
    void SetUpForRendering();
	void OnUpdate() override;

    void StartTimer() override
    {
        m_StartTime = static_cast<float>(glfwGetTime());
    }
    void EndTimer() override
    {
        m_EndTime = static_cast<float>(glfwGetTime());
    }
    void SwapBuffer() override;

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void error_callback(int error, const char* description);
    static void window_size_callback(GLFWwindow* window, int new_width, int new_height);
    static void window_close_call(GLFWwindow* window);
    static void cursor_position_callback(GLFWwindow* window, double x, double y);
    static void cursor_scroll_callback(GLFWwindow* window, double x_scroll, double y_scroll);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    operator GLFWwindow* () const { return m_pWindow; }

    NGLFWwindow() : m_pWindow{ nullptr }
    {

    }
    NGLFWwindow(int64_t width, int64_t height, const char* name, bool show = true)
	{
        if (!glfwInit())
            exit(EXIT_FAILURE);

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
        const char* glsl_version = "#version 100";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

#else
    // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 300";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
        //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

        m_pWindow = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), name, NULL, NULL);
        if (!m_pWindow)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(m_pWindow);

        if (glewInit() != GLEW_OK)
        {
            glfwTerminate();
            //std::cerr << "glfwMakeContextCurrent failed\n";
            return;
        }
        glfwSwapInterval(1);

        glfwSetErrorCallback(error_callback);
        glfwSetKeyCallback(m_pWindow, key_callback);
        glfwSetWindowSizeCallback(m_pWindow, window_size_callback);
        glfwSetMouseButtonCallback(m_pWindow, mouse_button_callback);
        glfwSetWindowCloseCallback(m_pWindow, window_close_call);
        glfwSetCursorPosCallback(m_pWindow, cursor_position_callback);
        glfwSetScrollCallback(m_pWindow, cursor_scroll_callback);
        // START THE LUA ENGINE AFTER THE WINDOW HAS BEEN CREATED
	}
	~NGLFWwindow();

private:
	// FRAMEBUFFERS
	
};