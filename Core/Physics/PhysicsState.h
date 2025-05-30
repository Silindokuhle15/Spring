#pragma once
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Component.h"
namespace physics
{
	typedef struct
	{
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
	} physics_state;

	class PhysicsState : public Component, public physics_state
	{
	public:
		physics::physics_state& operator ()() 
		{
			return *this;
		}
		void OnInit() override;
		/*
		Component& operator () () override
		{
			return *this;
		}
		*/
	};
}