#ifndef _PARTICLE_SYSTEM_H
#define _PARTICLE_SYSTEM_H
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "UUID.h"
#include "Range.h"

namespace primitives
{
	enum class EmitterShape
	{
		POINT,
		CIRCLE,
		CYLINDER,
		CONE,
		SPHERE,
		PRISM
	};

	struct EmitterFlags
	{
		bool m_IsEnabled;
		bool m_Fill;
		bool m_Unused1;
		bool m_RandomOrientation;
	};

	struct EmitterInfo
	{
		EmitterShape m_Shape;
		EmitterFlags m_Flags;
		glm::vec4 m_VectorOne;								//{{x, y, z}, {radius/half-angle/length}}
		glm::vec4 m_VectorTwo;								//{{dx, dy, dz}, height}
	};

	/*/
	struct ParticleSystem
	{
		EmitterInfo m_EmitterInfo;
		uint32_t m_BufferOffset;
		uint32_t m_NumParticles;
		uint32_t m_MaxNumParticles;
		uint32_t m_ParticleRate;

		float m_Ts;
		float m_AccumulatedTime;
		float m_Duration;
		float m_InitialSpeed;

		AssetHandle m_ShaderHandle;
		AssetHandle m_TextureHandle;
	};
	/**/
	struct ParticleSystemConfig
	{
		EmitterInfo m_EmitterInfo;
		uint32_t m_ParticleRate;
		uint32_t m_MaxNumParticles;
		float m_Duration;
		float m_InitialSpeed;
		AssetHandle m_ShaderHandle;
		AssetHandle m_TextureHandle;
	};

	struct ParticleSystemInstance
	{
		bool m_IsEnabled;
		bool m_Restart;
		primitives::Range m_Allocation;
		uint32_t m_ConfigIndex;
		uint32_t m_NumParticles;
		float m_AccumulatedTime;
		float m_Duration;
	};
}

#endif