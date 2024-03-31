#pragma once
#include "Event.h"
#include <vector>

class SceneEvent : public Event
{
public:

	SceneEvent(EventID id = EventID::None) 
	{
		m_ID = id;
		m_Resolved = false;
	}
	bool Resolve() override;
	operator EventID() const { return m_ID; }

	bool operator!= (const SceneEvent& other)
	{
		return !(m_ID == other.m_ID);
	}
private:

};

class SceneEventQueue
{
public:
	std::vector<Event*> m_Queue;
	SceneEventQueue() {};

	operator std::vector<Event*>() const { return m_Queue; }
	void Push(Event* value)
	{
		m_Queue.push_back(value);
	}

	void OnUpdate()
	{
		m_Queue.erase(std::remove_if(m_Queue.begin(), m_Queue.end(), [](auto& event) { return event->m_Resolved; }), m_Queue.end());
	}
};