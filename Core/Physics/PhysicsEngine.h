#pragma once
#include "Collider.h"


namespace physics
{
	typedef struct
	{
		double mass;
		double restitution;
		glm::vec3 position;
		glm::vec3 linear_velocity;
		glm::vec3 angular_velocity;
		glm::vec3 inertia;
	} PhysicsObject;


	class PhysicsEngine
	{
	public:

		std::vector<GenericCollider> m_CollerObjects;

		void ImpulseResolveCollision();
		void OnInit();
		void OnUpdate(float ts);
	};
}