#ifndef _RANGE_H_
#define _RANGE_H_
#include <cstdint>
namespace primitives
{
	struct Range
	{
		uint32_t Offset = 0;
		uint32_t Size = 0;
		bool operator <(const Range& other)
		{
			return Offset < other.Offset;
		}
		bool operator==(const Range& other)
		{
			return Offset == other.Offset;
		}
	};
}
#endif // !_RANGE_H_
