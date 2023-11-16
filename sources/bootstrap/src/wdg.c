#ifdef __cplusplus
extern "C" {
#endif

#include "def.h"
#include "wdg.h"

//#include <stm32l4xx_hal_rcc.h>
#include "stm32l4xx_hal_iwdg.h"

/******************************************************************************/
/*
 * IWDG Setup :
 * - LSI clock enable
 * - Registers :
 *   - IWDG_PR =  0x0000 0007 // divide by 256
 *   - IWDG_RLR = 0x0000 0FFF // down count from 4096
 *
 * So :
 * LSI = 32000 Hz;
 * With PR = 1/256, gives 125 Hz;
 * With RLR = 4096, gives 32,768 second;
 *
 */
#define IWDG_PR  0x00000007
#define IWDG_RLR 0x00000FFF

void RAMFUNCTION wdg_init(void)
{
#ifdef DEBUG
#if defined(HAL_IWDG_MODULE_ENABLED)
	__HAL_DBGMCU_FREEZE_IWDG();
#endif
#endif
	IWDG->KR  = IWDG_KEY_ENABLE;
	IWDG->KR  = IWDG_KEY_WRITE_ACCESS_ENABLE;
	IWDG->PR  = IWDG_PR;
	IWDG->RLR = IWDG_RLR;
	//while (IWDG->SR != 0x00u) { }
}


void RAMFUNCTION wdg_refresh(void)
{
	IWDG->KR = IWDG_KEY_RELOAD;
}

#ifdef __cplusplus
}
#endif
