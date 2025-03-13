/**
  * @file: auto_clk.h
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
  * 1.0.0 : 2025/01/29[TODO: your name]
  * Initial version
  *
  *
  */
#ifndef _AUTOCLK_H_
#define _AUTOCLK_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "bsp_clk.h"

/******************************************************************************/
/*!
 * @brief This enum define the oscillator id
 */
typedef enum
{
	OSC_MSI = 0b0,
	OSC_HSE = 0b1,
} osc_e;

typedef enum
{
	BYP_NONE = 0b00,
	BYP_HSE  = 0b01,
	BYP_LSE  = 0b10,
	BYP_BOTH = 0b11,
} bypass_e;

typedef enum
{
	CSS_NONE = 0b00,
	CSS_HSE  = 0b01,
	CSS_LSE  = 0b10,
	CSS_BOTH = 0b11,
} css_e;

struct osc_s {
	uint8_t source:1;
	uint8_t trim:1;
	// if osc = OSC_ID_MSI and LSE succeed :
	// - auto calibrate with MSI_PLL
	// if osc = OSC_ID_MSI and LSE failed (expected LSI succeed)
	// - periodically auto-trimming the oscillator by SW
	// if osc = OSC_ID_HSE,
	// - don't care, not able to trim HSE clock
	uint8_t :2;
	uint8_t bypass:2;
	// if osc != OSC_ID_HSE, HSE bypass is ignore
	// if lsc != OSC_ID_LSE, LSE bypass is ignore
	// 00 : HSE and LSE use xtal
	// 01 : HSE is bypass
	// 10 : LSE is bypass
	// 11 : HSE and LSE are bypass
	uint8_t css:2;
	// 00 : CSS (Clock Security System) disable
	// 01 : HSE CSS enable
	// 10 : LSE CSS enable
	// 11 : HSE and LSE CSS enable
};


union clk_cfg_param_u {
    uint32_t clk_cfg;
    struct {
    	union {
    		uint8_t osc_cfg;
    		struct osc_s osc;
    	};
    	union {
    		uint8_t lsc_cfg;
    		struct lsc_s lsc;
    	};
		uint32_t msi_trim:8;
		uint32_t :8;
    };
};

/******************************************************************************/
extern const char * const osc_str[];
extern const char * const bypass_str[];
extern const char * const css_str[];

void AutoClk_Init(void);
int32_t AutoClk_MeasAccurate(void);
/******************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* _AUTOCLK_H_ */
