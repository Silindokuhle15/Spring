#ifndef _BOUND_H_
#define _BOUND_H_

namespace primitives
{
	struct Bound2D
	{
		float xMin{ 0 }, yMin{ 0 }, xMax{ 0 }, yMax{ 0 };
	};

	struct Bound3D
	{
		float xMin{ 0 }, yMin{ 0 }, zMin{ 0 };
		float xMax{ 0 }, yMax{ 0 }, zMax{ 0 };
	};
}
#endif