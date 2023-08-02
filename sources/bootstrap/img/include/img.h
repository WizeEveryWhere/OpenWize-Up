#ifndef _IMG_H_
#define _IMG_H_

#if defined(__cplusplus)
extern "C"
{
#endif

#include "magic_def.h"

extern unsigned int __exchange_area_org__;
extern unsigned int __exchange_area_size__;
extern unsigned int __header_size__;

#define VTOR_ALIGNMENT 512
#define HEADER_SZ VTOR_ALIGNMENT

typedef enum
{
	BOOT_REQ_NONE   = 0x0,
	BOOT_REQ_UPDATE = 0x1,
	BOOT_REQ_BACK   = BOOT_REQ_UPDATE,
	BOOT_REQ_LOCAL  = 0x2,
	// ---
	BOOT_REQ_NB,
	BOOT_REQ_MASK   = 0xFFFFFFFF,
} boot_request_e;

struct __exch_info_s {
	unsigned int magic;
	unsigned int request;
	unsigned int src;
	unsigned int src_sz;
	unsigned int dest;
	unsigned int dest_sz;
	unsigned int header_sz;
	unsigned int reserved[10];
	unsigned int crc;
};

typedef int (*pf_t)(struct __exch_info_s* p);

union __img_header_s {
	unsigned int reserved[HEADER_SZ / sizeof(unsigned int)];
	struct {
		unsigned int magic;
		unsigned int size; // app size + header size
	};
};


#if defined(__cplusplus)
}
#endif

#endif // _IMG_H_
