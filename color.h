#pragma once
#include "glm/glm.hpp"

namespace color {

	class color : glm::vec3
	{
	public:
		color() :glm::vec3() {}
		color(float x, float y, float z) : glm::vec3(x, y, z) {} // Constructor...
		~color() {} // Destructor
	public:
		// Just the constants I wanna use...
	};

	const color RED(1.0f, 0.0f, 0.0f);
	const color GREEN(0.0f, 1.0f, 0.0f);
	const color BLUE(0.0f, 0.0f, 1.0f);
	const color BLACK (0.0f, 0.0f, 0.0f);
	const color WHITE(1.0f, 0.0f, 0.0f);
	const color GRAY(0.5f, 0.5f, 0.5f);

}