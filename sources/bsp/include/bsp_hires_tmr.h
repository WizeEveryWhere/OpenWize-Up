/**
  * @file: bsp_hires_tmr.h
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
  * 1.0.0 : 2023/11/03[GBI]
  * Initial version
  *
  *
  */
#ifndef _BSP_HIRES_TMR_H_
#define _BSP_HIRES_TMR_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include <stdint.h>

int32_t BSP_HiResTmr_EnDis(uint8_t bEnable);
uint32_t BSP_HiResTmr_Cnt(void);
uint32_t BSP_HiResTmr_Get(register uint8_t id);
void BSP_HiResTmr_Capture(register uint8_t id);

#ifdef __cplusplus
}
#endif
#endif /* _BSP_HIRES_TMR_H_ */
