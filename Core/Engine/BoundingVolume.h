#pragma once
#include "glm/glm.hpp"

class BoundingVolume
{
private:
	uint64_t m_ID;
	glm::vec3 m_Min;
	glm::vec3 m_Max;
public:
	const uint64_t GetID() const;
	void SetID(uint64_t id);
	const glm::vec3 GetMin() const;
	void SetMin(const glm::vec3& min);
	const glm::vec3 GetMax() const;
	void SetMax(const glm::vec3& max);

	BoundingVolume(uint64_t id, const glm::vec3& min, const glm::vec3& max);

public:
	static BoundingVolume CreateBoundingVolume(uint64_t id, const glm::vec3& centroid, float size);
};