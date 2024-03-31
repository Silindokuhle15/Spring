#pragma once
#include <cstdint>

enum class EventID
{
	None = 0,
	// MOUSE EVENTS
	L_MOUSEDOWN,
	M_MOUSEDOWN,
	R_MOUSEDOWN,
	MOUSEWHEEL,

	// KEYBOARD EVENTS
	A = 0x41,
	D = 0x44,
	Q = 0x51,
	S = 0x53,
	W = 0x57,
	Z = 0x5A
};

class Event
{
public:
	EventID  m_ID;
	bool m_Resolved;
	virtual bool Resolve() = 0;
	Event(EventID id = EventID::None) : m_ID{ id }, m_Resolved { false } {}
};

enum class MouseButton
{
	LEFT, MIDDLE, RIGHT
};

class MouseButtonDown : public Event
{
public:
	MouseButtonDown(int32_t x, int32_t y, MouseButton button) : m_X{ x }, m_Y{ y }, m_Button{ button } 
	{
		switch (button)
		{
		case MouseButton::LEFT:
			m_ID = EventID::L_MOUSEDOWN;
			break;
		case MouseButton::MIDDLE:
			m_ID = EventID::M_MOUSEDOWN;
			break;		
		case MouseButton::RIGHT:
			m_ID = EventID::R_MOUSEDOWN;
			break;
		default:
			m_ID = EventID::L_MOUSEDOWN;
		}
	}

	bool Resolve() override;
private:
	int32_t m_X, m_Y;
	MouseButton m_Button;
};

class MouseMove : public Event
{
public:
	MouseMove(int32_t x, int32_t y) :m_X{ x }, m_Y{ y } {}
	bool Resolve() override;
private:
	int32_t m_X, m_Y;
};

class MouseWheel : public Event
{
public:
	MouseWheel(int delta = 0 ) : m_WheelDelta{ delta } 
	{ 
		m_ID = EventID::MOUSEWHEEL;
	}
	bool Resolve() override;
	int32_t m_WheelDelta;
};