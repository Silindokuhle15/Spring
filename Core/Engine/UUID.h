#pragma once
#include <cstdint>
struct AssetHandle
{
public:
	uint64_t m_HWORD;
	uint64_t m_LWORD;

	bool operator==(const AssetHandle& other) const
	{
		return ((m_HWORD == other.m_HWORD) && (m_LWORD == other.m_LWORD));
	}
	bool operator<(const AssetHandle& other) const
	{
		if (m_HWORD < other.m_HWORD)
			return true;
		if (m_HWORD < other.m_HWORD)
			return false;
		return m_LWORD < other.m_LWORD;
	}
};