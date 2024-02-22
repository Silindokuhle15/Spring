#pragma once
#include <Windows.h>
#include "SceneEvent.h"
#include <memory>
#include "OpenGLFrameBuffer.h"

template <class WindowClass>
class BaseWindowClass
{
public:
	SceneEventQueue m_SceneEventQueue;
	HWND m_Hwnd;
	HINSTANCE m_Handle;

	BOOL CreateWin32Window(INT width, INT height, const char* app_name)
	{
		m_Handle = GetModuleHandle(NULL);
		// Register the window class.
		const char CLASS_NAME[] = "Win32Window";
		WNDCLASS wc = { };
		wc.lpfnWndProc = WindowClass::WindowProcedure;
		wc.hInstance = m_Handle;
		wc.lpszClassName = CLASS_NAME;
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

		RegisterClass(&wc);
		// Create the window.
		m_Hwnd = CreateWindowEx(
			0, // Optional window styles.
			CLASS_NAME, // Window class
			app_name, // Window text
			WS_OVERLAPPEDWINDOW, // Window style
			// Size and position
			CW_USEDEFAULT, CW_USEDEFAULT, width, height,
			NULL, // Parent window
			NULL, // Menu
			m_Handle, // Instance handle
			this // Additional application data
		);

		return (m_Hwnd ? TRUE : FALSE);
	}
	static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WindowClass* pWindowComponent = nullptr;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pWindowComponent = (WindowClass*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pWindowComponent);
			pWindowComponent->m_Hwnd = hwnd;
		}
		else 
		{
			pWindowComponent = (WindowClass*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}
		if (pWindowComponent)
		{
			return pWindowComponent->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	BaseWindowClass() : m_Hwnd{ NULL }, m_Handle{ NULL } {}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};

class Win32Window : public BaseWindowClass<Win32Window>
{
public:
	//const char* CLASS_NAME = "Win32Window";
	HDC m_Hdc;
	HGLRC m_Hrc;

	void CreateOpenGLContext();
	void DestroyOpenGLContext();
	void SetUpForRendering();

	void SwapBuffer();

	void OnUpdate();
	long long milliseconds_now();
	operator HWND() const { return m_Hwnd; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	Win32Window();
	~Win32Window();

private:
	// FRAMEBUFFERS
	OpenGLFrameBuffer m_DrawFrame;
	OpenGLFrameBuffer m_ReadFrame;
};