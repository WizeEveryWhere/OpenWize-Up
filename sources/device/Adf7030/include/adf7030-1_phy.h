/**
  * @file: adf7030-1_phy.h
  * @brief: This file define the prototype of related ADF7030 low level functions.
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
  * 1.0.0 : 2020/04/22[GBI]
  * Initial version
  *
  *
  */

/*!
 * @ingroup OpenWize'Up
 * @{
 *
 */
#ifndef _ADF7030_1_PHY_H_
#define _ADF7030_1_PHY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "adf7030-1__common.h"


#ifndef NOISE_MEAS_AVG_NB
/*! Define the number of measure on which the noise result is averaged */
#define NOISE_MEAS_AVG_NB (10u)
#endif

typedef enum {
    FW_MODULE_NAME,
    FW_MODULE_VERSION,
	ADI_REF,
}fw_module_info_e;

/******************************************************************************/

uint8_t adf7030_1_PulseTrigger(
    adf7030_1_device_t* const pDevice,
    adf7030_1_trigpin_e eTRIG
);

uint8_t adf7030_1_PulseReset(
    adf7030_1_device_t* const pDevice
);

uint8_t adf7030_1_PulseWakup(
    adf7030_1_device_t* const pDevice
);

/******************************************************************************/
/* Getter/Setter  */

uint16_t adf7030_1__GetRawRSSI(
	adf7030_1_spi_info_t* pSPIDevInfo
);

uint16_t adf7030_1__GetRawNoise(
    adf7030_1_spi_info_t* pSPIDevInfo,
	uint8_t               u8NbMeas
);

int16_t adf7030_1__GetRawAfcFreqErr(
    adf7030_1_spi_info_t* pSPIDevInfo
);

uint32_t adf7030_1__GetRawFrequency(
    adf7030_1_spi_info_t* pSPIDevInfo
);

uint8_t adf7030_1__SetRawFrequency(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              u32_Frequency
);

uint8_t adf7030_1__SetRawState(
    adf7030_1_spi_info_t*   pSPIDevInfo,
    adf7030_1_radio_state_e e_State
);

uint8_t adf7030_1__GetRawState(
    adf7030_1_spi_info_t* pSPIDevInfo
);

uint8_t adf7030_1__SetRawTXPower(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t               u8_PaCoarse,
    uint8_t               u8_PaFine,
    uint8_t               u8_PaMicro
);

uint8_t adf7030_1__GetFwModuleInfo(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t*              p_Data,
    uint8_t*              u8_Sz,
    uint8_t               eInfo
);

void adf7030_1__ClrIrqStatus(
    adf7030_1_spi_info_t *pSPIDevInfo,
    adf7030_1_intpin_e    eIntPin
);

uint32_t adf7030_1__GetIrqStatus(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_intpin_e    eIntPin
);

uint32_t adf7030_1__GetMiscFwStatus(
    adf7030_1_spi_info_t *pSPIDevInfo
);

/******************************************************************************/

uint8_t adf7030_1__SetupBuff(
    adf7030_1_spi_info_t* pSPIDevInfo
);

uint8_t adf7030_1__SetupLPM(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t               bFlag
);

uint8_t adf7030_1__SetupWakeSrc(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t               u8_WkUpSrc
);

uint8_t adf7030_1__SetupExtPaLna(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_gpio_pin_e  ePaPhyPin,
    adf7030_1_gpio_pin_e  eLnaPhyPin
);

uint8_t adf7030_1_SetupInt(
    adf7030_1_device_t* const pDevice,
    adf7030_1_intpin_e        eIntPin,
	uint8_t                   u8Flag
);

uint8_t adf7030_1_SetupTrig(
    adf7030_1_device_t* const pDevice,
    adf7030_1_trigpin_e       eTrigPin,
    adf7030_1_radio_state_e   eTrigState,
    uint8_t                   u8Flag
);


/******************************************************************************/

uint8_t adf7030_1__SetTxPacket(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t*              p_Data,
    uint8_t               u8_Sz
);

uint8_t adf7030_1__GetRxPacket(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t*              p_Data,
    uint8_t*              u8_Sz
);

/******************************************************************************/
// The following function use SPI polling on PHY state
uint8_t adf7030_1__MeasureNoise(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint16_t*             u16_Noise
);

uint8_t adf7030_1__Send(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t*              p_Data,
    uint8_t               u8_Sz
);

uint8_t adf7030_1__Recv(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t*              p_Data,
    uint8_t*              u8_Sz
);

/******************************************************************************/

uint8_t adf7030_1_SynchState(
    adf7030_1_device_t* const pDevice
);

/******************************************************************************/
/* Device */
uint8_t adf7030_1_Setup(
    adf7030_1_device_t*          const pDevice,
    adf7030_1_gpio_int_info_t*   const pINTDevInfo,
    adf7030_1_gpio_trig_info_t*  const pTRIGDevInfo,
	adf7030_1_gpio_reset_info_t* const pRESETDevInfo,
    adf7030_1_gpio_pin_e        eExtPaPin,
    adf7030_1_gpio_pin_e        eExtLnaPin
);

uint8_t adf7030_1_Init(
    adf7030_1_device_t* const pDevice,
	spi_dev_t         * pSpiDev
);

uint8_t adf7030_1_UnInit(
    adf7030_1_device_t* const pDevice
);

uint8_t adf7030_1_Configure(
    adf7030_1_device_t* const pDevice,
    uint8_t const *     pCfg,
    uint32_t            u32CfgSize
);

/******************************************************************************/
uint8_t adf7030_1_Enable(
    adf7030_1_device_t* const pDevice,
    uint8_t const *     pStartupCfg,
    uint32_t            u32_StartupCfgSize
);
uint8_t adf7030_1_Disable(
    adf7030_1_device_t* const pDevice
);

/******************************************************************************/
/* Irq */
uint8_t adf7030_1_HostGPIOIrq_Init(
    adf7030_1_device_t* const pDevice,
    uint32_t            u32Port,
    uint16_t            u16Pin,
    gpio_irq_trg_cond_e eSense,
    pf_cb_t             pfCallback,
    void*               const pCbParam
);

uint8_t adf7030_1_HostGPIOTrig_Init(
    adf7030_1_device_t* const pDevice,
    uint32_t            u32Port,
    uint16_t            u16Pin,
    uint8_t             bEnable
);

/******************************************************************************/
/* SPI */
uint8_t adf7030_1_HostSPI_Init(
    adf7030_1_device_t* const pDevice
);

uint8_t adf7030_1_HostSPI_UnInit(
    adf7030_1_device_t* const pDevice
);

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _ADF7030_1_PHY_H_ */

/*! @} */
