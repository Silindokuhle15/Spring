#include "BVH.h"

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
