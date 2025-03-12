/*!
  * @file bsp_gpio.c
  * @brief This file contains functions to deal with GPIOs.
  * 
  * @details
  *
  * @copyright 2019, GRDF, Inc.  All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted (subject to the limitations in the disclaimer
  * below) provided that the following conditions are met:
  *    - Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *    - Redistributions in binary form must reproduce the above copyright 
  *      notice, this list of conditions and the following disclaimer in the 
  *      documentation and/or other materials provided with the distribution.
  *    - Neither the name of GRDF, Inc. nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  *
  * @par Revision history
  *
  * @par 1.0.0 : 2019/12/14 [GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup gpio
 * @ingroup bsp
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_gpio.h"
#include "platform.h"
#include <stm32l4xx_hal.h>

/*!
  * @brief Retrieve GPIO pin from its id
  *
  * @param [in] id The pin id number
  *
  * @return the gpio pin
  *
  */
inline
uint16_t BSP_Gpio_GetPin(gpio_pin_id_e id)
{
	return (0x1 << id);
}

/*!
  * @brief Retrieve GPIO pin id from pin number
  *
  * @param [in] id The pin number
  *
  * @return the gpio pin id
  *
  */
inline
gpio_pin_id_e BSP_Gpio_GetPinId(uint16_t pin)
{
	return (gpio_pin_id_e)__builtin_clrsb((int)pin);
}

/*!
  * @brief Retrieve GPIO port address from its id
  *
  * @param [in] id The port id number
  *
  * @return the gpio port address
  *
  */
uint32_t BSP_Gpio_GetPort(gpio_port_id_e id)
{
	uint32_t port;
	switch (id)
	{
		case GPIO_PORT_ID_A :
			port = (uint32_t)GPIOA;
			break;
		case GPIO_PORT_ID_B :
			port = (uint32_t)GPIOB;
			break;
		case GPIO_PORT_ID_C :
			port = (uint32_t)GPIOC;
			break;
		case GPIO_PORT_ID_D:
			port = (uint32_t)GPIOD;
			break;
		case GPIO_PORT_ID_E :
			port = (uint32_t)GPIOE;
			break;
		default:
			port = (uint32_t)(NULL);
			break;
	}
	return port;
}

/*!
  * @brief Retrieve GPIO port number from port address
  *
  * @param [in] u32Port Gpio port address
  *
  * @return the gpio port id number
  *
  */
gpio_port_id_e BSP_Gpio_GetPortId(const uint32_t u32Port)
{
	gpio_port_id_e gpio_port;
	switch (u32Port) {
	case GPIOA_BASE :
		gpio_port = GPIO_PORT_ID_A;
		break;
	case GPIOB_BASE :
		gpio_port = GPIO_PORT_ID_B;
		break;
	case GPIOC_BASE :
		gpio_port = GPIO_PORT_ID_C;
		break;
	case GPIOD_BASE :
		gpio_port = GPIO_PORT_ID_D;
		break;
	case GPIOE_BASE :
		gpio_port = GPIO_PORT_ID_E;
		break;
	default:
		gpio_port = GPIO_PORT_ID_NB;
		break;
	}
	return gpio_port;
}

/*!
  * @brief This function set as input (or analog mode) the given gpio
  *
  * @param [in] sGpioId Gpio ids description (port and pin ids)
  *
  * @retval DEV_SUCCESS (see @link dev_res_e::DEV_SUCCESS @endlink)
  *
  */
uint8_t BSP_Gpio_Config (const gpio_id_t sGpioId, const uint16_t u16Cfg)
{
#define GPIO_MODE             (0x00000003u)
#define GPIO_OUTPUT_TYPE      (0x00000010u)

	GPIO_TypeDef  *GPIOx = (GPIO_TypeDef*)BSP_Gpio_GetPort(sGpioId.port);
	uint16_t u16Pin = BSP_Gpio_GetPin(sGpioId.pin);

	struct iomux_s io_cfg = {.io = u16Cfg};
	uint32_t position = 0x00u;

	assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
	assert_param(IS_GPIO_PIN(u16Pin));

	while (( u16Pin >> position) != 0x00u)
	{
		if ( ( u16Pin & (1uL << position) ) != 0x00u)
		{
			uint32_t temp;
			// If alternate function mode selection
			if ( (io_cfg.mode & GPIO_MODE_AF_PP) == GPIO_MODE_AF_PP)
			{
				// Configure the alternate function
				temp = GPIOx->AFR[position >> 3u];
				temp &= ~(0xFu << ((position & 0x07u) * 4u));
				temp |= ((io_cfg.af) << ((position & 0x07u) * 4u));
				GPIOx->AFR[position >> 3u] = temp;
			}
			// Configure IO Direction mode (Input, Output, Alternate or Analog)
			temp = GPIOx->MODER;
			temp &= ~(GPIO_MODER_MODE0 << (position * 2u));
			temp |= ((io_cfg.mode & GPIO_MODE) << (position * 2u));
			GPIOx->MODER = temp;

			if ( (io_cfg.mode != GPIO_MODE_INPUT) && (io_cfg.mode != GPIO_MODE_ANALOG) )
			{
				// Configure the IO Speed
				temp = GPIOx->OSPEEDR;
				temp &= ~(GPIO_OSPEEDR_OSPEED0 << (position * 2u));
				temp |= (io_cfg.speed << (position * 2u));
				GPIOx->OSPEEDR = temp;

				// Configure the IO Output Type
				temp = GPIOx->OTYPER;
				temp &= ~(GPIO_OTYPER_OT0 << position) ;
				temp |= (((io_cfg.type & GPIO_OUTPUT_TYPE) >> 4u) << position);
				GPIOx->OTYPER = temp;
			}
			/* Activate the Pull-up or Pull down resistor for the current IO */
			temp = GPIOx->PUPDR;
			temp &= ~(GPIO_PUPDR_PUPD0 << (position * 2u));
			temp |= ((io_cfg.pu_pd) << (position * 2u));
			GPIOx->PUPDR = temp;
		}
		position++;
	}

	return DEV_SUCCESS;
}

/*!
  * @brief This function set as input (or analog mode) the given gpio
  *
  * @param [in] u32Port Gpio port
  * @param [in] u16Pin  Gpio pin
  * @param [in] b_Flag  1 : set as input, 0: set in analog mode
  * 
  * @retval DEV_SUCCESS (see @link dev_res_e::DEV_SUCCESS @endlink)
  * 
  */
uint8_t BSP_Gpio_InputEnable (const uint32_t u32Port, const uint16_t u16Pin, const uint8_t b_Flag)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(b_Flag) {
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	}
	else {
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	}

	GPIO_InitStruct.Pin = u16Pin;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init((GPIO_TypeDef*)(u32Port), &GPIO_InitStruct);
	return DEV_SUCCESS;
}

/*!
  * @brief This function set as output (or analog mode) the given gpio
  *
  * @param [in] u32Port Gpio port
  * @param [in] u16Pin  Gpio pin
  * @param [in] b_Flag  1 : set as output, 0: set in analog mode
  * 
  * @retval DEV_SUCCESS (see @link dev_res_e::DEV_SUCCESS @endlink)
  * 
  */
uint8_t BSP_Gpio_OutputEnable(const uint32_t u32Port, const uint16_t u16Pin, const uint8_t b_Flag)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(b_Flag) {
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	}
	else {
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	}

	GPIO_InitStruct.Pin = u16Pin;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init((GPIO_TypeDef*)(u32Port), &GPIO_InitStruct);
	return DEV_SUCCESS;
}

/*!
  * @brief This function get the value of the given gpio
  *
  * @param [in]     u32Port Gpio port
  * @param [in]     u16Pin  Gpio pin
  * @param [in,out] b_Level  Will hold the gpio current value
  * 
  * @retval DEV_SUCCESS (see @link dev_res_e::DEV_SUCCESS @endlink)
  * 
  */
inline uint8_t BSP_Gpio_Get(const uint32_t u32Port, const uint16_t u16Pin, uint8_t *b_Level)
{
	*b_Level = (uint8_t)HAL_GPIO_ReadPin((GPIO_TypeDef*)(u32Port), u16Pin);
	return DEV_SUCCESS;
}

/*!
  * @brief This function set the value of the given gpio
  *
  * @param [in] u32Port Gpio port
  * @param [in] u16Pin  Gpio pin
  * @param [in] b_Level The value to set
  * 
  * @retval DEV_SUCCESS (see @link dev_res_e::DEV_SUCCESS @endlink)
  * 
  */
inline uint8_t BSP_Gpio_Set(const uint32_t u32Port, const uint16_t u16Pin, uint8_t b_Level)
{
	HAL_GPIO_WritePin((GPIO_TypeDef*)(u32Port), u16Pin, (GPIO_PinState)b_Level);
	return DEV_SUCCESS;
}

/*!
  * @brief This function set to 0 the given gpio
  *
  * @param [in] u32Port Gpio port
  * @param [in] u16Pin  Gpio pin
  * 
  * @retval DEV_SUCCESS (see @link dev_res_e::DEV_SUCCESS @endlink)
  * 
  */
inline uint8_t BSP_Gpio_SetLow (const uint32_t u32Port, const uint16_t u16Pin)
{
#if defined(USE_FULL_LL_DRIVER)
	LL_GPIO_ResetOutputPin((GPIO_TypeDef *)u32Port, u16Pin);
	return DEV_SUCCESS;
#else
	return BSP_Gpio_Set(u32Port, u16Pin, GPIO_PIN_RESET);
#endif

}

/*!
  * @brief This function set to 1 the given gpio
  *
  * @param [in] u32Port Gpio port
  * @param [in] u16Pin  Gpio pin
  * 
  * @retval DEV_SUCCESS (see @link dev_res_e::DEV_SUCCESS @endlink)
  * 
  */
inline uint8_t BSP_Gpio_SetHigh (const uint32_t u32Port, const uint16_t u16Pin)
{
#if defined(USE_FULL_LL_DRIVER)
		LL_GPIO_SetOutputPin((GPIO_TypeDef *)u32Port, u16Pin);
		return DEV_SUCCESS;
#else
	return BSP_Gpio_Set(u32Port, u16Pin, GPIO_PIN_SET);
#endif
}

#ifdef __cplusplus
}
#endif

/*! @} */
