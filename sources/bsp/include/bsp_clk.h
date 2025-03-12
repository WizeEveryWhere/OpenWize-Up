/**
  * @file: bsp_clk.h
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
#ifndef _BSP_CLK_H_
#define _BSP_CLK_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "common.h"

/******************************************************************************/

/*!
 * @brief This enum define the High Speed Oscillator id
 */
enum osc_id_e
{
	OSC_ID_MSI = 0b00,
	OSC_ID_HSI = 0b01,
	OSC_ID_HSE = 0b10,
	OSC_ID_NC  = 0b11,
};

/*!
 * @brief This enum define the system clock id
 */
enum sysclk_id_e
{
	SYSCLK_ID_MSI  = 0b00,
	SYSCLK_ID_HSI  = 0b01,
	SYSCLK_ID_HSE  = 0b10,
	SYSCLK_ID_PLL  = 0b11,
};

/*!
 * @brief This enum define the Low Speed Oscillator id
 */
enum lsc_id_e
{
	LSC_ID_LSE = 0b00,
	LSC_ID_LSI = 0b01,
	// ---
	LSC_ID_NONE = 0b11,
};

/*!
 * @brief This enum define the LSE clock drive capability
 */
enum lse_drive_e
{
	LSE_DRIVE_LOW        = 0b00,
	LSE_DRIVE_MEDIUMLOW  = 0b01,
	LSE_DRIVE_MEDIUMHIGH = 0b10,
	LSE_DRIVE_HIGH       = 0b11,
};

/*!
 * @brief This enum define UART, USART and LPUART base clock id
 */
enum uart_clk_id_e
{
	UART_CLK_ID_PCLK   = 0b00, /*!< PCLK */
	UART_CLK_ID_SYSCLK = 0b01, /*!< SYSCLK */
	UART_CLK_ID_HSI16  = 0b10, /*!< HSI16 */
	UART_CLK_ID_LSE    = 0b11, /*!< LSE */
} ;

/*!
 * @brief This enum define LPTIM base clock id
 */
enum lptim_clk_id_e
{
	LPTIM_CLK_ID_PCLK   = 0b00, /*!< PCLK */
	LPTIM_CLK_ID_LSI    = 0b01, /*!< LSI */
	LPTIM_CLK_ID_HSI16  = 0b10, /*!< HSI16 */
	LPTIM_CLK_ID_LSE    = 0b11, /*!< LSE */
} ;

/*!
 * @brief This enum define I2C base clock id
 */
enum i2c_clk_id_e
{
	I2C_CLK_ID_PCLK   = 0b00, /*!< PCLK */
	I2C_CLK_ID_SYSCLK = 0b01, /*!< SYSCLK */
	I2C_CLK_ID_HSI16  = 0b10, /*!< HSI16 */
	I2C_CLK_ID_nc     = 0b11, /*!< reserved */
} ;


struct pll_fact_s {
	union {
		uint32_t pll_fact;
		struct {
			uint8_t N;
			uint8_t R;
			uint8_t M;
			union {
				uint8_t state;
				struct {
					uint8_t exact:1;
					uint8_t init:1;
				};
			};
		};
	};
};

struct sys_info_s
{
	uint32_t clk_cfg;
	uint32_t pvd;
	int32_t lsi;
	int32_t lse;
	int32_t msi;
	int32_t hsi;
	int32_t hse;
	int32_t sys;
	union {
		int32_t state;
		struct {
			uint32_t lsc_err:1;
			uint32_t osc_err:1;
			uint32_t :30;
		};
	};
};

struct lsc_s {
	uint8_t drive:2;
	// LSE drive
	// if toggle == 0, the LSE use the given "drive" value
	// if toggle == 1, the LSE start with "HIGH" then switch the given "drive" value
	uint8_t toggle:1;
	uint8_t :5;
};

union peripheral_cfg_u {
    uint32_t peripheral_cfg;
    struct {
    	uint32_t baud:24;
    	uint32_t :4;
    	uint32_t base_clk:2;
		uint32_t swap:1;
		uint32_t enable:1;
    };
};

/******************************************************************************/

extern struct sys_info_s g_xSysInfo;

/******************************************************************************/
dev_res_e BSP_Clk_BDCR(uint8_t bEnable);

dev_res_e BSP_Clk_LSE(uint8_t bEnable);
dev_res_e BSP_Clk_LSI(uint8_t bEnable);
dev_res_e BSP_Clk_HSE(uint8_t bEnable);
dev_res_e BSP_Clk_HSI(uint8_t bEnable);
dev_res_e BSP_Clk_MSI(uint8_t bEnable);
void BSP_Clk_OscTrim(uint32_t u32RccOscType, uint16_t u16TrimValue);

dev_res_e BSP_Clk_PLL(uint32_t u32PllSrc, int32_t src_freq, uint8_t bEnable);

dev_res_e BSP_Clk_SetPllOsc(uint32_t u32RccOscType, uint8_t bEnable);
dev_res_e BSP_Clk_SetRtcOsc(uint32_t u32RccRtcclk);

dev_res_e BSP_Clk_SetSysClk(uint32_t u32RccSysclk, uint32_t FLatency);
uint32_t BSP_Clk_GetSysClk(void);


dev_res_e BSP_Clk_LscCfg(void);
dev_res_e BSP_Clk_OscCfg(void);

void BSP_Clk_Init(void);

void BSP_Clk_Peripheral(uint32_t u32Peripheral, uint8_t u8BaseClk);

/******************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* _BSP_CLK_H_ */
