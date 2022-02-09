/**
  * @file: bsp_boot.h
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
  * 1.0.0 : 2020/10/13[GBI]
  * Initial version
  *
  *
  */
#ifndef _BSP_BOOT_H_
#define _BSP_BOOT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef enum
{
	BOR_RESET      = 0x10,  // BOR reset (HW pin)
	COLD_RESET     = 0x01,  // Cold reset (HW pin)
	WARM_RESET     = 0x02,  // Warm reset (SW)
	INSTAB_DETECT  = 0x04, // Instability detected 'Application or other)
	UNAUTH_ACCESS  = 0x08,/* Unauthorized action :
	 	 	 	  * - access to protected areas
	 	 	 	  * - entering into forbidden low power mode (Standby, Stop, Shutdown)
	 	 	 	  * - reset option byte
	 	 	 	  */
}boot_reason_e;

uint8_t BSP_Boot_GetReason(void);
void BSP_Boot_Reboot(uint8_t bReset);

#ifdef __cplusplus
}
#endif
#endif /* _BSP_BOOT_H_ */
