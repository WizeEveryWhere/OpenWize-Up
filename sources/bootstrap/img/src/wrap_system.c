
extern unsigned int __vtors_location;
extern unsigned int __vtors_start;
extern unsigned int __vtors_end;

#include "stm32l4xx_hal_conf.h"

extern void __real_SystemInit(void);

void __wrap_SystemInit(void)
{
	// call original "void SystemInit(void)"
	__real_SystemInit();
	/* Configure the Vector Table location add offset address ------------------*/
	SCB->VTOR = (uint32_t)(&__vtors_start);
}
