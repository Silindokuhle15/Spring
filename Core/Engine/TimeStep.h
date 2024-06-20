#pragma once
class TimeStep
{
private:
	double m_CurrentTime;

public:
	TimeStep() : m_CurrentTime{0} {}
	operator double()  { return m_CurrentTime; }

	double& operator=(const double& val) {
		this->m_CurrentTime = val;
		return this->m_CurrentTime;
	}

};

