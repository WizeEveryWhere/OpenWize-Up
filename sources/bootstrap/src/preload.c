
#if defined(__cplusplus)
extern "C"
{
#endif

#include "preload.h"
#include "crc.h"

#define MAGIC_NB 2

extern unsigned int __get_part_tab__(void);
extern unsigned int __get_part_size__(void);
extern unsigned int __get_magic_tab__(void);

typedef enum
{
	PART_ACTIVE,
	PART_INACTIVE_0,
	PART_INACTIVE_1,
	// ---
	PART_LAST = PART_INACTIVE_1,
	PART_NB,
	// ---
	PART_NONE = 0xFF,
} part_id_e;

static unsigned int get_valid_parts(void);
static int get_part_info_local_req(unsigned char *dest_id, unsigned char *src_id, unsigned char valid);
static int get_part_info_none_req(unsigned char *dest_id, unsigned char *src_id, unsigned char valid);
static int get_part_info_none_req(unsigned char *dest_id, unsigned char *src_id, unsigned char valid);
static void init_exch_info(struct __exch_info_s* pp, unsigned char dest_id, unsigned char src_id, boot_request_e do_it);

static unsigned int get_valid_parts(void)
{
	register const unsigned int **p = (const unsigned int **)__get_part_tab__();
	register const unsigned int *m = (const unsigned int *)__get_magic_tab__();
	register unsigned int valid = 0;
	register unsigned char i;

	for (i = 0; i < PART_NB; i++)
	{
		if ( (**p == m[0]) || (**p == m[1]) )
		{
			valid |= 0x1 << i;
		}
		p++;
	}
	return valid;
}

static int get_part_info_local_req(
		register unsigned char *dest_id,
		register unsigned char *src_id,
		register unsigned char valid)
{
	register const unsigned int **p = (const unsigned int **)__get_part_tab__();
	// Set P0 as default destination
	*dest_id = PART_INACTIVE_0;
	(void)src_id;

	if ( valid < 0x4 )
	{
		// P1
		*dest_id = PART_INACTIVE_1;
	}
	else if ( valid == 0x7 )
	{
		// choose
		if ( *p[PART_INACTIVE_0] == *p[PART_ACTIVE] )
		//if ( *part_tab[PART_INACTIVE_0] == *part_tab[PART_ACTIVE] )
		{
			// active part is a copy of inactive part 0
			*dest_id = PART_INACTIVE_1;
		}
		// else // P0 is choose
	}
	// else // P0 is choose
	return 0;
}

static int get_part_info_updbck_req(
		register unsigned char *dest_id,
		register unsigned char *src_id,
		register unsigned char valid)
{
	register const unsigned int **p = (const unsigned int **)__get_part_tab__();
	/* Set P0 as default src. Used in case :
	 * - active part is a copy of inactive part 1
	 * - active part is not P0 neither P1
	 * - P1 is not active
	 */
	*dest_id = PART_ACTIVE;
	*src_id = PART_INACTIVE_0;

	// active, P0 and P1 are valid
	if ( valid == 0x7 )
	{
		// choose
		if ( *p[PART_INACTIVE_0] == *p[PART_ACTIVE] )
		{
			// active part is a copy of inactive part 0
			*src_id = PART_INACTIVE_1;
		}
		// else // P0 is choose
	}
	// At least P1 is valid. Choose it as default
	else if ( valid > 0x3 )
	{
		// P1
		*src_id = PART_INACTIVE_1;
	}
	else if ( valid < 0x2 )
	{
		// Can't process the "U/B" boot
		*src_id = PART_NONE;
		return -1;
	}
	// else P0 is valid. Choose it as default
	return 0;
}

static int get_part_info_none_req(
		register unsigned char *dest_id,
		register unsigned char *src_id,
		register unsigned char valid)
{
	(void)dest_id;
	if ( valid & 0x1 )
	{
		*src_id = (unsigned char)PART_ACTIVE;
		return 0;
	}
	// else // Can't process the "Nominal" boot
	return -1;
}

static void init_exch_info(
		register struct __exch_info_s* pp,
		register unsigned char dest_id,
		register unsigned char src_id,
		register boot_request_e do_it)
{
	register const unsigned int *p = (unsigned int *)__get_part_tab__();
	register const unsigned int *s = (unsigned int *)__get_part_size__();

	pp->src = 0xFFFFFFFF;
	pp->dest = 0xFFFFFFFF;
	pp->src_sz = 0;
	pp->dest_sz = 0;
	pp->magic = 0x0;
	pp->header_sz = HEADER_SZ;
	//pp->bootable = 0;

	register unsigned int i;
	for (i = 0; i < 10; i++)
	{
		pp->reserved[i] = 0x0;
	}

	if (dest_id < PART_NB)
	{
		pp->dest = (unsigned int)p[dest_id];
		pp->dest_sz = s[dest_id];
	}

	if (src_id < PART_NB)
	{
		pp->src = (unsigned int)p[src_id];
		if (do_it != BOOT_REQ_LOCAL )
		{
			pp->src_sz = *( (unsigned int*)pp->src + 1);
		}
		else
		{
			pp->src_sz = s[src_id];
		}

		register const unsigned int *m = (const unsigned int *)__get_magic_tab__();
		if(src_id == PART_ACTIVE)
		{
			if (*(unsigned int *)(p[PART_ACTIVE]) == m[0])
			{
				dest_id = PART_INACTIVE_1;
			}
			else
			{
				dest_id = PART_INACTIVE_0;
			}
			pp->magic = m[dest_id - 1];
			pp->dest = (unsigned int)p[dest_id];
			pp->dest_sz = s[dest_id];
		}
		else
		{
			pp->magic = m[src_id - 1];
		}
	}
	pp->request = BOOT_REQ_NONE;
}

boot_request_e preload(register struct __exch_info_s * pp )
{
	register int ret = 0;
	register boot_request_e request = pp->request;

	boot_request_e do_it;
	unsigned char src_id = PART_INACTIVE_0;
	unsigned char dest_id = PART_INACTIVE_0;
	unsigned char valid = (unsigned char)get_valid_parts();

	if (request > BOOT_REQ_LOCAL)
	{
		request = BOOT_REQ_NONE;
	}
	do_it = request;

	if ( (request == BOOT_REQ_NONE) )
	{
		ret = get_part_info_none_req(&dest_id, &src_id, valid);
		// From "get_part_info_none_req" ;
		// On success :
		// - dest_id = unmodified
		// - src_id  = PART_ACTIVE
		// - at least PART_ACTIVE is valid
		// - ret = 0
		//
		// On failure :
		// - dest_id = unmodified
		// - src_id  = unmodified
		// - at least PART_ACTIVE is not valid
		// - ret = -1
		//
		// if (ret ) Unable to run Nominal process,
		// 		try as Update/Backup case
	}
	if ( (request == BOOT_REQ_UPDATE) || (ret) )
	{
		ret = get_part_info_updbck_req(&dest_id, &src_id, valid);
		// From "get_part_info_updbck_req" ;
		// On success :
		// - dest_id = PART_ACTIVE
		// - src_id  = PART_INACTIVE_0 or PART_INACTIVE_1
		// - at least PART_INACTIVE_0 or PART_INACTIVE_1 is valid
		// - PART_ACTIVE is valid or not
		// - ret = 0
		//
		// On failure :
		// - dest_id = PART_ACTIVE
		// - src_id  = PART_NONE
		// - PART_INACTIVE_0 and PART_INACTIVE_1 are not valid
		// - PART_ACTIVE is valid or not
		// - ret = -1
		//
		if (ret)
		{
			if ( (valid & 0x1) )
			{
				// Unable to Update or Backup, but active part is valid
				// treat as Nominal case
				do_it = BOOT_REQ_NONE;
			}
			// if (ret && (valid == 0x0)) Unable to run Nominal nor Update/Backup process,
			// treat as Local case
		}
		else
		{
			do_it = BOOT_REQ_UPDATE;
		}
	}

	if ( (request == BOOT_REQ_LOCAL) || (ret) )
	{
		get_part_info_local_req(&dest_id, &src_id, valid);
		// From "get_part_info_local_req" ;
		// - dest_id = PART_INACTIVE_0 or PART_INACTIVE_1
		// - src_id  = unmodified
		// - ret = 0
		//
		do_it = BOOT_REQ_LOCAL;
	}

	init_exch_info(pp, dest_id, src_id, do_it);

	crc_init();
	pp->crc = crc_compute( (uint32_t*)pp, (uint32_t)(sizeof(struct __exch_info_s) - 4) / 4);
	crc_deinit();

	return do_it;
}

#if defined(__cplusplus)
}
#endif
