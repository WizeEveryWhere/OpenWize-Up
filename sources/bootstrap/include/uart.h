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
	#define USARTx_GPIO_CLK_ENABLE() LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB)

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

#else
	#include <stm32l4xx_ll_usart.h>
	#define USARTx                   UART4
	#define USARTx_CLK_ENABLE()      LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART4)
	#define USARTx_GPIO_CLK_ENABLE() LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)

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

#endif




#define USARTx_DeInit()                   LL_USART_DeInit(USARTx)


void hal_uart_init(void);
void hal_uart_send(uint8_t c);
uint8_t hal_uart_read(void);

#ifdef __cplusplus
}
#endif

#endif /* _UART_H_ */
