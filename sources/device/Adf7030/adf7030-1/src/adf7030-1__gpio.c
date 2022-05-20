/*!
 *****************************************************************************
  @file:	adf7030-1__gpio.h
  @brief:	adf7030-1 PHY Radio GPIO interface Functions.
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

/** \addtogroup adf7030-1__gpio SPI Command Interface
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "adf7030-1__gpio.h"

#include "adf7030-1_reg.h"
#include "adf7030-1__mem.h"
#include "adf7030-1__spi.h"

/**
 * @brief       Setup individual GPIO configuration on the PHY Radio
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  eGPIO           PHY Radio GPIO pin to be configured
 *
 * @param [in]  eCFG            PHY Radio GPIO pin configuration desired (see adf7030_1_gpio_cfg_e)
 *
 * @param [in]  bDirect         Boolean  -TRUE to directly change the current GPIO pin configuration
 *                                       -FALSE to setup GPIO pin configuration in the profile.
 *                                              The Host must issue a CFG_DEV command to trigger
 *                                              the new setting.
 *
 * @return      Status
 *  - #0      If the Radio PHY gpio pin output was cleared correctly.
 *  - #1      [D] If the communication with Radio PHY failed.
 */
uint8_t adf7030_1__GPIO_SetCfg(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_gpio_pin_e  eGPIO,
    adf7030_1_gpio_cfg_e  eCFG,
    uint8_t               bDirect
)
{
    /* Setup configuration address */
    uint32_t Addr = ((bDirect == 1) ? GPIO_BASE : PROFILE_GPCON_Base) + (uint32_t)eGPIO;
    
    /* Transfer configuration over SPI */
    adf7030_1__SPI_SetField( pSPIDevInfo,
                             (Addr >> 2) << 2,
                             (Addr & 0x3) << 3,
                             8,
                             (uint32_t)eCFG );
      
    /* Return SPI transfer status */
    return( (pSPIDevInfo->eXferResult)?(1):(0));
}
  


/**
 * @brief       Get individual GPIO configuration from the PHY Radio
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  eGPIO           PHY Radio GPIO pin to be configured
 *
 * @param [in]  peCFG           Pointer to PHY Radio GPIO pin configuration (see adf7030_1_gpio_cfg_e)
 *
 * @param [in]  bDirect         Boolean  -TRUE to directly change the current GPIO pin configuration
 *                                       -FALSE to setup GPIO pin configuration in the profile.
 *                                              The Host must issue a CFG_DEV command to trigger
 *                                              the new setting.
 *
 * @return      Status
 *  - #0      If the Radio PHY gpio pin output was cleared correctly.
 *  - #1      [D] If the communication with Radio PHY failed.
 */
uint8_t adf7030_1__GPIO_GetCfg(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_gpio_pin_e  eGPIO,
    adf7030_1_gpio_cfg_e* peCFG,
    uint8_t               bDirect
)
{
    /* Setup configuration address */
    uint32_t Addr = ((bDirect == 1) ? GPIO_BASE : PROFILE_GPCON_Base) + (uint32_t)eGPIO;
    
    /* Readback configuration over SPI */    
    *peCFG = (adf7030_1_gpio_cfg_e)adf7030_1__SPI_GetField( pSPIDevInfo,
                                                              (Addr >> 2) << 2,
                                                              (Addr & 0x3) << 3,
                                                              8 );
      
    /* Return SPI transfer status */
    return( (pSPIDevInfo->eXferResult)?(1):(0));
}
  

/**
 * @brief       Set an individual PHY Radio GPIO pin to 1
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  ePin            PHY Radio GPIO pin to be set
 *
 * @return      Status
 *  - #0      If the Radio PHY gpio pin output was cleared correctly.
 *  - #1      [D] If the communication with Radio PHY failed.
 */
uint8_t adf7030_1__GPIO_SetPin(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_busio_pin_e ePin
)
{   
    /* Transfer configuration over SPI */
    adf7030_1__SPI_SetMem32( pSPIDevInfo, GPIO_SET_REG, ePin );
    /* Return SPI transfer status */
    return( (pSPIDevInfo->eXferResult)?(1):(0));
}


/**
 * @brief       Clear an individual PHY Radio GPIO pin to 0
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  ePin            PHY Radio GPIO pin to be cleared
 *
 * @return      Status
 *  - #0      If the Radio PHY gpio pin output was cleared correctly.
 *  - #1      [D] If the communication with Radio PHY failed.
 */
uint8_t adf7030_1__GPIO_ClrPin(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_busio_pin_e ePin
)
{
    /* Transfer configuration over SPI */
    adf7030_1__SPI_SetMem32( pSPIDevInfo, GPIO_CLR_REG, ePin );
    /* Return SPI transfer status */
    return( (pSPIDevInfo->eXferResult)?(1):(0));
}

#ifdef __cplusplus
}
#endif

/** @} */ /* End of group adf7030-1__gpio */
/** @} */ /* End of group adf7030-1 adf7030-1 Driver */

