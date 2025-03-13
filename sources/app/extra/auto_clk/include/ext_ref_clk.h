/**
  * @file: ext_ref_clk.h
  * @brief: // TODO This file ...
  * 
  *****************************************************************************
  * @Copyright 2025, GRDF, Inc.  All rights reserved.
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
  * 1.0.0 : 2025/02/21[TODO: your name]
  * Initial version
  *
  *
  */
#ifndef _EXT_REF_CLK_H_
#define _EXT_REF_CLK_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/******************************************************************************/
#define TMR_SETUP(arrVal, pscVal) {.arr = arrVal, .psc = pscVal}

/******************************************************************************/

typedef struct
{
	uint16_t arr;
	uint16_t psc;
} tim_period_t;

typedef enum
{
	REF_CLK_2Hz,
	REF_CLK_20Hz,
	REF_CLK_200Hz,
	REF_CLK_2000Hz,
	REF_CLK_20000Hz,
#ifdef HAS_REF_xDIV
	REF_CLK_12500Hz,
	REF_CLK_25000Hz,
	REF_CLK_50000Hz,

	REF_CLK_125KHz,
	REF_CLK_250KHz,
	REF_CLK_500KHz,

	REF_CLK_3250KHz,
#endif
	// ---
	REF_CLK_NB
} ref_clk_freq_e;

/******************************************************************************/

void ExtRefClk_Enable(uint8_t eRefClk);
void ExtRefClk_Disable(void);
void ExtRefClk_Trim(uint8_t eRefClk);

/******************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* _EXT_REF_CLK_H_ */
