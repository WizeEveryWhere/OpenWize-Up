#ifndef _UART_H_
#define _UART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <stm32l4xx.h>
#include <stm32l4xx_ll_bus.h>
#include <stm32l4xx_ll_rcc.h>
#include <stm32l4xx_ll_gpio.h>

/******************************************************************************/

// Assume we come from Reset
/* At reset
 * 	GPIOx_MODER
 * 	   0xABFF FFFF (for port A)
 * 	   0xFFFF FEBF (for port B)
 * GPIOx_OSPEEDR
 *     0x0C00 0000 (for port A)
 *     0x0000 0000 (for the other ports)
 * GPIOx_PUPDR
 *     0x6400 0000 (for port A)
 *     0x0000 0100 (for port B)
 * GPIOx_AFRL
 *     0x0000 0000
 * GPIOx_AFRH
 *     0x0000 0000
 *
*/
/*
 * Port A : UART4 is on this port
 * Port B : LPUART1 is on this port
*/

/*
 * Just change GPIOB_MODER and take care about LPUART1:
 * - GPIOx_MODER_MSK 0xFFFFCFFF
*/

// Read GPIO PB6 (IO1)
// If 1 : enable the trace
// If 0 : disable the trace
#define TRACE_EN_GPIO_PORT GPIOB
#define TRACE_EN_GPIO_PIN 6
#define TRACE_EN_GPIO_CLK_ENABLE()  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB)
#define TRACE_EN_GPIO_CLK_DISABLE() LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOB)


#define TRACE_EN_GPIO_MODER_MSK() (~(0x3 << (TRACE_EN_GPIO_PIN << 1))) // 0xFFFFCFFF
#define TRACE_EN_GPIO_PUPD_MSK() (0x2 << (TRACE_EN_GPIO_PIN << 1)) // 0x2000

#define TRACE_EN_GPIO_SETUP() \
	register uint32_t temp; \
	/* Set GPIO PB6 Pull-Down */ \
	temp = TRACE_EN_GPIO_PORT->PUPDR & TRACE_EN_GPIO_MODER_MSK(); \
	temp |= TRACE_EN_GPIO_PUPD_MSK(); \
	TRACE_EN_GPIO_PORT->PUPDR = temp; \
	/* Set GPIO PB6 as Input */ \
	temp = TRACE_EN_GPIO_PORT->MODER & TRACE_EN_GPIO_MODER_MSK(); \
	TRACE_EN_GPIO_PORT->MODER = temp; \

#define TRACE_IS_ENABLE() (TRACE_EN_GPIO_PORT->IDR >> TRACE_EN_GPIO_PIN ) & 0x1

/******************************************************************************/
/*
 * Assume that :
 * - MSI clock is selected as SYSCLK
 * - MSI range is the default one (then MSI clock is 4 MHz)
 * - AHB prescaler is div by 1
 * - APB prescaler is div by 1
 * - PCLK1 is selected as UARTx clock
 */
#define PERIPHERAL_FREQ 4000000U
#define USARTx_BAUDRATE 115200U

/*
 * Setup UART to default with :
 * LL_USART_DATAWIDTH_8B
 * LL_USART_STOPBITS_1
 * LL_USART_PARITY_NONE
 * LL_USART_HWCONTROL_NONE
 * LL_USART_OVERSAMPLING_16
 * LL_USART_DIRECTION_TX_RX
 * USART_CR3_OVRDIS to 1
 *
 */
#define USARTx_CR1 0x0000000C
#define USARTx_CR2 0x00000000
#define USARTx_CR3 0x00001000

#if USE_LPUART1
	#include <stm32l4xx_ll_lpuart.h>
	#define USARTx                   LPUART1
	#define USARTx_CLK_ENABLE()      LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_LPUART1)
	#define USARTx_CLK_DISABLE()     LL_APB1_GRP2_DisableClock(LL_APB1_GRP2_PERIPH_LPUART1)

	#define USARTx_GPIO_CLK_ENABLE()  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB)
	#define USARTx_GPIO_CLK_DISABLE() LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOB)

	#define USARTx_GPIO_DeInit() \
	{ \
		LL_AHB2_GRP1_ForceReset(LL_AHB2_GRP1_PERIPH_GPIOB); \
		LL_AHB2_GRP1_ReleaseReset(LL_AHB2_GRP1_PERIPH_GPIOB); \
	}

	#define USARTx_GPIO_PORT GPIOB
	// USARTx_TX_PIN    LL_GPIO_PIN_11
	// USARTx_RX_PIN    LL_GPIO_PIN_10
	// USARTx_ALTERNATE LL_GPIO_AF_8
	#define GPIOx_MODER       0x00A00000
	#define GPIOx_OSPEEDR     0x00A00000
	#define GPIOx_PUDR        0x00500000
	#define GPIOx_AFRL        0x00000000
	#define GPIOx_AFRH        0x00008800

	#define GPIOx_MODER_MSK   0xFF0FFFFF
	#define GPIOx_OSPEEDR_MSK 0xFF0FFFFF
	#define GPIOx_PUDR_MSK    0xFF0FFFFF
	#define GPIOx_AFRL_MSK    0xFFFFFFFF
	#define GPIOx_AFRH_MSK    0xFFFF00FF

	#define USARTx_BRR __LL_LPUART_DIV(PERIPHERAL_FREQ, USARTx_BAUDRATE);

	#define USARTx_DeInit() \
	{ \
		LL_APB1_GRP2_ForceReset(LL_APB1_GRP2_PERIPH_LPUART1); \
		LL_APB1_GRP2_ReleaseReset(LL_APB1_GRP2_PERIPH_LPUART1); \
	}

#else
	#include <stm32l4xx_ll_usart.h>
	#define USARTx                   UART4
	#define USARTx_CLK_ENABLE()      LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART4)
    #define USARTx_CLK_DISABLE()     LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_UART4)

	#define USARTx_GPIO_CLK_ENABLE()  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
	#define USARTx_GPIO_CLK_DISABLE() LL_AHB2_GRP1_DisableClock(LL_AHB2_GRP1_PERIPH_GPIOA)

	#define USARTx_GPIO_DeInit() \
	{ \
		LL_AHB2_GRP1_ForceReset(LL_AHB2_GRP1_PERIPH_GPIOA); \
		LL_AHB2_GRP1_ReleaseReset(LL_AHB2_GRP1_PERIPH_GPIOA); \
	}

	#define USARTx_GPIO_PORT GPIOA
	// USARTx_TX_PIN    LL_GPIO_PIN_0
	// USARTx_RX_PIN    LL_GPIO_PIN_1
	// USARTx_ALTERNATE LL_GPIO_AF_8
	#define GPIOx_MODER       0x0000000A
	#define GPIOx_OSPEEDR     0x0000000A
	#define GPIOx_PUDR        0x00000005
	#define GPIOx_AFRL        0x00000088
	#define GPIOx_AFRH        0x00000000

	#define GPIOx_MODER_MSK   0xFFFFFFF0
	#define GPIOx_OSPEEDR_MSK 0xFFFFFFF0
	#define GPIOx_PUDR_MSK    0xFFFFFFF0
	#define GPIOx_AFRL_MSK    0xFFFFFF00
	#define GPIOx_AFRH_MSK    0xFFFFFFFF

	#define USARTx_BRR (uint16_t)(__LL_USART_DIV_SAMPLING16(PERIPHERAL_FREQ, USARTx_BAUDRATE))

	#define USARTx_DeInit() \
	{ \
		LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_UART4); \
		LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_UART4); \
	}
#endif

void hal_uart_init(void);
void hal_uart_send(uint8_t c);
uint8_t hal_uart_read(void);

#ifdef __cplusplus
}
#endif

#endif /* _UART_H_ */
