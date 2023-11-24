
#include "main.h"
#include "stm32l4xx_it.h"

extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim6;

#ifdef USE_UART4
	extern UART_HandleTypeDef huart4;
#endif

#ifdef USE_LPUART1
	extern UART_HandleTypeDef lphuart1;
#endif

/**
  * @brief This function handles RTC wake-up interrupt through EXTI line 20.
  */
void RTC_WKUP_IRQHandler(void)
{
	HAL_RTCEx_WakeUpTimerIRQHandler(&hrtc);
}

/**
  * @brief This function handles RTC alarm interrupt through EXTI line 18.
  */
void RTC_Alarm_IRQHandler(void)
{
	HAL_RTC_AlarmIRQHandler(&hrtc);
}

// FIXME:
extern void CalibTimer_IRQHandler(void);

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
	CalibTimer_IRQHandler();
}


#ifdef USE_UART4
/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
	if ( huart4.Instance->ISR & USART_ISR_RTOF)
	{
		huart4.RxISR(&huart4);
	}
	else
	{
		HAL_UART_IRQHandler(&huart4);
	}
}
#endif

#ifdef USE_LPUART1
/**
  * @brief This function handles LPUART1 global interrupt.
  */
void LPUART1_IRQHandler(void)
{
	if ( lphuart1.Instance->ISR & USART_ISR_RTOF)
	{
		lphuart1.RxISR(&lphuart1);
	}
	else
	{
		HAL_UART_IRQHandler(&lphuart1);
	}
}
#endif

/**
  * @brief This function handles TIM6 global interrupt, DAC channel1 and channel2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim6);
}


// TODO : fix that following for STMCube code generation
extern void BSP_GpioIt_Handler(int8_t i8_ItLineId);

/**
  * @brief This function handles EXTI line1 interrupt.
  */
void EXTI1_IRQHandler(void)
{
	register uint32_t msk;
	uint8_t num = 1;
	msk = 0x1 << num;
	if ( EXTI->PR1 & msk )
	{
		BSP_GpioIt_Handler(num);
		EXTI->PR1 = msk;
	}
}

/**
  * @brief This function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void)
{
	register uint32_t msk;
	uint8_t num = 2;
	msk = 0x1 << num;
	if ( EXTI->PR1 & msk )
	{
		BSP_GpioIt_Handler(num);
		EXTI->PR1 = msk;
	}
}

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
	register uint32_t msk;
	register uint8_t num;
	for(num = 5; num < 10; num++)
	{
		msk = 0x1 << num;
		if ( EXTI->PR1 & msk )
		{
			BSP_GpioIt_Handler(num);
			EXTI->PR1 = msk;
		}
	}
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
	register uint32_t msk;
	register uint8_t num;
	for (num = 10; num <= 15; num++)
	{
		msk = 0x1 << num;
		if ( EXTI->PR1 & msk )
		{
			BSP_GpioIt_Handler(num);
			EXTI->PR1 = msk;
		}
	}
}
