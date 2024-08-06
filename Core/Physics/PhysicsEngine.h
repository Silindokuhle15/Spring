#pragma once
#include "Collider.h"

namespace physics
{
	class PhysicsEngine
	{
	public:

		std::vector<Collider<GenericCollider>> m_CollerObjects;
		std::vector<PhysicsState> m_PhysicsObjects;
		void ImpulseResolveCollision();
		template<class T>
		void AddCollider(const Collider<T>& other, PhysicsState& state);
		void RemoveCollider(uint64_t index);
		void OnInit();
		void OnUpdate(TimeStep ts);
	};

	inline void PhysicsEngine::RemoveCollider(uint64_t index)
	{
		if (index > m_CollerObjects.size())
		{
			return;
		}
		m_CollerObjects.erase(m_CollerObjects.begin() + index);
	}

	template<class T>
	inline void PhysicsEngine::AddCollider(const Collider<T>&, PhysicsState& state)
	{

	}
	template<>
	inline void PhysicsEngine::AddCollider(const Collider<GenericCollider>& other, PhysicsState& state)
	{
		m_CollerObjects.push_back(other);
		m_PhysicsObjects.push_back(state);
	}
}