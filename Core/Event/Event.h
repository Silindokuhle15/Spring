#pragma once
#include <cstdint>
#include <vector>
namespace event
{
	enum class EventID
	{
		None = 0,
		WINDOW_RESIZE,
		WINDOW_CLOSE,
		// MOUSE EVENTS
		MOUSE_PRESS,
		MOUSEMOVE,
		MOUSEWHEEL,

		// KEYBOARD EVENTS
		ESCAPE,
		SPACE,
		UP,
		RIGHT,
		DOWN,
		LEFT,

		ZERO = 0x30,
		ONE = 0x31,
		TWO = 0x32,
		THREE = 0x33,
		FOUR = 0x34,
		FIVE = 0x35,
		SIX = 0x36,
		SEVEN = 0x37,
		EIGHT = 0x38,
		NINE = 0x39,

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
		EventID  m_ID;
	public:
		bool m_Handled;
		EventID GetID() const { return m_ID; }
		static EventID GetType(Event& event)
		{
			return event.m_ID;
		}
	protected:
		Event(EventID id = EventID::None) : m_ID{ id }, m_Handled{ false } {}
	};

	enum class MouseButton
	{
		LEFT, MIDDLE, RIGHT
	};

	class MousePressEvent : public Event
	{
	public:
		MousePressEvent(int64_t x, int64_t y, MouseButton button) :
			Event{ EventID::MOUSE_PRESS },
			m_X{ x }, m_Y{ y }, m_Button{ button }
		{
		}
	private:
		int64_t m_X, m_Y;
		MouseButton m_Button;
	};

	class MouseMoveEvent : public Event
	{
	public:
		int64_t GetX() const;
		int64_t GetY() const;
		MouseMoveEvent(int64_t x, int64_t y) :
			Event{ EventID::MOUSEMOVE },
			m_X{ x }, m_Y{ y }
		{
		}
	private:
		int64_t m_X, m_Y;
	};

	class MouseWheelEvent : public Event
	{
	public:
		MouseWheelEvent(int64_t delta = 0) :
			Event{ EventID::MOUSEWHEEL },
			m_WheelDelta{ delta }
		{
		}
		int64_t m_WheelDelta;
	};

	class WindowResizeEvent : public Event
	{
	public:
		uint64_t m_NewWidth;
		uint64_t m_NewHeight;
		WindowResizeEvent(uint64_t new_width, uint64_t new_height) :
			Event{ EventID::WINDOW_RESIZE },
			m_NewWidth{ new_width },
			m_NewHeight{ new_height }
		{
		}
	};
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() :
			Event{ EventID::WINDOW_CLOSE }
		{
		}
	};
	class KeyPressEvent : public Event
	{
	public:
		KeyPressEvent(EventID event_id) :
			Event(event_id)
		{

		}
	};

	class IEventListener
	{
	protected:
		explicit IEventListener()
		{
		}
	public:
		virtual void OnWindowClose(WindowCloseEvent& window_close);
		virtual void OnWindowResize(WindowResizeEvent& window_resize);
		virtual void OnMouseMove(MouseMoveEvent& mouse_move);
		virtual void OnMousePress(MousePressEvent& mouse_press);
		virtual void OnKeyPress(KeyPressEvent& key_press);
	};

	class Dispatcher 
	{
	private:
		Dispatcher():
			m_EventListeners{}
		{
		}
		std::vector<event::IEventListener*> m_EventListeners;
	public:
		static Dispatcher& Get();
		static void Dispatch(Event& event);
		static void RegisterListener(IEventListener& listener);

	};
}