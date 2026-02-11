#include "BVH.h"

uint32_t part1by1_16(uint16_t n)
{
	uint32_t m = n;
	m = (m | (m << 8)) & 0x00FF00FF;
	m = (m | (m << 4)) & 0x0F0F0F0F;
	m = (m | (m << 2)) & 0x33333333;
	m = (m | (m << 1)) & 0x55555555;
	return m;
}

uint64_t part1by1_32(uint32_t n)
{
	uint64_t m = n;
	m = (m | (m << 16)) & 0x0000FFFF0000FFFFULL;
	m = (m | (m << 8)) & 0x00FF00FF00FF00FFULL;
	m = (m | (m << 4)) & 0x0F0F0F0F0F0F0F0FULL;
	m = (m | (m << 2)) & 0x3333333333333333ULL;
	m = (m | (m << 1)) & 0x5555555555555555ULL;
	return m;
}

uint64_t part1by2_32(uint32_t n)
{
	uint64_t m = n & 0x1FFFFF;
	m = (m | (m << 32)) & 0x1f00000000ffff;
	m = (m | (m << 16)) & 0x1f0000ff0000ff;
	m = (m | (m << 8)) & 0x100f00f00f00f00f;
	m = (m | (m << 4)) & 0x10c30c30c30c30c3;
	m = (m | (m << 2)) & 0x1249249249249249;
	return m;
}

uint32_t morton_encode_2d16(uint16_t x, uint16_t y)
{
	return (part1by1_16(y) << 1 | part1by1_16(x));
}

uint64_t morton_encode_2d32(uint32_t x, uint32_t y)
{
	return (part1by1_32(y) << 1 | part1by1_32(x));
}

uint64_t morton_encode_3d32(uint32_t x, uint32_t y, uint32_t z)
{
	return (part1by2_32(z) << 2) | (part1by2_32(y) << 1) | part1by2_32(x);
}

uint32_t count_leading_zeros(uint32_t n)
{
#if defined(_MSC_VER)
	unsigned long index;
	if (_BitScanReverse(&index, n))
	{
		return 31 - index;
	}
	return 32;
#else
	return x ? __builtin_clz(n) : 32;
#endif
}


template<>
float unit_product(const primitives::Bound3D& a)
{
	float dx = std::max(0.0f, a.xMax - a.xMin);
	float dy = std::max(0.0f, a.yMax - a.yMin);
	float dz = std::max(0.0f, a.zMax - a.zMin);
	return dx * dy * dz;
}
template<>
primitives::Bound3D merge(const primitives::Bound3D& u, const primitives::Bound3D& v)
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

template<>
bool AABBIntersection(const primitives::Bound3D& a, const primitives::Bound3D& b)
{
	return
		((a.xMax - b.xMin) >= 0.0f) &
		(((b.xMax - a.xMin) >= 0.0f)) &
		((((a.yMax - b.yMin) >= 0.0f))) &
		(((((b.yMax - a.yMin) >= 0.0f)))) &
		((((((a.zMax - b.zMin) >= 0.0f))))) &
		(((((((b.zMax - a.zMin) >= 0.0f))))));
}


template<>
primitives::Bound3D intersection(const primitives::Bound3D& a, const primitives::Bound3D& b)
{
	if (!AABBIntersection(a, b))
		return { 0,0,0,0,0,0 };
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

template<>
primitives::Bound2D merge(const primitives::Bound2D& u, const primitives::Bound2D& v)
{
	return
	{
		std::min(u.xMin, v.xMin),
		std::min(u.yMin, v.yMin),
		std::max(u.xMax, v.xMax),
		std::max(u.yMax, v.yMax),
	};
}
template<>
bool AABBIntersection(const primitives::Bound2D& a, const primitives::Bound2D& b)
{
	bool intersect_x = a.xMax >= b.xMin &&
		a.xMin <= b.xMin;

	bool intersect_y = a.yMax >= b.yMin &&
		a.yMin <= b.yMax;
	return intersect_x && intersect_y;
}
template<>
primitives::Bound2D intersection(const primitives::Bound2D& a, const primitives::Bound2D& b)
{
	return
	{
		std::max(a.xMin, b.xMin),
		std::max(a.yMin, b.yMin),
		std::min(a.xMax, b.xMax),
		std::min(a.yMax, b.yMax)
	};
}
