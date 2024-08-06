#include "Ray.h"

const glm::vec3& Ray::GetStart() const
{
	// TODO: insert return statement here
	return m_Start;
}

const glm::vec3& Ray::GetEnd() const
{
	// TODO: insert return statement here
	return m_Start + glm::vec3(m_Length * m_Direction.x, m_Length * m_Direction.y, m_Length * m_Direction.z);
}

void Ray::SetDirection(const glm::vec3& dir)
{
	m_Direction = dir;
}

Ray::Ray(const glm::vec3& start, const glm::vec3& direction, const double& len):
	m_Start{start},
	m_Direction{direction},
	m_Length{0.0}
{
}

Ray::Ray(const glm::vec3& start, const glm::vec3& end) :
	m_Start{start},
	m_Direction{glm::normalize(end - start)},
	m_Length{glm::sqrt(glm::dot(end, start))}
{
}
