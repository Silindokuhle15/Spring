
#ifndef _PARTICLE_SYSTEM_PANEL_H_
#define _PARTICLE_SYSTEM_PANEL_H_
#include "Panel.h"
#include <algorithm>

class ParticleSystemPanel : public Panel
{
public:
	std::shared_ptr<Scene> m_ActiveScene;

	void Run() override;
	void ReInstantiateBuffers();
	ParticleSystemPanel() :
		m_PanelName{ "Particle System Panel" }
	{
	}

	ParticleSystemPanel(Layer* parent_layer) :
		m_PanelName{ "Particle System Panel" }
	{
		OnInit(parent_layer);
	}
	~ParticleSystemPanel()
	{
	}
	int selectedParticleSystem = -1;
	bool showParticleEditor = false;
	bool randomOrientation = false;
private:
	std::string m_PanelName;
};


inline void ParticleSystemPanel::Run()
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
				particleSystem.m_EmitterInfo.m_Flags.m_IsEnabled = false;
				particleSystem.m_EmitterInfo.m_Shape = primitives::EmitterShape::POINT;
				particleSystem.m_NumParticles = 0;
				particleSystem.m_MaxNumParticles = 16384;
				particleSystem.m_ParticleRate = 1;
				particleSystem.m_EmitterInfo.m_VectorOne = glm::vec4(0, 0, 0, 0);
				particleSystem.m_EmitterInfo.m_VectorTwo = glm::vec4(0, 0, 0, 0);
				particleSystem.m_Ts = 0.0f;
				particleSystem.m_AccumulatedTime = 0.0f;
				particleSystem.m_Duration = 50.0f;

				std::string combinedShaderPaths = "C:/dev/Spring/Assets/Shaders/ParticleCompute.glsl";
				AssetResource shaderResource{ AssetType::ComputeShaderResource, combinedShaderPaths };
				particleSystem.m_ShaderHandle = assetManager->GetResourceHandle(shaderResource);

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
			int maxNumParticles = ps.m_MaxNumParticles;
			int particleRate = ps.m_ParticleRate;
			float emitterPosition[] = { ps.m_EmitterInfo.m_VectorOne.x, ps.m_EmitterInfo.m_VectorOne.y , ps.m_EmitterInfo.m_VectorOne.z };
			float emitterOrientation[] = { ps.m_EmitterInfo.m_VectorTwo.x, ps.m_EmitterInfo.m_VectorTwo.y, ps.m_EmitterInfo.m_VectorTwo.z };
			float w1 = ps.m_EmitterInfo.m_VectorOne.w;
			float w2 = ps.m_EmitterInfo.m_VectorTwo.w;

			ImGui::Begin("Particle System Editor", &showParticleEditor);
			ImGui::Checkbox("Enabled", &ps.m_EmitterInfo.m_Flags.m_IsEnabled);

			ImGui::Separator();
			ImGui::TextUnformatted("Compute Handle");
			ImGui::DragScalar("HWORD", ImGuiDataType_U64, &ps.m_ShaderHandle.m_HWORD);
			ImGui::DragScalar("LWORD", ImGuiDataType_U64, &ps.m_ShaderHandle.m_LWORD);

			static std::vector<std::string> emitterShapes = { "Point", "Circle",  "Cylinder", "Cone", "Sphere", "Prism"};
			static int selectedShape = 0;

			ImGui::Separator();
			if(ImGui::Button("Emitter"));
			ImGui::SameLine();
			if (ImGui::Button(emitterShapes[selectedShape].c_str()))
			{
				ImGui::OpenPopup("Emitter Shape");
			}
			if (ImGui::BeginPopup("Emitter Shape"))
			{
				for (auto index = 0; index < emitterShapes.size(); index++)
				{
					if (ImGui::Selectable(emitterShapes[index].c_str()))
					{
						//selectedShape = index;
						ps.m_EmitterInfo.m_Shape = (primitives::EmitterShape)index;

					}
				}
				ImGui::EndPopup();
			}

			selectedShape = (int)ps.m_EmitterInfo.m_Shape;

			if (selectedShape == 0)														// Point Emitter
			{
				ImGui::InputFloat3("Position", emitterPosition);
				ImGui::Checkbox("Random Orientation", &ps.m_EmitterInfo.m_Flags.m_Unused1);
				if (ps.m_EmitterInfo.m_Flags.m_Unused1)
				{
					ImGui::SliderFloat3("Ranges", emitterOrientation, 0.0f, 1.0f);
				}
				else
				{
					ImGui::InputFloat3("Orientation", emitterOrientation);
				}
				ps.m_EmitterInfo.m_Shape = primitives::EmitterShape::POINT;

				ps.m_EmitterInfo.m_VectorOne.x = emitterPosition[0];
				ps.m_EmitterInfo.m_VectorOne.y = emitterPosition[1];
				ps.m_EmitterInfo.m_VectorOne.z = emitterPosition[2];
				ps.m_EmitterInfo.m_VectorOne.w = w1;

				ps.m_EmitterInfo.m_VectorTwo.x = emitterOrientation[0];
				ps.m_EmitterInfo.m_VectorTwo.y = emitterOrientation[1];
				ps.m_EmitterInfo.m_VectorTwo.z = emitterOrientation[2];
				ps.m_EmitterInfo.m_VectorTwo.w = w2;
			}

			if (selectedShape == 1)														// Circle Emitter
			{
				ImGui::InputFloat3("Center", emitterPosition);
				ImGui::InputFloat3("Normal", emitterOrientation);
				ImGui::InputFloat("Radius", &w1);
				ImGui::Checkbox("Fill", &ps.m_EmitterInfo.m_Flags.m_Fill);
				ps.m_EmitterInfo.m_Shape = primitives::EmitterShape::CIRCLE;

				ps.m_EmitterInfo.m_VectorOne.x = emitterPosition[0];
				ps.m_EmitterInfo.m_VectorOne.y = emitterPosition[1];
				ps.m_EmitterInfo.m_VectorOne.z = emitterPosition[2];
				ps.m_EmitterInfo.m_VectorOne.w = w1;

				ps.m_EmitterInfo.m_VectorTwo.x = emitterOrientation[0];
				ps.m_EmitterInfo.m_VectorTwo.y = emitterOrientation[1];
				ps.m_EmitterInfo.m_VectorTwo.z = emitterOrientation[2];
				ps.m_EmitterInfo.m_VectorTwo.w = w2;
			}

			if (selectedShape == 2)														// Cylinder Emitter
			{

				ImGui::InputFloat3("Center", emitterPosition);
				ImGui::InputFloat3("Normal", emitterOrientation);
				ImGui::InputFloat("Radius", &w1);
				ImGui::InputFloat("Height", &w2);
				ImGui::Checkbox("Fill", &ps.m_EmitterInfo.m_Flags.m_Fill);
				ps.m_EmitterInfo.m_Shape = primitives::EmitterShape::CYLINDER;

				ps.m_EmitterInfo.m_VectorOne.x = emitterPosition[0];
				ps.m_EmitterInfo.m_VectorOne.y = emitterPosition[1];
				ps.m_EmitterInfo.m_VectorOne.z = emitterPosition[2];
				ps.m_EmitterInfo.m_VectorOne.w = w1;

				ps.m_EmitterInfo.m_VectorTwo.x = emitterOrientation[0];
				ps.m_EmitterInfo.m_VectorTwo.y = emitterOrientation[1];
				ps.m_EmitterInfo.m_VectorTwo.z = emitterOrientation[2];
				ps.m_EmitterInfo.m_VectorTwo.w = w2;
			}

			if (selectedShape == 3)														// Cone Emitter
			{
				ImGui::InputFloat3("Apex", emitterPosition);
				ImGui::InputFloat3("Direction", emitterOrientation);
				ImGui::InputFloat("Half Angle", &w1);
				ImGui::InputFloat("Height", &w2);
				ImGui::Checkbox("Fill", &ps.m_EmitterInfo.m_Flags.m_Fill);
				ps.m_EmitterInfo.m_Shape = primitives::EmitterShape::CONE;

				ps.m_EmitterInfo.m_VectorOne.x = emitterPosition[0];
				ps.m_EmitterInfo.m_VectorOne.y = emitterPosition[1];
				ps.m_EmitterInfo.m_VectorOne.z = emitterPosition[2];
				ps.m_EmitterInfo.m_VectorOne.w = w1;

				ps.m_EmitterInfo.m_VectorTwo.x = emitterOrientation[0];
				ps.m_EmitterInfo.m_VectorTwo.y = emitterOrientation[1];
				ps.m_EmitterInfo.m_VectorTwo.z = emitterOrientation[2];
				ps.m_EmitterInfo.m_VectorTwo.w = w2;
			}

			if (selectedShape == 4)														// Sphere Emitter
			{
				ImGui::InputFloat3("Center", emitterPosition);
				ImGui::InputFloat("Radius", &w1);
				ImGui::Checkbox("Fill", &ps.m_EmitterInfo.m_Flags.m_Fill);
				ps.m_EmitterInfo.m_Shape = primitives::EmitterShape::SPHERE;

				ps.m_EmitterInfo.m_VectorOne.x = emitterPosition[0];
				ps.m_EmitterInfo.m_VectorOne.y = emitterPosition[1];
				ps.m_EmitterInfo.m_VectorOne.z = emitterPosition[2];
				ps.m_EmitterInfo.m_VectorOne.w = w1;

				ps.m_EmitterInfo.m_VectorTwo.x = emitterOrientation[0];
				ps.m_EmitterInfo.m_VectorTwo.y = emitterOrientation[1];
				ps.m_EmitterInfo.m_VectorTwo.z = emitterOrientation[2];
				ps.m_EmitterInfo.m_VectorTwo.w = w2;
			}
			
			float prismLength = 10.0f;
			float prismWidth = 10.0f;
			float prismHeight = 10.0f;
			if (selectedShape == 5)														// Prism Emitter
			{
				ImGui::InputFloat4("Center", emitterPosition);
				ImGui::InputFloat("L", &prismLength);
				ImGui::InputFloat("W", &prismWidth);
				ImGui::InputFloat("H", &prismHeight);

				ps.m_EmitterInfo.m_VectorOne.x = emitterPosition[0];
				ps.m_EmitterInfo.m_VectorOne.y = emitterPosition[1];
				ps.m_EmitterInfo.m_VectorOne.z = emitterPosition[2];
				ps.m_EmitterInfo.m_VectorOne.w = w1;

				ps.m_EmitterInfo.m_VectorTwo.x = emitterOrientation[0];
				ps.m_EmitterInfo.m_VectorTwo.y = emitterOrientation[1];
				ps.m_EmitterInfo.m_VectorTwo.z = emitterOrientation[2];
				ps.m_EmitterInfo.m_VectorTwo.w = w2;
			}

			float initialSpeed = 1.0f;
			float colorOne[] = { 0,0,0, 1 };
			float colorTwo[] = { 0.45f, 0.55f, 0.45f, 1.0f };
			float colorThree[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			static bool editColorOverLifetime = false;
			ImGui::Separator();
			ImGui::TextUnformatted("Particle Lifecycle");
			ImGui::InputInt("Max Particles", &maxNumParticles);
			ImGui::InputInt("Particles per second", &particleRate);
			ImGui::InputFloat("Duration", &ps.m_Duration);
			ImGui::InputFloat("Initial Speed", &initialSpeed);
			if(ImGui::Button("Size over lifetime"));
			if(ImGui::Button("Speed over lifetime"));
			if(ImGui::Checkbox("Color over lifetime", &editColorOverLifetime));
			if(editColorOverLifetime)
			{
				ImGui::ColorEdit4("Color 1", colorOne);
				ImGui::ColorEdit4("Color 2", colorTwo);
				ImGui::ColorEdit4("Color 3", colorThree);
			}
			ImGui::Separator();
			ImGui::TextUnformatted("Stats");
			ImGui::InputInt("Number of Particles", &numParticles);
			ImGui::InputFloat("TimeStep", &ps.m_Ts);
			ImGui::InputFloat("Accumulated Time", &ps.m_AccumulatedTime);

			ImGui::Separator();
			if (ImGui::Button("Re-instantiated Buffers"))
			{
				ReInstantiateBuffers();
			}

			if (numParticles >= 0)
			{
				ps.m_NumParticles = numParticles;
			}

			if (maxNumParticles >= 0)
			{
				ps.m_MaxNumParticles = maxNumParticles;
			}

			if (particleRate >= 0)
			{
				ps.m_ParticleRate = particleRate;
			}
			ImGui::End();
		}
	}
}

inline void ParticleSystemPanel::ReInstantiateBuffers()
{
	/*
	auto& activeScene = m_ActiveScene;
	auto& particleSystem = activeScene->m_ParticleSystems[selectedParticleSystem];

	GLuint numPts = particleSystem.m_MaxNumParticles;
	particleSystem.Positions = (glm::vec4*)glMapNamedBufferRange(particleSystem.PositionBuffer, 0, sizeof(glm::vec4) * numPts, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	particleSystem.Velocities = (glm::vec4*)glMapNamedBufferRange(particleSystem.VelocityBuffer, 0, sizeof(glm::vec4) * numPts, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	particleSystem.LifetimeAndSize = (glm::vec4*)glMapNamedBufferRange(particleSystem.LifetimeAndSizeBuffer, 0, sizeof(glm::vec4) * numPts, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
	particleSystem.Color = (glm::vec4*)glMapNamedBufferRange(particleSystem.ColorBuffer, 0, sizeof(glm::vec4) * numPts, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

	particleSystem.m_AccumulatedTime = 0.0f;
	particleSystem.m_NumParticles = 0;

	static std::mt19937_64 rng(std::random_device{}());

	if (particleSystem.m_EmitterInfo.m_Shape == primitives::EmitterShape::POINT)
	{
		std::uniform_real_distribution<float> rangesX(-particleSystem.m_EmitterInfo.m_VectorTwo.x, particleSystem.m_EmitterInfo.m_VectorTwo.x);
		std::uniform_real_distribution<float> rangesY(-particleSystem.m_EmitterInfo.m_VectorTwo.y, particleSystem.m_EmitterInfo.m_VectorTwo.y);
		std::uniform_real_distribution<float> rangesZ(-particleSystem.m_EmitterInfo.m_VectorTwo.z, particleSystem.m_EmitterInfo.m_VectorTwo.z);

		for (auto index = 0; index < particleSystem.m_MaxNumParticles; index++)
		{
			particleSystem.Positions[index] = particleSystem.m_EmitterInfo.m_VectorOne;
			if (particleSystem.m_EmitterInfo.m_Flags.m_IsEnabled)
			{
				float x = rangesX(rng);
				float y = rangesY(rng);
				float z = rangesZ(rng);
				particleSystem.Velocities[index] = glm::vec4(x, y, z, 0.0);
			}
			else
			{
				particleSystem.Velocities[index] = glm::vec4(
					particleSystem.m_EmitterInfo.m_VectorTwo.x,
					particleSystem.m_EmitterInfo.m_VectorTwo.y,
					particleSystem.m_EmitterInfo.m_VectorTwo.z,
					particleSystem.m_EmitterInfo.m_VectorTwo.w
				);
			}
			particleSystem.LifetimeAndSize[index] = glm::vec4(0.0f, particleSystem.m_Duration, 0.0f, 1.0f);
			particleSystem.Color[index] = glm::vec4(0.0f);
		}
	}

	if (particleSystem.m_EmitterInfo.m_Shape == primitives::EmitterShape::CIRCLE)
	{
		auto radius = particleSystem.m_EmitterInfo.m_VectorOne.w;
		auto center = glm::vec3(particleSystem.m_EmitterInfo.m_VectorOne);
		auto normal = glm::vec3(particleSystem.m_EmitterInfo.m_VectorTwo);

		std::uniform_real_distribution<float> rangesR(-radius, radius);
		std::uniform_real_distribution<float> angle(-glm::pi<float>(), glm::pi<float>());

		glm::vec3 a{ 0 };
		if (normal.x < 0.9)
		{
			a = glm::vec3(1, 0, 0);
		}
		else
		{
			a = glm::vec3(0, 1, 0);
		}
		float v1 = 0.0;
		float v2 = 0.0;

		auto u = glm::cross(normal, a);
		auto v = glm::cross(normal, u);
		for (auto index = 0; index < particleSystem.m_MaxNumParticles; index++)
		{
			if (particleSystem.m_EmitterInfo.m_Flags.m_Fill)
			{
				v1 = rangesR(rng);
				v2 = rangesR(rng);
			}
			else
			{
				float randomAngle = angle(rng);
				v1 = radius * glm::cos(randomAngle);
				v2 = radius * glm::sin(randomAngle);
			}
			auto newPos = center + v1 * u + v2 * v;
			particleSystem.Positions[index] = glm::vec4(newPos, radius);
			particleSystem.Velocities[index] = particleSystem.m_EmitterInfo.m_VectorTwo;
			particleSystem.LifetimeAndSize[index] = glm::vec4(0.0f, particleSystem.m_Duration, 0.0f, 1.0f);
			particleSystem.Color[index] = glm::vec4(0.0f);
		}
	}

	if (particleSystem.m_EmitterInfo.m_Shape == primitives::EmitterShape::CYLINDER)
	{
		auto radius = particleSystem.m_EmitterInfo.m_VectorOne.w;
		auto center = glm::vec3(particleSystem.m_EmitterInfo.m_VectorOne);
		auto height = particleSystem.m_EmitterInfo.m_VectorTwo.w;
		auto normal = glm::vec3(particleSystem.m_EmitterInfo.m_VectorTwo);

		std::uniform_real_distribution<float> rangesR(-radius, radius);
		std::uniform_real_distribution<float> angleDist(-glm::pi<float>(), glm::pi<float>());
		std::uniform_real_distribution<float> heightDist(0.0f, 1.0f);
		float v1 = 0.0f;
		float v2 = 0.0f;
		float h1 = 0.0f;
		glm::vec3 a{ 0 };
		if (normal.x < 0.9)
		{
			a = glm::vec3(1, 0, 0);
		}
		else
		{
			a = glm::vec3(0, 1, 0);
		}
		auto u = glm::cross(normal, a);
		auto v = glm::cross(normal, u);

		for (auto index = 0; index < particleSystem.m_MaxNumParticles; index++)
		{
			if (particleSystem.m_EmitterInfo.m_Flags.m_Fill)
			{
				v1 = rangesR(rng);
				v2 = rangesR(rng);
				h1 = height * heightDist(rng);
			}
			else
			{
				float randomAngle = angleDist(rng);
				v1 = radius * glm::cos(randomAngle);
				v2 = radius * glm::sin(randomAngle);
			}
			auto newPos = center + v1 * u + v2 * v + h1 * normal;
			particleSystem.Positions[index] = glm::vec4(newPos, radius);
			particleSystem.Velocities[index] = particleSystem.m_EmitterInfo.m_VectorTwo;
			particleSystem.LifetimeAndSize[index] = glm::vec4(0.0f, particleSystem.m_Duration, 0.0f, 1.0f);
			particleSystem.Color[index] = glm::vec4(0.0f);
		}
	}

	if (particleSystem.m_EmitterInfo.m_Shape == primitives::EmitterShape::CONE)
	{
		auto halfAngle = particleSystem.m_EmitterInfo.m_VectorOne.w;
		auto apex = glm::vec3(particleSystem.m_EmitterInfo.m_VectorOne);
		auto height = particleSystem.m_EmitterInfo.m_VectorTwo.w;
		auto normal = glm::vec3(particleSystem.m_EmitterInfo.m_VectorTwo);
		std::uniform_real_distribution<float> angleDist(-halfAngle, halfAngle);
		std::uniform_real_distribution<float> radiusDist(-glm::pi<float>(), glm::pi<float>());
		std::uniform_real_distribution<float> heightDist(0,1);

		glm::vec3 a{ 0 };
		if (normal.x < 0.9)
		{
			a = glm::vec3(1, 0, 0);
		}
		else
		{
			a = glm::vec3(0, 1, 0);
		}
		auto u = glm::normalize(glm::cross(normal, a));
		auto v = glm::normalize(glm::cross(normal, u));

		glm::vec3 newPoint{ 0 };
		for (auto index = 0; index < particleSystem.m_MaxNumParticles; index++)
		{
			float randomAngle = angleDist(rng);
			float randomHeight = height * heightDist(rng);
			float angle = 0.0f;
			float planeAngle = radiusDist(rng);
			if (particleSystem.m_EmitterInfo.m_Flags.m_Fill)
			{
				angle = randomAngle;
				float randomRadius = randomHeight * glm::tan(glm::radians(angle));
				auto axisPoint = apex + normal * randomHeight;
				newPoint = axisPoint + randomRadius * (glm::cos(planeAngle) * u + glm::sin(planeAngle) * v);
			}
			else
			{
				angle = halfAngle;
				float randomRadius = randomHeight * glm::tan(glm::radians(angle));
				auto axisPoint = apex + normal * height;
				newPoint = axisPoint + randomRadius * (glm::cos(planeAngle) * u + glm::sin(planeAngle) * v);
			}

			auto velocity = glm::normalize(newPoint - apex);
			particleSystem.Positions[index] = glm::vec4(newPoint, halfAngle);
			particleSystem.Velocities[index] = glm::vec4(velocity, particleSystem.m_EmitterInfo.m_VectorTwo.w);
			particleSystem.LifetimeAndSize[index] = glm::vec4(0.0f, particleSystem.m_Duration, 0.0f, 1.0f);
			particleSystem.Color[index] = glm::vec4(0.0f);
		}

	}

	if (particleSystem.m_EmitterInfo.m_Shape == primitives::EmitterShape::SPHERE)
	{
		auto radius = particleSystem.m_EmitterInfo.m_VectorOne.w;
		auto center = glm::vec3(particleSystem.m_EmitterInfo.m_VectorOne);
		float v1 = 0.0f;
		float v2 = 0.0f;
		std::uniform_real_distribution<float> rangesR(-radius, radius);
		//std::uniform_real_distribution<float> angle(-glm::pi<float>(), glm::pi<float>());
		std::uniform_real_distribution<float> dist01(0.0, 1.0);
		constexpr float M_PI = 3.1415962f;
		for (auto index = 0; index < particleSystem.m_MaxNumParticles; index++)
		{
			glm::vec3 randomOffset{ 0 };
			if (particleSystem.m_EmitterInfo.m_Flags.m_Fill)
			{
				v1 = rangesR(rng);
				v2 = rangesR(rng);
			}
			else
			{
				float u = dist01(rng);
				float u1 = dist01(rng);
				float u2 = dist01(rng);

				// Correct 3D radial distribution
				float d = std::cbrt(u * (radius * radius * radius));
				// Uniform direction on sphere
				float theta = 2.0 * M_PI * u1;
				float z = 2.0 * u2 - 1.0;     // cos(phi)
				float s = std::sqrt(1.0 - z * z);

				float x = d * s * std::cos(theta);
				float y = d * s * std::sin(theta);
				float zc = d * z;
				randomOffset = glm::vec3(x, y, zc);
			}
			particleSystem.Positions[index] = glm::vec4(center + randomOffset, radius);
			particleSystem.Velocities[index] = particleSystem.m_EmitterInfo.m_VectorTwo;
			particleSystem.LifetimeAndSize[index] = glm::vec4(0.0f, particleSystem.m_Duration, 0.0f, 1.0f);
			particleSystem.Color[index] = glm::vec4(0.0f);
		}
	}

	glUnmapNamedBuffer(particleSystem.PositionBuffer);
	glUnmapNamedBuffer(particleSystem.VelocityBuffer);
	glUnmapNamedBuffer(particleSystem.LifetimeAndSizeBuffer);
	glUnmapNamedBuffer(particleSystem.ColorBuffer);
	/**/
}

#endif