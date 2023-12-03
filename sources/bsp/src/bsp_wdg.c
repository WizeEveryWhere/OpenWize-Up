/**
  * @file: bsp_wdg.c
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
  * @par 1.0.0 : 2023/08/27 [GBI]
  * Initial version
  *
  *
  */
#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_wdg.h"
#include "platform.h"
#include <stm32l4xx_hal.h>

void BSP_Iwdg_Refresh(void)
{
	IWDG->KR = IWDG_KEY_RELOAD;
}


#ifdef __cplusplus
}
#endif
