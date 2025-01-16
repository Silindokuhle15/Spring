#pragma once
#include "Collider.h"
#include <unordered_map>


namespace physics
{
	class CollisionDescription
	{
	public:
		glm::quat m_Collision;
	};
	class PhysicsEngine
	{
	public:
		std::vector<std::pair<uint64_t, uint64_t>> m_Collisions;
		std::vector<std::pair<Collider<GenericCollider>, PhysicsState*>> m_PhysicsObjects;
		void ImpulseResolveCollision();
		template<typename T, typename U>
		bool Intersect(std::pair<T, U>& first, std::pair<T, U>& second);
		template<class T>
		void AddCollider(const Collider<T>& other, PhysicsState* state);
		void RemoveCollider(uint64_t index);
		void OnInit();
		void OnUpdate(TimeStep ts);

		~PhysicsEngine()
		{
			if (!(m_PhysicsObjects.empty()))
			{
				m_PhysicsObjects.clear();
			}

			if (!(m_Collisions.empty()))
			{
				m_Collisions.clear();
			}
		}
	};

	inline void PhysicsEngine::RemoveCollider(uint64_t index)
	{
		uint64_t current_index = 0;
		for (auto physics_object = m_PhysicsObjects.begin(); physics_object != m_PhysicsObjects.end(); physics_object++)
		{
			if (current_index == index)
			{
				m_PhysicsObjects.erase(physics_object);
				m_PhysicsObjects.shrink_to_fit();
				break;
			}
			current_index++;
		}
	}

	template<typename T, typename U>
	inline bool PhysicsEngine::Intersect(std::pair<T, U>& first, std::pair<T, U>& second)
	{
		return false;
	}

	template<>
	inline bool PhysicsEngine::Intersect(std::pair<Collider<GenericCollider>, PhysicsState*>& obj_1, std::pair<Collider<GenericCollider>, PhysicsState*>& obj_2)
	{
		auto& first_collider = obj_1.first;
		auto first_state = obj_1.second;
		auto& second_collider = obj_2.first;
		auto second_state = obj_2.second;

		obj_1.first.FindMinMax(*first_state);
		obj_2.first.FindMinMax(*second_state);
		
		double proj_sum_x = (first_collider.proj_x + second_collider.proj_x);
		double proj_sum_y = (first_collider.proj_y + second_collider.proj_y);
		double proj_sum_z = (first_collider.proj_z + second_collider.proj_z);

		double max_sum_x = (first_collider.max_x - second_collider.min_x) >= (second_collider.max_x - first_collider.min_x) ? (first_collider.max_x - second_collider.min_x) : (second_collider.max_x - first_collider.min_x);
		double max_sum_y = (first_collider.max_y - second_collider.min_y) >= (second_collider.max_y - first_collider.min_y) ? (first_collider.max_y - second_collider.min_y) : (second_collider.max_y - first_collider.min_y);
		double max_sum_z = (first_collider.max_z - second_collider.min_z) >= (second_collider.max_z - first_collider.min_z) ? (first_collider.max_z - second_collider.min_z) : (second_collider.max_z - first_collider.min_z);

		bool intersect_x = { (first_collider.min_x <= second_collider.min_x && first_collider.max_x >= second_collider.min_x) || (second_collider.min_x <= first_collider.min_x && second_collider.max_x >= first_collider.min_x) };
		bool intersect_y = { (first_collider.min_y <= second_collider.min_y && first_collider.max_y >= second_collider.min_y) || (second_collider.min_y <= first_collider.min_y && second_collider.max_y >= first_collider.min_y) };
		bool intersect_z = { (first_collider.min_z <= second_collider.min_z && first_collider.max_z >= second_collider.min_z) || (second_collider.min_z <= first_collider.min_z && second_collider.max_z >= first_collider.min_z) };

		return (intersect_x && intersect_y && intersect_z);
		return false;
	}
	template<class T>
	inline void PhysicsEngine::AddCollider(const Collider<T>&, PhysicsState* state)
	{

	}
	template<>
	inline void PhysicsEngine::AddCollider(const Collider<GenericCollider>& other, PhysicsState* state)
	{
		m_PhysicsObjects.push_back(std::make_pair(other, state));
	}
}