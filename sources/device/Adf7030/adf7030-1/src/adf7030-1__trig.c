/*!
 *****************************************************************************
  @file:	adf7030-1__trig.h
  @brief:
  @version:	$Revision:
  @date:	$Date:
 -----------------------------------------------------------------------------
Copyright (c) 2017, Analog Devices, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted (subject to the limitations in the disclaimer below) provided that
the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Neither the name of Analog Devices, Inc. nor the names of its contributors
    may be used to endorse or promote products derived from this software without
    specific prior written permission.

NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.

*****************************************************************************/

/** \addtogroup adf7030-1 adf7030-1 Driver
 *  @{
 */

/** \addtogroup adf7030-1__trig SPI Command Interface
 *  @{
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
 * @param [in]  pDevice         Pointer to the device info structure of the
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  eTRIG           PHY Radio TRIG pin to be configured (enum adf7030_1_trigpin_e)
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

/**
 * @brief       Enable the PHY external trigger
 *
 * @param [in]  pDevice         Pointer to the device info structure of the
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  ExCmd           PHY Radio TRIG to be enabled (enum adf7030_1_radio_extended_e)
 *
 *
 * @return      Status
 *  - #0      If the Radio PHY reset was transfert to the adf7030-1.
 *  - #1      [D] If the communication with Radio PHY failed or if Host GPIO
 *            trigger has not been previously configured
 */
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

/** @} */ /* End of group adf7030-1__trig */
/** @} */ /* End of group adf7030-1 adf7030-1 Driver */

