#pragma once
#include "BaseWin32Window.h"
#include <string>
#include "Event.h"

constexpr uint32_t COLORBUFFER_BITS = 32;
constexpr uint32_t DEPTHBUFFER_BITS = 24;
constexpr uint32_t STENCILBUFFER_BITS = 8;
constexpr int GL_CTX_VERSION_MAJOR = 4;
constexpr int GL_CTX_VERSION_MINOR = 5;
constexpr int renderContextParameters[] =
{
	WGL_CONTEXT_MAJOR_VERSION_ARB, GL_CTX_VERSION_MAJOR,
	WGL_CONTEXT_MINOR_VERSION_ARB, GL_CTX_VERSION_MINOR,
	WGL_CONTEXT_FLAGS_ARB, 0,
	0
};
class OpenGLRenderContext
{
private:
	int m_VersionMajor;
	int m_VersionMinor;
	HGLRC m_RenderContext;
	HDC m_DeviceContext;
public:
	OpenGLRenderContext();
	~OpenGLRenderContext();
public:
	void initializeDevice(HWND window_handle);
	bool createLegacyContext();
	bool createModernContext();
	bool choosePixelFormat() const;
	bool makeCurrent() const;
	void presentNextImage() const;
	void setVersion();
	void cleanUpDevice(HWND window_handle);
};

class Win32Window : public BaseWindowClass<Win32Window>
{
public:
	OpenGLRenderContext m_OpenGLRenderContext;
	float m_Ts;
	bool CreateOpenGLContext();
	void DestroyOpenGLContext();
	void SetUpForRendering();

	void SwapBuffer() override;
	void OnUpdate() override;
	
	operator HWND() const { return m_WindowHandle; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	Win32Window();
	Win32Window(uint32_t width, uint32_t height, const char* name, bool show = true);
	~Win32Window();

	void SetWidth(uint32_t width);
	const uint32_t GetWidth() const;
	void SetHeight(uint32_t height);
	const uint32_t GetHeight() const;
	void SetTitle(std::string title);
	void (*OnEvent)(event::Event& event);
protected:
	uint32_t m_Width;
	uint32_t m_Height;
	std::string m_Title;
private:
};

