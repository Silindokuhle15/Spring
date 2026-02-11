#include "Event.h"
namespace event
{
	int64_t event::MouseMoveEvent::GetX()const
	{
		return m_X;
	}
	int64_t event::MouseMoveEvent::GetY()const
	{
		return m_Y;
	}

	Dispatcher& event::Dispatcher::Get()
	{
		static Dispatcher globalDispatcher;
		return globalDispatcher;
	}

	void event::Dispatcher::Dispatch(Event& event)
	{
		auto& globalDispatcher = Get();
		for (auto& listener : globalDispatcher.m_EventListeners)
		{
			if (event::Event::GetType(event) == EventID::MOUSEMOVE)
			{
				listener->OnMouseMove(*(MouseMoveEvent*)&event);
			}
			if (event::Event::GetType(event) == EventID::SPACE)
			{
				listener->OnKeyPress(*(KeyPressEvent*)&event);
			}
		}
	}

	void event::Dispatcher::RegisterListener(IEventListener& listener)
	{
		auto& globalDispatcher = Get();
		globalDispatcher.m_EventListeners.push_back(&listener);
	}

	void event::IEventListener::OnWindowClose(WindowCloseEvent& window_close)
	{
	}

	void event::IEventListener::OnWindowResize(WindowResizeEvent& window_resize)
	{
	}

	void event::IEventListener::OnMouseMove(MouseMoveEvent& mouse_move)
	{
	}

	void event::IEventListener::OnMousePress(MousePressEvent& mouse_press)
	{
	}

	void event::IEventListener::OnKeyPress(KeyPressEvent& key_press)
	{
	}

}