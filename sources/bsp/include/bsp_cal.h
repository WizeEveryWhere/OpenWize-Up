/**
  * @file: bsp_cal.h
  * @brief: // TODO This file ...
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
  * 1.0.0 : 2023/10/30[GBI]
  * Initial version
  *
  *
  */

/*! @addtogroup cal
 *  @ingroup bsp
 *  @{
 */

#ifndef _BSP_CAL_H_
#define _BSP_CAL_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include <stdint.h>

typedef struct
{
	uint16_t arr;
	uint16_t psc;
} timer_period_t;

typedef struct
{
	timer_period_t sT15;
	timer_period_t sT1;
} timer_setup_t;

typedef enum
{
	O_CLK_1Hz,
	O_CLK_1KHz,
	O_CLK_1625KHz,
	// ---
	O_CLK_NB
} out_clk_freq_e;

extern timer_setup_t sTimSetup[O_CLK_NB];
void Calibration_Done(uint32_t u32PulseCnt);
void Calibrate_Run(void);
void Calibrate_Cancel(void);
int32_t Calibrate_WaitDone(uint32_t ticktmo);

void BSP_TmrCalib_Enable(void);
void BSP_TmrCalib_Disable(void);

void BSP_TmrClk_Enable(timer_period_t sT15, timer_period_t sT1, uint8_t bOCEnable);
void BSP_TmrClk_Disable(void);
void BSP_TmrClk_Trim(timer_period_t sT15, timer_period_t sT1);
void BSP_TmrClk_SetRef(uint8_t eOutClkFreq, timer_period_t sT15, timer_period_t sT1);
void BSP_TmrClk_GetRef(uint8_t eOutClkFreq, timer_period_t *sT15, timer_period_t *sT1);


#ifdef __cplusplus
}
#endif
#endif /* _BSP_CAL_H_ */

/*! @} */
