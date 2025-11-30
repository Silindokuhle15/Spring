#pragma once
#include "BaseWin32Window.h"
#include <string>
#include "Event.h"

class Win32Window : public BaseWindowClass<Win32Window>
{
public:
	HDC m_Hdc;
	HGLRC m_Hrc;
	TimeStep m_Ts;

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
	const uint64_t GetWidth() const;
	void SetHeight(int64_t height);
	const uint64_t GetHeight() const;
	void SetTitle(std::string title);

	void (*OnEvent)(event::Event& event);

	uint64_t StartTimer() override {
		m_StartTime = static_cast<uint64_t>(milliseconds_now());
		return m_StartTime;
	}
	uint64_t EndTimer() override {
		m_EndTime = static_cast<uint64_t>(milliseconds_now());
		return m_EndTime;
	}
protected:
	long long milliseconds_now();
	uint64_t m_Width;
	uint64_t m_Height;
	std::string m_Title;
private:
};

