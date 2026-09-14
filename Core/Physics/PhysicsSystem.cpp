#include "PhysicsSystem.h"
#include "Scene.h"

void physics::PhysicsSystem::AddBVBoundEntry(const entt::entity& entity, const physics::PhysicsState& physics_state, const primitives::Bound3D& bound)
{
	uint32_t bits = 21;
	auto& pos = physics_state.position;
	auto& orientation = physics_state.orientation;

	glm::vec3 localMin(bound.xMin, bound.yMin, bound.zMin);
	glm::vec3 localMax(bound.xMax, bound.yMax, bound.zMax);
	glm::vec3 center = (localMin + localMax) * 0.5f;
	glm::vec3 halfExtents = (localMax - localMin) * 0.5f;
	glm::vec3 worldCenter = pos + orientation * center;
	glm::mat3 R = glm::mat3_cast(orientation);
	glm::mat3 absR;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			absR[i][j] = glm::abs(R[i][j]);
		}
	}
	glm::vec3 worldHalfExtents = absR * halfExtents;
	glm::vec3 worldMin = worldCenter - worldHalfExtents;
	glm::vec3 worldMax = worldCenter + worldHalfExtents;
	primitives::Bound3D worldBound
	{
		worldMin.x, worldMin.y, worldMin.z,
		worldMax.x, worldMax.y, worldMax.z,
	};

	auto y = static_cast<uint32_t>(glm::floor((worldMax.x - m_CollisionVolumeSize) / (2.0f * m_CollisionVolumeSize) * glm::pow(2, bits)));
	auto z = static_cast<uint32_t>(glm::floor((worldMax.y - m_CollisionVolumeSize) / (2.0f * m_CollisionVolumeSize) * glm::pow(2, bits)));
	auto x = static_cast<uint32_t>(glm::floor((worldMax.z - m_CollisionVolumeSize) / (2.0f * m_CollisionVolumeSize) * glm::pow(2, bits)));

	auto morton_code = morton_encode_3d32(x, y, z);
	m_BVEntries.push_back(BVNode<primitives::Bound3D>{entity, morton_code, worldBound, nullptr, nullptr});
}

void physics::PhysicsSystem::Update(Scene& scene, float time_step)
{
	m_BVEntries.clear();
	m_NodeBuffer.clear();
	auto physicsStateView = scene.GetView<physics::PhysicsState>();
	for (auto [entity, physicsState] : physicsStateView.each())
	{
		physicsState.position += physicsState.orientation * physicsState.linear_acceleration * time_step;
	}

	auto boundView = scene.GetView< physics::PhysicsState, primitives::Bound3D>();
	for(auto [entity, physicsState, bound] : boundView.each())
	{
		AddBVBoundEntry(entity, physicsState, bound);
	}
	m_NodeBuffer.reserve(131072);
	m_BVHTreeRoot = create_tree<primitives::Bound3D>(m_BVEntries);
	for (auto& bound : m_BVEntries)
	{
		detect_overlapping_bounds<primitives::Bound3D>(bound, m_BVHTreeRoot, m_CollisionPairs, m_NodeBuffer);
	}
}

const std::vector<physics::CollisionDescription>& physics::PhysicsSystem::GetCollisionPairs() const
{
	return m_CollisionPairs;
}
