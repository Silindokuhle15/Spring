#pragma once
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class PointLight
{
public:

	glm::vec3 m_Position;
	glm::vec3 m_Color;
	unsigned int m_UniformColorLocation;
	float m_Shininess;
	float m_Strength;

public:

	PointLight() :
		m_Position{ glm::vec3(0.0, 0.0, 0.0) },
		m_Color{ glm::vec3(1.0) }, 
		m_UniformColorLocation{0},
		m_Shininess{0},
		m_Strength{0}
	{
		// DO Something here later
	}

	PointLight(glm::vec3 pos, glm::vec3 col) : m_Position{ pos }, m_Color{ col }, m_UniformColorLocation{ 0 },
		m_Shininess{ 0 },
		m_Strength{ 0 }
	{

	}
	void SetPosition(float x, float y, float z) noexcept { m_Position = glm::vec3(x, y, z); }
	void SetPosition(float pos[3]) noexcept { m_Position = glm::vec3(pos[0], pos[1], pos[2]); }
	void SetPosition( glm::vec3 pos) noexcept { m_Position = pos; }
	void SetColor(glm::vec3 col) noexcept { m_Color = col; }
	void SetColor(float col[3]) noexcept { m_Color = glm::vec3(col[0], col[1], col[2]); }
	void SetColor(float x, float y, float z) noexcept { m_Color = glm::vec3(x, y, z); }

	glm::vec3 GetPosition() const { return m_Position; }
	glm::vec3 GetColor() const { return m_Color; }

	unsigned int GetUniformColorLocation() const { return m_UniformColorLocation; }

	void MoveUp() { m_Position += glm::normalize(glm::vec3(.0f, 10.0f, 0.0f)); }
	void MoveDown() { m_Position -= glm::normalize(glm::vec3(.0f, 10.0f, 0.0f)); }
	void MoveRight() { m_Position += glm::normalize(glm::vec3(10.0f, .0f, 0.0f)); }
	void MoveLeft() { m_Position -= glm::normalize(glm::vec3(10.0f, .0f, 0.0f)); }

};