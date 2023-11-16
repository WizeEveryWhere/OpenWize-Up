
#include "bootstrap.h"

#include <stm32l4xx.h>
#include <stm32l4xx_hal_rcc.h>

/******************************************************************************/
#include "def.h"

#include "trace.h"
#include "preload.h"
#include "swap.h"
#include "protect.h"

#ifdef HAS_CRC_COMPUTE
#include "crc.h"
#endif

#ifndef SYSMEM_ADDRESS
#define SYSMEM_ADDRESS (uint32_t)0x1FFF0000
#endif

__attribute__((weak)) const pf_t pfBL_2ndStage;

/******************************************************************************/
#define JMP(addr) \
    __asm__("mov pc,%0" \
            : /*output*/ \
            : /*input*/ \
            "r" (addr) \
           );


__attribute__ ((always_inline))
__attribute__ ((no_return))
static void NVIC_VectorReset(void)
{
  __DSB();
  SCB->AIRCR  = (uint32_t)( (0x5FAUL << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk );
  __DSB();
  while(1) { __NOP(); }
}

__attribute__ ((always_inline))
__attribute__ ((no_return))
static void reboot(void)
{
	NVIC_VectorReset();
}

void infinite_loop(void)
{
	register uint32_t cnt = 8000000;
	TRACE(TRACE_MSG_FAILURE);
	while (cnt) {__NOP(); cnt--; }
	reboot();
}
/******************************************************************************/

__attribute__((noinit)) uint8_t u8UnstabCnt;
__attribute__((noinit)) uint8_t u8UnauthCnt;

uint32_t boot_get_info(void)
{
	register __IO uint32_t tempreg = 0;
	uint32_t boot_info = 0;

	// Enable PWR_CLK
	__HAL_RCC_PWR_CLK_ENABLE();
	// Get Register
	tempreg = PWR->SR1;
	// clear Standby flag and all wake-up flag
	SET_BIT(PWR->SCR, (PWR_SCR_CSBF | PWR_SCR_CWUF) );
	// Enable backup domain access
	SET_BIT(PWR->CR1, PWR_CR1_DBP);
	// Disable PWR_CLK
	__HAL_RCC_PWR_CLK_DISABLE();

	// Get wake-up pins
	boot_info |= (tempreg & PWR_SR1_WUF) << WKUP_PIN_POS;
	// Get if it was in Standby mode
	boot_info |= (tempreg & PWR_SR1_SBF)?(STANDBY_WKUP_MSK):(0);
	// Get if it was internal wake-up
	boot_info |= (tempreg & PWR_SR1_WUFI)?(INTERNAL_WKUP_MSK):(0);

	// Get Boot reason
	tempreg = RCC->CSR >> RCC_CSR_FWRSTF_Pos;
	// Clear reset flags
	SET_BIT(RCC->CSR, RCC_CSR_RMVF);

	// Get Boot reason
	boot_info |= tempreg;
	return boot_info;
}

static
void boot_save_info(uint32_t u32BootInfo)
{
	register __IO uint32_t can_save = 0;

	// Check if RTC is enable
	can_save = RCC->BDCR & RCC_BDCR_RTCEN;
	if (can_save)
	{
		u8UnstabCnt = (RTC->BOOT_INFO_BKPR & BOOT_UNSTAB_CNT_MASK) >> BOOT_UNSTAB_CNT_POS;
		u8UnauthCnt = (RTC->BOOT_INFO_BKPR & BOOT_UNAUTH_CNT_MASK) >> BOOT_UNAUTH_CNT_POS;
	}

	if(u32BootInfo == 0)
	{
		u8UnstabCnt = 0;
		u8UnauthCnt = 0;
	}
	// check if instability
	if(u32BootInfo & INSTAB_DETECT)
	{
		// increment boot_cnt
		u8UnstabCnt++;
	}
	// check if unauth access
	if(u32BootInfo & UNAUTH_ACCESS)
	{
		// increment unauth_cnt
		u8UnauthCnt++;
	}
	// Check Backup domain
	if (can_save)
	{
		// If enable, then save boot info can be saved into backup registers
		u32BootInfo &= BOOT_INFO_MASK;
		u32BootInfo |= (u8UnauthCnt << BOOT_UNAUTH_CNT_POS);
		u32BootInfo |= (u8UnstabCnt << BOOT_UNSTAB_CNT_POS);

		// Save the BootState
		RTC->BOOT_INFO_BKPR = u32BootInfo;
	}
	//else
	{
		// If not enable, maybe application never start.
		// So keep boot info as local variable in RAM
	}
}

/******************************************************************************/
__attribute__ ((naked))
__attribute__ ((no_return)) //__attribute__ ((section (".boot")))
void boot_strap(void)
{
	register struct __exch_info_s* pp = (struct __exch_info_s *)&(__exchange_area_org__);
	register boot_request_e do_it;

	TRACE_INIT();

	unsigned int boot_info = boot_get_info();
	do_it = preload(pp);
	TRACE(TRACE_MSG_ENTER);

#ifdef HAS_CRC_COMPUTE
	crc_init();
	pp->crc = crc_compute( (uint32_t*)pp, (uint32_t)(sizeof(struct __exch_info_s) - 4) / 4);
	crc_deinit();
#else
	pp->crc = 0;
#endif

	if (do_it == BOOT_REQ_UPDATE)
	{
		TRACE(TRACE_MSG_REQ_UPDATE);
		TRACE(TRACE_MSG_SWAP);
		swap(pp);
		boot_save_info(0);
	}
	else
	{
		register unsigned int start;
#ifdef USE_ENABLE_IWDG
		protect(pp);
#endif
		if (do_it == BOOT_REQ_NONE )
		{
			start = pp->src + HEADER_SZ;
			// jump to app
			SCB->VTOR = start;
			TRACE(TRACE_MSG_REQ_NONE);
			TRACE(TRACE_MSG_APP);
		}
		else // do_it == BOOT_REQ_LOCAL
		{
			if (do_it == BOOT_REQ_LOCAL)
			{
				TRACE(TRACE_MSG_REQ_LOCAL);
			}
			else
			{
				TRACE(TRACE_MSG_REQ_UNK);
			}

#ifdef HAS_2ND_STAGE_OR_SYS_BL
			// 2nd Stage BL is required
			if(pfBL_2ndStage)
			{
				TRACE(TRACE_MSG_BL2);
				pfBL_2ndStage(pp);
			}
			else
			{
				// TODO :
				// read gpio to check if ST bootloader is authorized
				start = SYSMEM_ADDRESS;
			    __HAL_RCC_SYSCFG_CLK_ENABLE();
			    MODIFY_REG(SYSCFG->MEMRMP, SYSCFG_MEMRMP_MEM_MODE, SYSCFG_MEMRMP_MEM_MODE_0);
			    TRACE(TRACE_MSG_SYS);
			}
#else
			// panic...try to generate hardfault
			* (uint32_t *)(0xFFFFFFFF) = 0;
#endif
		}
		TRACE_FINI();
		boot_save_info(boot_info);
#ifdef USE_ENABLE_IWDG
		wdg_init();
		wdg_refresh();
#endif
	    __set_MSP(*(volatile uint32_t*)start);
	    JMP( (*(volatile uint32_t*)(start + 4)) );
	}
	TRACE(TRACE_MSG_REBOOT);
	reboot();
}

