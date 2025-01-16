#pragma once
#include "glm/glm.hpp"

class Ray
{
public:

	const glm::vec3 GetStart() const;
	const glm::vec3 GetEnd() const;
	const glm::vec3 GetDirection() const;
	void SetDirection(const glm::vec3& dir);
	void SetStart(const glm::vec3& start);
	Ray() :
		m_Start{ glm::vec3(0.0) },
		m_Direction{ glm::vec3(0,0,1) },
		m_Length{ 200.0 }
	{

	}
	Ray(const glm::vec3& start, const glm::vec3& direction, const double& len);
	Ray(const glm::vec3& start, const glm::vec3& end);
private:
	glm::vec3 m_Start, m_Direction;
	double m_Length;
};