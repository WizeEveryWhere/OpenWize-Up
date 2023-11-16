#ifndef _IMG_H_
#define _IMG_H_

#if defined(__cplusplus)
extern "C"
{
#endif

#include "magic_def.h"

/******************************************************************************/

#ifndef BOOT_INFO_BKPR
	#define BOOT_INFO_BKPR BKP31R
#endif

#define COLD_RESET    0x0C // Cold reset (NRST pin or power lost)
#define WARM_RESET    0x10 // Warm reset (SW)
#define INSTAB_DETECT 0x60 // Instability detected 'Application or other)
#define UNAUTH_ACCESS 0x83 /* Unauthorized action :
 	 	 	  * - access to protected areas
 	 	 	  * - entering into forbidden low power mode (Standby, Stop, Shutdown)
 	 	 	  * - reset option byte
 	 	 	  */
#define BOOT_REASON_MSK     0x000000FF
#define BOOT_REASON_POS     0

#define WKUP_PIN_MSK        0x00001F00
#define WKUP_PIN_POS        8

#define STANDBY_WKUP_MSK    0x00002000
#define STANDBY_WKUP_POS    13

#define INTERNAL_WKUP_MSK   0x00004000
#define INTERNAL_WKUP_POS   14

#define BACKUP_RESET_MSK    0x00008000
#define BACKUP_RESET_POS    15

#define BOOT_INFO_MASK       0x0000FFFF
#define BOOT_INFO_POS        0

#define BOOT_UNSTAB_CNT_MASK 0x00FF0000
#define BOOT_UNSTAB_CNT_POS  16

#define BOOT_UNAUTH_CNT_MASK 0xFF000000
#define BOOT_UNAUTH_CNT_POS  24

/******************************************************************************/

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

/******************************************************************************/

typedef int (*pf_t)(struct __exch_info_s* p);

/*
union __img_header_s {
	unsigned int reserved[HEADER_SZ / sizeof(unsigned int)];
	struct {
		unsigned int magic;
		unsigned int size; // app size + header size
	};
};
*/

#if defined(__cplusplus)
}
#endif

#endif // _IMG_H_
