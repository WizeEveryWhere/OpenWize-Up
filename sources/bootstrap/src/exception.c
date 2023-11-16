#if defined(__cplusplus)
extern "C"
{
#endif

extern void infinite_loop(void);

#define INFINITE_LOOP() infinite_loop()// while (1) { }

void NMI_Handler(void) { INFINITE_LOOP(); }
void HardFault_Handler(void)
{
	INFINITE_LOOP();
}
void MemManage_Handler(void)
{
	INFINITE_LOOP();
}
void BusFault_Handler(void)
{
	INFINITE_LOOP();
}
void UsageFault_Handler(void)
{
	INFINITE_LOOP();
}

/******************************************************************************/
#include "def.h"

#include <stdint.h>
#include <stm32l4xx.h>

const uint32_t CoreClock = 4000000;
uint8_t wait_tmo;
void SysTick_Handler(void)
{
	SysTick->CTRL = 0UL;
	wait_tmo = 0;
}

void RAMFUNCTION SysTick_StopTmoMs( void )
{
	// Stop the SysTick.
    SysTick->CTRL = 0UL;
    wait_tmo = 0;
}

uint32_t RAMFUNCTION SysTick_StartTmoMs( uint32_t msTmo )
{
	// Stop the SysTick.
	SysTick_StopTmoMs();
	wait_tmo = 1;
    // Configure SysTick to interrupt at the requested rate.
    return SysTick_Config( (CoreClock / 1000U) *msTmo );
}

/******************************************************************************/
#if defined(__cplusplus)
}
#endif
