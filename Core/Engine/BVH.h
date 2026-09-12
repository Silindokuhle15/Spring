#ifndef _BVH_H_
#define _BVH_H_
#include <cassert>
#include <iostream>
#include <glm/glm.hpp>
#include "Bound.h"
#include "Allocator.h"
#include "Morton.h"
#include "CollisionDescription.h"

struct BVEntry
{
	entt::entity ID;
	uint64_t MORTON_CODE = 0xFFFFFFFFFFFFFFFFULL;
public:
	BVEntry(const entt::entity& entity, uint64_t morton_code) :
		ID{entity},
		MORTON_CODE{ morton_code }
	{

	}
	bool operator<(const BVEntry& other) const
	{
		return MORTON_CODE < other.MORTON_CODE;
	}
};

template<typename U>
float unit_product(const U& u);

template<typename U>
bool AABBIntersection(const U& u, const U& v);

template<typename U>
U merge(const U& u, const U& v);

template<typename U>
U intersection(const U& u, const U& v);

template<typename T>
class BVNode
{
public:
	BVEntry m_MortonCode;
	T m_Bounds;
	BVNode* m_Left;
	BVNode* m_Right;
	BVNode(const entt::entity& entity , uint64_t morton_code, const T& bounds, BVNode* left, BVNode* right) :
		m_MortonCode{entity, morton_code},
		m_Bounds{bounds},
		m_Left{left},
		m_Right{right}
	{
	}
	static void print(const BVNode<T>* node)
	{
		//std::cout << "Node ID: " << node->m_MortonCode.m_Entity;
		//T::print(node->m_Bounds);
	}
};

template<typename U>
uint64_t get_split_position(const BVNode<U>* list, uint64_t start, uint64_t end)
{
	if (start == end) return start;
	uint64_t first_code = list[start].m_MortonCode.MORTON_CODE;
	uint64_t last_code = list[end].m_MortonCode.MORTON_CODE;

	if (first_code == last_code) return (start + end) / 2;

	uint64_t common_prefix = count_leading_zeros(first_code ^ last_code);
	uint64_t split = start;
	uint64_t step = end - start;

	while (step > 0)
	{
		uint64_t new_split = split + (step >> 1); //divides step by 2
		if (new_split >= end)
		{
			step >>= 1;
			continue;
		}

		uint64_t split_prefix = count_leading_zeros(first_code ^ list[new_split].m_MortonCode.MORTON_CODE);
		if (split_prefix > common_prefix)
		{
			split = new_split;
		}
		step >>= 1;
	}
	return split;
}

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
BVNode<U>* create_sub_tree(const std::vector<BVNode<U>>& list, uint64_t start, uint64_t end, ArenaAllocator<BVNode<U>>& allocator)
{
	if (list.empty())
	{
		return nullptr;
	}
	if (start == end)
	{
		return allocator.allocate<BVNode<U>>(BVNode<U>{ list[end].m_MortonCode.ID, list[end].m_MortonCode.MORTON_CODE, list[end].m_Bounds, nullptr, nullptr });
	}
	uint64_t mid = get_split_position<U>(list.data(), start, end);
	BVNode<U>* left = create_sub_tree(list, start, mid, allocator);
	BVNode<U>* right = create_sub_tree(list, mid + 1, end, allocator);

	if (right && left)
	{
		const U& lb = left->m_Bounds;
		const U& rb = right->m_Bounds;
		U merged = merge<U>(lb, rb);
		return allocator.allocate<BVNode<U>>(BVNode<U>(entt::null,  -1, merged, left, right));
	}
	else if (right && !left)
	{
		return right;
	}
	else if (!right && left)
	{
		return left;
	}
	return nullptr;
}

template<typename U>
BVNode<U>* create_tree(std::vector<BVNode<U>>& list)
{
	static ArenaAllocator<BVNode<U>> allocator(65536*2);
	allocator.reset();
	std::sort(
		list.begin(), list.end(),
		[&](const BVNode<U>& u, const BVNode<U>& v) { 
			return (u.m_MortonCode < v.m_MortonCode);
		});
	return create_sub_tree<U>(list, 0, static_cast<uint32_t>(list.size() - 1), allocator );
}

template<typename U>
void detect_overlapping_bounds(const BVNode<U>& leaf_node, const BVNode<U>* tree_node, std::vector<physics::CollisionDescription>& total_intersections, std::vector<const BVNode<U>*>& buffer)
{
	buffer.push_back(tree_node);
	while (!buffer.empty())
	{
		auto node = buffer.back();
		buffer.pop_back();
		if (!AABBIntersection<U>(leaf_node.m_Bounds, node->m_Bounds)) continue;
		if ((!node->m_Left) && (!node->m_Right) && (leaf_node.m_MortonCode.ID != node->m_MortonCode.ID))
		{
			total_intersections.push_back({ leaf_node.m_MortonCode.ID, node->m_MortonCode.ID });
		}
		if (node->m_Left)
		{
			buffer.push_back(node->m_Left);
		}
		if (node->m_Right)
		{
			buffer.push_back(node->m_Right);
		}
	}
}
#endif