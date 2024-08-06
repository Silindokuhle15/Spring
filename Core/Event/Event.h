#pragma once
#include <cstdint>

enum class EventID
{
	None = 0,
	// MOUSE EVENTS
	L_MOUSEDOWN,
	M_MOUSEDOWN,
	R_MOUSEDOWN,
	MOUSEMOVE,
	MOUSEWHEEL,

	// KEYBOARD EVENTS
	ESCAPE,
	SPACE,
	UP,
	RIGHT,
	DOWN,
	LEFT,

	A = 0x41,
	B = 0x42,
	C = 0x43,
	D = 0x44,
	E = 0x45,
	F = 0x46,
	G = 0x47,
	H = 0x48,
	I = 0x49,
	J = 0x4A,
	K = 0x4B,
	L = 0x4C,
	M = 0x4D,
	N = 0x4E,
	O = 0x4F,
	P = 0x50,
	Q = 0x51,
	R = 0x52,
	S = 0x53,
	T = 0x54,
	U = 0x55,
	V = 0x56,
	W = 0x57,
	X = 0x58,
	Y = 0x59,
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
	MouseButtonDown(int64_t x, int64_t y, MouseButton button) : m_X{ x }, m_Y{ y }, m_Button{ button } 
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
	int64_t m_X, m_Y;
	MouseButton m_Button;
};

class MouseMove : public Event
{
public:
	const int64_t GetX() const;
	const int64_t GetY() const;
	MouseMove(int64_t x, int64_t y) :m_X{ x }, m_Y{ y }
	{
		m_ID = EventID::MOUSEMOVE;
	}
	bool Resolve() override;
private:
	int64_t m_X, m_Y;
};

class MouseWheel : public Event
{
public:
	MouseWheel(int64_t delta = 0 ) : m_WheelDelta{ delta } 
	{ 
		m_ID = EventID::MOUSEWHEEL;
	}
	bool Resolve() override;
	int64_t m_WheelDelta;
};