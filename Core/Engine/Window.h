#pragma once
#include <Windows.h>
#include <windowsx.h>
#include "SceneEvent.h"
#include "OpenGLFrameBuffer.h"
#include "TimeStep.h"

template <class WindowClass>
class BaseWindowClass
{
public:
	SceneEventQueue m_SceneEventQueue;
	HWND m_Hwnd;
	HINSTANCE m_Handle;

	TimeStep m_StartTime;
	TimeStep m_EndTime;

	virtual void OnUpdate() = 0;
	virtual void StartTimer() = 0;
	virtual void EndTimer() = 0;
	virtual void SwapBuffer() = 0;

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
	TimeStep ts;

	void CreateOpenGLContext();
	void DestroyOpenGLContext();
	void SetUpForRendering();

	void SwapBuffer() override;

	void OnUpdate() override;
	
	operator HWND() const { return m_Hwnd; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	Win32Window();
	Win32Window(uint32_t width, uint32_t height, const char* name, bool show = true);
	~Win32Window();

	void StartTimer() override {
		m_StartTime = milliseconds_now();
	}
	void EndTimer() override {
		m_EndTime = milliseconds_now();
	}


protected:
	long long milliseconds_now();

private:
	// FRAMEBUFFERS
	OpenGLFrameBuffer m_DrawFrame;
	OpenGLFrameBuffer m_ReadFrame;
};

