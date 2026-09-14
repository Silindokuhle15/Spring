#ifndef _PHYSICS_SYSTEM_H_
#define _PHYSICS_SYSTEM_H_
#include <entt.hpp>
#include <vector>
#include "Bound.h"
#include "PhysicsState.h"
#include "CollisionDescription.h"
#include "BVH.h"
class Scene;
namespace physics {
	class PhysicsSystem
	{
		virtual void AddBVBoundEntry(const entt::entity& entity, const physics::PhysicsState& physics_state, const primitives::Bound3D& bound);
	public:
		void Update(Scene& scene, float time_step);
		const std::vector<physics::CollisionDescription>& GetCollisionPairs() const;
	private:
		std::vector<BVNode<primitives::Bound3D>> m_BVEntries;
		std::vector<const BVNode<primitives::Bound3D>*> m_NodeBuffer;
		float m_CollisionVolumeSize = 1024.0f;
		BVNode<primitives::Bound3D>* m_BVHTreeRoot = nullptr;
		std::vector<physics::CollisionDescription> m_CollisionPairs;
	};
}
#endif // !_PHYSICS_SYSTEM_H_
