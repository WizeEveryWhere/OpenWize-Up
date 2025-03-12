/*!
  * @file bsp_gpio.h
  * @brief This file defines functions to deal with GPIOs.
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

#ifndef _BSP_GPIO_H_
#define _BSP_GPIO_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

/*!
 * @cond INTERNAL
 * @{
 */

#define GPIO_PIN(name) (uint16_t)name##_Pin
#define GPIO_PORT(name) (uint32_t)name##_GPIO_Port

#define IOMUX_INIT(_pud, _speed, _mode, _state, _af) \
	.pu_pd = _pud, .speed = _speed, .mode = _mode & 0x3, .type = _mode >> 4, .state = _state, .af = _af

#define IOMUX_DEFAULT_ANALOG() \
	.pu_pd = GPIO_NOPULL, .speed = GPIO_SPEED_FREQ_LOW, .mode = GPIO_MODE_ANALOG & 0x3, .type = GPIO_MODE_ANALOG >> 4, .state = 0, .af = 0

#define _IOMUX_INIT_(_id, _pud, _speed, _mode, _state, _af) \
	[_id] = { IOMUX_INIT(_pud, _speed, _mode, _state, _af) }

#define _IOMUX_DEFAULT_ANALOG_(_id) \
	[_id] = { IOMUX_DEFAULT_ANALOG }

/*!
 * @brief This struct define TODO
 */
struct iomux_s
{
	union {
		uint16_t io;
		struct {
			uint8_t pu_pd:2;
			uint8_t speed:2;
			uint8_t mode:2;
			uint8_t type:1;
			uint8_t state:1;
			// ---
			uint8_t af:4;
			uint8_t :4;
		};
	};
};

/*!
 * @brief This enum define GPIO port
 */
typedef enum {
    GPIO_PORT_ID_A, /*!< Port id A */
    GPIO_PORT_ID_B, /*!< Port id B */
    GPIO_PORT_ID_C, /*!< Port id C */
	GPIO_PORT_ID_D, /*!< Port id D */
	GPIO_PORT_ID_E, /*!< Port id E */
	//GPIO_PORT_ID_H, /*!< Port id H */
	GPIO_PORT_ID_NB /*!< maximum number of ports */
} gpio_port_id_e;

/*!
 * @brief This enum define GPIO pin
 */
typedef enum {
    GPIO_PIN_ID_0,  /*!< Pin id 0 */
	GPIO_PIN_ID_1,  /*!< Pin id 1 */
	GPIO_PIN_ID_2,  /*!< Pin id 2 */
	GPIO_PIN_ID_3,  /*!< Pin id 3 */
	GPIO_PIN_ID_4,  /*!< Pin id 4 */
	GPIO_PIN_ID_5,  /*!< Pin id 5 */
	GPIO_PIN_ID_6,  /*!< Pin id 6 */
	GPIO_PIN_ID_7,  /*!< Pin id 7 */
	GPIO_PIN_ID_8,  /*!< Pin id 8 */
	GPIO_PIN_ID_9,  /*!< Pin id 9 */
	GPIO_PIN_ID_10, /*!< Pin id 10 */
	GPIO_PIN_ID_11, /*!< Pin id 11 */
	GPIO_PIN_ID_12, /*!< Pin id 12 */
	GPIO_PIN_ID_13, /*!< Pin id 13 */
	GPIO_PIN_ID_14, /*!< Pin id 14 */
	GPIO_PIN_ID_15, /*!< Pin id 15 */
	// ---
	GPIO_PIN_ID_NB  /*!< maximum number of pin */
} gpio_pin_id_e;

/*!
 * @brief This struct define GPIO with its ids (port and pin ids)
 */
typedef struct gpio_id_s
{
	gpio_port_id_e port; /*!< Port id */
	gpio_pin_id_e  pin;  /*!< Pin id */
} gpio_id_t;

/*!
 * @}
 * @endcond
 */
uint16_t BSP_Gpio_GetPin(gpio_pin_id_e id);
gpio_pin_id_e BSP_Gpio_GetPinId(uint16_t pin);

uint32_t BSP_Gpio_GetPort(gpio_port_id_e id);
gpio_port_id_e BSP_Gpio_GetPortId(const uint32_t u32Port);

uint8_t BSP_Gpio_Config (const gpio_id_t sGpioId, const uint16_t u16Cfg);


uint8_t BSP_Gpio_InputEnable (const uint32_t u32Port, const uint16_t u16Pin, const uint8_t bFlag);
uint8_t BSP_Gpio_OutputEnable(const uint32_t u32Port, const uint16_t u16Pin, const uint8_t bFlag);

uint8_t BSP_Gpio_Get(const uint32_t u32Port, const uint16_t u16Pin, uint8_t *bLevel);
uint8_t BSP_Gpio_Set(const uint32_t u32Port, const uint16_t u16Pin, uint8_t bLevel);
uint8_t BSP_Gpio_SetLow (const uint32_t u32Port, const uint16_t u16Pin);
uint8_t BSP_Gpio_SetHigh (const uint32_t u32Port, const uint16_t u16Pin);

#ifdef __cplusplus
}
#endif
#endif /* _BSP_GPIO_H_ */

/*! @} */
