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
	glm::vec3 pos;
	glm::vec2 tex;
	float  ID;
	glm::vec3 norm;
} Vertex;

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

enum class GameState
{
	LOADING = 0,
	RUNNING = 1,
	PAUSED = 2,
	EXIT = 3
};

std::vector<std::string> getWords(const std::string& s, const char* delim);