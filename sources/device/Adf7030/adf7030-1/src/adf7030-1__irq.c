/*!
 *****************************************************************************
  @file:	adf7030-1__irq.h
  @brief:	GPIO Interrupt Interface layer between PHY Radio and the Host.
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

/** \addtogroup adf7030-1__irq SPI Command Interface
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif


#include "adf7030-1__irq.h"

#include "adf7030-1_reg.h"
#include "adf7030-1__spi.h"
#include "adf7030-1__mem.h"
#include "adf7030-1__gpio.h"

/**
 * @brief       Configures the PHY interrupt source mask register
 *
 * @note        This function transfers bit [31:8] of nIntMap into the Radio PHY
 *              interrupt mask resgister. Lower bits [7:0] are written into the PHY
 *              profile packet interrupt location. See all available interrupt 
 *              sources adf7030_1_ext_irq_e.
 *              
 * @param [in]  pDevice         Pointer to the ADF7030-1 instance information structure.
 *   
 * @param [in]  eIntPin         Interrupt id (adf7030_1_intpin_e) to configure.
 *             
 * @param [in]  nIntMap         Interrupt events which will be triggering PHY irq line.         
 *             
 * @return      Status
 *  - #0    If PHY intance irq mask was succesfully configured
 *  - #1    [D] PHY irq mask configuration failed
 */

uint8_t adf7030_1__IRQ_SetMap(
    adf7030_1_device_t* const pDevice,
    adf7030_1_intpin_e eIntPin,
    uint32_t           nIntMap
)
{
    /* Pointer to IRQ GPIO Pin info */
    adf7030_1_gpio_int_info_t * pIntGPIOInfo = &pDevice->IntGPIOInfo[eIntPin];

    uint32_t irq_msk;
    // non_frame_irq
    irq_msk = nIntMap & 0xFFFFFF00;

    if(irq_msk || !(nIntMap)) {
        /* Setup bit [31:8] of nIntMap into IRQ_CTRL_MASK0_Addr or IRQ_CTRL_MASK1_Addr */
        if(adf7030_1__SPI_wr_word_b_a( &pDevice->SPIInfo,
                                       IRQ_CTRL_MASK0_Addr + (eIntPin << 2),
                                       1,
                                       &irq_msk) )
        {
            return 1;
        }
    }
    // frame_irq
    irq_msk = nIntMap & 0xFF;
    if(irq_msk || !(nIntMap)) {
        /* Setup bit [7:0] of nIntMap into GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE or GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE */
        adf7030_1__SPI_SetBytes( &pDevice->SPIInfo,
                                GENERIC_PKT_FRAME_CFG1_Addr + 2 + (uint32_t)eIntPin,
                                irq_msk,
                                1,
                                NULL);
    }
    /* Save the current Radio PHY interrupt mask into the current instance GPIO Pin info structure */
    pIntGPIOInfo->nIntMap = nIntMap;

    return ( (pDevice->SPIInfo.eXferResult)?(1):(0) );
}


/**
 * @brief       Readbacks the PHY interrupt source mask register
 *
 * @note        This function transfers the Radio PHY interrupt mask resgister
 *              back into the Host interrupt pin configuration structure.
 *              See all available interrupt sources adf7030_1_ext_irq_e.
 *              
 * @param [in]  pDevice         Pointer to the ADF7030-1 instance information structure.
 *   
 * @param [in]  eIntPin         Interrupt id (adf7030_1_intpin_e) to configure.
 *             
 * @return      Status
 *  - #0    If PHY intance irq mask was succesfully readback
 *  - #1    [D] PHY irq mask readback failed
 */

uint8_t adf7030_1__IRQ_GetMap(
    adf7030_1_device_t* const pDevice,
    adf7030_1_intpin_e  eIntPin
)
{
    /* Pointer to IRQ GPIO Pin info */
    adf7030_1_gpio_int_info_t * pIntGPIOInfo = &pDevice->IntGPIOInfo[eIntPin];
        
    /* Readback Radio PHY interrupt mask bits [31:8] into the current instance GPIO Pin info structure */
    if(adf7030_1__SPI_rd_word_b_a( &pDevice->SPIInfo,
                                   IRQ_CTRL_MASK0_Addr + (eIntPin << 2),
                                   1,
                                   &pIntGPIOInfo->nIntMap ) )
    {
        return 1;
    }
    
    /* Readback Radio PHY interrupt mask bits [7:0] into the current instance GPIO Pin info structure */
    *((uint8_t *)&pIntGPIOInfo->nIntMap) = (uint8_t) adf7030_1__SPI_GetBytes( &pDevice->SPIInfo,
                                                                GENERIC_PKT_FRAME_CFG1_Addr + 2 + (uint32_t)eIntPin,
                                                                1,
                                                                NULL);

    return ( (pDevice->SPIInfo.eXferResult)?(1):(0) );
}


/**
 * @brief       Readbacks the PHY interrupt status register
 *
 * @note        This function readback the Radio PHY interrupt status register
 *              back into the Host interrupt pin configuration structure.
 *              See all available interrupt sources adf7030_1_ext_irq_e.
 *              
 * @param [in]  pDevice         Pointer to the ADF7030-1 instance information structure.
 *   
 * @param [in]  eIntPin         Interrupt id (adf7030_1_intpin_e) to configure.
 *             
 * @return      Status
 *  - #0    If PHY intance irq status was succesfully readback
 *  - #1    [D] PHY irq status readback failed
 */

uint8_t adf7030_1__IRQ_GetStatus(
    adf7030_1_device_t* const pDevice,
    adf7030_1_intpin_e  eIntPin
)
{
    /* Pointer to IRQ GPIO Pin info */
    adf7030_1_gpio_int_info_t * pIntGPIOInfo = &pDevice->IntGPIOInfo[eIntPin];
        
    /* Readback Radio PHY interrupt status into the current instance GPIO Pin info structure */
    return(adf7030_1__SPI_rd_word_b_a( &pDevice->SPIInfo,
                                       IRQ_CTRL_STATUS0_Addr + (eIntPin << 2),
                                       1,
                                       &pIntGPIOInfo->nIntStatus));
}


/**
 * @brief       Clear the PHY eIntPin interrupt pin
 *
 * @note        This function clears the Radio PHY interrupt status register
 *              by "write one to clear" operation. If PHY status register is 0,
 *              external GPIO line will be deasserted.
 *              See all available interrupt sources adf7030_1_ext_irq_e.
 *              
 * @param [in]  pDevice         Pointer to the ADF7030-1 instance information structure.
 *   
 * @param [in]  eIntPin         Interrupt id (adf7030_1_intpin_e) to configure.
 *             
 * @param [in]  nIntClear       Interrupt events to clear.         
 *             
 * @return      Status
 *  - #0    If PHY intance irq status was succesfully cleared
 *  - #1    [D] PHY irq status clearing failed
 */

uint8_t adf7030_1__IRQ_ClrStatus(
    adf7030_1_device_t* const pDevice,
    adf7030_1_intpin_e eIntPin,
    uint32_t           nIntClear
)
{
    /* Pointer to IRQ GPIO Pin info */
    adf7030_1_gpio_int_info_t * pIntGPIOInfo = &pDevice->IntGPIOInfo[eIntPin];
        
    /* Clear Radio PHY interrupt status */
    if( adf7030_1__SPI_wr_word_b_a( &pDevice->SPIInfo,
                                   IRQ_CTRL_STATUS0_Addr + (eIntPin << 2),
                                   1,
                                   &nIntClear) )
    {
    	return 1;
    }

    pIntGPIOInfo->nIntStatus &= ~nIntClear;
    return 0;
}

/**
 * @brief       Readback and Clear the PHY eIntPin interrupt pin
 *
 * @note        This function readback the Radio PHY interrupt status register
 *              and use the value immadiately clear by "write one to clear" operation.
 *              If PHY status register is 0, external GPIO line will be deasserted.
 *              In this case, pGPIOInfo->nIntStatus holds the state of the PHY
 *              status register pior to clearing it.
 *              See all available interrupt sources adf7030_1_ext_irq_e.
 *              
 * @param [in]  pDevice         Pointer to the ADF7030-1 instance information structure.
 *   
 * @param [in]  eIntPin         Interrupt id (adf7030_1_intpin_e) to configure.
 *             
 * @return      Status
 *  - #0    If PHY intance irq status was succesfully readback and cleared
 *  - #1    [D] PHY irq status reading or clearing failed
 */

uint8_t adf7030_1__IRQ_GetClrStatus(
    adf7030_1_device_t* const pDevice,
    adf7030_1_intpin_e  eIntPin
)
{
    /* Pointer to IRQ GPIO Pin info */
    adf7030_1_gpio_int_info_t * pIntGPIOInfo = &pDevice->IntGPIOInfo[eIntPin];
        
    /* Readback Radio PHY interrupt status into the current instance GPIO Pin info structure */
    if( !( adf7030_1__SPI_rd_word_b_a( &pDevice->SPIInfo,
                                              IRQ_CTRL_STATUS0_Addr + (eIntPin << 2),
                                              1,
                                              &pIntGPIOInfo->nIntStatus)) )
    {
        /* Clear Radio PHY interrupt status */
        return adf7030_1__SPI_wr_word_b_a( &pDevice->SPIInfo,
                                              IRQ_CTRL_STATUS0_Addr + (eIntPin << 2),
                                              1,
                                              &pIntGPIOInfo->nIntStatus);
    }

    return 1;
}



/**
 * @brief       Setup PHY Radio GPIO irq
 *
 * @note        User can use this function prior to issuing CFG_DEV command to 
 *              configure the PHY Radio pinmux setting for each individual IRQ pin.            
 *              
 * @param [in]  pDevice         Pointer to the ADF7030-1 instance information structure.
 *   
 * @param [in]  eIntPin         Interrupt id (adf7030_1_intpin_e) to configure.
 *             
 * @return      Status
 *  - #0    If PHY irq GPIO was configured
 *  - #1    [D] PHY irq GPIO configuration failed
 */
uint8_t adf7030_1__IRQ_SetGPIOPin(
    adf7030_1_device_t* const pDevice,
    adf7030_1_intpin_e  eIntPin
)
{
    /* Pointer to IRQ GPIO Pin info */
    adf7030_1_gpio_int_info_t * pIntGPIOInfo = &pDevice->IntGPIOInfo[eIntPin];
    
    return adf7030_1__GPIO_SetCfg( &pDevice->SPIInfo,
                            pIntGPIOInfo->ePhyPin,
                            (eIntPin == ADF7030_1_INTPIN1) ? GPIO_IRQ_OUT1 : GPIO_IRQ_OUT0,
                            0 );
}

#ifdef __cplusplus
}
#endif

/** @} */ /* End of group adf7030-1__irq */
/** @} */ /* End of group adf7030-1 adf7030-1 Driver */

