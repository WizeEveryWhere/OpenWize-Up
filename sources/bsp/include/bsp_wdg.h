/**
  * @file: bsp_wdg.h
  * @brief: // TODO This file ...
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
  * @par 1.0.0 : 2023/08/27[GBI]
  * Initial version
  *
  *
  */
#ifndef _BSP_WDG_H_
#define _BSP_WDG_H_
#ifdef __cplusplus
extern "C" {
#endif

/*
 * It is assumed that the boot_strap :
 * - Initialize the IWDG with : (32.768 second)
 *   - IWDG_PR =  0x0000 0007 // divide by 256
 *   - IWDG_RLR = 0x0000 0FFF
 * - Set Option Byte :
 *   - IWDG_STDBY = 0 : Independent watchdog counter is frozen in Standby mode
 *   - IWDG_STOP  = 0 : Independent watchdog counter is frozen in Stop mode
 *   - IDWG_SW    = 1 : Software independent watchdog
 * - Increment the "unstable counter" if reset is due to :
 *   - IWDG timeout
 *   - WWDG timeout
 * - Increment the "unauthorized counter" if reset is due to :
 *   - Option Byte Loading
 *   - Firewall access
 *   - Illegal Low Power mode entry
 *
 *
 *
 */

void BSP_Iwdg_Refresh(void);

#ifdef __cplusplus
}
#endif
#endif /* _BSP_WDG_H_ */
