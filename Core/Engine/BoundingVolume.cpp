#include "BoundingVolume.h"

const uint64_t BoundingVolume::GetID() const
{
	return m_ID;
}

void BoundingVolume::SetID(uint64_t id)
{
	m_ID = id;
}

const glm::vec3 BoundingVolume::GetMin() const
{
	return m_Min;
}

void BoundingVolume::SetMin(const glm::vec3& min)
{
	m_Min = min;
}

const glm::vec3 BoundingVolume::GetMax() const
{
	return m_Max;
}

void BoundingVolume::SetMax(const glm::vec3& max)
{
	m_Max = max;
}

BoundingVolume::BoundingVolume(uint64_t id, const glm::vec3& min, const glm::vec3& max):
	m_ID{id},
	m_Min{min},
	m_Max{max}
{
}

BoundingVolume BoundingVolume::CreateBoundingVolume(uint64_t id, const glm::vec3& centroid, float size)
{
	glm::vec3 min{ centroid - glm::vec3(0.5f) };
	glm::vec3 max{ centroid + glm::vec3(0.5f) };
	return BoundingVolume(id, min, max);
}
