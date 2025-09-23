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

/*
class BVNode2D
{
public:
	uint64_t m_Id;
	Bound2D m_Bounds;
	BVNode2D* m_Left;
	BVNode2D* m_Right;

	BVNode2D(const Bound2D& bounds, uint32_t id, BVNode2D* left, BVNode2D* right);
};

//BVNode2D* create_tree();
//BVNode2D* create_sub_tree(const BVEntry* list, const Bound2D* bounds, uint32_t start, uint32_t end);
//void print_tree(const BVNode2D* node, int depth = 0);


/*
BVNode2D* create_tree()
{


	std::sort(list.begin(), list.end(),
		[](const BVEntry& u, const BVEntry& v)
		{
			return (u.MORTON_CODE == v.MORTON_CODE) ? (u.ID < v.ID) : (u.MORTON_CODE < v.MORTON_CODE);
		});
	return create_sub_tree(list.data(), bounds.data(), 0, static_cast<uint32_t>(list.size() - 1));
}

BVNode2D* create_sub_tree(const BVEntry* list, const Bound2D* bounds, uint32_t start, uint32_t end)
{
	if (start == end)
	{
		uint64_t id = list[start].ID - 1;
		return new BVNode2D(bounds[id], list[start].ID, nullptr, nullptr);
	}
	uint32_t mid = get_split_position(list, start, end);
	BVNode2D* left = create_sub_tree(list, bounds, start, mid);
	BVNode2D* right = create_sub_tree(list, bounds, mid + 1, end);

	const Bound2D& lb = left->m_Bounds;
	const Bound2D& rb = right->m_Bounds;

	Bound2D merged =
	{
		std::min(lb.xMin, rb.xMin),
		std::min(lb.yMin, rb.yMin),
		std::max(lb.xMax, rb.xMax),
		std::max(lb.yMax, rb.yMax)
	};
	return new BVNode2D(merged, static_cast<uint64_t>(-1), left, right);
}

void print_tree(const BVNode2D* node, int depth)
{
	if (!node) return;
	for (int i = 0; i < depth; ++i)
		std::cout << "  ";

	std::cout << "Node ID: " << node->m_Id
		<< " | Bounds: [" << node->m_Bounds.xMin << "," << node->m_Bounds.yMin
		<< "] - [" << node->m_Bounds.xMax << "," << node->m_Bounds.yMax << "]\n";

	print_tree(node->m_Left, depth + 1);
	print_tree(node->m_Right, depth + 1);
}
*/
/*
BVNode2D::BVNode2D(const Bound2D& bounds, uint32_t id, BVNode2D* left, BVNode2D* right) :
	m_Bounds{ bounds },
	m_Id{ id },
	m_Left{ left },
	m_Right{ right }
{
}

*/