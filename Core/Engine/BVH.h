#pragma once

#include <cstddef>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <new>
#include <algorithm>
#include <iostream>
#include <glm/glm.hpp>

template<typename T>
class BVHArenaAllocator
{
private:
	std::vector<void*> m_Blocks;
	void* m_CurrentBlock = nullptr;
	size_t m_Capacity;
	size_t m_Size;

public:
	explicit BVHArenaAllocator(size_t initial_capacity = 1024) :
		m_Capacity{initial_capacity},
		m_Size{0}
	{
		allocate_block(m_Capacity);
	}
	~BVHArenaAllocator()
	{
		for (void* block : m_Blocks)
		{
			std::free(block);
		}
	}

	BVHArenaAllocator(const BVHArenaAllocator&) = delete;
	BVHArenaAllocator& operator=(const BVHArenaAllocator&) = delete;

	template<typename... Args>
	T* allocate(Args&... args)
	{
		if (m_Size >= m_Capacity)
		{
			grow();
		}

		void* addr = static_cast<char*>(m_CurrentBlock) + m_Size + sizeof(T);
		T* obj = new(addr)T(std::forward<Args>(args)...);
		++m_Size;
		return obj;
	}

	void reset()
	{
		m_Size = 0;
		m_CurrentBlock = m_Blocks.front();
	}

private:
	void allocate_block(size_t capacity)
	{
		void* block = std::malloc(sizeof(T) * capacity);
		assert(block && "BVHArenaAllocator: Allocation failed");

		m_Blocks.push_back(block);
		m_CurrentBlock = block;
	}

	void grow()
	{
		m_Capacity *= 2;
		allocate_block(m_Capacity);
		m_Size = 0;
	}
};

// UTILITY FUNCTIONS

uint32_t part1by1_16(uint16_t n);
uint64_t part1by1_32(uint32_t n);
uint64_t part1by2_32(uint32_t n);
uint32_t morton_encode_2d16(uint16_t x, uint16_t y);
uint64_t morton_encode_2d32(uint32_t x, uint32_t y);
uint64_t morton_encode_3d32(uint32_t x, uint32_t y, uint32_t z);
uint32_t count_leading_zeros(uint32_t n);

struct BVEntry
{
	uint64_t ID = 0;
	uint64_t MORTON_CODE = 0xFFFFFFFFFFFFFFFFULL;
};

struct Bound2D
{
	uint64_t xMin{ 0 },  yMin{ 0 }, xMax{ 0 }, yMax{ 0 };
	static Bound2D merge(const Bound2D& u, const Bound2D& v)
	{
		return
		{
			std::min(u.xMin, v.xMin),
			std::min(u.yMin, v.yMin),
			std::max(u.xMax, v.xMax),
			std::max(u.yMax, v.yMax),
		};
	}
	static float unit_product(const Bound2D& u)
	{
		return static_cast<float>((u.xMax - u.xMin) * (u.yMax - u.yMin ));
	}
	static bool AABBIntersection(const Bound2D& a, const Bound2D& b)
	{
		bool intersect_x = a.xMax >= b.xMin &&
			a.xMin <= b.xMin;

		bool intersect_y = a.yMax >= b.yMin &&
			a.yMin <= b.yMax;
		return intersect_x && intersect_y;
	}
	static Bound2D intersection(const Bound2D& a, const Bound2D& b)
	{
		return
		{
			std::max(a.xMin, b.xMin),
			std::max(a.yMin, b.yMin),
			std::min(a.xMax, b.xMax),
			std::min(a.yMax, b.yMax)
		};
	}
	static void print(const Bound2D& bound)
	{
		std::cout
			<< "| Bounds: [" << bound.xMin << "," << bound.yMin
			<< "] - [" << bound.xMax << "," << bound.yMax << "]\n";
	}
};
struct Bound3D
{
	float xMin{ 0 }, yMin{ 0 }, zMin{ 0 };
	float xMax{ 0 }, yMax{ 0 }, zMax{ 0 };
	static Bound3D merge(const Bound3D& u, const Bound3D& v)
	{
		return
		{
			std::min(u.xMin, v.xMin),
			std::min(u.yMin, v.yMin),
			std::min(u.zMin, v.zMin),
			std::max(u.xMax, v.xMax),
			std::max(u.yMax, v.yMax),
			std::max(u.zMax, v.zMax)
		};
	}
	static float unit_product(const Bound3D& a)
	{
		glm::vec3 xx{ a.xMax - a.xMin, 0.0f, 0.0f };
		glm::vec3 yy{ 0.0f, a.yMax - a.yMin, 0.0f };
		glm::vec3 zz{ 0.0f, 0.0f, a.zMax - a.zMin };
		return glm::dot(xx, glm::cross(yy, zz));
	}
	static bool AABBIntersection(const Bound3D& a, const Bound3D& b)
	{
		bool intersect_x = a.xMax >= b.xMin &&
			a.xMin <= b.xMin;

		bool intersect_y = a.yMax >= b.yMin &&
			a.yMin <= b.yMax;

		bool intersect_z = a.zMax >= b.zMin &&
			a.zMin <= b.zMax;

		return intersect_x && intersect_y && intersect_z;
	}
	static Bound3D intersection(const Bound3D& a, const Bound3D& b)
	{
		return
		{
			std::max(a.xMin, b.xMin),
			std::max(a.yMin, b.yMin),
			std::max(a.zMin, b.zMin),
			std::min(a.xMax, b.xMax),
			std::min(a.yMax, b.yMax),
			std::min(a.zMax, b.zMax)
		};
	}
	static void print(const Bound3D& bound)
	{
		std::cout
			<< "| Bounds: [" << bound.xMin << "," << bound.yMin << "," << bound.zMin
			<< "] - [" << bound.xMax << "," << bound.yMax << ","  << bound.zMax << "]\n";
	}
};

template<typename T>
class BVNode
{
public:
	uint64_t m_Id;
	T m_Bounds;
	BVNode* m_Left;
	BVNode* m_Right;
	BVNode(const T& bounds, uint64_t id, BVNode* left, BVNode* right):
		m_Id{id},
		m_Bounds{bounds},
		m_Left{left},
		m_Right{right}
	{
	}
	static void print(const BVNode<T>* node)
	{
		std::cout << "Node ID: " << node->m_Id;
		T::print(node->m_Bounds);
	}
};
uint64_t get_split_position(const BVEntry* list, uint64_t start, uint64_t end);

template<typename U>
void print_tree(const BVNode<U>* node, int depth = 0)
{
	if (!node) return;
	for (int i = 0; i < depth; ++i)
		std::cout << "  ";

	BVNode<U>::print(node);
	print_tree<U>(node->m_Left, depth + 1);
	print_tree<U>(node->m_Right, depth + 1);
}

template<typename U>
BVNode<U>* create_sub_tree(const std::vector<BVEntry> list, const std::vector<U>& bounds, uint64_t start, uint64_t end)
{
	if (start == end)
	{
		uint64_t id = list[start].ID;
		assert(id < bounds.size());
		return new BVNode<U>(bounds[id], list[start].ID, nullptr, nullptr);
	}
	uint64_t mid = get_split_position(list.data(), start, end);
	BVNode<U>* left = create_sub_tree(list, bounds, start, mid);
	BVNode<U>* right = create_sub_tree(list, bounds, mid + 1, end);

	const U& lb = left->m_Bounds;
	const U& rb = right->m_Bounds;

	U merged = U::merge(lb, rb);

	return new BVNode<U>(merged, static_cast<uint64_t>(-1), left, right);
}

template<typename U>
BVNode<U>* create_tree(std::vector<BVEntry>& list, const std::vector<U>& bounds)
{
	std::sort(
		list.begin(), list.end(),
		[](const BVEntry& u, const BVEntry& v) { return (u.MORTON_CODE == v.MORTON_CODE) ? (u.ID < v.ID) : (u.MORTON_CODE < v.MORTON_CODE); }
	);
	return create_sub_tree<U>(list, bounds, 0, static_cast<uint64_t>(list.size() - 1));
}

template<typename U>
std::vector<uint64_t> detect_overlapping_bounds(const U& bound, const BVNode<U>* tree_node)
{
	if (!tree_node)
		return std::vector<uint64_t>();

	auto treeBounds = tree_node->m_Bounds;
	auto merge = U::merge(bound, treeBounds);
	auto intersection = U::intersection(bound, treeBounds);
	auto mergeProduct = U::unit_product(merge);
	auto intersectionProduct = U::unit_product(intersection);
	float IoU = intersectionProduct / mergeProduct;

	std::vector<uint64_t> totalIntersections;

	if ((0.0f < IoU) && (IoU <= 1.0f) && U::AABBIntersection(bound, treeBounds))
	{
		if (tree_node->m_Left)
		{
			auto leftIntersections = detect_overlapping_bounds(bound, tree_node->m_Left);
			totalIntersections.insert(totalIntersections.end(), leftIntersections.begin(), leftIntersections.end());
		}

		if (tree_node->m_Right)
		{
			auto rightIntersections = detect_overlapping_bounds(bound, tree_node->m_Right);
			totalIntersections.insert(totalIntersections.end(), rightIntersections.begin(), rightIntersections.end());
		}

		if (!tree_node->m_Left && !tree_node->m_Right)
		{
			totalIntersections.push_back(tree_node->m_Id);
		}
	}
	return totalIntersections;
}