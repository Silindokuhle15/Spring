#pragma once
#include <Windows.h>
#include "OpenGLFrameBuffer.h"

class Win32Window
{
public:
	HDC m_Hdc;
	HGLRC m_Hrc;
	HWND m_Hwnd;

	void CreateOpenGLContext();
	void DestroyOpenGLContext();
	void SetUpForRendering();

	void SwapBuffer();

	void OnUpdate();

	long long milliseconds_now();

	// Custom Window Procedure
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	Win32Window();
	~Win32Window();

private:
	const char* CLASS_NAME = "Main Window Class";
	HINSTANCE m_Handle;

	// FRAMEBUFFERS
	OpenGLFrameBuffer m_DrawFrame;
	OpenGLFrameBuffer m_ReadFrame;
};