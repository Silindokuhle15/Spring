#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <cstdint>

constexpr const char* CLASS_NAME = "Win32Window";
template <class WindowClass>
class BaseWindowClass
{
public:
	HINSTANCE m_ApplicationHandle;
	HWND m_WindowHandle;
	virtual void OnUpdate() = 0;
	virtual void SwapBuffer() = 0;

	bool RegisterWindowClass()
	{
		WNDCLASS wc = { };
		wc.lpfnWndProc = WindowClass::WindowProcedure;
		wc.hInstance = m_ApplicationHandle;
		wc.lpszClassName = CLASS_NAME;
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

		if (!RegisterClass(&wc))
		{
			return false;
		}
		return true;
	}
	bool UnRegisterWindowClass()
	{
		if (!UnregisterClass(CLASS_NAME, m_ApplicationHandle))
		{
			return false;
		}
		return true;
	}

	BOOL CreateWin32Window(uint32_t width, uint32_t height, const char* app_name)
	{
		m_ApplicationHandle = GetModuleHandle(NULL);
		if (!RegisterWindowClass())
		{
			return FALSE;
		}
		//auto width = GetSystemMetrics(SM_CXSCREEN);
		//auto height = GetSystemMetrics(SM_CYSCREEN);
		RECT rect{};
		auto windowStyle = WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
		m_WindowHandle = CreateWindowEx(
			0,
			CLASS_NAME,
			app_name,
			windowStyle,
			0, 0, static_cast<int>(rect.right), static_cast<int>(rect.bottom),
			NULL,
			NULL,
			m_ApplicationHandle,
			this // Additional application data
		);
		if (!m_WindowHandle)
		{
			return FALSE;
		}
		return TRUE;
	}
	static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WindowClass* pWindowComponent = nullptr;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pWindowComponent = (WindowClass*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pWindowComponent);
			pWindowComponent->m_WindowHandle = hwnd;
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

	BaseWindowClass() : m_ApplicationHandle{ NULL }, m_WindowHandle{ NULL } {}
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};