
#include "bootstrap.h"

#include <stm32l4xx.h>
#include <stm32l4xx_hal_rcc.h>

/******************************************************************************/
#include "preload.h"
#include "swap.h"

#ifdef HAS_CRC_COMPUTE
#include "crc.h"
#endif

#ifndef SYSMEM_ADDRESS
#define SYSMEM_ADDRESS (uint32_t)0x1FFF0000
#endif

/******************************************************************************/
#define JMP(addr) \
    __asm__("mov pc,%0" \
            : /*output*/ \
            : /*input*/ \
            "r" (addr) \
           );


static __attribute__ ((always_inline)) __attribute__ ((no_return))
void NVIC_VectorReset(void)
{
  __DSB();
  SCB->AIRCR  = (uint32_t)( (0x5FAUL << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_VECTRESET_Msk );
  __DSB();
  while(1) { __NOP(); }
}

static __attribute__ ((always_inline)) __attribute__ ((no_return))
void reboot(void) { NVIC_VectorReset(); }

static void init_protect(
		register unsigned char dest_id,
		register unsigned char src_id,
		register boot_request_e do_it
		)
{
	if (do_it == BOOT_REQ_UPDATE)
	{
		// Update or backup is requested :
		// - We should be able to erase, and write to ACTIVE_AREA
		// - We should be able to read INACTIVE_AREA with id == src_id
		// - We should not be able to erase, write nor read INACTIVE_AREA with id != src_id
		//
		// No protection really required, we are in the bootstrap. But some
		// protection have to be enable somewhere.
		// Assuming that protections were previously enabled somewhere else,
		// then just "open all".
		//
	}
	else
	{
		// Local Update or Nominal is requested :
		// - We should not be able to erase, write nor read ACTIVE_AREA
		// - We should be able to erase and write to INACTIVE_AREA with id == dest_id
		// - We should not be able to erase, write nor read INACTIVE_AREA with id != dest_id
		//
		// From "STBootLoader" :
		// - Only Erase, write (read permit) one of the INACTIVE_AREA
		// From "Application" :
		// - Only Erase, write (read permit) one of the INACTIVE_AREA
		//
		// Assuming that protections were previously enabled somewhere else,
		// then just "open one INACTIVE_AREA".
		//
		// PCROP granularity is double word
		// PCROP1_STRT =
		// PCROP1_END  =
		// PCROP_RDP   = ?
		// RDERRIE
		//
		// FLASH_OPTR = 0x08000000 // nBOOT0 = 1, nSWBOOT0 = 0, nBOOT1 = 0
		//
		// WRP granularity is flash page size (2KB)
		// WRP1A_STRT
		// WRP1A_END
		// WRP1B_STRT
		// WRP1B_END
		//
	}
}

/******************************************************************************/
__attribute__((weak)) const pf_t pfBL_2ndStage;

__attribute__ ((no_return)) //__attribute__ ((section (".boot")))
void boot_strap(void)
{
	register struct __exch_info_s* pp = (struct __exch_info_s *)&(__exchange_area_org__);
	register boot_request_e do_it;

	do_it = preload(pp);

#ifdef HAS_CRC_COMPUTE
	crc_init();
	pp->crc = crc_compute( (uint32_t*)pp, (uint32_t)(sizeof(struct __exch_info_s) - 4) / 4);
	crc_deinit();
#else
	pp->crc = 0;
#endif

	if (do_it == BOOT_REQ_UPDATE)
	{
		swap(pp);
	}
	else
	{
		register unsigned int start;

		if (do_it == BOOT_REQ_NONE )
		{
			start = pp->src + HEADER_SZ;
			// jump to app
			SCB->VTOR = start;
		}
		else // do_it == BOOT_REQ_LOCAL
		{
			// 2nd Stage BL is required
			if(pfBL_2ndStage)
			{
				pfBL_2ndStage(pp);
			}
			else
			{
				// TODO :
				// read gpio to check if ST bootloader is authorized
				start = SYSMEM_ADDRESS;
			    __HAL_RCC_SYSCFG_CLK_ENABLE();
			    MODIFY_REG(SYSCFG->MEMRMP, SYSCFG_MEMRMP_MEM_MODE, SYSCFG_MEMRMP_MEM_MODE_0);
			}
		}
	    __set_MSP(*(volatile uint32_t*)start);
	    JMP( (*(volatile uint32_t*)(start + 4)) );
	}
	reboot();
}

