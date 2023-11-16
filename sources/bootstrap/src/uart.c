#if defined(__cplusplus)
extern "C"
{
#endif

#include "def.h"
#include "uart.h"

/******************************************************************************/

static void RAMFUNCTION hal_gpio_init(void);

static void RAMFUNCTION hal_gpio_init(void)
{
	register uint32_t temp;

	temp = USARTx_GPIO_PORT->AFR[0] & GPIOx_AFRL_MSK;
	USARTx_GPIO_PORT->AFR[0] = temp | GPIOx_AFRL;

	temp = USARTx_GPIO_PORT->AFR[1] & GPIOx_AFRH_MSK;
	USARTx_GPIO_PORT->AFR[1] = temp | GPIOx_AFRH;

	temp = USARTx_GPIO_PORT->MODER & GPIOx_MODER_MSK;
	USARTx_GPIO_PORT->MODER = temp | GPIOx_MODER;

	temp = USARTx_GPIO_PORT->OSPEEDR & GPIOx_OSPEEDR_MSK;
	USARTx_GPIO_PORT->OSPEEDR = temp | GPIOx_OSPEEDR;

	temp = USARTx_GPIO_PORT->PUPDR & GPIOx_PUDR_MSK;
	USARTx_GPIO_PORT->PUPDR = temp | GPIOx_PUDR;
}

void RAMFUNCTION hal_uart_init(void)
{
	USARTx_CLK_ENABLE();
	USARTx_GPIO_CLK_ENABLE();
	CLEAR_BIT(USARTx->CR1, USART_CR1_UE);
	hal_gpio_init();
	WRITE_REG(USARTx->CR1, USARTx_CR1);
	WRITE_REG(USARTx->CR2, USARTx_CR2);
	WRITE_REG(USARTx->CR3, USARTx_CR3);

	USARTx->BRR = USARTx_BRR;
	SET_BIT(USARTx->CR1, USART_CR1_UE);
}

void RAMFUNCTION hal_uart_fini(void)
{
	USARTx_DeInit();
	USARTx_CLK_DISABLE();

	USARTx_GPIO_DeInit();
	USARTx_GPIO_CLK_DISABLE();
}

void RAMFUNCTION hal_uart_send(uint8_t c)
{
	USARTx->TDR = c;
	while (! ((READ_BIT(USARTx->ISR, USART_ISR_TC) == (USART_ISR_TC)) ? 1UL : 0UL) )
	{
	}
}

uint8_t RAMFUNCTION hal_uart_read(void)
{
  while(! ((READ_BIT(USARTx->ISR, USART_ISR_RXNE) == (USART_ISR_RXNE)) ? 1UL : 0UL))
  {

  }
  return (uint8_t)(READ_BIT(USARTx->RDR, USART_RDR_RDR) & 0xFFU);
}

/******************************************************************************/
#if defined(__cplusplus)
}
#endif
