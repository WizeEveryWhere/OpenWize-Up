#if defined(__cplusplus)
extern "C"
{
#endif

#include "def.h"
#include "flash.h"

#include <stm32l4xx_hal.h>
#include <stm32l4xx_hal_cortex.h>

/******************************************************************************/
extern uint8_t wait_tmo;
extern void SysTick_StopTmoMs( void );
extern uint32_t SysTick_StartTmoMs( uint32_t msTmo );

#define FLASH_WAIT_OP_TMO 4000 // 4194 ms max with systick
/******************************************************************************/
#define IS_DUAL_BANK() (FLASH_SIZE - FLASH_BANK_SIZE)?(1):(0)
#define TOTAL_PAGES() (FLASH_SIZE / FLASH_PAGE_SIZE)
#define LAST_PAGES() ((FLASH_SIZE / FLASH_PAGE_SIZE) - 1)

/******************************************************************************/
#define FLASH_NB_DOUBLE_WORDS_IN_ROW  32

/******************************************************************************/
static uint32_t RAMFUNCTION hal_flash_save_idcen(void)
{
	register uint32_t saved_acr = FLASH->ACR & (FLASH_ACR_ICEN | FLASH_ACR_DCEN);
	// Disable instruction and Data cache
	CLEAR_BIT(FLASH->ACR, saved_acr);
	return saved_acr;
}

static void RAMFUNCTION hal_flash_restore_idcen(uint32_t acr)
{
	// Flush the caches to be sure of the data consistency
	SET_BIT(FLASH->ACR, (FLASH_ACR_ICRST | FLASH_ACR_DCRST ) );
	CLEAR_BIT(FLASH->ACR, (FLASH_ACR_ICRST | FLASH_ACR_DCRST) );
	// Enable the caches
	SET_BIT(FLASH->ACR, (acr & (FLASH_ACR_ICEN | FLASH_ACR_DCEN) ) );
}

static uint32_t RAMFUNCTION hal_flash_wait_last_op(uint32_t Timeout)
{
	register uint32_t error;
	register uint32_t *sr = &FLASH->SR;

    if(Timeout != 0xFFFFFFFFU)
    {
    	if(SysTick_StartTmoMs(Timeout))
    	{
    		// failed to init tmo
    		Timeout = 0xFFFFFFFFU;
    	}
    }

	do
	{
		if ( (Timeout != 0xFFFFFFFFU) && ( wait_tmo == 0)  )
		{
			return 0xFFFFFFFFU;
		}
	} while((*sr & FLASH_SR_BSY) == FLASH_SR_BSY);

	SysTick_StopTmoMs();

	error = *sr & ( FLASH_FLAG_SR_ERRORS  & ~(FLASH_FLAG_PEMPTY));
	// Clear Errors and End of Operation flag
	*sr = error | FLASH_SR_EOP;
	return error;
}

/******************************************************************************/
void RAMFUNCTION hal_flash_clear_errors(void)
{
	WRITE_REG(FLASH->SR, (FLASH_FLAG_SR_ERRORS & ~(FLASH_FLAG_PEMPTY) ));
}

uint32_t RAMFUNCTION hal_flash_get_page(register uint32_t u32Address)
{
	return (u32Address - FLASH_BASE) / FLASH_PAGE_SIZE;
}

uint32_t RAMFUNCTION hal_flash_is_aligned(register uint32_t u32Address)
{
    return ((u32Address) % FLASH_PAGE_SIZE)?(0):(1);
}

void RAMFUNCTION hal_flash_lock(register uint8_t bLock)
{
	register FLASH_TypeDef *pflash_reg = FLASH;
	register uint32_t temp = pflash_reg->CR;
	if(bLock)
	{
		pflash_reg->CR = temp | FLASH_CR_LOCK;
	}
	else
	{
		if (temp & FLASH_CR_LOCK)
		{
			pflash_reg->KEYR = FLASH_KEY1;
			pflash_reg->KEYR = FLASH_KEY2;
		}
	}
}

uint32_t RAMFUNCTION hal_flash_erase(uint32_t Page, uint32_t NbPages, uint32_t *PageError)
{
	register uint32_t status;
	// Wait for last operation to be completed
	status = hal_flash_wait_last_op((uint32_t)FLASH_WAIT_OP_TMO);

	if (!status)
	{
		register uint32_t page_index;
		// Disable instruction and Data cache
		uint32_t saved_acr = hal_flash_save_idcen();

		// Init. last acceptable page
		register uint32_t last_page = TOTAL_PAGES();
	    if (IS_DUAL_BANK())
	    {
	    	last_page /= 2;
	    }
	    last_page--;

		// Initialization of PageError variable
	    *PageError = 0xFFFFFFFFU;

		for(page_index = Page; page_index < (Page + NbPages); page_index++)
		{
			if (page_index > last_page)
			{
				FLASH_PageErase(page_index - last_page - 1, 2);
			}
			else
			{
				FLASH_PageErase(page_index, 1);
			}

			// Wait for last operation to be completed
			status = hal_flash_wait_last_op((uint32_t)FLASH_WAIT_OP_TMO);

			// If the erase operation is completed, disable the PER Bit
			CLEAR_BIT(FLASH->CR, (FLASH_CR_PER | FLASH_CR_PNB));

			if (status)
			{
				// In case of error, stop erase procedure and return the faulty address
				*PageError = page_index;
				break;
			}
		}

		// Flush then enable the caches, as saved
		hal_flash_restore_idcen(saved_acr);
  }
  return status;
}

uint32_t RAMFUNCTION hal_flash_write(uint32_t dest, uint32_t src, uint32_t nbLine)
{
	uint32_t status;
	// Wait for last operation to be completed
	status = hal_flash_wait_last_op((uint32_t)FLASH_WAIT_OP_TMO);
	if (!status)
	{
		// Disable instruction and Data cache
		uint32_t saved_acr = hal_flash_save_idcen();
		//uint32_t primask_bit;
		// Double Word programming as default
		uint32_t prog_bit = FLASH_CR_PG;
		register uint8_t row_index;

		register __IO uint32_t *dest_addr = (__IO uint32_t*)dest;
		register __IO uint32_t *src_addr = (__IO uint32_t*)src;

		// Fast programming
		// Warning : During fast programming, the CPU clock frequency (HCLK) must be at least 8 MHz.
		// prog_bit = FLASH_CR_FSTPG;
		//

		// Loop on each data
		register uint32_t nb_line = nbLine;
		while(nb_line)
		{
			nb_line--;
			// Set PG or FSTPG bit
			SET_BIT(FLASH->CR, prog_bit);
			// ------
			{
				row_index = 2;
			}
			// ------
			// Program double word of the row */
			do
			{
				// Write one word
				*dest_addr = *src_addr;
				{
					__ISB();
				}
				dest_addr++;
				src_addr++;
				row_index--;
			} while (row_index != 0U);
			// ------
			// Wait for last operation to be completed
			status = hal_flash_wait_last_op((uint32_t)FLASH_WAIT_OP_TMO);
			// Disable the PG or FSTPG Bit
			CLEAR_BIT(FLASH->CR, prog_bit);
			if(status)
			{
				break;
			}
		}
		// Flush then enable the caches, as saved
		hal_flash_restore_idcen(saved_acr);
	}
	return status;
}

/******************************************************************************/
/*
 * Note on "Option Byte" :
 * - Flash memory address : 0x1FFF7800
 * - ST programmed value  : 0xFFEFF8AA
 * 		Bit 27 nBOOT0: nBOOT0 option bit
 * 			= 1: nBOOT0 = 1
 * 		Bit 26 nSWBOOT0: Software BOOT0
 * 			= 1: BOOT0 taken from PH3/BOOT0 pin
 * 		Bit 25 SRAM2_RST: SRAM2 Erase when system reset
 * 			= 1: SRAM2 is not erased when a system reset occurs
 * 		Bit 24 SRAM2_PE: SRAM2 parity check enable
 * 			= 1: SRAM2 parity check disable
 * 		Bit 23 nBOOT1: Boot configuration
 * 			= 1
 *      Bit 19 WWDG_SW: Window watchdog selection
 * 			= 1: Software window watchdog
 * 		Bit 18 IWDG_STDBY: Independent watchdog counter freeze in Standby mode
 * 			= 1: Independent watchdog counter is running in Standby mode
 * 		Bit 17 IWDG_STOP: Independent watchdog counter freeze in Stop mode
 * 			= 1: Independent watchdog counter is running in Stop mode
 * 		Bit 16 IDWG_SW: Independent watchdog selection
 * 			= 1: Software independent watchdog
 * 		Bit 14 nRST_SHDW
 * 			= 1: No reset generated when entering the Shutdown mode
 * 		Bit 13 nRST_STDBY
 * 			= 1: No reset generate when entering the Standby mode
 * 		Bit 12 nRST_STOP
 * 			= 1: No reset generated when entering the Stop mode
 * 		Bits 10:8 BOR_LEV: BOR reset Level
 * 			= 000: BOR Level 0. Reset level threshold is around 1.7 V
 * 		Bits 7:0 RDP: Read protection level
 * 			= 0xAA: Level 0, read protection not active
 */

void RAMFUNCTION hal_flash_ob_lock(register uint8_t bLock)
{
	register uint32_t temp = FLASH->CR;
	if(bLock)
	{
		FLASH->CR = temp | FLASH_CR_OPTLOCK;
	}
	else
	{
		if (temp & FLASH_CR_OPTLOCK)
		{
			FLASH->OPTKEYR = FLASH_OPTKEY1;
			FLASH->OPTKEYR = FLASH_OPTKEY2;
		}
	}
}

uint32_t RAMFUNCTION hal_flash_ob_read(void)
{
	return FLASH->OPTR;
}

uint32_t RAMFUNCTION hal_flash_ob_write(uint32_t ob_value)
{
	uint32_t status;

	// WARNING : RDP_Level_2 is irreversible
	// So is forbidden,
	// ...but can be changed it if you are sure it's what you want to do.
	if ( (ob_value & 0xFF) == OB_RDP_LEVEL_2)
	{
		return -1;
	}
	// ----
	status = hal_flash_wait_last_op((uint32_t)FLASH_WAIT_OP_TMO);
	if (!status)
	{
		// Setup the OB register
		FLASH->OPTR = ob_value;

		// Start Option modification
		SET_BIT(FLASH->CR, FLASH_CR_OPTSTRT);

		// Wait for operation complete
		status = hal_flash_wait_last_op((uint32_t)FLASH_WAIT_OP_TMO);

		// ...not required (even if ST HAL do it ?)
		// CLEAR_BIT(FLASH->CR, FLASH_CR_OPTSTRT);
	}
	return status;
}

uint32_t RAMFUNCTION hal_flash_ob_launch(void)
{
	// Set the bit to force the option byte reloading
	SET_BIT(FLASH->CR, FLASH_CR_OBL_LAUNCH);
	// Wait for operation complete
	return hal_flash_wait_last_op((uint32_t)FLASH_WAIT_OP_TMO);
}


/******************************************************************************/
#if defined(__cplusplus)
}
#endif
