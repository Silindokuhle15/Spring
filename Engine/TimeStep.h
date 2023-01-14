#pragma once
class TimeStep
{
private:
	float m_CurrentTime;

public:
	TimeStep() : m_CurrentTime{0} {}
	operator float() const { return m_CurrentTime; }

};

