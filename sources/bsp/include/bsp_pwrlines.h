/**
  * @file bsp_pwrlines.h
  * @brief This file defines functions to enable/disable power board peripherals
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
  * @par 1.0.0 : 2020/09/22 [GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup OpenWize'Up_bsp
 * @{
 */

#ifndef _BSP_PWRLINES_H_
#define _BSP_PWRLINES_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

/*******************************************************************************/

/*!
  * @brief This enum define the mask to set/clr the "power line"
  */
typedef enum {
	FE_EN_MSK         = 0b0001,             /*!< FE enable mask */
	PA_EN_MSK         = 0b0010,             /*!< PA enable mask */
	RF_EN_MSK         = 0b0100 | FE_EN_MSK, /*!< RF enable mask */
	INT_EEPROM_EN_MSK = 0b1000,             /*!< Internal EEPROM enable mask */
} pwr_id_msk;

void BSP_PwrLine_Clr (uint16_t u16PwrLines);
void BSP_PwrLine_Set (uint16_t u16PwrLines);
uint16_t BSP_PwrLine_Get (uint16_t u16PwrLines);
void BSP_PwrLine_Init (void);

/*******************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* _BSP_PWRLINES_H_ */

/*! @} */
