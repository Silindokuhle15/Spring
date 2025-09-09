#include <entt.hpp>
#include "Scene.h"

class Character
{
public : 
	Character(entt::entity entity, Scene * p_scene):
		m_Entity{entity},
		m_pScene{p_scene}
	{
	}

	template<typename T, typename... Args>
	T& AddComponent(Args && ...args )
	{
		 return m_pScene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent()
	{
		return m_pScene->m_Registry.get<T>(m_Entity);
	}

	template<typename T>
	bool HasComponent()
	{
		return m_pScene->m_Registry.all_of<T>(m_Entity);
	}

	template<typename T>
	T& RemoveComponent()
	{
		return m_pScene->m_Registry.remove<T>(m_Entity);
	}

	entt::entity GetCharacterID() const { return m_Entity; }
	static Character GetCharacterPtr(entt::entity id, Scene* pscene);

private:
	entt::entity m_Entity;
	Scene* m_pScene;
};
