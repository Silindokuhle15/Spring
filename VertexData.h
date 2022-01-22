#pragma once
#include "color.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class VertexData
{
private:
	std::vector<glm::vec3> m_positions;
	std::vector<glm::vec3> m_colors;
	glm::vec3 m_Position;
	color::color m_Color;
public:
	VertexData(const glm::vec3& t_pos, const color::color& t_col) :
		m_Position{ t_pos }, m_Color{ t_col }{} // Constructor
	~VertexData() {} // Destructor...
	explicit VertexData():
		m_Position{0.0f, 0.0f, 0.f}, m_Color{ color::WHITE }
	{
		// Nothing to do here
	}
	VertexData(const std::string line) : m_Position{}, m_Color{ color::WHITE }
	{
		std::string TempX = line.substr(1, 10);
		std::string TempY = line.substr(11, 20);
		std::string TempZ = line.substr(21, 30);

		float x = atof(TempX.c_str());
		float y = atof(TempY.c_str());
		float z = atof(TempZ.c_str());
		this->m_Position = glm::vec3(x, y, z);
	}
	VertexData(const float x, const float y, const float z, const color::color t_Color) :
		m_Position{ glm::vec3(x,y,z) }, m_Color{t_Color}
	{
		// Nothing to do there for now
	}
};

enum class VertexAttribute
{
	POSITION = 0, COLOR = 1
};