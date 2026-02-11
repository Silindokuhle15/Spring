#ifndef _PHYSICS_STATE_H_
#define _PHYSICS_STATE_H_
#include "UUID.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <iostream>

namespace physics
{
	class PhysicsState
	{
	public:
		glm::quat orientation;
		glm::vec3 position;
		float mass;
		glm::vec3 velocity;
		float restitution;
		glm::vec3 linear_acceleration;
		float unused_1;
		glm::vec3 angular_acceleration;
		float unused_2;
		glm::vec3 inertia;
		float unused_3;
	};

	class CollisionPairDescription
	{
	public:
		AssetHandle m_First;
		AssetHandle m_Second;

		CollisionPairDescription(const AssetHandle& first, const AssetHandle& second);
	};

}

#endif