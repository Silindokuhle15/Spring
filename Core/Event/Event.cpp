#include "Event.h"

bool MouseWheel::Resolve()
{
	m_WheelDelta = 0;
	m_Resolved = true;
	return m_Resolved;
}

bool MouseButtonDown::Resolve()
{
	m_Resolved = true;
	return m_Resolved;
}

const int64_t MouseMove::GetX() const
{
	return m_X;
}

const int64_t MouseMove::GetY() const
{
	return m_Y;
}

bool MouseMove::Resolve()
{ 
	m_Resolved = true;
	return m_Resolved;
}
