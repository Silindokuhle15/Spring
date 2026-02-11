#include "Serializer.h"

void Serializer::Serialize(Scene* scene)
{
	std::ofstream ofs("Test.lua", std::ios::out);
	if (!ofs.is_open())
	{
		std::cerr << "Failed to Create File" << std::endl;
		return;
	}
	ofs << "particle_systems = {" << std::endl;

	auto numParticleSystems = scene->m_ParticleSystems.size();
	for (auto index = 0; index < numParticleSystems; index++)
	{
		auto& particleSystem = scene->m_ParticleSystems[index];
		ofs << "\t{" << std::endl;
		ofs << "\t\t" << particleSystem.m_IsEnabled << "," << std::endl;
		ofs << "\t\t" << particleSystem.m_NumParticles << "," << std::endl;
		ofs << "\t\t{" << particleSystem.m_EmitterPosition.x << ", " << particleSystem.m_EmitterPosition.y << ", " << particleSystem.m_EmitterPosition.z << ", " << particleSystem.m_EmitterPosition.w << "}," << std::endl;
		ofs << "\t\t{" << particleSystem.m_EmitterOrientation.x << ", " << particleSystem.m_EmitterOrientation.y << ", " << particleSystem.m_EmitterOrientation.z << ", " << particleSystem.m_EmitterOrientation.w << "}," << std::endl;
		ofs << "\t\t{" << particleSystem.m_ShaderHandle.m_HWORD << ", " << particleSystem.m_ShaderHandle.m_LWORD << "}," << std::endl;
		ofs << "\t\t" << particleSystem.m_Ts << "," << std::endl;
		ofs << "\t\t" << particleSystem.m_AccumulatedTime << "," << std::endl;
		ofs << "\t\t" << particleSystem.m_Duration << std::endl;
		if (index < numParticleSystems - 1)
		{
			ofs << "\t}," << std::endl;
		}
		else
		{
			ofs << "\t}" << std::endl;
		}
	}
	ofs << "}" << std::endl;
	ofs.close();
}

void Serializer::Deserilize(const char* file_path)
{
}
