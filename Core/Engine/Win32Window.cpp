#include "Win32Window.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Win32Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplWin32_WndProcHandler(m_Hwnd, uMsg, wParam, lParam);
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(m_Hwnd, uMsg, wParam, lParam);
    }
    return 0;
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
Win32Window::Win32Window()
{
    CreateWin32Window(1920, 1080, "Empty Window");
    ShowWindow(m_Hwnd, 10);
    SetUpForRendering();
}

Win32Window::Win32Window(uint64_t width, uint64_t height, const char* name, bool show):
    m_Width{width},
    m_Height{height},
    m_Title{name}
{
    CreateWin32Window(width, height, name);
    show ? ShowWindow(m_Hwnd, 10) : 0;
    SetUpForRendering();
}


Win32Window::~Win32Window()
{
    //wglMakeCurrent(ourWindowHandleToDeviceContext, NULL); Unnecessary; wglDeleteContext will make the context not current
    //wglDeleteContext(ourOpenGLRenderingContext);
}
void Win32Window::SetWidth(int64_t width)
{
    m_Width = width;
}
const uint64_t Win32Window::GetWidth() const
{
    return m_Width;
}
void Win32Window::SetHeight(int64_t height)
{
    m_Height = height;
}
const uint64_t Win32Window::GetHeight() const
{
    return m_Height;
}
void Win32Window::SetTitle(std::string title)
{
    m_Title = title;
}
std::vector<uint64_t> Win32Window::GetMousePosition() const
{
    CURSORINFO ci{ sizeof(CURSORINFO) };
    uint64_t x{ 0 }, y{ 0 };
    if (GetCursorInfo(&ci))
    {
        POINT pt{ ci.ptScreenPos };
        x = pt.x;
        y = pt.y;
    }
    return std::vector<uint64_t>(x, y);
}
void Win32Window::DestroyOpenGLContext()
{
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
    m_Ts = (m_EndTime - m_StartTime);
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