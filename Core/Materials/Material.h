#pragma once
#include "glm/glm.hpp"

class Material
{
public:
	uint64_t m_MaterialID;
	float m_d;
	float m_illum;
	glm::vec3 m_Ka;
	float m_Ni;
	glm::vec3 m_Kd;
	float m_Ns;
	glm::vec3 m_Ks;
	float unused_1;
;

	void OnInit();
	void OnUpdate();

	Material(uint64_t id=0xFFFFFFFF) :
		m_MaterialID{ id },
		m_d{},
		m_illum{},
		m_Ka{},
		m_Kd{},
		m_Ks{},
		m_Ni{},
		m_Ns{},
		unused_1{}
	{

	}
};