#pragma once
#include "Common.h"
#include "Component.h"
#include "Script.h"
#include "PhysicsState.h"

class Character
{
public:
	virtual void OnInit();
	virtual void OnUpdate(TimeStep ts);
	template<typename U>
	void AddComponent(U* component)
	{
		const std::type_info& info = typeid(U*);
		std::string type_name = info.name();				
		m_Components[type_name] = (Component*)(component);
	}

	template<typename U> 
	U* GetComponent()
	{
		const std::type_info& info = typeid(U*);
		std::string key = info.name();
		if (m_Components.contains(key))
		{
			return dynamic_cast<U*>(m_Components[key]);
		}
		return dynamic_cast<U*>(m_Components[key]);
	}

	Character() {}
	~Character() {}
private:
	std::unordered_map<std::string,  Component*> m_Components;
};
using CharacterPtr = std::shared_ptr<Character>;