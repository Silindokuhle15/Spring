#pragma once
#include <entt.hpp>
#include "Scene.h"

class Character
{
public : 
	Character(entt::entity entity, Scene * p_scene):
		m_Entity{entity},
		m_pScene{p_scene}
	{
		if (!m_pScene)
		{
			throw std::runtime_error("Character:Character with null scene !!!");
		}
	}

	template<typename T, typename... Args>
	T& AddComponent(Args && ...args )
	{
		if (!m_pScene)
		{
			throw std::runtime_error("AddComponent On Invalid Character !!!");
		}
		 return m_pScene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent()
	{
		if (!m_pScene)
		{
			throw std::runtime_error("GetComponent On Invalid Character !!!");
		}
		return m_pScene->m_Registry.get<T>(m_Entity);
	}

	template<typename T>
	bool HasComponent()
	{
		if (!m_pScene)
		{
			throw std::runtime_error("HasComponent On Invalid Character !!!");
		}
		return m_pScene->m_Registry.all_of<T>(m_Entity);
	}

	template<typename T>
	auto RemoveComponent()
	{
		if (!m_pScene)
		{
			throw std::runtime_error("RemoveComponent On Invalid Character !!!");
		}
		return m_pScene->m_Registry.remove<T>(m_Entity);
	}

	entt::entity& GetCharacterID() { return m_Entity; }
	static Character GetCharacterPtr(entt::entity id, Scene* pscene);
	Scene* GetScenePointer() const { return m_pScene; }
private:
	entt::entity m_Entity;
	Scene* m_pScene;
};
