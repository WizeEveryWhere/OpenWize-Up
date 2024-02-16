/**
  * @file: phy_test.h
  * @brief: This file define some phy test usefull function
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
  * @par 1.0.0 : 2021/04/07 [GBI]
  * Initial version
  *
  *
  */

/*!
 *  @addtogroup itf
 *  @ingroup app
 *  @{
 */
 
#ifndef _ITF_PHY_TEST_H_
#define _ITF_PHY_TEST_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"
#include "common.h"

#include "phy_layer_private.h"

test_mode_info_t EX_PHY_TestInit(void);
phy_test_mode_e EX_PHY_Test(test_mode_info_t eTestModeInfo);


void EX_PHY_SetCpy(void);

void EX_PHY_OnOff(uint8_t bOn);
void EX_PHY_SetPa(uint8_t bEnable);
int32_t EX_PHY_GetPa(void);
int32_t EX_PHY_RssiCalibrate(void);
int32_t EX_PHY_AutoCalibrate(void);
int32_t EX_PHY_SetPowerEntry(phy_power_entry_t *pPhyPwrEntry);
int32_t EX_PHY_GetPowerEntry(phy_power_entry_t *pPhyPwrEntry);

int32_t EX_PHY_GetIhmRssi(int16_t *i16_IntPart, uint8_t *u8_DecPart);
int32_t EX_PHY_GetIhmNoise(int16_t *i16_IntPart, uint8_t *u8_DecPart);

#ifdef __cplusplus
}
#endif
#endif /* _ITF_PHY_TEST_H_ */

/*! @} */
