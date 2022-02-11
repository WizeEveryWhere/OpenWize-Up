/**
  * @file: bsp_pwrlines.h
  * @brief: This file defines functions to enable/disable power board peripherals
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
  * 1.0.0 : 2020/09/22[GBI]
  * Initial version
  *
  *
  */
#ifndef _BSP_PWRLINES_H_
#define _BSP_PWRLINES_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

/*******************************************************************************/

typedef enum {
	FE_EN_MSK         = 0b0001,
	PA_EN_MSK         = 0b0010,
	RF_EN_MSK         = 0b0100 | FE_EN_MSK,
	INT_EEPROM_EN_MSK = 0b1000,
}pwr_id_msk;

void BSP_PwrLine_Clr (uint16_t u16PwrLines);
void BSP_PwrLine_Set (uint16_t u16PwrLines);
uint16_t BSP_PwrLine_Get (uint16_t u16PwrLines);
void BSP_PwrLine_Init (void);

/*******************************************************************************/

typedef enum {
	LP_SLEEP_MODE,
	LP_STOP1_MODE,
	LP_STOP2_MODE,
	LP_STDBY_MODE,
	LP_SHTDWN_MODE,
} lp_mode_e;

void BSP_LowPower_Enter(lp_mode_e eLpMode);

/*******************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* _BSP_PWRLINES_H_ */
