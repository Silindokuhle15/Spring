#ifndef UUID_H
#define UUID_H
#include "sysdep.h"

#undef uuid_t
typedef struct
{
	unsigned32 time_low;
	unsigned16 time_mid;
	unsigned16 time_hi_and_version;
	unsigned8 clock_seq_hi_and_reserved;
	unsigned8 clock_seq_low;
	unsigned8 node[6];
} uuid_t;
typedef struct
{
	uuid_time_t ts;
	uuid_node_t node;
	unsigned16 cs;
}uuid_state;

int uuid_create(uuid_t* uuid);

void uuid_create_md5_from_name(
	uuid_t* uuid,
	uuid_t nsid,
	void* name,
	int name_len
);

void uuid_create_sha1_from_name(
	uuid_t* uuid,
	uuid_t nsid,
	void* name,
	int name_len
);

int uuid_compare(uuid_t* u1, uuid_t* u2);

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
		if (m_HWORD > other.m_HWORD)
			return false;
		return m_LWORD < other.m_LWORD;
	}
};

AssetHandle UUIDToAssetHandle(const uuid_t& uuid);
AssetHandle UUIDToAssetHandle(const uuid_t* uuid);
void AssetHandleToUUID(const AssetHandle& handle, uuid_t* uuid);

AssetHandle CreateAssetHandleFromPath(const char* file_path);

#endif
