#ifndef _RENDER_PANEL_H_
#define _RENDER_PANEL_H_
#include "Panel.h"

class RenderPanel : public Panel
{
public:
	std::shared_ptr<Scene> m_ActiveScene;

	void Run() override;
	void ReInstantiateBuffers();
	RenderPanel() :
		m_PanelName{ "Render Panel" }
	{
	}

	RenderPanel(Layer* parent_layer) :
		m_PanelName{ "Render Panel" }
	{
		OnInit(parent_layer);
	}
	~RenderPanel()
	{
	}
	int selectedParticleSystem = -1;
	bool showParticleEditor = false;
private:
	std::string m_PanelName;
};


inline void RenderPanel::Run()
{
	auto& activeScene = m_ActiveScene;
	if (activeScene != nullptr)
	{
		ImGui::Begin(m_PanelName.c_str());

		// ---------- Header ----------
		ImGui::TextUnformatted("Particle Systems");
		ImGui::SameLine();
		if (ImGui::SmallButton("+"))
		{
			ImGui::OpenPopup("AddSystemPopup");
		}

		ImGui::Separator();

		// ---------- System list ----------
		for (int i = 0; i < activeScene->m_ParticleSystems.size(); i++)
		{
			ImGui::PushID(i);
			char buffer[32] = "";
			sprintf(buffer, "Particle System %d", i);
			if (ImGui::Selectable(buffer, selectedParticleSystem == i))
			{
				selectedParticleSystem = i;
				showParticleEditor = true;
			}

			ImGui::PopID();
		}

		// ---------- Right-click trigger (no popup body here) ----------
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
			ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("AddSystemPopup");
		}

		// ---------- SINGLE popup definition ----------
		if (ImGui::BeginPopup("AddSystemPopup"))
		{
			if (ImGui::MenuItem("Add Particle System"))
			{
				auto assetManager = m_ActiveScene->GetAssetManager();
				primitives::ParticleSystem particleSystem;
				particleSystem.m_IsEnabled = false;
				particleSystem.m_NumParticles = 16384;
				particleSystem.m_EmitterPosition = glm::vec4(0, 0, -7.5, 1);
				particleSystem.m_EmitterOrientation = glm::quat(0, 0, 1, 0);
				particleSystem.m_Ts = 0.0f;
				particleSystem.m_AccumulatedTime = 0.0f;
				particleSystem.m_Duration = 20.0f;

				std::string combinedShaderPaths = "C:/dev/Spring/Assets/Shaders/ParticleCompute.glsl";
				AssetResource shaderResource{ AssetType::ComputeShaderResource, combinedShaderPaths };
				particleSystem.m_ShaderHandle = assetManager->GetResourceHandle(shaderResource);

				GLuint bufferSize = sizeof(glm::vec4) * particleSystem.m_NumParticles;
				glCreateBuffers(1, &particleSystem.PositionBuffer);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSystem.PositionBuffer);
				glNamedBufferData(particleSystem.PositionBuffer, bufferSize, nullptr, GL_DYNAMIC_DRAW);

				glCreateBuffers(1, &particleSystem.VelocityBuffer);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSystem.VelocityBuffer);
				glNamedBufferData(particleSystem.VelocityBuffer, bufferSize, nullptr, GL_DYNAMIC_DRAW);

				activeScene->m_ParticleSystems.push_back(particleSystem);
				selectedParticleSystem =
					(int)activeScene->m_ParticleSystems.size() - 1;
				showParticleEditor = true;

				ReInstantiateBuffers();
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		// ---------- Editor window ----------
		if (showParticleEditor && selectedParticleSystem >= 0)
		{
			auto& ps = activeScene->m_ParticleSystems[selectedParticleSystem];
			int numParticles = ps.m_NumParticles;
			ImGui::Begin("Particle System Editor", &showParticleEditor);
			ImGui::Checkbox("Enabled", &ps.m_IsEnabled);
			ImGui::InputInt("Number of Particles", &numParticles);
			ps.m_NumParticles = numParticles;
			float emitterPosition[] = { ps.m_EmitterPosition.x, ps.m_EmitterPosition.y , ps.m_EmitterPosition.z, 1.0f };
			float emitterOrientation[] = { ps.m_EmitterOrientation.w, ps.m_EmitterOrientation.x, ps.m_EmitterOrientation.y, ps.m_EmitterOrientation.z };
			ImGui::TextUnformatted("Emitter");
			ImGui::InputFloat4("Position", emitterPosition);
			ImGui::InputFloat4("Orientation", emitterOrientation);
			char buffer[256] = "";
			sprintf(buffer, "HWORD: %d LWORD %d", (int)ps.m_ShaderHandle.m_HWORD, (int)ps.m_ShaderHandle.m_LWORD);
			ImGui::TextUnformatted(buffer);
			ImGui::InputFloat("TimeStep", &ps.m_Ts);
			ImGui::InputFloat("Accumulated Time", &ps.m_AccumulatedTime);
			ImGui::InputFloat("Duration", &ps.m_Duration);
			ImGui::Separator();
			if (ImGui::Button("Re-instantiated Buffers"))
			{
				ReInstantiateBuffers();
			}

			ps.m_EmitterPosition.x = emitterPosition[0];
			ps.m_EmitterPosition.y = emitterPosition[1];
			ps.m_EmitterPosition.z = emitterPosition[2];
			ps.m_EmitterPosition.w = emitterPosition[3];
			ps.m_EmitterOrientation.w = emitterOrientation[0];
			ps.m_EmitterOrientation.x = emitterOrientation[1];
			ps.m_EmitterOrientation.y = emitterOrientation[2];
			ps.m_EmitterOrientation.z = emitterOrientation[3];
			ImGui::End();
		}
	}
}

inline void RenderPanel::ReInstantiateBuffers()
{
	auto& activeScene = m_ActiveScene;
	auto& particleSystem = activeScene->m_ParticleSystems[selectedParticleSystem];

	GLuint numPts = particleSystem.m_NumParticles;
	particleSystem.Positions = (glm::vec4*)glMapNamedBufferRange(particleSystem.PositionBuffer, 0, sizeof(glm::vec4) * numPts, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	particleSystem.Velocities = (glm::vec4*)glMapNamedBufferRange(particleSystem.VelocityBuffer, 0, sizeof(glm::vec4) * numPts, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	particleSystem.m_AccumulatedTime = 0.0f;

	GLuint nx = 32;
	GLuint ny = 32;
	GLuint nz = 16;
	auto emitterPosition = glm::vec3(
		particleSystem.m_EmitterPosition.x,
		particleSystem.m_EmitterPosition.y,
		particleSystem.m_EmitterPosition.z
	);
	float radius = 5.0f;
	glm::vec3 center = emitterPosition;
	for (GLuint z = 0; z < nz; z++)
	{
		for (GLuint y = 0; y < ny; y++)
		{
			for (GLuint x = 0; x < nx; x++)
			{
				GLuint index = x + y * nx + z * nx * ny;

				// normalized [0,1]
				float fx = (nx > 1) ? float(x) / (nx - 1) : 0.5f;
				float fy = (ny > 1) ? float(y) / (ny - 1) : 0.5f;
				float fz = (nz > 1) ? float(z) / (nz - 1) : 0.5f;

				// spherical coordinates
				float theta = fx * glm::two_pi<float>(); // 0..2pi
				float phi = fy * glm::pi<float>();     // 0..pi

				// volume-uniform radius
				//float r = radius * cbrtf(fz);
				float r = radius;

				glm::vec3 p;
				p.x = r * sin(phi) * cos(theta);
				p.y = r * sin(phi) * sin(theta);
				p.z = r * cos(phi);

				particleSystem.Positions[index] = glm::vec4(center + p, 1.0f);
				particleSystem.Velocities[index] = glm::vec4(0.0f);
			}
		}
	}

	glUnmapNamedBuffer(particleSystem.PositionBuffer);
	glUnmapNamedBuffer(particleSystem.VelocityBuffer);
}

#endif