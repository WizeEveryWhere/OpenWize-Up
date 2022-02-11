/**
  * @file: adf7030-1__trig.c
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
  * 1.0.0 : 2020/05/18[TODO: your name]
  * Initial version
  *
  *
  */
#ifdef __cplusplus
extern "C" {
#endif

#include "adf7030-1__trig.h"

#include "adf7030-1_reg.h"
#include "adf7030-1__spi.h"
#include "adf7030-1__mem.h"
#include "adf7030-1__gpio.h"
#include "adf7030-1__state.h"

/**
 * @brief       Setup PHY gpio to for external trigger command
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  TRIG            PHY Radio TRIG pin to be configured (enum adf7030_1_trigpin_e)
 *
 * @param [in]  GPIO            PHY Radio GPIO pin to be configured (enum ADF7030_1_GPIO)
 *
 * @param [in]  goState         PHY Radio State to execute. See adf7030_1_radio_state_e
 *                              enum for available States.
 *
 * @return      Status
 *  - #0      If the Radio PHY reset was transfert to the adf7030-1.
 *  - #1      [D] If the communication with Radio PHY failed or if Host GPIO
 *            trigger has not been previously configured
 */
uint8_t adf7030_1__TRIG_SetGPIOPin(
	adf7030_1_device_t* const pDevice,
    adf7030_1_trigpin_e     eTRIG,
    adf7030_1_radio_state_e goState
)
{
    /* Pointer to SPI dev info */
    adf7030_1_spi_info_t * pSPIDevInfo = &pDevice->SPIInfo;

    /* Setup Host GPIO for trigger */
    adf7030_1_gpio_trig_info_t * pTrigGPIOInfo = &pDevice->TrigGPIOInfo[eTRIG];

    if(pTrigGPIOInfo->eTrigStatus & HOST_READY)
    {
        /* Setup PHY Radio pinmux */
        adf7030_1__GPIO_SetCfg( pSPIDevInfo,
                                pTrigGPIOInfo->ePhyPin,
                                (eTRIG == ADF7030_1_TRIGPIN1) ? GPIO_TRIG_IN1 : GPIO_TRIG_IN0,
                                0 );

        /* Setup trigger PHY Radio command */
        adf7030_1__SPI_SetField( pSPIDevInfo,
                                SM_CONFIG_GPIO_CMD_0_Addr + (eTRIG << 1),
                                0,
                                16, //Clear the cfg too.
                                (RADIO_CMD | (uint16_t)goState) );

        pTrigGPIOInfo->nTrigCmd = RADIO_CMD | (uint16_t)goState;
        pTrigGPIOInfo->eTrigStatus |= PHY_READY;
    }
    else
    {
        /* Driver prevent setting PHY Radio GPIO unless Host GPIO has been configured */
        return 1;
    }

    /* Return SPI transfer status */
    return( (pSPIDevInfo->eXferResult)?(1):(0) );
}


uint8_t adf7030_1__TRIG_Enable(
	adf7030_1_device_t* const pDevice,
    adf7030_1_radio_extended_e ExCmd
)
{
    /* Pointer to SPI dev info */
    adf7030_1_spi_info_t * pSPIDevInfo = &pDevice->SPIInfo;

    if( (adf7030_1__STATE_PhyCMD_Ex(pSPIDevInfo, ExCmd)) )
    {
        return 1;
    }

    /* Fix adf7030-1 bug when both triggers are enabled */
    if(ExCmd == TRIGPIN_ALL)
    {
        uint32_t fix = (*((uint8_t *)&pDevice->TrigGPIOInfo[ADF7030_1_TRIGPIN0].nTrigCmd + 1)) |
                       (*((uint8_t *)&pDevice->TrigGPIOInfo[ADF7030_1_TRIGPIN1].nTrigCmd + 1)) |
                       0x00000808;
        adf7030_1__SPI_SetField( pSPIDevInfo,
                                 0x40003818UL,
                                 0,
                                 16,
                                 fix );

        /* Return SPI transfer status */
        return ( (pSPIDevInfo->eXferResult)?(1):(0) );
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
