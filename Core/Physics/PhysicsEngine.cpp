#include "PhysicsEngine.h"
// I WILL NEED TO RE WRITE THIS
void physics::PhysicsEngine::ImpulseResolveCollision()
{
	auto scale_vector = [&](glm::vec3& vec, double scale)
		{
			return glm::vec3(vec.x* scale, vec.y* scale, vec.z* scale);
		};

	auto ApplyLinearImpuse = [&](PhysicsState& ob, glm::vec3& force)
		{
			return ob.velocity = ob.velocity + scale_vector(force, 1.0f / ob.mass);
			
		};

	auto ApplyAngularImpuse = [&](PhysicsState& ob, glm::vec3& force)
		{
			float x = ob.inertia.x;
			float y = ob.inertia.y;
			float z = ob.inertia.z;

			return ob.velocity = ob.velocity + glm::vec3(force.x * x, force.y * y, force.z * z);
		};

	auto ImpuseResovefunc = [&](PhysicsState& obj_a, PhysicsState& obj_b, glm::vec3& contact_point) {

		// FIND THE COLLISION NORMAL
		glm::vec3 normal = glm::normalize(obj_a.position - obj_b.position);
		// CALCULATE THE RELATIVE VELOCITY
		glm::vec3 relative_a = contact_point - obj_a.position;
		glm::vec3 relative_b = contact_point - obj_b.position;

		glm::vec3 angular_vel_a = glm::cross(obj_a.velocity, relative_a);
		glm::vec3 angular_vel_b = glm::cross(obj_b.velocity, relative_b);

		glm::vec3 fullVelocityA = obj_a.velocity + angular_vel_a;
		glm::vec3 fullVelocityB = obj_b.velocity + angular_vel_b;

		glm::vec3 contact_velocity = fullVelocityB - fullVelocityA;

		glm::vec3 inertiaA = glm::cross(
			obj_a.inertia * glm::cross(relative_a, normal), relative_a);

		glm::vec3 inertiaB = glm::cross(
			obj_b.inertia * glm::cross(relative_b, normal), relative_b);

		double totalMass = 1.0f / obj_a.mass + 1.0f / obj_b.mass;

		double impuseForce = glm::dot(contact_point, normal);

		double angularEffect = glm::dot(inertiaA + inertiaB, normal);

		double restitution = 0.66f;

		double impulse = (-1.0f + restitution) * impuseForce / (totalMass + angularEffect);

		glm::vec3 fullImpuse = { normal.x * impulse, normal.y * impulse, normal.z * impulse };

		glm::vec3 negative_impuse = scale_vector(fullImpuse, -1.0f);

		ApplyLinearImpuse(obj_a, negative_impuse);
		ApplyLinearImpuse(obj_b, fullImpuse);


		glm::vec3 var = glm::cross(relative_a, negative_impuse);
		glm::vec3 foo = glm::cross(relative_b, fullImpuse);

		ApplyAngularImpuse(obj_a, var);
		ApplyAngularImpuse(obj_b, foo);

		};
}

void physics::PhysicsEngine::OnInit()
{
}

void physics::PhysicsEngine::OnUpdate(TimeStep ts)
{
	/*
	for (size_t index = 0; index < m_PhysicsObjects.size(); index++)
	{
		auto& physics_state = m_PhysicsObjects[index].second;
		auto new_x = 0.5*physics_state.linear_acceleration.x * ts*ts;
		auto new_y = 0.5*physics_state.linear_acceleration.y * ts*ts;
		auto new_z = 0.5*physics_state.linear_acceleration.z * ts*ts;
		physics_state.position = physics_state.position + glm::vec3(new_x, new_y, new_z);

		float rotation_rate = ts * 1.0f;
		//physics_state.orientation = physics_state.orientation + glm::rotate(physics_state.orientation, glm::radians(rotation_rate), physics_state.angular_acceleration);
		
	}
	*/
	m_Collisions.clear();

	for (size_t u_index = 0; u_index < m_PhysicsObjects.size(); u_index++)
	{
		for (size_t v_index = m_PhysicsObjects.size() - 1; v_index > 0; v_index--)
		{
			if (!(u_index == v_index))
			{
				auto& u = m_PhysicsObjects[u_index];
				auto& v = m_PhysicsObjects[v_index];

				auto intersection = Intersect(u, v);

				if (intersection)
				{
					// DON'T KNOW WHAT TO DO HERE RIGHT NOW
					auto var = m_Collisions.find(std::make_pair(v_index, u_index));
					if (var == m_Collisions.end())
					{
						m_Collisions.insert(std::make_pair(u_index, v_index));
					}
				}
			}
		}
	}
}
