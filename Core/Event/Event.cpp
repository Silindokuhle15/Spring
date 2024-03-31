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

bool MouseMove::Resolve()
{ 
	m_Resolved = true;
	return m_Resolved;
}
