#include "PhysicsState.h"
namespace physics
{
    physics::CollisionPairDescription::CollisionPairDescription(const AssetHandle& first, const AssetHandle& second) :
        m_First{ first },
        m_Second{ second }
    {
    }

}
