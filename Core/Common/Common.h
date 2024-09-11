#pragma once
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Timestep.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <set>
#include <random>
#include <string>
#include <memory>

typedef struct
{
	glm::vec3 pos;
	glm::vec2 tex;
	float  ID;
	glm::vec3 norm;
} Vertex;

typedef enum
{
	TRIANGLE_STRIP, WIRE_FRAME
} PrimitiveMode;
enum class VertexAttrib
{
	Position, TexCoord, ID, Normal
};

enum class GameState
{
	LOADING = 0,
	RUNNING = 1,
	PAUSED = 2,
	EXIT = 3
};