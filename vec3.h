#pragma once
#include <initializer_list>
class vec3
{
private:
	float m_value[3];
	vec3() = default;

public:
	vec3(float x, float y, float z) :
		m_value{x,y,z} {}// Constructor;
	~vec3() {} // Destruto

	float getX() const;
	float getY() const;
	float getZ() const;

	//Some useful Overloads
};

float vec3::getX() const { return m_value[0]; }
float vec3::getY() const { return m_value[1]; }
float vec3::getZ() const { return m_value[2]; }