#pragma once
#include "vec3.h"

namespace color {

	class color : public vec3
	{
	public:
		color(float x, float y, float z) : vec3(x, y, z) {} // Constructor...
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