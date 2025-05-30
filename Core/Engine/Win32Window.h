#pragma once
#include "BaseWin32Window.h"
#include <string>

class Win32Window : public BaseWindowClass<Win32Window>
{
public:
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
	Win32Window(uint64_t width, uint64_t height, const char* name, bool show = true);
	~Win32Window();


	void SetWidth(int64_t width);
	void SetHeight(int64_t height);
	void SetTitle(std::string title);
	void StartTimer() override {
		m_StartTime = static_cast<float>(milliseconds_now());
	}
	void EndTimer() override {
		m_EndTime = static_cast<float>(milliseconds_now());
	}


protected:
	long long milliseconds_now();
	int64_t m_Width;
	int64_t m_Height;
	std::string m_Title;

private:


};

