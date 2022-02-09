/**
  * @file: phy_test.h
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
  * 1.0.0 : 2021/04/07[TODO: your name]
  * Initial version
  *
  *
  */
#ifndef _EXTRA_PHY_PHY_TEST_H_
#define _EXTRA_PHY_PHY_TEST_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp.h"
#include "common.h"

#include "phy_layer_private.h"

phy_test_mode_e EX_PHY_Test(phy_test_mode_e eMode, uint8_t eType);
void EX_PHY_SetCpy(void);

#ifdef __cplusplus
}
#endif
#endif /* _EXTRA_PHY_PHY_TEST_H_ */
