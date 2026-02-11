#ifndef _PARTICLE_SYSTEM_H
#define _PARTICLE_SYSTEM_H
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "UUID.h"

namespace primitives
{
	struct ParticleSystem
	{
		bool m_IsEnabled;
		uint64_t m_NumParticles;
		glm::vec4 m_EmitterPosition;
		glm::quat m_EmitterOrientation;
		AssetHandle m_ShaderHandle;
		float m_Ts;
		float m_AccumulatedTime;
		float m_Duration;

		uint32_t LifetimeAndSizeBuffer;
		uint32_t PositionBuffer;
		uint32_t VelocityBuffer;
		uint32_t ColorBuffer;

		glm::vec4* LifetimeAndSize = nullptr;
		glm::vec4* Positions = nullptr;
		glm::vec4* Velocities = nullptr;
		glm::vec4* Color = nullptr;
	};
}

#endif