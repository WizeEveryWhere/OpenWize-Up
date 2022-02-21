/**
  * @file default_device_config.h
  * @brief  This file define the ADF7030-1 interrupt
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
  * 1.0.0 : 2020/04/22 [GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup OpenWize'Up
 * @{
 *
 */
#ifndef _DEFAULT_DEVICE_CONFIG_H_
#define _DEFAULT_DEVICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"
#include "adf7030-1__common.h"

#include "pin_cfg.h"

/*!
   Define the PHY Radio main clock source.\n
   1 -  Use XTAL.\n
   0 -  Use TCXO.\n
*/
#define REF_CLOCK_OVERRIDE 0

/* CS port to which the CSN line of adf7030-1 is connected */
#define ADF7030_1_SPI_CS_GPIO_PORT    (uint32_t)ADF7030_SS_GPIO_Port
#define ADF7030_1_SPI_CS_GPIO_PIN     ADF7030_SS_Pin

// RESET
#define ADF7030_1_RESET_GPIO_PORT    (uint32_t)ADF7030_RST_GPIO_Port
#define ADF7030_1_RESET_GPIO_PIN     ADF7030_RST_Pin

// PA
#define ADF7030_1_EXT_PA_GPIO_PHY_PIN  ADF7030_1_GPIO6
// ADF7030_1_GPIO7 connected to GND

/* GPIO port and pin to which default interrupt pin0 of adf7030-1 is connected */
#define ADF7030_1_INT0_GPIO_PORT       (uint32_t)ADF7030_GPIO3_GPIO_Port
#define ADF7030_1_INT0_GPIO_PIN        ADF7030_GPIO3_Pin
#define ADF7030_1_INT0_GPIO_PHY_PIN    ADF7030_1_GPIO3

/* GPIO port and pin to which default interrupt pin1 of adf7030-1 is connected */
#define ADF7030_1_INT1_GPIO_PORT       (uint32_t)ADF7030_GPIO5_GPIO_Port
#define ADF7030_1_INT1_GPIO_PIN        ADF7030_GPIO5_Pin
#define ADF7030_1_INT1_GPIO_PHY_PIN    ADF7030_1_GPIO5

/* GPIO port and pin to which default trigger pin0 of the adf7030-1 is connected */
#define ADF7030_1_TRIG0_GPIO_PORT      (uint32_t)ADF7030_GPIO2_GPIO_Port
#define ADF7030_1_TRIG0_GPIO_PIN       ADF7030_GPIO2_Pin
#define ADF7030_1_TRIG0_GPIO_PHY_PIN   ADF7030_1_GPIO2

/* GPIO port and pin to which default trigger pin1 of the adf7030-1 is connected */
#define ADF7030_1_TRIG1_GPIO_PORT      (uint32_t)ADF7030_GPIO4_GPIO_Port
#define ADF7030_1_TRIG1_GPIO_PIN       ADF7030_GPIO4_Pin
#define ADF7030_1_TRIG1_GPIO_PHY_PIN   ADF7030_1_GPIO4

/* GPIO port and pin as SPORT clock */
#define ADF7030_1_SPORT_CLK_GPIO_PORT      (uint32_t)ADF7030_GPIO0_GPIO_Port
#define ADF7030_1_SPORT_CLK_GPIO_PIN       ADF7030_GPIO0_Pin
#define ADF7030_1_SPORT_CLK_GPIO_PHY_PIN   ADF7030_1_GPIO0

/* GPIO port and pin as SPORT data */
#define ADF7030_1_SPORT_DATA_GPIO_PORT      (uint32_t)ADF7030_GPIO1_GPIO_Port
#define ADF7030_1_SPORT_DATA_GPIO_PIN       ADF7030_GPIO1_Pin
#define ADF7030_1_SPORT_DATA_GPIO_PHY_PIN   ADF7030_1_GPIO1

#ifdef __cplusplus
}
#endif
#endif /* _DEFAULT_DEVICE_CONFIG_H_ */

/*! @} */
