
#if defined(__cplusplus)
extern "C"
{
#endif

#include "def.h"
#include "protect.h"
#include "flash.h"
#include "wdg.h"

#include <stm32l4xx_hal.h>
//#include <stm32l4xx_hal_cortex.h>

/******************************************************************************/

/*
 * Case 1 : Expect that Main Flash could be blank
 *    - nBOOT1 [23]   = X
 *    - nBOOT0 [27]   = X
 *    - nSWBOOT0 [26] = 1
 * 0b xxxxx1xx xxxxxxxx xxxxxxxx xxxxxxxx
 *
 * Case 2 : Force to Boot from Main Flash (Don't care if programmed or not)
 *    - nBOOT1 [23]   = X
 *    - nBOOT0 [27]   = 1
 *    - nSWBOOT0 [26] = 0
 * 0b xxxx10xx xxxxxxxx xxxxxxxx xxxxxxxx
 *
 * Case 3 : Force to Boot from SRAM
 *    - nBOOT1 [23]   = 0
 *    - nBOOT0 [27]   = 0
 *    - nSWBOOT0 [26] = 0
 * 0b xxxx00xx 0xxxxxxx xxxxxxxx xxxxxxxx
 *
 * Case 1 0b xxxx01xx 0xxxxxxx xxxxxxxx xxxxxxxx
 * Case 2 0b xxxx10xx 0xxxxxxx xxxxxxxx xxxxxxxx
 * Case 3 0b xxxx00xx 0xxxxxxx xxxxxxxx xxxxxxxx
 *
 */

/*
 * Required setup for Flash Option Byte :
 * 		Bit 27 nBOOT0: nBOOT0 option bit
 * 			= keep previous setting
 * 		Bit 26 nSWBOOT0: Software BOOT0
 * 			= keep previous setting
 * 		Bit 25 SRAM2_RST: SRAM2 Erase when system reset
 * 			= keep previous setting
 * 		Bit 24 SRAM2_PE: SRAM2 parity check enable
 * 			= keep previous setting
 * 		Bit 23 nBOOT1: Boot configuration
 * 			= keep previous setting
 * 		Bit 18 : IWDG_STDBY: Independent watchdog counter freeze in Standby mode
 * 			= 0: Independent watchdog counter is frozen in Standby mode
 * 		Bit 17 : IWDG_STOP: Independent watchdog counter freeze in Stop mode
 * 			= 0: Independent watchdog counter is frozen in Stop mode
 * 		Bit 16 : IDWG_SW: Independent watchdog selection
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
#define OB_nBOOT // Get previous settings
#define OB_SRAM2 // Get previous settings
#define OB_WWDG  // Get previous settings
#define OB_IWDG (OB_IWDG_STDBY_FREEZE | OB_IWDG_STOP_FREEZE | OB_IWDG_SW)
#define OB_nRST (OB_SHUTDOWN_NORST | OB_STANDBY_NORST | OB_STOP_NORST)
#define OB_BOR OB_BOR_LEVEL_0
#define OB_RDP OB_RDP_LEVEL_0

#define OB_KEEP_PREV_MSK 0xFFF88800

#define OB_SETTING_REG (OB_IWDG | OB_nRST | OB_BOR | OB_RDP)

static inline uint32_t RAMFUNCTION get_wdt_ob(uint32_t ob_org)
{
	uint32_t ob_reg;
	ob_reg = ob_org;
	// Mask keep bits
	ob_reg &= OB_KEEP_PREV_MSK;
	// Apply setting
	ob_reg |= OB_SETTING_REG;
	return ob_reg;
}

/*
 * RDP = 0x11 to set protection level 1
 *
 * Changing RDP from level 1 to 0, then following are erased :
 * - Backup registers (RT_BKPxR in RTC)
 * - SRAM2 memory
 * - Flash memory :
 *   - if PCROP_RDP bit is set : mass erase
 *   - if PCROP_RDP bit is clear : partial erase (all pages not in PCROP area)
 *   Warning, PCROP area is executable only
 *
 *
 *
 *
 */

/*
void RAMFUNCTION hal_flash_ob_unlock(void)
{
	if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) != RESET)
	{
		// Authorize the FLASH Registers access
		WRITE_REG(FLASH->KEYR, FLASH_KEY1);
		WRITE_REG(FLASH->KEYR, FLASH_KEY2);
	}

	if (READ_BIT(FLASH->CR, FLASH_CR_OPTLOCK) != RESET)
	{
		// Authorizes the Option Byte register programming
		WRITE_REG(FLASH->OPTKEYR, FLASH_OPTKEY1);
		WRITE_REG(FLASH->OPTKEYR, FLASH_OPTKEY2);
	}

	// Verify Flash is unlocked
	if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0U)
	{
		status = HAL_ERROR;
	}
}

void RAMFUNCTION hal_flash_ob_launch(void)
{
	// Set the bit to force the option byte reloading
	SET_BIT(FLASH->CR, FLASH_CR_OBL_LAUNCH);
	status = hal_flash_wait_last_op((uint32_t)FLASH_WAIT_OP_TMO);

	// Set the OPTLOCK Bit to lock the FLASH Option Byte Registers access
	SET_BIT(FLASH->CR, FLASH_CR_OPTLOCK);
	SET_BIT(FLASH->CR, FLASH_CR_LOCK);
}

typedef struct
{
	uint32_t u32OPTR;
	uint16_t u16PCROP1_START;
	uint16_t u16PCROP1_END;

	uint8_t u8WRP1A_START;
	uint8_t u8WRP1A_END;
	uint8_t u8WRP1B_START;
	uint8_t u8WRP1B_END;

} ob_setup_t;

#define OB_SETUP_RDP
#define OB_SETUP_PCROP1_START
#define OB_SETUP_PCROP1_END
#define OB_SETUP_WRP1A_START
#define OB_SETUP_WRP1A_END
#define OB_SETUP_WRP1B_START
#define OB_SETUP_WRP1B_END

void RAMFUNCTION hal_flash_ob_write(uint32_t Address, uint8_t *Data, uint32_t DataLength)
{

	register const unsigned int *p = (const unsigned int *)__get_part_tab__();
	register const unsigned int *s = (unsigned int *)__get_part_size__();

	uint32_t len = 0;
	// Unlock the FLASH & Option Bytes Registers access
	hal_flash_ob_unlock();

	// Clear error programming flags
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_SR_ERRORS);

	// Write RDP Level
	WRITE_REG(FLASH->OPTR, *(Data));
	len += 4;

	// Write OPTR
	if (DataLength >= 4)
	{
		WRITE_REG(FLASH->OPTR, (*(Data) | (*(Data + 1) << 8) | (*(Data + 2) << 16) | (*(Data + 3) << 24)));
	}


	//
	uint32_t first_page;
	uint32_t last_page;
	uint32_t temp;

	first_page = hal_flash_get_page(p[0]);
	last_page = hal_flash_get_page(p[0] + s[0]);
	temp = first_page | (last_page << 16);
	WRITE_REG(FLASH->WRP1AR,  temp);



	WRITE_REG(FLASH->WRP1BR, temp);


	// ***********************************
	// Write PCROP1SR
	if (DataLength >= 10) // 6 = 4 + 2
	{
		WRITE_REG(FLASH->PCROP1SR, (*(Data + 8) | (*(Data + 9) << 8)));
	}

	// Write PCROP1ER
	if (DataLength >= 20) // 10 =
	{
		WRITE_REG(FLASH->PCROP1ER, (*(Data + 16) | (*(Data + 17) << 8) | (*(Data + 19) << 24)));
	}

	// Write WRP1AR
	if (DataLength >= 28)
	{
		WRITE_REG(FLASH->WRP1AR, (*(Data + 24) | (*(Data + 25) << 8) | (*(Data + 26) << 16) | (*(Data + 27) << 24)));
	}

	// Write WRP1BR
	if (DataLength >= 36)
	{
		WRITE_REG(FLASH->WRP1BR, (*(Data + 32) | (*(Data + 33) << 8) | (*(Data + 34) << 16) | (*(Data + 35) << 24)));
	}

	// **********************************

	SET_BIT(FLASH->CR, FLASH_CR_OPTSTRT);

	// Register system reset callback
	Common_SetPostProcessingCallback(OPENBL_OB_Launch);
}
*/

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

int RAMFUNCTION protect(register struct __exch_info_s * pp)
{
	uint32_t status = 0;
	uint32_t current;
	uint32_t expected;

	// Get current loaded OB
	current = hal_flash_ob_read();
	// Set OB for WDG
	expected = get_wdt_ob(current);
	// If the current OB is not what we expect, then re-prog OB
	if (expected != current)
	{
		// Unlock Flash reg
		hal_flash_lock(0);
		// Unlock OB reg
		hal_flash_ob_lock(0);
		// Writ OB
		status = hal_flash_ob_write(expected);

//#ifdef OB_LAUNCH_REQUIRED
		if (!status)
		{
			// This will generate a RESET
			status = hal_flash_ob_launch();
		}
//#endif
		// Lock OB reg : not required (flash_lock will do it)
		// hal_flash_ob_lock(1);
		// Lock Flash reg
		hal_flash_lock(1);
	}
	return (int)status;
}

/******************************************************************************/
#if defined(__cplusplus)
}
#endif
