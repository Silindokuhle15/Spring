#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <cstdint>
#include "TimeStep.h"

template <class WindowClass>
class BaseWindowClass
{
public:
	HWND m_Hwnd;
	HINSTANCE m_Handle;

	TimeStep m_StartTime;
	TimeStep m_EndTime;

	virtual void OnUpdate() = 0;
	virtual void StartTimer() = 0;
	virtual void EndTimer() = 0;
	virtual void SwapBuffer() = 0;

	BOOL CreateWin32Window(uint64_t width, uint64_t height, const char* app_name)
	{
		m_Handle = GetModuleHandle(NULL);
		const char CLASS_NAME[] = "Win32Window";
		WNDCLASS wc = { };
		wc.lpfnWndProc = WindowClass::WindowProcedure;
		wc.hInstance = m_Handle;
		wc.lpszClassName = CLASS_NAME;
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

		
		RECT rect{};
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

		auto idth = GetSystemMetrics(SM_CXSCREEN);
		auto eight = GetSystemMetrics(SM_CYSCREEN);
		RegisterClass(&wc);
		m_Hwnd = CreateWindowEx(
			0, // Optional window styles.
			CLASS_NAME, // Window class
			app_name, // Window text
			WS_OVERLAPPEDWINDOW, // Window style
			// Size and position
			0, 0, static_cast<int>(rect.right), static_cast<int>(rect.bottom),
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