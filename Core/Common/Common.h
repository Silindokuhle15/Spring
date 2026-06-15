#pragma once
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Timestep.h"
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>
#include <tuple>
#include <set>
#include <random>
#include <string>
#include <memory>
#include <sstream>

typedef struct
{
	double x;
	double y;
	double z;
}Vector3;

typedef struct
{
	double x;
	double y;
	double z;
	double w;
}Vector4;

typedef enum
{
	TRIANGLE_STRIP, WIRE_FRAME
} PrimitiveMode;
enum class ShaderDataType
{
	None = 0, Bool, Float, Float2, Float3, Float4, Mat2, Mat3, Mat4
};

struct LayoutInfo
{
	ShaderDataType Type;
	std::string Name;
};

