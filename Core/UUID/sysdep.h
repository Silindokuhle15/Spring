#ifndef SYSDEP_H
#define SYSDEP_H

#include "copyrt.h"
#define WINIC 0

#ifdef WINIC
#include <Windows.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#endif
#include "global.h"
#include "md5.h"
#include "sha1.h"
#define UUIDS_PER_TICK 1024

#define LOCK
#define UNLOCK

typedef unsigned long	unsigned32;
typedef unsigned short	unsigned16;
typedef unsigned char	unsigned8;
typedef unsigned char	byte;

#ifdef WINIC
#define unsigned64_t unsigned __int64
#define I64(C) C
#else
#define unsigned64_t unsigned long long
#define I64(C) C##LL
#endif


typedef unsigned64_t uuid_time_t;
typedef struct
{
	char nodeID[6];
} uuid_node_t;

void get_ieee_node_identifier(uuid_node_t* node);
void get_system_time(uuid_time_t* uuid_time);
void get_random_info(char seed[16]);

#endif