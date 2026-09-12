#ifndef _COLLISION_DESCRIPTION_H_
#define _COLLISION_DESCRIPTION_H_
#include <entt.hpp>
namespace physics
{
	struct CollisionDescription
	{
		entt::entity entityA;
		entt::entity entityB;
	};
}
#endif // !_COLLISION_DESCRIPTION_H_
