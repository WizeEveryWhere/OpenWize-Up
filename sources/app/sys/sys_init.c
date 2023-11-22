/**
  * @file sys_init.c
  * @brief This file implement (just for convenient) some "system" initialization
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
  * @par 1.0.0 : 2020/11/04 [GBI]
  * Initial version
  *
  *
  */

/*!
 *  @addtogroup sys
 *  @ingroup app
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "FreeRTOS.h"
#include "task.h"

#include "parameters_cfg.h"
#include "parameters.h"

#include "bsp.h"

#include "crypto.h"
#include "time_evt.h"
#include "phy_layer.h"

#include "phy_layer_private.h"

#include "wize_app.h"

#include "storage.h"
#include "bsp_pwrlines.h"

extern const adf7030_1_gpio_reset_info_t DEFAULT_GPIO_RESET;
extern const adf7030_1_gpio_int_info_t DEFAULT_GPIO_INT[ADF7030_1_NUM_INT_PIN];
#ifdef USE_PHY_TRIG
	extern const adf7030_1_gpio_trig_info_t DEFAULT_GPIO_TRIG[ADF7030_1_NUM_TRIG_PIN];
#endif

/*!
 * @brief This is the context for the ADF7030-1 device
 */
static adf7030_1_device_t adf7030_1_ctx;

/*!
 * @brief This store the phy device structure
 */
phydev_t sPhyDev;

/*!
 * @brief This function initialize the "system part"
 */
void Sys_Init(void)
{
	uint8_t u8LogLevel;
	uint8_t u8Tstmp;

	// Do not buffer stdout, so that single chars are output without any delay to the console.
	setvbuf(stdout, NULL, _IONBF, 0);
	// Do not buffer stdin, so that single chars are output without any delay to the console.
	setvbuf(stdin, NULL, _IONBF, 0);

  	/* Show the welcome message */
#ifndef HAS_NO_BANNER
  	printf("\n###########################################################\n");
  	printf("%s\n", WIZE_ALLIANCE_BANNER);
  	printf("\n###########################################################\n");
#endif

	// Setup adf device
	assert(0 == Phy_adf7030_setup( &sPhyDev,
                               &adf7030_1_ctx,
                               (adf7030_1_gpio_int_info_t *)&DEFAULT_GPIO_INT,

#ifdef USE_PHY_TRIG
							   (adf7030_1_gpio_trig_info_t *)&DEFAULT_GPIO_TRIG,
#else
							   (adf7030_1_gpio_trig_info_t *)NULL,
#endif
                               (adf7030_1_gpio_reset_info_t *)&DEFAULT_GPIO_RESET,
							   ADF7030_1_GPIO6,
                               ADF7030_1_GPIO_NONE
                               ) );


	// Init storage
	Storage_Init(0);

	// Init Logger
#ifdef LOGGER_USE_FWRITE
  	Logger_Setup((int32_t (*)(const char*, size_t, size_t, FILE*))fwrite, stdout);
#else
	Logger_Setup((int32_t (*)(const char*, FILE*))fputs, stdout);
#endif

	// Change logger level
	if ( Param_Access(LOGGER_LEVEL, (uint8_t*)(&u8LogLevel), 0) == 0 )
	{
		u8LogLevel = LOG_LV_FRM_OUT | LOG_LV_ERR | LOG_LV_WRN | LOG_LV_INF | LOG_LV_DBG;
	}
	if ( Param_Access(LOGGER_TIME_OPT, (uint8_t*)(&u8Tstmp), 0) == 0 )
	{
		u8Tstmp = LOG_TSTAMP_HIRES | LOG_TSTAMP_TRUNC;
	}
	Logger_SetLevel( u8LogLevel, u8Tstmp );

	WizeApi_CtxClear();// FIXME
	// -----------------------
	WizeApp_CtxRestore();
   	// Setup Time Event
  	TimeEvt_Setup();

  	// Setup Time Mgr
  	WizeApi_TimeMgr_Setup(&sTimeUpdCtx);
  	// Setup Wize API
  	WizeApi_SesMgr_Setup(&sPhyDev, &sInstCtx, &sAdmCtx,	&sDwnCtx);
  	WizeApi_Enable(1);
}

/*!
 * @brief This function finalize the "system part"
 */
void Sys_Fini(void)
{
	WizeApp_CtxSave();
}


/*!
 * @brief Start the RTOS scheduler
 */
__attribute__ (( always_inline )) void Sys_Start(void)
{
    /* Start scheduler */
	vTaskStartScheduler();
}

#ifdef __cplusplus
}
#endif

/*! @} */
