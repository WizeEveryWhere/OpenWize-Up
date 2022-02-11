/**
  * @file:	adf7030-1__gpio.h
  * @brief:	...
  *
  *****************************************************************************
  * @Copyright 2019, GRDF, Inc.  All rights reserved.
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
  *****************************************************************************
  *
  * Revision history
  * ----------------
  * 1.0.0 : 2020/05/18[TODO: your name]
  * Initial version
  *
  *
  */

#ifndef _ADF7030_1__GPIO_H_
#define _ADF7030_1__GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#include "adf7030-1__common.h"

/** \addtogroup adf7030-1 adf7030-1 Driver
 *  @{
 */

/** \addtogroup adf7030-1__gpio GPIO Interface
 *  @{
 */


/*! Enumeration of different PHY Radio gpio configuration
 *
 */
typedef enum {
    
    /*! External IRQ0 input */
    GPIO_TRIG_IN0       = 4,
    
    /*! External IRQ1 input */
    GPIO_TRIG_IN1       = 5,
    
    /*! External IRQ0 output */
    GPIO_IRQ_OUT0       = 6,
    
    /*! External IRQ1 output */
    GPIO_IRQ_OUT1       = 7,

    /*! Sport interface baseband TX data input */
    SPORT_TX_DATA       = 12,

    /*! Sport interface baseband RX data output */
    SPORT_RX_DATA       = 13,

    /*! Sport interface baseband TRX clock output */
    SPORT_TRX_CLOCK     = 14,
    
    /*! General Purpose Input 0 */
    GPIO_IN0            = 16,

    /*! General Purpose Input 1 */
    GPIO_IN1            = 17,

    /*! General Purpose Input 2 */
    GPIO_IN2            = 18,

    /*! General Purpose Input 3 */
    GPIO_IN3            = 19,

    /*! General Purpose Input 4 */
    GPIO_IN4            = 20,

    /*! General Purpose Input 5 */
    GPIO_IN5            = 21,

    /*! General Purpose Input 6 */
    GPIO_IN6            = 22,

    /*! General Purpose Input 7 */
    GPIO_IN7            = 23,
      
    /*! General Purpose Output 0 */
    GPIO_OUT0           = 24,

    /*! General Purpose Output 1 */
    GPIO_OUT1           = 25,

    /*! General Purpose Output 2 */
    GPIO_OUT2           = 26,

    /*! General Purpose Output 3 */
    GPIO_OUT3           = 27,

    /*! General Purpose Output 4 */
    GPIO_OUT4           = 28,

    /*! General Purpose Output 5 */
    GPIO_OUT5           = 29,

    /*! General Purpose Output 6 */
    GPIO_OUT6           = 30,

    /*! General Purpose Output 7 */
    GPIO_OUT7           = 31,
      
    /*! User defined synchronous Clock output */
    GPCLK_OUT           = 36,
    
    /*! Debug mode */
    DEBUG_MODE          = 63
      
} adf7030_1_gpio_cfg_e;


/*! Enumeration of Trigger event options
 *
 */
typedef enum {
    
    /*! External PHY Radio trigger on a rising edge */
    RISING_EDGE         = 0,
    
    /*! External PHY Radio trigger on a falling edge */
    FALLING_EDGE        = 1,

    /*! External PHY Radio trigger on either rising or falling edge */
    BOTH_EDGE           = 2,

    /*! External PHY Radio trigger on a low level */
    LOW_LEVEL           = 3,

    /*! External PHY Radio trigger on a high level */
    HIGH_LEVEL          = 4

} adf7030_1_gpio_trig_evt_e;


/*! Enumeration of GPIO dataout bus pins
 *
 */
typedef enum {   
    /*! General purpose PHY Radio IO Bus pin 0 */
    ADF7030_1_IOBUS_PIN0        = 1UL << ADF7030_1_GPIO0,

    /*! General purpose PHY Radio IO Bus pin 1 */
    ADF7030_1_IOBUS_PIN1        = 1UL << ADF7030_1_GPIO1,
      
    /*! General purpose PHY Radio IO Bus pin 2 */
    ADF7030_1_IOBUS_PIN2        = 1UL << ADF7030_1_GPIO2,
      
    /*! General purpose PHY Radio IO Bus pin 3 */
    ADF7030_1_IOBUS_PIN3        = 1UL << ADF7030_1_GPIO3,
      
    /*! General purpose PHY Radio IO Bus pin 4 */
    ADF7030_1_IOBUS_PIN4        = 1UL << ADF7030_1_GPIO4,
      
    /*! General purpose PHY Radio IO Bus pin 5 */
    ADF7030_1_IOBUS_PIN5        = 1UL << ADF7030_1_GPIO5,
      
    /*! General purpose PHY Radio IO Bus pin 6 */
    ADF7030_1_IOBUS_PIN6        = 1UL << ADF7030_1_GPIO6,
      
    /*! General purpose PHY Radio IO Bus pin 7 */
    ADF7030_1_IOBUS_PIN7        = 1UL << ADF7030_1_GPIO7,
    
} adf7030_1_busio_pin_e;
      

/* Setup individual GPIO configuration on the PHY Radio */
uint8_t adf7030_1__GPIO_SetCfg(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_gpio_pin_e  eGPIO,
    adf7030_1_gpio_cfg_e  eCFG,
    uint8_t               bDirect
);

/* Get individual GPIO configuration from the PHY Radio */
uint8_t adf7030_1__GPIO_GetCfg(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_gpio_pin_e  eGPIO,
    adf7030_1_gpio_cfg_e* peCFG,
    uint8_t               bDirect
);

/* Set individual GPIO pin of the PHY Radio */
uint8_t adf7030_1__GPIO_SetPin(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_busio_pin_e ePin
);

/* Clear individual GPIO pin of the PHY Radio */
uint8_t adf7030_1__GPIO_ClrPin(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_busio_pin_e ePin
);


#define adf7030_1__GPIO_PINCFG(ePIN, eCFG) adf7030_1__GPIO_SetCfg( pSPIDevInfo, ePIN, eCFG, 0)
//#define adf7030_1__GPIO_TRIG_Cfg(PIN, TRIG, State) adf7030_1__GPIO_SetupExtTrig(pSPIDevInfo, TRIG, PIN, State)

#ifdef __cplusplus
}
#endif

#endif /* _ADF7030_1__GPIO_H_ */
