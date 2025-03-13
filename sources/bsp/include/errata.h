#ifndef _ERRATA_H_
#define _ERRATA_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
//#include <stm32l4xx_hal.h>

/******************************************************************************/
// STM silicon errata

/*----------------------------------------------------------------------------*/
/* ES0387 - Rev 11 */

// System
/* 2.2.5 Unstable LSI when it clocks RTC or CSS on LSE */

/* 2.2.8 HSE oscillator long startup at low voltage */
/*
 * Description :
 * When VDD is below 2.7 V, the HSE oscillator may take longer than specified
 * to start up. Several hundred milliseconds might elapse before the HSERDY
 * flag in the RCC_CR register is set.
 *
 * Workaround :
 * The following sequence is recommended:
 * 1. Configure PH0 and PH1 as standard GPIOs in output mode and low-level state.
 * 2. Enable the HSE oscillator.
 */
inline void ES0387_REV11_2_2_8(void)
{
	/*
	 * Note : default values after reset in GPIOH registers
	 * MODER : 0x0000 000F
	 * IDR :  0x0000 XXXX
	 * All other register are : 0x0000 0000
	 *
	*/
	__HAL_RCC_GPIOH_CLK_ENABLE();
#define _MODER 0x00000005 // Set as output
#define _ODR   0x00000000 // Set to zero level
	GPIOH->MODER = _MODER;
	GPIOH->ODR = _ODR;
	__HAL_RCC_GPIOH_CLK_DISABLE();
}


/* 2.2.9 LSE crystal oscillator may be disturbed by transitions on PC13 */

/* 2.2.12 Debugging Sleep/Stop mode with WFE/WFI entry */

/* 2.2.14 Option validity error set after reset */

// DMA
/* 2.4.2 Byte and half-word accesses not supported */

// TIM
/* 2.13.4 HSE/32 is not available for TIM16 input capture if RTC clock is disabled or other than HSE */

// RTC
/* 2.15.2 RTC interrupt can be masked by another RTC interrupt */
/* 2.15.4 Alarm flag may be repeatedly set when the core is stopped in debug */

// UART
/* 2.17.3 Data corruption due to noisy receive line */

// LPUART
/* 2.18.2 Possible LPUART transmitter issue when using low BRR[15:0] value */

/******************************************************************************/


#ifdef __cplusplus
}
#endif
#endif /* _ERRATA_H_ */
