/**
  * @file: adf7030-1__trig.h
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
  * 1.0.0 : 2020/05/18[TODO: your name]
  * Initial version
  *
  *
  */

#ifndef _ADF7030_1__TRIG_H_
#define _ADF7030_1__TRIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

#include "adf7030-1__common.h"

uint8_t adf7030_1__TRIG_SetGPIOPin(
	adf7030_1_device_t* const pDevice,
    adf7030_1_trigpin_e     eTRIG,
    adf7030_1_radio_state_e goState
);

uint8_t adf7030_1__TRIG_Enable(
	adf7030_1_device_t* const pDevice,
    adf7030_1_radio_extended_e ExCmd
);

#ifdef __cplusplus
}
#endif

#endif /* _ADF7030_1__TRIG_H_ */
