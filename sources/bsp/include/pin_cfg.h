/**
  * @file pin_cfg.h
  * @brief TODO
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
  * @par 1.0.0 : 2019/11/20 [GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup OpenWize'Up_bsp
 * @{
 */

#ifndef _PIN_CFG_H_
#define _PIN_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stm32l4xx_hal.h>

/*!
 * @cond INTERNAL
 * @{
 */

/**************************** GPIO pinout *************************************/
// SWD
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA

// Debug
#define DEBUG_TXD_Pin GPIO_PIN_0
#define DEBUG_TXD_GPIO_Port GPIOA
#define DEBUG_RXD_Pin GPIO_PIN_1
#define DEBUG_RXD_GPIO_Port GPIOA

// Radio
#define FE_TRX_Pin GPIO_PIN_2
#define FE_TRX_GPIO_Port GPIOA

#define ADF7030_RST_Pin GPIO_PIN_3
#define ADF7030_RST_GPIO_Port GPIOA
#define ADF7030_SS_Pin GPIO_PIN_4
#define ADF7030_SS_GPIO_Port GPIOA

#define RADIO_SCK_Pin GPIO_PIN_5
#define RADIO_SCK_GPIO_Port GPIOA
#define RADIO_MISO_Pin GPIO_PIN_6
#define RADIO_MISO_GPIO_Port GPIOA
#define RADIO_MOSI_Pin GPIO_PIN_7
#define RADIO_MOSI_GPIO_Port GPIOA

#define ADF7030_GPIO5_Pin GPIO_PIN_0
#define ADF7030_GPIO5_GPIO_Port GPIOB
#define ADF7030_GPIO4_Pin GPIO_PIN_1
#define ADF7030_GPIO4_GPIO_Port GPIOB
#define ADF7030_GPIO3_Pin GPIO_PIN_2
#define ADF7030_GPIO3_GPIO_Port GPIOB

#define ADF7030_GPIO2_Pin GPIO_PIN_12
#define ADF7030_GPIO2_GPIO_Port GPIOB
#define ADF7030_GPIO1_Pin GPIO_PIN_13
#define ADF7030_GPIO1_GPIO_Port GPIOB
#define ADF7030_GPIO0_Pin GPIO_PIN_14
#define ADF7030_GPIO0_GPIO_Port GPIOB

#define FE_BYP_Pin GPIO_PIN_15
#define FE_BYP_GPIO_Port GPIOB

#define FE_EN_Pin GPIO_PIN_8
#define FE_EN_GPIO_Port GPIOA

// V_REF_EN
#define PA_V_EN_Pin GPIO_PIN_13
#define PA_V_EN_GPIO_Port GPIOC

// IO
#define IO6_Pin GPIO_PIN_9
#define IO6_GPIO_Port GPIOA
#define IO5_Pin GPIO_PIN_10
#define IO5_GPIO_Port GPIOA
#define IO4_Pin GPIO_PIN_11
#define IO4_GPIO_Port GPIOA
#define IO3_Pin GPIO_PIN_12
#define IO3_GPIO_Port GPIOA
#define IO2_Pin GPIO_PIN_5
#define IO2_GPIO_Port GPIOB
#define IO1_Pin GPIO_PIN_6
#define IO1_GPIO_Port GPIOB

// I2C
#define INT_SDA_Pin GPIO_PIN_7
#define INT_SDA_GPIO_Port GPIOB
#define INT_SCL_Pin GPIO_PIN_8
#define INT_SCL_GPIO_Port GPIOB

#define EXT_SCL_Pin GPIO_PIN_10
#define EXT_SCL_GPIO_Port GPIOB
#define EXT_SDA_Pin GPIO_PIN_11
#define EXT_SDA_GPIO_Port GPIOB

#define EEPROM_CTRL_Pin GPIO_PIN_9
#define EEPROM_CTRL_GPIO_Port GPIOB

/*!
 * @}
 * @endcond
 */

#ifdef __cplusplus
}
#endif

#endif /* _PIN_CFG_H_ */

/*! @} */
