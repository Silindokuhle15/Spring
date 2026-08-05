#include "Win32Window.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT Win32Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplWin32_WndProcHandler(m_WindowHandle, uMsg, wParam, lParam);
    switch (uMsg)
    {
        case WM_SIZE:
            break;
        case WM_MOVE:
            break;
        case WM_DESTROY:
        {
            event::WindowCloseEvent event;
            OnEvent(event);
        }break;
        case WM_MOUSEMOVE:
        {
            event::MouseMoveEvent event(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            OnEvent(event);
        }break;
        case WM_KEYDOWN:
        {
            if (wParam == VK_SPACE)
            {
                event::KeyPressEvent spaceEvent(event::EventID::SPACE);
                OnEvent(spaceEvent);
                break;
            }
            if (wParam == VK_LEFT)
            {
                event::KeyPressEvent leftEvent(event::EventID::LEFT);
                OnEvent(leftEvent);
                break;
            }
            if(wParam == VK_UP)
            {
                event::KeyPressEvent upEvent(event::EventID::UP);
                OnEvent(upEvent);
                break;
            }
            if(wParam == VK_RIGHT)
            {
                event::KeyPressEvent rightEvent(event::EventID::RIGHT);
                OnEvent(rightEvent);
                break;
            }
            if(wParam == VK_DOWN)
            {
                event::KeyPressEvent downEvent(event::EventID::DOWN);
                OnEvent(downEvent);
                break;
            }
            if(wParam == 0x57)
            {
                event::KeyPressEvent wEvent(event::EventID::W);
                OnEvent(wEvent);
                break;
            }
            if(wParam == 0x53)
            {
                event::KeyPressEvent sEvent(event::EventID::S);
                OnEvent(sEvent);
                break;
            }
            if(wParam == 0x41)
            {
                event::KeyPressEvent aEvent(event::EventID::A);
                OnEvent(aEvent);
                break;
            }
            if(wParam == 0x44)
            {
                event::KeyPressEvent dEvent(event::EventID::D);
                OnEvent(dEvent);
                break;
            }
            if(wParam == 0x51)
            {
                event::KeyPressEvent qEvent(event::EventID::Q);
                OnEvent(qEvent);
                break;
            }
            if(wParam == 0x5A)
            {
                event::KeyPressEvent zEvent(event::EventID::Z);
                OnEvent(zEvent);
                break;
            }
        }break;
        default:
            return DefWindowProc(m_WindowHandle, uMsg, wParam, lParam);
    }
    return 0;
}

bool Win32Window::CreateOpenGLContext()
{
    OpenGLRenderContext tempContext;
    tempContext.initializeDevice(m_WindowHandle);
    if (!tempContext.choosePixelFormat())
    {
        return false;
    }
    if (!tempContext.createLegacyContext())
    {
        return false;
    }
    if (!tempContext.makeCurrent())
    {
        return false;
    }
    if (glewInit() != GLEW_OK)
    {
        return false;
    }
    if (wglewIsSupported("WGL_ARB_create_context") != GL_TRUE)
    {
        return false;
    }
    tempContext.cleanUpDevice(m_WindowHandle);
    m_OpenGLRenderContext.initializeDevice(m_WindowHandle);
    if (!m_OpenGLRenderContext.createModernContext())
    {
        return false;
    }
    if (!m_OpenGLRenderContext.makeCurrent())
    {
        return false;
    }
    return true;
}
Win32Window::Win32Window():
    m_OpenGLRenderContext{}
{
    CreateWin32Window(1920, 1080, "Empty Window");
    ShowWindow(m_WindowHandle, SW_SHOWMAXIMIZED);
    SetUpForRendering();
}

Win32Window::Win32Window(uint32_t width, uint32_t height, const char* name, bool show):
    m_Width{width},
    m_Height{height},
    m_Title{name},
    m_Ts{0.0f},
    m_OpenGLRenderContext{},
    OnEvent{nullptr}
{
    if (!CreateWin32Window(width, height, name))
    {
        return;
    }
    show ? ShowWindow(m_WindowHandle, SW_SHOWMAXIMIZED) : 0;
    
    SetUpForRendering();
}

Win32Window::~Win32Window()
{
    DestroyOpenGLContext();
}

void Win32Window::SetWidth(uint32_t width)
{
    m_Width = width;
}
const uint32_t Win32Window::GetWidth() const
{
    return m_Width;
}
void Win32Window::SetHeight(uint32_t height)
{
    m_Height = height;
}
const uint32_t Win32Window::GetHeight() const
{
    return m_Height;
}
void Win32Window::SetTitle(std::string title)
{
    m_Title = title;
}

void Win32Window::DestroyOpenGLContext()
{
    m_OpenGLRenderContext.cleanUpDevice(m_WindowHandle);
}

void Win32Window::SetUpForRendering()
{
    CreateOpenGLContext();
}

void Win32Window::SwapBuffer()
{
    m_OpenGLRenderContext.presentNextImage();
}

void Win32Window::OnUpdate()
{
    MSG msg = {};
    while (PeekMessage(&msg, m_WindowHandle, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

OpenGLRenderContext::OpenGLRenderContext():
    m_VersionMajor{0},
    m_VersionMinor{0},
    m_DeviceContext{nullptr},
    m_RenderContext{nullptr}
{
}

OpenGLRenderContext::~OpenGLRenderContext()
{
    if (m_RenderContext)
    {
        wglDeleteContext(m_RenderContext);
        m_RenderContext = nullptr;
    }
}

void OpenGLRenderContext::initializeDevice(HWND window_handle)
{
    m_DeviceContext = GetDC(window_handle);
    if (!m_DeviceContext)
    {
        auto error = GetLastError();
        return;
    }
}

bool OpenGLRenderContext::createLegacyContext()
{
    m_RenderContext = wglCreateContext(m_DeviceContext);
    if (!m_RenderContext)
    {
        auto error = GetLastError();
        return false;
    }
    return true;
}

bool OpenGLRenderContext::createModernContext()
{
    m_RenderContext = wglCreateContextAttribsARB(m_DeviceContext, 0, renderContextParameters);
    if (!m_RenderContext)
    {
        auto error = GetLastError();
        return false;
    }
    return true;
}

bool OpenGLRenderContext::choosePixelFormat() const
{
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,     //Flags
        PFD_TYPE_RGBA,                                                  // The kind of framebuffer. RGBA or palette.
        COLORBUFFER_BITS,                                               // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        DEPTHBUFFER_BITS,                                               // Number of bits for the depthbuffer
        STENCILBUFFER_BITS,                                             // Number of bits for the stencilbuffer
        0,                                                              // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    int chosenPixelFormat = ChoosePixelFormat(m_DeviceContext, &pfd);
    if (!chosenPixelFormat)
    {
        return false;
    }
    if (!SetPixelFormat(m_DeviceContext, chosenPixelFormat, &pfd))
    {
        return false;
    }
    return true;
}

bool OpenGLRenderContext::makeCurrent() const
{
    wglMakeCurrent(NULL, NULL);
    if (!wglMakeCurrent(m_DeviceContext, m_RenderContext))
    {
        auto error = GetLastError();
        return false;
    }
    return true;
}

void OpenGLRenderContext::presentNextImage() const
{
    if (!SwapBuffers(m_DeviceContext))
    {
        auto error = GetLastError();
    }
}

void OpenGLRenderContext::setVersion()
{
    const GLubyte* GLVersionString = glGetString(GL_VERSION);
    if (GLVersionString)
    {
        int OpenGLVersion[2] = { 0,0 };
        glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
        glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
        m_VersionMajor = OpenGLVersion[0];
        m_VersionMinor = OpenGLVersion[1];
    }
}

void OpenGLRenderContext::cleanUpDevice(HWND window_handle)
{
    if (m_RenderContext)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(m_RenderContext);
        m_RenderContext = nullptr;
    }
    if (m_DeviceContext)
    {
        ReleaseDC(window_handle, m_DeviceContext);
        m_DeviceContext = nullptr;
    }
}
