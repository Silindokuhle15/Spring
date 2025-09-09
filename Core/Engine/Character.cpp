#include "Character.h"

Character Character::GetCharacterPtr(entt::entity id, Scene* pscene)
{
	return Character{ id, pscene };
}
