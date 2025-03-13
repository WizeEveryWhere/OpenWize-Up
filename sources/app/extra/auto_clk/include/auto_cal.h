/**
  * @file: auto_cal.h
  * @brief: // TODO This file ...
  * 
  * @details
  *
  * @copyright 2025, GRDF, Inc.  All rights reserved.
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
  * @par 1.0.0 : 2025/02/06[GBI]
  * Initial version
  *
  *
  */

/*! @addtogroup autocal
 *  @ingroup extra
 *  @{
 */

#ifndef _AUTOCAL_H_
#define _AUTOCAL_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void AutoCal_Init(void);
int32_t AutoCal_LSIMeas(uint32_t u32RefFreq, uint8_t u8NbLoop);
int32_t AutoCal_HSEMeas(uint32_t u32RefFreq, uint8_t u8NbLoop);
int32_t AutoCal_SysMeas(uint32_t u32RefFreq, uint8_t u8NbLoop);

void AutoCal_Tmr_Enable(uint8_t bOCxEnable);
void AutoCal_Tmr_Disable(uint8_t bOCxEnable);

#ifdef __cplusplus
}
#endif
#endif /* _AUTOCAL_H_ */

/*! @} */
