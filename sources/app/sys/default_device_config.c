/**
  * @file default_device_config.c
  * @brief This file define the ADF7030-1 interrupt 
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
  * @par 1.0.0 : 2020/04/22 [GBI]
  * Initial version
  *
  *
  */

/*!
 *  @addtogroup sys
 *  @ingroup app
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "default_device_config.h"

/*!
 * @cond INTERNAL
 * @{
 */

/* Default device configuration*/

KEEP_VAR(const adf7030_1_gpio_reset_info_t DEFAULT_GPIO_RESET) =
{
    /*! Host GPIO port to which the interrupt pin is connected */
    .u32Port = ADF7030_1_RESET_GPIO_PORT,
    /*! Host GPIO pin within the GPIO port */
    .u16Pin = ADF7030_1_RESET_GPIO_PIN
};

KEEP_VAR(const adf7030_1_gpio_int_info_t DEFAULT_GPIO_INT[ADF7030_1_NUM_INT_PIN]) =
{
    {
        /*! GPIO port to which the interrupt pin is connected */
        .u32Port = ADF7030_1_INT0_GPIO_PORT,
        /*! GPIO pin within the GPIO port */
        .u16Pin = ADF7030_1_INT0_GPIO_PIN,
        /*! PHY Radio GPIO pin */
        .ePhyPin = ADF7030_1_INT0_GPIO_PHY_PIN,
        /*! Radio PHY interrupt mask configuration */
        .nIntMap = 0,
        /*! Last IRQ status */
        .nIntStatus = 0,
		/*! Interrupt Call back*/
		.pfIntCb = NULL,
		/*! Interrupt Call back parameter*/
		.pIntCbParam = NULL
    },
    {
        /*! GPIO port to which the interrupt pin is connected */
        .u32Port = ADF7030_1_INT1_GPIO_PORT,
        /*! GPIO pin within the GPIO port */
        .u16Pin = ADF7030_1_INT1_GPIO_PIN,
        /*! PHY Radio GPIO pin */
        .ePhyPin = ADF7030_1_INT1_GPIO_PHY_PIN,
        /*! Radio PHY interrupt mask configuration */
        .nIntMap = 0,
        /*! Last IRQ status */
        .nIntStatus = 0,
		/*! Interrupt Call back*/
		.pfIntCb = NULL,
		/*! Interrupt Call back parameter*/
		.pIntCbParam = NULL
    }
};

KEEP_VAR(const adf7030_1_gpio_trig_info_t DEFAULT_GPIO_TRIG[ADF7030_1_NUM_TRIG_PIN]) =
{
	{
		/*! Host GPIO port to which the interrupt pin is connected */
		.u32Port = ADF7030_1_TRIG0_GPIO_PORT,
		/*! Host GPIO pin within the GPIO port */
		.u16Pin = ADF7030_1_TRIG0_GPIO_PIN,
		/*! PHY Radio GPIO pin */
		.ePhyPin = ADF7030_1_TRIG0_GPIO_PHY_PIN,
		/*! PHY Radio Command to execute on trigger */
		.nTrigCmd = PHY_TX,
		/*! Current trigger status */
		.eTrigStatus = 0
	},
	{
		/*! Host GPIO port to which the interrupt pin is connected */
		.u32Port = 0,
		/*! Host GPIO pin within the GPIO port */
		.u16Pin = ADF7030_1_TRIG1_GPIO_PIN,
		/*! PHY Radio GPIO pin */
		.ePhyPin = ADF7030_1_TRIG1_GPIO_PHY_PIN,
		/*! PHY Radio Command to execute on trigger */
		.nTrigCmd = PHY_RX,
		/*! Current trigger status */
		.eTrigStatus = 0
	}
};

/*!
 * @}
 * @endcond
 */


#ifdef __cplusplus
}
#endif

/*! @} */
