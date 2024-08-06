#pragma once
#include "glm/glm.hpp"

namespace physics
{
	typedef struct
	{
		double mass;
		double restitution;
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 acceleration;
		glm::vec3 inertia;
	} PhysicsState;
}