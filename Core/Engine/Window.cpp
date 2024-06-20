#include "Window.h"
#include "imgui_impl_win32.h"
#include <memory>


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Win32Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    ImGui_ImplWin32_WndProcHandler(m_Hwnd, uMsg, wParam, lParam);

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_LBUTTONDOWN:
        m_SceneEventQueue.Push(new MouseButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MouseButton::LEFT));
        break;
    case WM_LBUTTONUP:
        break;
    case WM_MBUTTONDOWN:
        m_SceneEventQueue.Push(new MouseButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MouseButton::MIDDLE));
        break;
    case WM_MBUTTONUP:
        break;
    case WM_RBUTTONDOWN:
        m_SceneEventQueue.Push(new MouseButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MouseButton::RIGHT));
        break;
    case WM_RBUTTONUP:
        break;
    case WM_XBUTTONDOWN:
        break;
    case WM_XBUTTONUP:
        break;
    case WM_MOUSEMOVE:
        m_SceneEventQueue.Push(new MouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
        break;
    case WM_MOUSEWHEEL:
        m_SceneEventQueue.Push(new MouseWheel(GET_WHEEL_DELTA_WPARAM(wParam)));
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_UP :
            m_SceneEventQueue.Push(new SceneEvent(EventID::UP));
            break;
        case VK_RIGHT:
            m_SceneEventQueue.Push(new SceneEvent(EventID::RIGHT));
            break;
        case VK_DOWN:
            m_SceneEventQueue.Push(new SceneEvent(EventID::DOWN));
            break;
        case VK_LEFT:
            m_SceneEventQueue.Push(new SceneEvent(EventID::LEFT));
            break;
        case VK_SPACE:
            m_SceneEventQueue.Push(new SceneEvent(EventID::SPACE));
            break;
        case VK_ESCAPE:
            m_SceneEventQueue.Push(new SceneEvent(EventID::ESCAPE));
            break;

        case 0x41:
            m_SceneEventQueue.Push(new SceneEvent(EventID::A));
            break;
        case 0x42:
            m_SceneEventQueue.Push(new SceneEvent(EventID::B));
            break;
        case 0x43:
            m_SceneEventQueue.Push(new SceneEvent(EventID::C));
            break;
        case 0x44:
            m_SceneEventQueue.Push(new SceneEvent(EventID::D));
            break;
        case 0x45:
            m_SceneEventQueue.Push(new SceneEvent(EventID::E));
            break;
        case 0x46:
            m_SceneEventQueue.Push(new SceneEvent(EventID::F));
            break;
        case 0x47:
            m_SceneEventQueue.Push(new SceneEvent(EventID::G));
            break;
        case 0x48:
            m_SceneEventQueue.Push(new SceneEvent(EventID::H));
            break;
        case 0x49:
            m_SceneEventQueue.Push(new SceneEvent(EventID::I));
            break;
        case 0x4A:
            m_SceneEventQueue.Push(new SceneEvent(EventID::J));
            break;
        case 0x4B:
            m_SceneEventQueue.Push(new SceneEvent(EventID::K));
            break;
        case 0x4C:
            m_SceneEventQueue.Push(new SceneEvent(EventID::L));
            break;
        case 0x4D:
            m_SceneEventQueue.Push(new SceneEvent(EventID::M));
            break;
        case 0x4E:
            m_SceneEventQueue.Push(new SceneEvent(EventID::N));
            break;
        case 0x4F:
            m_SceneEventQueue.Push(new SceneEvent(EventID::O));
            break;
        case 0x50:
            m_SceneEventQueue.Push(new SceneEvent(EventID::P));
            break;
        case 0x51:
            m_SceneEventQueue.Push(new SceneEvent(EventID::Q));
            break;
        case 0x52:
            m_SceneEventQueue.Push(new SceneEvent(EventID::R));
            break;
        case 0x53:
            m_SceneEventQueue.Push(new SceneEvent(EventID::S));
            break;
        case 0x54:
            m_SceneEventQueue.Push(new SceneEvent(EventID::T));
            break;
        case 0x55:
            m_SceneEventQueue.Push(new SceneEvent(EventID::U));
            break;
        case 0x56:
            m_SceneEventQueue.Push(new SceneEvent(EventID::V));
            break;
        case 0x57:
            m_SceneEventQueue.Push(new SceneEvent(EventID::W));
            break;
        case 0x58:
            m_SceneEventQueue.Push(new SceneEvent(EventID::X));
            break;
        case 0x59:
            m_SceneEventQueue.Push(new SceneEvent(EventID::Y));
            break;
        case 0x5A:
            m_SceneEventQueue.Push(new SceneEvent(EventID::Z));
            break;
        }
        break;
    case WM_KEYUP:
        break;

    default:
        return DefWindowProc(m_Hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

Win32Window::Win32Window()
{
    CreateWin32Window(1920, 1080, "Empty Window");
    ShowWindow(m_Hwnd, 10);
}

Win32Window::Win32Window(uint32_t width, uint32_t height, const char* name, bool show)
{
    CreateWin32Window(width, height, name);
    show ? ShowWindow(m_Hwnd, 10) : 0;
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
}

void Win32Window::SetUpForRendering()
{
    CreateOpenGLContext();
}

void Win32Window::SwapBuffer()
{
    wglSwapIntervalEXT(1);
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
    m_SceneEventQueue.OnUpdate();
    ts = m_EndTime - m_StartTime;
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