/**
  * @file: update_area.h
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
  * 1.0.0 : 2023/07/11[GBI]
  * Initial version
  *
  *
  */
#ifndef UPDATE_AREA_H_
#define UPDATE_AREA_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "update.h"

#ifndef BUILD_STANDALONE_APP
	#include "img.h"
#endif

update_status_e UpdateArea_Setup(void);
update_status_e UpdateArea_Initialize(uint8_t eType, uint16_t u16BlkCnt);
update_status_e UpdateArea_Proceed(uint8_t eType, uint16_t u16Id, const uint8_t *pData);
update_status_e UpdateArea_Finalize(uint8_t eType, uint32_t u32HashSW, uint32_t img_sz);
update_status_e UpdateArea_CheckImg(uint32_t u32HashSW);
update_status_e UpdateArea_WriteHeader(uint32_t img_sz);
void UpdateArea_SetBootReq(uint32_t boot_req);
void UpdateArea_SetBootable(void);

#ifdef __cplusplus
}
#endif
#endif /* UPDATE_AREA_H_ */
