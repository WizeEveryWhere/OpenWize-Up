	.syntax unified
	.cpu cortex-m4
	.fpu softvfp
	.thumb

.global	g_pfnVectors
.global	Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word	_sidata
/* start address for the .data section. defined in linker script */
.word	_sdata
/* end address for the .data section. defined in linker script */
.word	_edata
/* start address for the .bss section. defined in linker script */
.word	_sbss
/* end address for the .bss section. defined in linker script */
.word	_ebss

.equ  BootRAM,        0xF1E0F85F
/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
*/

    @.section	.text.Reset_Handler
    .section    .boot.Reset_Handler
	.weak	Reset_Handler
	.type	Reset_Handler, %function
Reset_Handler:
    ldr   sp, =_estack    /* Atollic update: set stack pointer */

    /* Copy the data segment initializers from flash to SRAM */
    movs	r1, #0
    b	LoopCopyDataInit

CopyDataInit:
	ldr	r3, =_sidata
	ldr	r3, [r3, r1]
	str	r3, [r0, r1]
	adds	r1, r1, #4

LoopCopyDataInit:
	ldr	r0, =_sdata
	ldr	r3, =_edata
	adds	r2, r0, r1
	cmp	r2, r3
	bcc	CopyDataInit

@ Initbss
	ldr	r2, =_sbss
	b	LoopFillZerobss
/* Zero fill the bss segment. */
FillZerobss:
	movs	r3, #0
	str	r3, [r2], #4

LoopFillZerobss:
	ldr	r3, = _ebss
	cmp	r2, r3
	bcc	FillZerobss

	bl boot_strap

LoopForever:
    b LoopForever
    
.size	Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
    @.section	.text.Default_Handler,"ax",%progbits
    .section    .boot.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
	b	Infinite_Loop
	.size	Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex-M4.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/

.global __get_part_tab__
.global __get_part_size__
.global __get_magic_tab__

.global _tab_add_
.global _part_tab_
.global _part_size_
.global _magic_tab_

 	.section	.isr_vector,"a",%progbits
	.type	g_pfnVectors, %object
	.size	g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
	.word	_estack
	.word	Reset_Handler
	.word	NMI_Handler
	.word	HardFault_Handler
	.word	MemManage_Handler
	.word	BusFault_Handler
	.word	UsageFault_Handler
	.word	0
	.word	0
	.word	0
	.word	0
	.word	SVC_Handler
	.word	DebugMon_Handler
	.word	0
	.word	PendSV_Handler
	.word	SysTick_Handler
_no_vector_area_1_ :
	.word	0 @ WWDG_IRQHandler
	.word	0 @ PVD_PVM_IRQHandler
	.word	0 @ TAMP_STAMP_IRQHandler
	.word	0 @ RTC_WKUP_IRQHandler @_rtc_wkup_irq_handler_ :
	.word	0 @ FLASH_IRQHandler @_flash_irq_handler_ :
    .word   0 @ RCC_IRQHandler

    .word   0 @ EXTI0_IRQHandler
    .word   0 @ EXTI1_IRQHandler
    .word   0 @ EXTI2_IRQHandler
    .word   0 @ EXTI3_IRQHandler
    .word   0 @ EXTI4_IRQHandler
    .word   0 @ DMA1_Channel1_IRQHandler
    .word   0 @ DMA1_Channel2_IRQHandler
    .word   0 @ DMA1_Channel3_IRQHandler
    .word   0 @ DMA1_Channel4_IRQHandler
    .word   0 @ DMA1_Channel5_IRQHandler
    .word   0 @ DMA1_Channel6_IRQHandler
    .word   0 @ DMA1_Channel7_IRQHandler
    .word   0 @ ADC1_IRQHandler
    .word   0 @ CAN1_TX_IRQHandler
    .word   0 @ CAN1_RX0_IRQHandler
    .word   0 @ CAN1_RX1_IRQHandler
    .word   0 @ CAN1_SCE_IRQHandler
    .word   0 @ EXTI9_5_IRQHandler
    .word   0 @ TIM1_BRK_TIM15_IRQHandler
    .word   0 @ TIM1_UP_TIM16_IRQHandler
    .word   0 @ TIM1_TRG_COM_IRQHandler
    .word   0 @ TIM1_CC_IRQHandler
    .word   0 @ TIM2_IRQHandler
    .word   0 @ TIM3_IRQHandler
    .word   0
    .word   0 @ I2C1_EV_IRQHandler
    .word   0 @ I2C1_ER_IRQHandler
    .word   0 @ I2C2_EV_IRQHandler
    .word   0 @ I2C2_ER_IRQHandler
    .word   0 @ SPI1_IRQHandler
    .word   0 @ SPI2_IRQHandler
    .word   0 @ USART1_IRQHandler
    .word   0 @ USART2_IRQHandler
    .word   0 @ USART3_IRQHandler
    .word   0 @ EXTI15_10_IRQHandler
    .word   0 @ RTC_Alarm_IRQHandler
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0 @ SDMMC1_IRQHandler
    .word   0
    .word   0 @ SPI3_IRQHandler
    .word   0 @ UART4_IRQHandler
    .word   0
    .word   0 @ TIM6_DAC_IRQHandler
    .word   0
    .word   0 @ DMA2_Channel1_IRQHandler
    .word   0 @ DMA2_Channel2_IRQHandler
    .word   0 @ DMA2_Channel3_IRQHandler
    .word   0 @ DMA2_Channel4_IRQHandler
    .word   0 @ DMA2_Channel5_IRQHandler
    .word   0 @ DFSDM1_FLT0_IRQHandler
    .word   0 @ DFSDM1_FLT1_IRQHandler
    .word   0
    .word   0 @ COMP_IRQHandler
    .word   0 @ LPTIM1_IRQHandler
    .word   0 @ LPTIM2_IRQHandler
    .word   0
    .word   0 @ DMA2_Channel6_IRQHandler
    .word   0 @ DMA2_Channel7_IRQHandler
    .word   0 @ LPUART1_IRQHandler
    .word   0 @ QUADSPI_IRQHandler
    .word   0 @ I2C3_EV_IRQHandler
    .word   0 @ I2C3_ER_IRQHandler
    .word   0 @ SAI1_IRQHandler
    .word   0
    .word   0
    .word   0 @ TSC_IRQHandler
    .word   0
    .word   0
    .word   0 @ RNG_IRQHandler
    .word   0 @ FPU_IRQHandler
    .word   0 @ CRS_IRQHandler
    .word   0 @ I2C4_EV_IRQHandler
    .word   0 @ I2C4_ER_IRQHandler
__get_part_add__ :
    cmp r0, #2
    bhi __get_fail__
    lsls r0, #2    @ x4
    adr r4, _part_tab_
    ldr r0, [r4, r0]
    bx lr
__get_part_tab__ :
    adr r0, _part_tab_
    bx lr
__get_part_size__ :
    adr r0, _part_size_
    bx lr
__get_magic_tab__ :
    @adr sb, _magic_tab_
    adr r0, _magic_tab_
    bx lr
__get_fail__ :
    mvn r0, #0
    bx lr
    bx lr
_tab_add_ :
_part_tab_: @ Part tab address
    .word _A_org_   @Active part
    .word _I0_org_  @Inactive part 0
    .word _I1_org_  @Inactive part 1
_part_size_ :
    .word _A_size_
    .word _I0_size_
    .word _I1_size_
_magic_tab_ : @ Magic tab address
    .word _magic_I0_
    .word _magic_I1_
_end_vector_area_ :
    .word _end_vector_area_

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

	.weak	NMI_Handler
	.thumb_set NMI_Handler,Default_Handler

	.weak	HardFault_Handler
	.thumb_set HardFault_Handler,Default_Handler

	.weak	MemManage_Handler
	.thumb_set MemManage_Handler,Default_Handler

	.weak	BusFault_Handler
	.thumb_set BusFault_Handler,Default_Handler

	.weak	UsageFault_Handler
	.thumb_set UsageFault_Handler,Default_Handler

	.weak	SVC_Handler
	.thumb_set SVC_Handler,Default_Handler

	.weak	DebugMon_Handler
	.thumb_set DebugMon_Handler,Default_Handler

	.weak	PendSV_Handler
	.thumb_set PendSV_Handler,Default_Handler

	.weak	SysTick_Handler
	.thumb_set SysTick_Handler,Default_Handler
