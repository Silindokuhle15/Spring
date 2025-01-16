#pragma once
class TimeStep
{
private:
	float m_CurrentTime;

	
public:

	TimeStep(float ts = 0.0f) : m_CurrentTime{0} {}
	operator float()  { return m_CurrentTime; }

	float& operator=(const float& val) {
		this->m_CurrentTime = val;
		return this->m_CurrentTime;
	}

	TimeStep operator + (TimeStep& other)
	{
		return TimeStep(m_CurrentTime + other.m_CurrentTime);
	}

};

