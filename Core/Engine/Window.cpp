#include "Window.h"

LRESULT Win32Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_QUIT:
        break;

    case WM_KEYDOWN:
        break;

    case WM_KEYUP:
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}


Win32Window::Win32Window()
{
    m_Handle = GetModuleHandle(NULL);
    //m_Handle = hInstance;

    // Register the window class.
    const char CLASS_NAME[] = "Sample Window Class";
    WNDCLASS wc = { };
    wc.lpfnWndProc = Win32Window::WindowProc;
    wc.hInstance = m_Handle;
    wc.lpszClassName = CLASS_NAME;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

    RegisterClass(&wc);
    // Create the window.
    m_Hwnd = CreateWindowEx(
        0, // Optional window styles.
        CLASS_NAME, // Window class
        "Rocket Launch Simulator", // Window text
        WS_OVERLAPPEDWINDOW, // Window style
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080,
        NULL, // Parent window
        NULL, // Menu
        m_Handle, // Instance handle
        NULL // Additional application data
    );

    ShowWindow(m_Hwnd, 10);
}

Win32Window::~Win32Window()
{
    //wglMakeCurrent(ourWindowHandleToDeviceContext, NULL); Unnecessary; wglDeleteContext will make the context not current
    //wglDeleteContext(ourOpenGLRenderingContext);
}


void Win32Window::CreateOpenGLContext()
{
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        8,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    m_Hdc = GetDC(m_Hwnd);

    int  letWindowsChooseThisPixelFormat;
    letWindowsChooseThisPixelFormat = ChoosePixelFormat(m_Hdc, &pfd);
    SetPixelFormat(m_Hdc, letWindowsChooseThisPixelFormat, &pfd);

    //HGLRC ourOpenGLRenderingContext = wglCreateContext(m_Hdc);
    //wglMakeCurrent(m_Hdc, ourOpenGLRenderingContext);

    HGLRC tempContext = wglCreateContext(m_Hdc);
    wglMakeCurrent(m_Hdc, tempContext);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        //AfxMessageBox(_T("GLEW is not initialized!"));
    }

    int attribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 5,
        WGL_CONTEXT_FLAGS_ARB, 0,
        0
    };

    if (wglewIsSupported("WGL_ARB_create_context") == 1)
    {
        m_Hrc = wglCreateContextAttribsARB(m_Hdc, 0, attribs);
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(tempContext);
        wglMakeCurrent(m_Hdc, m_Hrc);
    }
    else
    {	//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
        m_Hrc = tempContext;
    }
    //Checking GL version
    const GLubyte* GLVersionString = glGetString(GL_VERSION);

    //Or better yet, use the GL3 way to get the version number
    int OpenGLVersion[2];
    glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

    //MessageBoxA(0, (char*)glGetString(GL_VERSION), "OPENGL VERSION", 0);
}

void Win32Window::DestroyOpenGLContext()
{

    /*
    // IMGUI SHUTDOWN
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    */
}


void Win32Window::SetUpForRendering()
{
    CreateOpenGLContext();
}

void Win32Window::SwapBuffer()
{
    SwapBuffers(m_Hdc);
}

void Win32Window::OnUpdate()
{
    MSG msg = {};
    if (PeekMessage(&msg, m_Hwnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

long long Win32Window::milliseconds_now() {
    static LARGE_INTEGER s_frequency;
    static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
    if (s_use_qpc) {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return (1000LL * now.QuadPart) / s_frequency.QuadPart;
    }
    else {
        return GetTickCount64();
    }
}