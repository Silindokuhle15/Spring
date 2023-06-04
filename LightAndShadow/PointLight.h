#pragma once
#include "glm.hpp"
#include "ext.hpp"

class PointLight
{
public:

	glm::vec3 m_Position;
	glm::vec4 m_Color;

	unsigned int m_UniformColorLocation;

	float m_Shininess;
	float m_Strength;

public:

	PointLight() :
		m_Position{ glm::vec3(0.0, 0.0, 0.0) },
		m_Color{ glm::vec4(1.0) }
	{
		// DO Something here later
	}
	void SetPosition(const glm::vec3 pos) noexcept { m_Position = pos; }
	void SetColor(const glm::vec4 col) noexcept { m_Color = col; }

	glm::vec3 GetPosition() const { return m_Position; }
	glm::vec3 GetColor() const { return m_Color; }

	unsigned int GetUniformColorLocation() const { return m_UniformColorLocation; }

	void MoveUp() { m_Position += glm::normalize(glm::vec3(.0f, 10.0f, 0.0f)); }
	void MoveDown() { m_Position -= glm::normalize(glm::vec3(.0f, 10.0f, 0.0f)); }
	void MoveRight() { m_Position += glm::normalize(glm::vec3(10.0f, .0f, 0.0f)); }
	void MoveLeft() { m_Position -= glm::normalize(glm::vec3(10.0f, .0f, 0.0f)); }

};