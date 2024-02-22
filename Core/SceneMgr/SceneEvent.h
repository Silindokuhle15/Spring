#pragma once
#include <vector>

enum class EventID 
{
	None = 0,
	A = 0x41,
	D = 0x44,
	Q = 0x51,
	S = 0x53,
	W = 0x57,
	Z = 0x5A	
};

template <class EventClass>
class Event
{
public:
	bool m_Resolved;
	virtual bool Resolve() = 0;
	Event() : m_Resolved{false}{}
};

class SceneEvent : public Event<SceneEvent>
{
public:
	EventID m_EventID;
	SceneEvent() :m_EventID{ EventID::None } {};
	SceneEvent(EventID id) : m_EventID{ id } {}
	bool Resolve() override;

	operator EventID() const { return m_EventID; }

	bool operator!= (const SceneEvent& other)
	{
		return !(m_EventID == other.m_EventID);
	}
private:

};

class SceneEventQueue
{
public:
	std::vector<SceneEvent> m_Queue;
	SceneEventQueue() {};

	operator std::vector<SceneEvent>() const { return m_Queue; }
	void Push(SceneEvent& value)
	{
		m_Queue.push_back(value);
	}

	void OnUpdate()
	{
		m_Queue.erase(std::remove_if(m_Queue.begin(), m_Queue.end(), [](auto& event) { return event.m_Resolved; }), m_Queue.end());
	}
};