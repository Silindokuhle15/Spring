#include "UUID.h"
#include "copyrt.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sysdep.h"
#include <winsock.h>
#include <string>

uuid_t NameSpace_DNS =
{
	 0x6ba7b810,
	 0x9dad,
	 0x11d1,
	 0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8
};

static uuid_state st;
static int read_state(unsigned16* clockseq, uuid_time_t* timestamp, uuid_node_t* node)
{
	static int inited = 0;
	FILE* fp;
	if (!inited)
	{
		fp = fopen("state", "rb");
		if (fp == NULL)
		{
			return 0;
		}
		fread(&st, sizeof(st), 1, fp);
		fclose(fp);
		inited = 1;
	}
	*clockseq = st.cs;
	*timestamp = st.ts;
	*node = st.node;
	return 1;
}
static void write_state(unsigned16 clockseq, uuid_time_t timestamp, uuid_node_t node)
{
	static int inited = 0;
	static uuid_time_t next_save;
	FILE* fp;
	if (!inited)
	{
		next_save = timestamp;
		inited = 1;
	}

	st.cs = clockseq;
	st.ts = timestamp;
	st.node = node;
	if (timestamp >= next_save)
	{
		fp = fopen("state", "wb");
		fwrite(&st, sizeof(st), 1, fp);
		fclose(fp);
		next_save = timestamp + (10 * 10 * 1000 * 1000);
	}
}
static void format_uuid_v1(uuid_t* uuid, unsigned16 clock_seq, uuid_time_t timestamp, uuid_node_t node)
{
	uuid->time_low = (unsigned long)(timestamp & 0xFFFFFFFF);
	uuid->time_mid = (unsigned short)((timestamp >> 32) & 0xFFFF);
	uuid->time_hi_and_version = (unsigned short)((timestamp >> 48) & 0x0FFF);
	uuid->time_hi_and_version |= (1 << 12);
	uuid->clock_seq_low = clock_seq & 0xFF;
	uuid->clock_seq_hi_and_reserved = (clock_seq & 0x3F00) >> 8;
	uuid->clock_seq_hi_and_reserved |= 0x80;
	memcpy(&uuid->node, &node, sizeof uuid->node);
}
static void format_uuid_v3or5(uuid_t* uuid, unsigned char hash[16], int v)
{
	memcpy(uuid, hash, sizeof(*uuid));
	uuid->time_low = ntohl(uuid->time_low);
	uuid->time_mid = ntohs(uuid->time_mid);
	uuid->time_hi_and_version = ntohs(uuid->time_hi_and_version);

	uuid->time_hi_and_version &= 0x0FFF;
	uuid->time_hi_and_version |= (v << 12);
	uuid->clock_seq_hi_and_reserved &= 0x3F;
	uuid->clock_seq_hi_and_reserved |= 0x80;
}
static void get_current_time(uuid_time_t* timestamp)
{
	static int inited = 0;
	static uuid_time_t time_last;
	static unsigned16 uuids_this_tick;
	uuid_time_t time_now;

	if (!inited)
	{
		get_system_time(&time_now);
		uuids_this_tick = UUIDS_PER_TICK;
		inited = 1;
	}
	for (;;)
	{
		get_system_time(&time_now);

		if (time_last != time_now)
		{
			uuids_this_tick = 0;
			time_last = time_now;
			break;
		}
		if (uuids_this_tick < UUIDS_PER_TICK)
		{
			uuids_this_tick++;
			break;
		}
	}
	*timestamp = time_now + uuids_this_tick;
}
static unsigned16 true_random(void)
{
	static int inited = 0;
	uuid_time_t time_now;

	if (!inited)
	{
		get_system_time(&time_now);
		time_now = time_now / UUIDS_PER_TICK;
		srand((unsigned int)((time_now >> 32) ^ time_now) & 0xffffffff);
		inited = 1;
	}
	return rand();
}

int uuid_create(uuid_t* uuid)
{
	uuid_time_t timestamp, last_time;
	unsigned16 clockseq;
	uuid_node_t node;
	uuid_node_t last_node;
	int f;

	LOCK;
	get_current_time(&timestamp);
	get_ieee_node_identifier(&node);

	f = read_state(&clockseq, &last_time, &last_node);

	if (!f || memcmp(&node, &last_node, sizeof(node)))
	{
		clockseq = true_random();
	}
	else if (timestamp < last_time)
	{
		clockseq++;
	}

	write_state(clockseq, timestamp, node);
	UNLOCK;

	format_uuid_v1(uuid, clockseq, timestamp, node);
	return 1;
}

void uuid_create_md5_from_name(uuid_t* uuid, uuid_t nsid, void* name, int name_len)
{
	MD5_CTX c;
	unsigned char hash[16];
	uuid_t net_nsid;

	net_nsid = nsid;
	net_nsid.time_low = htonl(net_nsid.time_low);
	net_nsid.time_mid = htons(net_nsid.time_mid);
	net_nsid.time_hi_and_version = htons(net_nsid.time_hi_and_version);

	MD5Init(&c);
	MD5Update(&c, (unsigned char*)&net_nsid, sizeof(net_nsid));
	MD5Update(&c, (unsigned char*)name, name_len);
	MD5Final(hash, &c);

	format_uuid_v3or5(uuid, hash, 3);
}

void uuid_create_sha1_from_name(uuid_t* uuid, uuid_t nsid, void* name, int name_len)
{
	SHA1_CTX c;
	unsigned char hash[20];
	uuid_t net_nsid;
	/* put name space ID in network byte order so it hashes the same
	no matter what endian machine we’re on */
	net_nsid = nsid;
	net_nsid.time_low = htonl(net_nsid.time_low);
	net_nsid.time_mid = htons(net_nsid.time_mid);
	net_nsid.time_hi_and_version = htons(net_nsid.time_hi_and_version);
	sha1_init(&c);
	sha1_update(&c, (BYTE*)(&net_nsid), sizeof(net_nsid));
	sha1_update(&c, (BYTE*)name, name_len);
	sha1_final(&c, (BYTE*)hash);
	/* the hash is in network byte order at this point */
	format_uuid_v3or5(uuid, hash, 5);
}

#define CHECK(f1, f2) if(f1 != f2) return f1 < f2 ? -1 : 1;
int uuid_compare(uuid_t* u1, uuid_t* u2)
{
	int i;

	CHECK(u1->time_low, u2->time_low);
	CHECK(u1->time_mid, u2->time_mid);
	CHECK(u1->time_hi_and_version, u2->time_hi_and_version);
	CHECK(u1->clock_seq_hi_and_reserved, u2->clock_seq_hi_and_reserved);

	for (i = 0; i < 6; i++)
	{
		if (u1->node[i] < u2->node[i])
			return -1;
		if (u1->node[i] > u2->node[i])
			return 1;
	}
	return 0;
}

AssetHandle UUIDToAssetHandle(const uuid_t& uuid)
{
	AssetHandle handle{};

	handle.m_HWORD =
		(uint64_t(uuid.time_low) << 32) |
		(uint64_t(uuid.time_mid) << 16) |
		(uint64_t(uuid.time_hi_and_version));

	handle.m_LWORD =
		(uint64_t(uuid.clock_seq_hi_and_reserved) << 56) |
		(uint64_t(uuid.clock_seq_low) << 48) |
		(uint64_t(uuid.node[0]) << 40) |
		(uint64_t(uuid.node[1]) << 32) |
		(uint64_t(uuid.node[2]) << 24) |
		(uint64_t(uuid.node[3]) << 16) |
		(uint64_t(uuid.node[4]) << 8) |
		(uint64_t(uuid.node[5]));

	return handle;
}

AssetHandle UUIDToAssetHandle(const uuid_t* uuid)
{
	AssetHandle handle{};

	handle.m_HWORD =
		(uint64_t(uuid->time_low) << 32) |
		(uint64_t(uuid->time_mid) << 16) |
		(uint64_t(uuid->time_hi_and_version));

	handle.m_LWORD =
		(uint64_t(uuid->clock_seq_hi_and_reserved) << 56) |
		(uint64_t(uuid->clock_seq_low) << 48) |
		(uint64_t(uuid->node[0]) << 40) |
		(uint64_t(uuid->node[1]) << 32) |
		(uint64_t(uuid->node[2]) << 24) |
		(uint64_t(uuid->node[3]) << 16) |
		(uint64_t(uuid->node[4]) << 8) |
		(uint64_t(uuid->node[5]));

	return handle;
}

void AssetHandleToUUID(const AssetHandle& handle, uuid_t* uuid)
{
	uuid->time_low = uint32_t(handle.m_HWORD >> 32);
	uuid->time_mid = uint16_t(handle.m_HWORD >> 16);
	uuid->time_hi_and_version = uint16_t(handle.m_HWORD);

	uuid->clock_seq_hi_and_reserved = uint8_t(handle.m_LWORD >> 56);
	uuid->clock_seq_low = uint8_t(handle.m_LWORD >> 48);

	uuid->node[0] = uint8_t(handle.m_LWORD >> 40);
	uuid->node[1] = uint8_t(handle.m_LWORD >> 32);
	uuid->node[2] = uint8_t(handle.m_LWORD >> 24);
	uuid->node[3] = uint8_t(handle.m_LWORD >> 16);
	uuid->node[4] = uint8_t(handle.m_LWORD >> 8);
	uuid->node[5] = uint8_t(handle.m_LWORD);
}

AssetHandle CreateAssetHandleFromPath(const char* file_path)
{
	uuid_t id;
	AssetHandle assetHandle{};
	std::string filePath{ file_path };
	uuid_create_sha1_from_name(&id, NameSpace_DNS, (void*)filePath.c_str(), filePath.size());
	return UUIDToAssetHandle(id);
}
