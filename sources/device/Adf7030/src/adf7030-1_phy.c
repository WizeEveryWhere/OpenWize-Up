/**
  * @file: adf7030-1_phy.c
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
  * 1.0.0 : 2020/04/22[GBI]
  * Initial version
  *
  *
  */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <bsp.h>
#include <machine/endian.h>

#include "adf7030-1_phy.h"
#include "adf7030-1_phy_fw.h"
#include "adf7030-1_phy_hw.h"

#include "adf7030-1_reg.h"
#include "adf7030-1__common.h"
#include "adf7030-1__spi.h"
#include "adf7030-1__mem.h"
#include "adf7030-1__gpio.h"
#include "adf7030-1__irq.h"
#include "adf7030-1__trig.h"


//#include "adf7030-1__patch.h"
#include "adf7030-1__cfg.h"
#include "adf7030-1__state.h"

/* SPI speed */
#define ADF7030_1_SPI_DEV_BITRATE         6000000 // Max ADF7030 : 12.5 MHz
#define ADF7030_1_SPI_DEV_BITRATE_FAST    ADF7030_1_SPI_DEV_BITRATE << 1

#define PTR_TRX_BASE_MIN 0x2bcUL // 0x2bc << 2 = 0xAF0

uint8_t a_SpiTxBuf [ADF7030_1_SPI_BUFFER_SIZE];
uint8_t a_SpiRxBuf [ADF7030_1_SPI_BUFFER_SIZE];

/******************************************************************************/
/* Pulse */

/**
 * @brief       Pulse Host GPIO Trigger pin
 *
 * @param [in]  pDevInfo        Pointer to the ADF7030-1 instance information.
 * @param [in]  eTRIG           PHY Trigger PIN to pulse.
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS                If Trigger GPIO has been pulsed
 *  - #ADF7030_1_GPIO_DEV_FAILED        If Trigger GPIO failed to pulse
*/
uint8_t adf7030_1_PulseTrigger(
    adf7030_1_device_t* const pDevice,
    adf7030_1_trigpin_e eTRIG
)
{
    if (pDevice == NULL) { return 1;}
    /* Setup pointer to Instance GPIO trigger info */
    adf7030_1_gpio_trig_info_t * pTrigGPIOInfo = &pDevice->TrigGPIOInfo[eTRIG];
    if((pTrigGPIOInfo->u32Port != 0UL) && (pTrigGPIOInfo->u16Pin != 0UL))
    {
        /* Toggle PHY Radio GPIO pin from the Glue Host */
        if(BSP_Gpio_SetHigh(pTrigGPIOInfo->u32Port, pTrigGPIOInfo->u16Pin) != DEV_SUCCESS)
        {
            return 1;
        }

        if(BSP_Gpio_SetLow(pTrigGPIOInfo->u32Port, pTrigGPIOInfo->u16Pin) != DEV_SUCCESS)
        {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief       Pulse Host GPIO Reset pin
 *
 * @param [in]  pDevInfo        Pointer to the ADF7030-1 instance information.
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS                If Reset GPIO has been pulsed
 *  - #ADF7030_1_GPIO_DEV_FAILED        If Reset GPIO failed to pulse
*/
uint8_t adf7030_1_PulseReset(
    adf7030_1_device_t* const pDevice
)
{
    if (pDevice == NULL) { return 1;}
    /* Setup pointer to Instance GPIO reset info */
    adf7030_1_gpio_reset_info_t * pResetGPIOInfo = &pDevice->ResetGPIOInfo;
    if((pResetGPIOInfo->u32Port != 0UL) && (pResetGPIOInfo->u16Pin != 0UL))
    {
        /* Pulse low PHY Radio GPIO pin from the Glue Host */
        if(BSP_Gpio_SetLow(pResetGPIOInfo->u32Port, pResetGPIOInfo->u16Pin) != DEV_SUCCESS)
        {
            return 1;
        }
        msleep(1);
        // TODO : micro-sleep of at least 2µs
        if(BSP_Gpio_SetHigh(pResetGPIOInfo->u32Port, pResetGPIOInfo->u16Pin) != DEV_SUCCESS)
        {
            return 1;
        }
    }
    return 0;
}

/*!
 * @brief  This function pulse (depend on configured wake-up source) a wake-up
 *         to the ADF7030.
 *
 * @param [in] pDevice Pointer to ADF7030-1 device instance.
 *
 * @return      Status
 *  - #ADF7030_1_INVALID_HANDLE  [D] If the given SPI ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_SUCCESS         Otherwise.
 */
uint8_t adf7030_1_PulseWakup(
    adf7030_1_device_t* const pDevice
)
{
	p_spi_dev_t pSpiDev;
    uint8_t u8Level;
    if (pDevice == NULL) { return ADF7030_1_INVALID_OPERATION;}
#ifdef TRIG_AS_WAKE_UP
    if( (pDevice->TrigGPIOInfo[ADF7030_1_TRIGPIN0].eTrigStatus != ENABLED) &&
        (pDevice->TrigGPIOInfo[ADF7030_1_TRIGPIN1].eTrigStatus != ENABLED)
        )
    {
#endif
    	pSpiDev = ((p_spi_dev_t)pDevice->SPIInfo.hSPIDevice);
    	BSP_Gpio_SetHigh(pSpiDev->ss_port, pSpiDev->ss_pin);
        BSP_Gpio_SetLow(pSpiDev->ss_port, pSpiDev->ss_pin);
        // FIXME :
        //do {
        //	// should rise high level after 92µs typ.
        //    BSP_Gpio_Get((uint32_t)RADIO_MISO_GPIO_Port, RADIO_MISO_Pin, &u8Level);
        //} while(u8Level);
        msleep(1);
        BSP_Gpio_SetHigh(pSpiDev->ss_port, pSpiDev->ss_pin);
#ifdef TRIG_AS_WAKE_UP
    }
    else
    {
        if( (pDevice->TrigGPIOInfo[ADF7030_1_TRIGPIN0].eTrigStatus == ENABLED) )
        {
            adf7030_1_PulseTrigger(pDevice, ADF7030_1_TRIGPIN0);
        }
        else {
            adf7030_1_PulseTrigger(pDevice, ADF7030_1_TRIGPIN1);
        }
    }
#endif
    return 0;
}


/******************************************************************************/
/* Getter/Setter  */

/*!
 * @brief  This function get the RSSI
 *
 * @param [in] pSPIDevInfo   Pointer to ADF7030-1 SPI device instance.
 *
 * @return Current RSSI
 */
uint16_t adf7030_1__GetRawRSSI(
    adf7030_1_spi_info_t* pSPIDevInfo
)
{
    if (pSPIDevInfo == NULL) { return 0;}
    /* PHY Radio RSSI fixpoint format Q9.2 */
    return (uint16_t)(adf7030_1__READ_FIELD(GENERIC_PKT_LIVE_LINK_QUAL_RSSI));
}

/*!
 * @brief  This function get the Noise
 *
 * @note  The PHY device has to be in CCA state before calling this function
 *
 * @param [in] pSPIDevInfo   Pointer to ADF7030-1 SPI device instance.
 * @param [in] u8NbMeas      The number of noise measure to average one.
 *
 * @return Current Noise
 */
uint16_t adf7030_1__GetRawNoise(
    adf7030_1_spi_info_t* pSPIDevInfo,
	uint8_t               u8NbMeas
)
{
    uint8_t u8i, u8j;
    uint32_t u32_Sum;
    uint16_t u16_NoiseMeas;
    uint16_t u16_NoiseAvg;
    cca_read_back_t ccaReadBack;

    if (pSPIDevInfo == NULL) { return 0;}

    u32_Sum = 0;
    u8j = 0;
    u16_NoiseAvg = 0;
    for (u8i = 1; u8i <= u8NbMeas ; u8i++)
    {
        ccaReadBack = (cca_read_back_t)adf7030_1__SPI_GetMem32(pSPIDevInfo,  PROFILE_CCA_READBACK_Addr );
        u16_NoiseMeas = ccaReadBack.CCA_READBACK_b.VALUE;
        if (ccaReadBack.CCA_READBACK_b.LIVE_STATUS == 1 || u16_NoiseMeas == 0){
            u8i--; u8j++;
            if (u8j > (u8NbMeas*10) )
            {
            	break;
            }
        }
        else {
            u32_Sum += u16_NoiseMeas;
            u16_NoiseAvg = u32_Sum / u8i;
        }
        if (u8j > 1000) {
        	break;
        }
    }
    return u16_NoiseAvg;
}

/*!
 * @brief  This function get the AFC frequency error
 *
 * @param [in] pSPIDevInfo   Pointer to ADF7030-1 SPI device instance.
 *
 * @return Current AFC frequency error
 */
inline int16_t adf7030_1__GetRawAfcFreqErr(
    adf7030_1_spi_info_t* pSPIDevInfo
)
{
    if (pSPIDevInfo == NULL) { return 0;}
    return (int16_t)adf7030_1__READ_FIELD(AFC_FREQUENCY_ERROR_READBACK);
}

/*!
 * @brief  This function get the current frequency
 *
 * @param [in] pSPIDevInfo   Pointer to ADF7030-1 SPI device instance.
 *
 * @return Current frequency (Hz)
 */
inline uint32_t adf7030_1__GetRawFrequency(
    adf7030_1_spi_info_t* pSPIDevInfo
)
{
    if (pSPIDevInfo == NULL) { return 0;}
    return adf7030_1__SPI_GetMem32(pSPIDevInfo, PROFILE_CH_FREQ_VAL_Addr);
}

/*!
 * @brief  This function change the current frequency
 *
 * @param [in] pSPIDevInfo   Pointer to ADF7030-1 SPI device instance.
 * @param [in] u32_Frequency The required frequency (Hz).
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If the transfert was succesfull to the adf7030-1.
 *  - #ADF7030_1_INVALID_HANDLE  [D] If the given SPI ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the transfert failed.
 */
inline uint8_t adf7030_1__SetRawFrequency(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t u32_Frequency
)
{
    if (pSPIDevInfo == NULL) { return 1;}
    adf7030_1__SPI_SetMem32(pSPIDevInfo, PROFILE_CH_FREQ_VAL_Addr, u32_Frequency);
    return ( (pSPIDevInfo->eXferResult)?(1):(0) );
}

/*!
 * @brief  This function change the FW state (no polling)
 *
 * @details This function send command to change the FW state, but dosen't check
 *          this change is effective.
 *
 * @param [in] pSPIDevInfo Pointer to ADF7030-1 SPI device instance.
 * @param [in] e_State     The required state.
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If the transfert was succesfull to the adf7030-1.
 *  - #ADF7030_1_INVALID_HANDLE  [D] If the given SPI ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the transfert failed.
 */
inline uint8_t adf7030_1__SetRawState(
    adf7030_1_spi_info_t*   pSPIDevInfo,
    adf7030_1_radio_state_e e_State
)
{
    if (pSPIDevInfo == NULL) { return 1;}
    //return adf7030_1__STATE_PhyCMD( pSPIDevInfo, e_State );

    uint8_t nPhyCmd = RADIO_CMD | (uint8_t)e_State;
	adf7030_1__SPI_ReadWrite_Fast( pSPIDevInfo,
								   &nPhyCmd,
								   &pSPIDevInfo->nStatus.VALUE,
								   1 );
	return ( (pSPIDevInfo->eXferResult)?(1):(0) );
}

/*!
 * @brief  This function get the current FW state.
 *
 * @param [in]  pSPIDevInfo Pointer to ADF7030-1 SPI device instance.
 *
 * @return Current FW state
 */
inline uint8_t adf7030_1__GetRawState(
    adf7030_1_spi_info_t* pSPIDevInfo
)
{
    misc_fw_t misc_fw;
    if (pSPIDevInfo == NULL) { return 0xFF;}
    misc_fw = (misc_fw_t)adf7030_1__SPI_GetMem32(pSPIDevInfo, MISC_FW_Addr);
    pSPIDevInfo->ePhyError = misc_fw.FW_b.ERR_CODE;
    pSPIDevInfo->nPhyState = misc_fw.FW_b.CURR_STATE;
    return (misc_fw.FW_b.CURR_STATE);
}

/*!
 * @brief  This function set the transmitting power.
 *
 * @param [in] pSPIDevInfo Pointer to ADF7030-1 SPI device instance.
 * @param [in] u8_PaCoarse Coarse Power value (select a kind of curve)
 * @param [in] u8_PaFine   Fine Power value (select one point the curve)
 * @param [in] u8_PaMicro  Micro Power value (adjust with a 0.5dDm step)
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If the transfert was succesfull to the adf7030-1.
 *  - #ADF7030_1_INVALID_HANDLE  [D] If the given SPI ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the transfert failed.
 */
uint8_t adf7030_1__SetRawTXPower(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t u8_PaCoarse,
    uint8_t u8_PaFine,
    uint8_t u8_PaMicro
)
{
    radio_dig_tx_cfg0_t tx_cfg0;
    if (pSPIDevInfo == NULL) { return 1;}
    tx_cfg0 = (radio_dig_tx_cfg0_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, PROFILE_RADIO_DIG_TX_CFG0_Addr));
    // Setup PA1 power
    tx_cfg0.RADIO_DIG_TX_CFG0_b.PA_COARSE = u8_PaCoarse;
    tx_cfg0.RADIO_DIG_TX_CFG0_b.PA_FINE = u8_PaFine;
    tx_cfg0.RADIO_DIG_TX_CFG0_b.PA_MICRO = u8_PaMicro;
    adf7030_1__SPI_SetMem32(pSPIDevInfo, PROFILE_RADIO_DIG_TX_CFG0_Addr, tx_cfg0.RADIO_DIG_TX_CFG0);
    return ( (pSPIDevInfo->eXferResult)?(1):(0) );
}

/*!
 * @brief  This function get the fw module info (Name or Version)
 *
 * @param [in]  pSPIDevInfo Pointer to ADF7030-1 SPI device instance.
 * @param [out] p_Data      Pointer to the data buffer with device information.
 * @param [out] u8_Sz       Reference to the variable to hold the data size.
 * @param [in]  eInfo       Information type (FW_MODULE_VERSION or FW_MODULE_NAME).
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If the transfert was succesfull to the adf7030-1.
 *  - #ADF7030_1_INVALID_HANDLE  [D] If the given SPI ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the transfert failed.
 */
uint8_t adf7030_1__GetFwModuleInfo(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t *p_Data,
    uint8_t *u8_Sz,
    uint8_t eInfo
)
{
    uint8_t e_Ret;
    data_blck_desc_t sBlock;
    if (pSPIDevInfo == NULL) { return ADF7030_1_INVALID_OPERATION;}
    switch(eInfo){
    	case ADI_REF:
            sBlock.Addr = __ADI_REFERENCE_BASE;
            sBlock.Size = 10; //__ADI_REFERENCE_SIZE;
    		break;
        case FW_MODULE_VERSION:
            sBlock.Addr = __FIRMWARE_MODULE_VERSION_BASE;
            sBlock.Size = __FIRMWARE_MODULE_VERSION_SIZE;
            break;
        case FW_MODULE_NAME:
        default:
            sBlock.Addr = __FIRMWARE_MODULE_NAME_BASE;
            sBlock.Size = __FIRMWARE_MODULE_NAME_SIZE;
        break;
    }
    sBlock.WordXfer = 0;
    sBlock.pData = p_Data;
    e_Ret = adf7030_1__ReadDataBlock( pSPIDevInfo, &sBlock);
    *u8_Sz = sBlock.Size;
    return e_Ret;
}

void adf7030_1__ClrIrqStatus(
    adf7030_1_spi_info_t *pSPIDevInfo,
    adf7030_1_intpin_e    eIntPin
)
{
    uint8_t * pSPI_TX_BUFF;
    uint8_t * pSPI_RX_BUFF;
    /* Setup SPI tx and rx buffer address */
    pSPI_TX_BUFF = pSPIDevInfo->pSPI_TX_BUFF;
    pSPI_RX_BUFF = pSPIDevInfo->pSPI_RX_BUFF;
    // Clear (Write) irq status
    // The next 2 define are build as :
    // 2 LSB bytes : "cmd + offset" (relative to PNTR_IRQ_CTRL_ADDR
    // first byte :
    // CNM RNW BNR ANP LNS MPHTR
    //  0   0   1   1   0   100   0x34
#define IRQ0_WRITE 0x00003408
#define IRQ1_WRITE 0x0000340C
#define IRQx_PAD 0xFFFFFFFF
#if ( __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    *(uint32_t *)pSPI_TX_BUFF = (eIntPin)?(IRQ1_WRITE):(IRQ0_WRITE);
#else
    *(uint32_t *)pSPI_TX_BUFF = __htonl( (eIntPin)?(IRQ1_WRITE):(IRQ0_WRITE) );
#endif
    *( ((uint32_t *)pSPI_TX_BUFF)+1) = IRQx_PAD;
#undef IRQx_PAD
    adf7030_1__SPI_ReadWrite_Fast( pSPIDevInfo,
                                  pSPI_TX_BUFF + 2,
                                  pSPI_RX_BUFF + 2,
                                  4 );
    pSPIDevInfo->nStatus.VALUE = *(pSPI_RX_BUFF+3);
}

uint32_t adf7030_1__GetIrqStatus(
    adf7030_1_spi_info_t *pSPIDevInfo,
    adf7030_1_intpin_e    eIntPin
)
{
    uint8_t * pSPI_TX_BUFF;
    uint8_t * pSPI_RX_BUFF;
    uint32_t u32IrqStatus;
    /* Setup SPI tx and rx buffer address */
    pSPI_TX_BUFF = pSPIDevInfo->pSPI_TX_BUFF;
    pSPI_RX_BUFF = pSPIDevInfo->pSPI_RX_BUFF;
    // Read irq status
    // The next 2 define are build as :
    // 2 LSB bytes : "cmd + offset" (relative to PNTR_IRQ_CTRL_ADDR
    // first byte :
    // CNM RNW BNR ANP LNS MPHTR
    //  0   1   1   1   0   100   0x74
#define IRQ0_READ 0x007408FFUL
#define IRQ1_READ 0x00740CFFUL
#define IRQx_PAD 0xFFFFFFFFUL
#if ( __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    *(uint32_t *)pSPI_TX_BUFF = (eIntPin)?(IRQ1_READ):(IRQ0_READ);
#else
    *(uint32_t *)pSPI_TX_BUFF = __htonl( (eIntPin)?(IRQ1_READ):(IRQ0_READ) );
#endif
    *( ((uint32_t *)pSPI_TX_BUFF)+1) = IRQx_PAD;
#undef IRQx_PAD
    adf7030_1__SPI_ReadWrite_Fast( pSPIDevInfo,
                                  pSPI_TX_BUFF + 1,
                                  pSPI_RX_BUFF + 1,
                                  5 );
    pSPIDevInfo->nStatus.VALUE = *(pSPI_RX_BUFF+3);
    u32IrqStatus = (uint32_t)(*((uint16_t *)(pSPI_RX_BUFF+4)));
    return u32IrqStatus;
}

uint32_t adf7030_1__GetMiscFwStatus(
    adf7030_1_spi_info_t *pSPIDevInfo
)
{
    uint8_t * pSPI_TX_BUFF;
    uint8_t * pSPI_RX_BUFF;
    uint32_t  misc_fw;
    /* Setup SPI tx and rx buffer address */
    pSPI_TX_BUFF = pSPIDevInfo->pSPI_TX_BUFF;
    pSPI_RX_BUFF = pSPIDevInfo->pSPI_RX_BUFF;
    // update Phy State
#define MISC_FW_READ 0x00000078UL
#define MISC_FW_ADDR 0x400042B4UL
#define MISC_FW_PAD  0xFFFFFFFFUL
#if ( __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    *(uint32_t *)pSPI_TX_BUFF = MISC_FW_READ;
    *( ((uint32_t *)pSPI_TX_BUFF)+1) = MISC_FW_ADDR;
#else
    *(uint32_t *)pSPI_TX_BUFF = __htonl( MISC_FW_READ);
    *( ((uint32_t *)pSPI_TX_BUFF)+1) = __htonl( MISC_FW_ADDR);
#endif
    *( ((uint32_t *)pSPI_TX_BUFF)+2) = MISC_FW_PAD;
    adf7030_1__SPI_ReadWrite_Fast( pSPIDevInfo,
                                  pSPI_TX_BUFF + 3,
                                  pSPI_RX_BUFF + 3,
                                  11 );
    pSPIDevInfo->nStatus.VALUE = *(pSPI_RX_BUFF+9);

#if ( __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    misc_fw.FW = ( *((uint32_t *)(pSPI_RX_BUFF+10)) );
#else
    misc_fw = __ntohl( *((uint32_t *)(pSPI_RX_BUFF+10)) );
#endif
    return misc_fw;
}

/******************************************************************************/

/*!
 * @brief  This function setup the TX and RX packet buffer
 *
 * @param [in] pSPIDevInfo Pointer to ADF7030-1 SPI device instance.
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If the transfert was succesfull to the adf7030-1.
 *  - #ADF7030_1_INVALID_HANDLE  [D] If the given SPI ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the transfert failed.
 */
uint8_t adf7030_1__SetupBuff(
    adf7030_1_spi_info_t* pSPIDevInfo
)
{
    buff_cfg0_t buff_cfg_0;
    buff_cfg1_t buff_cfg_1;
    if (pSPIDevInfo == NULL) { return ADF7030_1_INVALID_OPERATION;}
    buff_cfg_0 = (buff_cfg0_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, GENERIC_PKT_BUFF_CFG0_Addr));
    buff_cfg_1 = (buff_cfg1_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, GENERIC_PKT_BUFF_CFG1_Addr));
    // TX buffer
    buff_cfg_0.BUFF_CFG0_b.PTR_TX_BASE = PTR_TRX_BASE_MIN;
    buff_cfg_1.BUFF_CFG1_b.TX_SIZE = 256;
    // RX buffer
    buff_cfg_0.BUFF_CFG0_b.PTR_RX_BASE = PTR_TRX_BASE_MIN + 256;
    buff_cfg_1.BUFF_CFG1_b.RX_SIZE = 256;

    adf7030_1__SPI_SetMem32(pSPIDevInfo, GENERIC_PKT_BUFF_CFG0_Addr, buff_cfg_0.BUFF_CFG0);
    adf7030_1__SPI_SetMem32(pSPIDevInfo, GENERIC_PKT_BUFF_CFG1_Addr, buff_cfg_1.BUFF_CFG1);
    return ( (pSPIDevInfo->eXferResult)?(1):(0) );
}

/*!
 * @brief  This function setup, enable/disable the low power mode.
 *
 * @param [in] pSPIDevInfo Pointer to ADF7030-1 SPI device instance.
 * @param [in] bFlag       Enable/disable the LPM
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If the transfert was succesfull to the adf7030-1.
 *  - #ADF7030_1_INVALID_HANDLE  [D] If the given SPI ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the transfert failed.
 */
uint8_t adf7030_1__SetupLPM(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t bFlag
)
{
    if (pSPIDevInfo == NULL) { return 1;}
#ifdef USE_ADF7030_RTC // FIXME
    lpm_cfg1_t lpm_cfg1;
#endif
    lpm_cfg0_t lpm_cfg0 = (lpm_cfg0_t)adf7030_1__SPI_GetMem32(pSPIDevInfo, PROFILE_LPM_CFG0_Addr);
    lpm_cfg0.LPM_CFG0_b.RETAIN_SRAM = bFlag;
    lpm_cfg0.LPM_CFG0_b.ENABLE = bFlag;
#ifdef USE_ADF7030_RTC // FIXME
    //lpm_cfg0.LPM_CFG0 |= 0x1 << 17;
    //lpm_cfg0.LPM_CFG0_b.RTC_LF_SRC_SEL = 0; // Internal RC oscillator 26kHz
    //lpm_cfg0.LPM_CFG0_b.RTC_RECONFIG_EN = 1; // reconfigure RTC on CFG_DEV
    //lpm_cfg0.LPM_CFG0_b.RTC_EN = bFlag; // Enable RTC
    //lpm_cfg1.LPM_CFG1 = 50000;
    //adf7030_1__SPI_SetMem32(pSPIDevInfo, PROFILE_LPM_CFG1_Addr, lpm_cfg1.LPM_CFG1);
#endif
    adf7030_1__SPI_SetMem32(pSPIDevInfo, PROFILE_LPM_CFG0_Addr, lpm_cfg0.LPM_CFG0);
    return ( (pSPIDevInfo->eXferResult)?(1):(0) );
}

/*!
 * @brief  This function setup the wake-up source.
 *
 * @param [in] pSPIDevInfo Pointer to ADF7030-1 SPI device instance.
 * @param [in] u8_WkUpSrc  The source allowed to wake-up the ADF7030
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If the transfert was succesfull to the adf7030-1.
 *  - #ADF7030_1_INVALID_HANDLE  [D] If the given SPI ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the transfert failed.
 */
uint8_t adf7030_1__SetupWakeSrc(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t u8_WkUpSrc
)
{
    wake_source_t wake_src;
    if (pSPIDevInfo == NULL) { return 1;}
    wake_src = (wake_source_t)adf7030_1__SPI_GetMem32(pSPIDevInfo, SM_CONFIG_WAKE_SOURCE_Addr);
    wake_src.WAKE_SOURCE_b.IRQ = (u8_WkUpSrc & WAKE_UP_CS) >> 1;
    wake_src.WAKE_SOURCE_b.EXT = u8_WkUpSrc & (WAKE_UP_IRQ0 | WAKE_UP_IRQ1);
#ifdef USE_ADF7030_RTC // FIXME
    wake_src.WAKE_SOURCE_b.EXT  |= WAKE_UP_RTC;
#endif
    adf7030_1__SPI_SetMem32(pSPIDevInfo, SM_CONFIG_WAKE_SOURCE_Addr, wake_src.WAKE_SOURCE);
    return ( (pSPIDevInfo->eXferResult)?(1):(0) );
}

/*!
 * @brief  This function setup the external PA and LNA control PHY pin.
 *
 * @param [in] pSPIDevInfo Pointer to ADF7030-1 SPI device instance.
 * @param [in] ePaPhyPin   PHY pin id used for external PA control
 * @param [in] ePaPhyPin   PHY pin id used for external LNA control
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If the transfert was succesfull to the adf7030-1.
 *  - #ADF7030_1_INVALID_HANDLE  [D] If the given SPI ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the transfert failed.
 */
uint8_t adf7030_1__SetupExtPaLna(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_gpio_pin_e  ePaPhyPin,
    adf7030_1_gpio_pin_e  eLnaPhyPin
)
{
    radio_dig_tx_cfg1_t radio_dig_tx_cfg1;
    if (pSPIDevInfo == NULL) { return 1;}

    radio_dig_tx_cfg1 = (radio_dig_tx_cfg1_t)adf7030_1__SPI_GetMem32(pSPIDevInfo, PROFILE_RADIO_DIG_TX_CFG1_Addr);
    if(ePaPhyPin != ADF7030_1_GPIO_NONE){
        radio_dig_tx_cfg1.RADIO_DIG_TX_CFG1_b.EXT_PA_FRAMING_EN = 1;
        radio_dig_tx_cfg1.RADIO_DIG_TX_CFG1_b.EXT_PA_PIN_SEL = ePaPhyPin;
    }
    else {
        radio_dig_tx_cfg1.RADIO_DIG_TX_CFG1_b.EXT_PA_FRAMING_EN = 0;
    }
    if(eLnaPhyPin != ADF7030_1_GPIO_NONE){
        radio_dig_tx_cfg1.RADIO_DIG_TX_CFG1_b.EXT_LNA_FRAMING_EN = 1;
        radio_dig_tx_cfg1.RADIO_DIG_TX_CFG1_b.EXT_LNA_PIN_SEL = eLnaPhyPin;
    }
    else {
        radio_dig_tx_cfg1.RADIO_DIG_TX_CFG1_b.EXT_LNA_FRAMING_EN = 0;
    }
    adf7030_1__SPI_SetMem32(pSPIDevInfo, PROFILE_RADIO_DIG_TX_CFG1_Addr, radio_dig_tx_cfg1.RADIO_DIG_TX_CFG1);
    return ( (pSPIDevInfo->eXferResult)?(1):(0) );
}
/******************************************************************************/

/*!
 * @brief  This function enable/disable iNTERRUP (Host GPIO and PHY iNTERRUPT pin).
 *
 * @param [in] pDevice Pointer to ADF7030-1 device instance.
 * @param [in] eIntPin The trigger id to use (1 or 2)
 * @param [in] u8Flag  Enable or disable the iNTERRUPT pin
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If successfully configured.
 *  - #ADF7030_1_INVALID_HANDLE  [D]  If the given ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_GPIO_DEV_FAILED If Host GPIO configuration failed.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the communication with Radio PHY failed.
 */
uint8_t adf7030_1_SetupInt(
    adf7030_1_device_t* const pDevice,
    adf7030_1_intpin_e        eIntPin,
    uint8_t                   u8Flag
)
{
    uint8_t eRet;
    adf7030_1_gpio_int_info_t * pIntGPIOInfo = &pDevice->IntGPIOInfo[eIntPin];
    pf_cb_t pFn;
    void* pCbParam;
    uint32_t u32PhyIrqMask;
    gpio_irq_trg_cond_e eSense;

    if (pDevice == NULL) { return 1;}

    eSense = GPIO_IRQ_RISING_EDGE;
    u32PhyIrqMask = pIntGPIOInfo->nIntMap;
    pFn = pIntGPIOInfo->pfIntCb;
    pCbParam = pIntGPIOInfo->pIntCbParam;

    if(pCbParam == NULL)
    {
        pCbParam = (void*)(pDevice);
    }
    // Disable if bFlag is 0
    if( u8Flag == 0 ){
        eSense = GPIO_IRQ_NONE_EDGE;
        u32PhyIrqMask = 0;
    }
    // Disable if pf cb is NULL
    if( pFn == NULL ){
        eSense = GPIO_IRQ_NONE_EDGE;
        u32PhyIrqMask = 0;
        pCbParam = NULL;
    }
    /* Configures the PHY interrupt 0 & 1 source mask register*/
    eRet = adf7030_1__IRQ_SetMap( pDevice, eIntPin, u32PhyIrqMask );
    if (!eRet) {
        eRet = adf7030_1__IRQ_ClrStatus( pDevice, eIntPin, 0xFFFFFFFF );
    }
    /* Setup Host GPIO irq 0 */
    if (!eRet) {
        eRet = adf7030_1_HostGPIOIrq_Init( pDevice, pIntGPIOInfo->u32Port, pIntGPIOInfo->u16Pin, eSense, pFn, pCbParam );
    }
    return eRet;
}

/*!
 * @brief  This function enable/disable Trigger (Host GPIO and PHY trigger pin).
 *
 * @param [in] pDevice    Pointer to ADF7030-1 device instance.
 * @param [in] eTrigPin   The trigger id to use (1 or 2)
 * @param [in] eTrigState The PHY command to execute on trigger
 * @param [in] u8Flag     Enable or disable the Trigger pin
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If successfully configured.
 *  - #ADF7030_1_INVALID_HANDLE  [D]  If the given ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_GPIO_DEV_FAILED If Host GPIO configuration failed.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the communication with Radio PHY failed.
 */
uint8_t adf7030_1_SetupTrig(
    adf7030_1_device_t* const pDevice,
    adf7030_1_trigpin_e       eTrigPin,
    adf7030_1_radio_state_e   eTrigState,
    uint8_t u8Flag
)
{
    uint8_t eRet;
    adf7030_1_radio_extended_e eTrig;
    adf7030_1_gpio_trig_info_t* pTrigGPIOInfo = &pDevice->TrigGPIOInfo[eTrigPin];

    eRet = adf7030_1_HostGPIOTrig_Init( pDevice, pTrigGPIOInfo->u32Port, pTrigGPIOInfo->u16Pin, u8Flag );
    if (!eRet) {
        pTrigGPIOInfo->eTrigStatus = u8Flag;
           if (u8Flag) {
               eRet = adf7030_1__TRIG_SetGPIOPin( pDevice, eTrigPin, eTrigState );
           }
    }
    if (!eRet)
    {
        eTrig = TRIGPIN_NONE;
        if(pDevice->TrigGPIOInfo[0].eTrigStatus == ENABLED )
        {
            eTrig |= 0b01;
        }
        if(pDevice->TrigGPIOInfo[1].eTrigStatus == ENABLED )
        {
            eTrig |= 0b10;
        }
        eRet = adf7030_1__TRIG_Enable( pDevice, eTrig );
    }
    return eRet;
}

/******************************************************************************/

/*!
 * @brief  This function Transmit a packet to PHY TX (Polling).
 *
 * @param [in] pDevice Pointer to ADF7030-1 device instance.
 * @param [in] p_Data  Pointer on data buffer to copy in the TX packet.
 * @param [in] u8_Sz   Variable that hold the size of the data buffer.
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If successfully initialized the Host GPIOs.
 *  - #ADF7030_1_INVALID_HANDLE  [D]  If the given ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the transfert failed.
 */
uint8_t adf7030_1__Send(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t *p_Data,
    uint8_t u8_Sz
)
{
    uint8_t e_Ret;
    if (pSPIDevInfo == NULL) { return 1;}
    e_Ret = adf7030_1__SetTxPacket( pSPIDevInfo, p_Data, u8_Sz );
    if (e_Ret ) { return e_Ret;}
    // Change state to TX
    e_Ret =  adf7030_1__STATE_PhyCMD( pSPIDevInfo, PHY_TX );
    if (e_Ret ) { return e_Ret;}
    // Wait TX ended
    e_Ret = adf7030_1__STATE_Poll_PhyState(pSPIDevInfo, PHY_ON, 0);
    return e_Ret;
}

/*!
 * @brief  This function Receive a packet from PHY RX (Polling).
 *
 * @param [in]  pDevice Pointer to ADF7030-1 device instance.
 * @param [out] p_Data  Pointer on buffer to copy in the RX packet.
 * @param [out] *u8_Sz  Reference variable to hold the size of the packet (with CRC).
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If successfully initialized the Host GPIOs.
 *  - #ADF7030_1_INVALID_HANDLE  [D]  If the given ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the transfert failed.
 */
uint8_t adf7030_1__Recv(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t *p_Data,
    uint8_t *u8_Sz
)
{
    uint8_t e_Ret;
    if (pSPIDevInfo == NULL) { return 1;}
    e_Ret = adf7030_1__STATE_PhyCMD( pSPIDevInfo, PHY_RX );
    if (e_Ret != ADF7030_1_SUCCESS) { return e_Ret;}
    e_Ret = adf7030_1__STATE_Poll_PhyState(pSPIDevInfo, PHY_RX, 0);
    e_Ret = adf7030_1__STATE_Poll_PhyState(pSPIDevInfo, PHY_ON, 0);
    if (e_Ret != ADF7030_1_SUCCESS) { return e_Ret;}
    e_Ret = adf7030_1__GetRxPacket(pSPIDevInfo, p_Data, u8_Sz);
    return e_Ret;
}

/*!
 * @brief  This function copy the given data buffer into the ADF7030 RX packet buffer.
 *
 * @param [in] pDevice Pointer to ADF7030-1 device instance.
 * @param [in] p_Data  Pointer on data buffer to copy in the TX packet.
 * @param [in] u8_Sz   Variable that hold the size of the data buffer.
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If successfully initialized the Host GPIOs.
 *  - #ADF7030_1_INVALID_HANDLE  [D]  If the given ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the transfert failed.
 */
uint8_t adf7030_1__SetTxPacket(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t *p_Data,
    uint8_t u8_Sz
)
{
    uint8_t e_Ret;
    data_blck_desc_t sBlock;
    buff_cfg0_t buff_cfg_0;
    if (pSPIDevInfo == NULL) { return 1;}
    buff_cfg_0 = (buff_cfg0_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, GENERIC_PKT_BUFF_CFG0_Addr));
    sBlock.Addr = PARAM_ADF7030_1_SRAM_BASE;
    sBlock.Addr |= buff_cfg_0.BUFF_CFG0_b.PTR_TX_BASE << 2;
    sBlock.pData = p_Data;
    sBlock.WordXfer = 0;
    sBlock.Size = u8_Sz;
    e_Ret = adf7030_1__WriteDataBlock( pSPIDevInfo, &sBlock);
    adf7030_1__WRITE_FIELD(GENERIC_PKT_FRAME_CFG1_PAYLOAD_SIZE, (uint32_t)u8_Sz );
    return e_Ret;
}

/*!
 * @brief  This function copy the ADF7030 RX buffer into the given buffer.
 *
 * @param [in]  pDevice Pointer to ADF7030-1 device instance.
 * @param [out] p_Data  Pointer on buffer to copy in the RX packet.
 * @param [out] *u8_Sz  Reference variable to hold the size of the packet (with CRC).
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If successfully initialized the Host GPIOs.
 *  - #ADF7030_1_INVALID_HANDLE  [D]  If the given ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the transfert failed.
 */
uint8_t adf7030_1__GetRxPacket(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t *p_Data,
    uint8_t *u8_Sz
)
{
    uint8_t e_Ret = 1;
    data_blck_desc_t sBlock;
    buff_cfg0_t buff_cfg_0;
    uint16_t frame_len;
    if (pSPIDevInfo == NULL) { return e_Ret;}

    frame_len = adf7030_1__READ_FIELD(GENERIC_PKT_FRAME_CFG3_RX_LENGTH);
    buff_cfg_0 = (buff_cfg0_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, GENERIC_PKT_BUFF_CFG0_Addr));
    sBlock.Addr = PARAM_ADF7030_1_SRAM_BASE;
    sBlock.Addr |= buff_cfg_0.BUFF_CFG0_b.PTR_RX_BASE << 2;
    sBlock.pData = p_Data;
    sBlock.WordXfer = 0;
    sBlock.Size = frame_len;
    /* Readback received frame data */
    e_Ret = adf7030_1__ReadDataBlock( pSPIDevInfo, &sBlock);
    *u8_Sz = (uint8_t)frame_len;
    return e_Ret;
}

/*!
 * @brief This function measure the noise
 *
 * @details The result is the average of NOISE_MEAS_AVG_NB valid measures.
 *          Measure is validated if the LIVE_STATUS of cca_readback register, equal 0.
 *
 * @param [in]  pDevice   Pointer to ADF7030-1 device instance.
 * @param [out] u16_Noise Reference variable to hold the measure.
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If successfully initialized the Host GPIOs.
 *  - #ADF7030_1_INVALID_HANDLE  [D]  If the given ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the transfert failed.
 */
uint8_t adf7030_1__MeasureNoise(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint16_t *u16_Noise
)
{
    uint8_t e_Ret = 1;
    if (pSPIDevInfo == NULL) { return e_Ret;}

    e_Ret = adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo, PHY_ON, PHY_ON );
    e_Ret = adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo, CCA, CCA );
    if ( e_Ret ) {
        return e_Ret;
    }

    *u16_Noise = adf7030_1__GetRawNoise( pSPIDevInfo, NOISE_MEAS_AVG_NB );
    return e_Ret;
}
/******************************************************************************/
/*!
 * @brief  This function synchronize the Host current state with the PHY state
 *
 * @param [in]  pDevice Pointer to ADF7030-1 device instance.
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS         If successfully initialized the Host GPIOs.
 *  - #ADF7030_1_INVALID_HANDLE  [D]  If the given ADF7030-1 device instance is invalid.
 *  - #ADF7030_1_SPI_COMM_FAILED [D] If the transfert failed.
 */
uint8_t adf7030_1_SynchState(
    adf7030_1_device_t* const pDevice
)
{
    uint8_t eRet = 1;
    uint32_t pAddrIn[3] = {IRQ_CTRL_STATUS0_Addr, IRQ_CTRL_STATUS1_Addr, MISC_FW_Addr};

    struct {
        irq_status_t irq_status[2];
        misc_fw_t    misc_fw;
    } synch_state_reg;

    adf7030_1_spi_info_t* pSPIDevInfo;
    if (pDevice != NULL)
    {
        pSPIDevInfo = &(pDevice->SPIInfo);
        if ( pSPIDevInfo != NULL) {
            eRet = adf7030_1__SPI_rd_word_r_a( pSPIDevInfo, pAddrIn, (uint32_t*)&synch_state_reg, 3 );
            //pDevice->IntGPIOInfo[0].nIntStatus = pDevice->IntGPIOInfo[0].nIntMap & synch_state_reg.irq_status[0].STATUS;
            //pDevice->IntGPIOInfo[1].nIntStatus = pDevice->IntGPIOInfo[1].nIntMap & synch_state_reg.irq_status[1].STATUS;
            pDevice->IntGPIOInfo[0].nIntStatus = synch_state_reg.irq_status[0].STATUS;
            pDevice->IntGPIOInfo[1].nIntStatus = synch_state_reg.irq_status[1].STATUS;

            pSPIDevInfo->ePhyError = synch_state_reg.misc_fw.FW_b.ERR_CODE;
            pSPIDevInfo->nPhyState = synch_state_reg.misc_fw.FW_b.CURR_STATE;
        }
    }
    return eRet;
}

/******************************************************************************/

/**
 * @brief Initialize the adf7030-1 instance.
 *
 * @details This function initialize the static information related to the host requirements.
 *
 * @param [in] pDevice       Pointer to ADF7030-1 device instance.
 * @param [in] pINTDevInfo   Pointer to ADF7030-1 Interrupt pin informations
 * @param [in] pTRIGDevInfo  Pointer to ADF7030-1 Trigger pin informations
 * @param [in] pRESETDevInfo Pointer to ADF7030-1 Reset pin informations
 * @param [in] eExtPaPin     Used ADF7030-1 pin as external PA control
 * @param [in] eExtLnaPin    Used ADF7030-1 pin as external LNA control
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS        If successfully initialized the Host GPIOs.
 *  - #ADF7030_1_INVALID_HANDLE [D]  If the given ADF7030-1 device instance is invalid.
 */
uint8_t adf7030_1_Setup(
    adf7030_1_device_t*          const pDevice,
    adf7030_1_gpio_int_info_t*   const pINTDevInfo,
    adf7030_1_gpio_trig_info_t*  const pTRIGDevInfo,
    adf7030_1_gpio_reset_info_t* const pRESETDevInfo,
    adf7030_1_gpio_pin_e        eExtPaPin,
    adf7030_1_gpio_pin_e        eExtLnaPin
)
{
    adf7030_1_spi_info_t* pSPIDevInfo = NULL;

    if ( pDevice != NULL) {
        pSPIDevInfo = &(pDevice->SPIInfo);
        pSPIDevInfo->hSPIDevice = NULL;
        pSPIDevInfo->eXferResult = ADF7030_1_SUCCESS;

        pSPIDevInfo->nStatus.VALUE = 0;
        pSPIDevInfo->nPhyState = PHY_SLEEP;
        pSPIDevInfo->nPhyNextState = PHY_SLEEP;

        pSPIDevInfo->nClkFreq = ADF7030_1_SPI_DEV_BITRATE;
        pSPIDevInfo->nClkFreq_Fast = ADF7030_1_SPI_DEV_BITRATE_FAST;
        pSPIDevInfo->nClkFreq_Current = 0;
#if (ADF7030_1_PHY_ERROR_REPORT_ENABLE == 1)
        pSPIDevInfo->ePhyError = SM_NOERROR;
        pSPIDevInfo->bPhyErrorCheck = 1;
#endif
        // don't modify the following register
        pSPIDevInfo->PHY_PNTR[PNTR_SETUP_ADDR]    = SPI_HOST_BASE;
    	pSPIDevInfo->PHY_PNTR[PNTR_SRAM_ADDR]     = BRAM_BASE; //SRAM_BASE;
    	pSPIDevInfo->PHY_PNTR[PNTR_MCR_LOW]       = MCR_BASE;
    	pSPIDevInfo->PHY_PNTR[PNTR_MCR_HIGH]      = 0x40004100;
    	pSPIDevInfo->PHY_PNTR[PNTR_IRQ_CTRL_ADDR] = IRQ_CTRL_BASE;
    	// Only 3 are available from PHY
    	pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR]  = 0x20000000;
    	pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM1_ADDR]  = 0x20000000;
    	pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM2_ADDR]  = 0x20000000;

        /* Set the SPI TX and RX buffer */
        pSPIDevInfo->pSPI_TX_BUFF = a_SpiTxBuf;
        pSPIDevInfo->pSPI_RX_BUFF = a_SpiRxBuf;
        /* Initialise underlying GPIO service information structure for IRQs */
        if (pINTDevInfo)
        {
            memcpy(
                    (void *)pDevice->IntGPIOInfo,
                    (void *)pINTDevInfo,
                    sizeof(adf7030_1_gpio_int_info_t) * ADF7030_1_NUM_INT_PIN
                    );
        }
        else {
            memset(
                    (void *)&pDevice->IntGPIOInfo,
                    0,
                    sizeof(adf7030_1_gpio_int_info_t) * ADF7030_1_NUM_INT_PIN
                    );
        }
        /* Initialise GPIO service information structure for Triggers pins */
        if(pTRIGDevInfo)
        {
            memcpy(
                    (void *)pDevice->TrigGPIOInfo,
                    (void *)pTRIGDevInfo,
                    sizeof(adf7030_1_gpio_trig_info_t) * ADF7030_1_NUM_TRIG_PIN
                    );
        }
        else {
            memset(
                    (void *)&pDevice->TrigGPIOInfo,
                    0,
                    sizeof(adf7030_1_gpio_trig_info_t) * ADF7030_1_NUM_TRIG_PIN
                    );
        }
        /* Initialise GPIO service information structure for the Reset pin */
        if(pRESETDevInfo)
        {
            memcpy(
                    (void *)&pDevice->ResetGPIOInfo,
                    (void *)pRESETDevInfo,
                    sizeof(adf7030_1_gpio_reset_info_t)
                    );
        }
        else {
            memset(
                    (void *)&pDevice->ResetGPIOInfo,
                    0,
                    sizeof(adf7030_1_gpio_reset_info_t)
                    );
        }

        pDevice->eExtPaPin = eExtPaPin;
        pDevice->eExtLnaPin = eExtLnaPin;
        /*Mark the device as opened */
        pDevice->eState = ADF7030_1_STATE_OPENED;
    }
    else {
        return 1;
    }
    return 0;
}

/**
 * @brief Initialize the adf7030-1 required Host part.
 *
 * @details This function setup the SPI, Interrupt, Trigger and Reset Host GPIOs
 *          Trigger pin.
 *
 * @param [in] pDevice     Pointer to ADF7030-1 device instance.
 * @param [in] u8SpiDevNum The Host SPI id used by the ADF7030-1 device instance.
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS           If successfully initialized the Host GPIOs.
 *  - #ADF7030_1_INVALID_HANDLE    [D]  If the given ADF7030-1 device handle is invalid.
 *  - #ADF7030_1_DEVICE_NOT_OPENED [D]  if the given device is not yet opened.
 *  - #ADF7030_1_SPI_DEV_FAILED    Failed to setup GPIO service.
 *  - #ADF7030_1_GPIO_DEV_FAILED   Failed to setup SPI driver.
 * @sa adf7030_1_Init()
 */
uint8_t adf7030_1_Init(
    adf7030_1_device_t* const pDevice,
	spi_dev_t         * pSpiDev
)
{
    adf7030_1_spi_info_t* pSPIDevInfo = NULL;
    uint8_t u8i;

    if ( pDevice == NULL) { return 1; }

    if (pDevice->eState & ADF7030_1_STATE_OPENED) {
        pSPIDevInfo = &(pDevice->SPIInfo);
        /* Set the SPI device number */

        pSPIDevInfo->hSPIDevice = (void*)pSpiDev;
        //pSPIDevInfo->nDeviceNum = u8SpiDevNum;
        /* Call the SPI driver initialization routine */
        if ( adf7030_1_HostSPI_Init(pDevice) )
        {
        	return 1;
        }

        /* Call the Host GPIO service initialization routine for each IRQs*/
        adf7030_1_gpio_int_info_t* pIntGPIOInfo = pDevice->IntGPIOInfo;
        for (u8i = 0; u8i < ADF7030_1_NUM_INT_PIN; u8i++)
        {
            if (pIntGPIOInfo[u8i].u32Port != 0)
            {
                if( adf7030_1_HostGPIOIrq_Init(
                                                        pDevice,
                                                        pIntGPIOInfo[u8i].u32Port,
                                                        pIntGPIOInfo[u8i].u16Pin,
                                                        GPIO_IRQ_RISING_EDGE,
                                                        NULL,
                                                        NULL )
                                                        )
                {
                    return 1;
                }
            }
        }

        /* Call the Host GPIO service initialization routine for each triggers*/
        adf7030_1_gpio_trig_info_t* pTrigGPIOInfo = pDevice->TrigGPIOInfo;
        for (u8i = 0; u8i < ADF7030_1_NUM_TRIG_PIN; u8i++)
        {
            if (pTrigGPIOInfo[u8i].u32Port !=0 )
            {
                if( adf7030_1_HostGPIOTrig_Init(
                                                        pDevice,
                                                        pTrigGPIOInfo[u8i].u32Port,
                                                        pTrigGPIOInfo[u8i].u16Pin,
                                                        0)
                                                        )
                {
                    return 1;
                }
            }
        }

        adf7030_1_gpio_reset_info_t * pResetGPIOInfo = &pDevice->ResetGPIOInfo;
        if(pResetGPIOInfo->u32Port != 0 && pResetGPIOInfo->u16Pin !=0 )
        {
            /* Setup Host GPIO high */
            if(BSP_Gpio_SetHigh(
                    pResetGPIOInfo->u32Port,
                    pResetGPIOInfo->u16Pin)
                    != DEV_SUCCESS)
            {
                return 1;
            }
            /* Setup Host GPIO as output */
            if(BSP_Gpio_OutputEnable(
                    pResetGPIOInfo->u32Port,
                    pResetGPIOInfo->u16Pin,
                    1)
                    != DEV_SUCCESS)
            {
                return 1;
            }
            /* Toggle the Reset pin the the PHY Radio */
            //if(( adf7030_1_PulseReset(pDevice)) )
            //{
            //    return 1;
            //}
        }
        pDevice->eState |= ADF7030_1_STATE_INITIALIZED;
    }
    else {
        return 1;
    }
    return 0;
}

/**
 * @brief Uninitialize the adf7030-1 instance.
 *
 * @details This function clean the previously configured Host Interrupt and
 *          Trigger GPIOs.
 *
 * @param [in] pDevice Pointer to ADF7030-1 device instance.
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS           If successfully uninitialized the Host GPIOs.
 *  - #ADF7030_1_INVALID_HANDLE    [D] If the given ADF7030-1 device handle is invalid.
 *  - #ADF7030_1_DEVICE_NOT_OPENED [D] if the given device is not yet opened.
 *  - #ADF7030_1_SPI_DEV_FAILED    Failed to terminate GPIO service.
 *  - #ADF7030_1_GPIO_DEV_FAILED   Failed to terminate SPI driver.
 * @sa adf7030_1_Init()
 */
uint8_t adf7030_1_UnInit(
    adf7030_1_device_t*  const pDevice
)
{
    if ( pDevice == NULL) { return 1; }
    uint8_t u8i;
    /* Unitialize IRQ service */
    adf7030_1_gpio_int_info_t* pIntGPIOInfo = pDevice->IntGPIOInfo;
    for (u8i = 0; u8i < ADF7030_1_NUM_INT_PIN; u8i++)
    {
        if (pIntGPIOInfo[u8i].u32Port != 0)
        {
            if( adf7030_1_HostGPIOIrq_Init(
                                                    pDevice,
                                                    pIntGPIOInfo[u8i].u32Port,
                                                    pIntGPIOInfo[u8i].u16Pin,
                                                    GPIO_IRQ_NONE_EDGE,
                                                    NULL,
                                                    NULL)
                                                    )
            {
                return 1;
            }
        }
    }
    /* Unitialize TRIG service */
    adf7030_1_gpio_trig_info_t* pTrigGPIOInfo = pDevice->TrigGPIOInfo;
    for (u8i = 0; u8i < ADF7030_1_NUM_TRIG_PIN; u8i++)
    {
        if (pTrigGPIOInfo[u8i].u32Port !=0 )
        {
            if( adf7030_1_HostGPIOTrig_Init(
                                             pDevice,
                                             pTrigGPIOInfo[u8i].u32Port,
                                             pTrigGPIOInfo[u8i].u16Pin,
                                             0)
                                            )
            {
                return 1;
            }
        }
    }
    /* Unitialize SPI driver */
    if( adf7030_1_HostSPI_UnInit(pDevice) )
    {
        return 1;
    }
    pDevice->eState &= ~ADF7030_1_STATE_INITIALIZED;
    return 0;
}

/**
 * @brief Configure the adf7030-1 hardware device.
 *
 * @note This function configure the PHY device by sending the given reference
 *       configuration.
 *
 * @param [in] pDevice    Pointer to ADF3030-1 device instance.
 * @param [in] pCfg       Pointer to the reference configuration
 * @param [in] u32CfgSize The size of the given configuration
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS             If PHY instance was successfully setup
 *  - #ADF7030_1_INVALID_HANDLE      [D] If PHY instance does not exist
 *  - #ADF7030_1_INVALID_OPERATION   [D] If PHY instance was not previously initialized
 *  - #ADF7030_1_SPI_DEV_POLL_EXPIRE [D] If the Radio PHY failed to transition to new state.
 *  - #ADF7030_1_SPI_COMM_FAILED     [D] If the communication with Radio PHY failed.
*/
uint8_t adf7030_1_Configure(
    adf7030_1_device_t* const pDevice,
    uint8_t const *     pCfg,
    uint32_t            u32CfgSize
)
{
    if ( pDevice == NULL) { return 1; }
    /* Pointer to the ADF7030-1 instance SPI information */
    adf7030_1_spi_info_t * pSPIDevInfo = &pDevice->SPIInfo;

#if (ADF7030_1_PHY_ERROR_REPORT_ENABLE == 1)
    /* Disable automatic PhyError reporting by default */
    pSPIDevInfo->bPhyErrorCheck = false;
#endif

    if (pDevice->eState & ADF7030_1_STATE_INITIALIZED )
    {
        /* Wake up the PHY Radio and wait until we are in PHY_OFF */
        if( adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo,
                                                         PHY_OFF,
                                                         PHY_OFF )
                                                       )
        {
            return 1;
        }
        /* Send the Default usecase configuration to the part if specified */
        if((pCfg != NULL) && (u32CfgSize != 0UL))
        {
            pDevice->eState &= ~ADF7030_1_STATE_CONFIGURED;
            if( adf7030_1__SendConfiguration( pSPIDevInfo,
                                                        (uint8_t *)pCfg,
                                                        u32CfgSize ) )
            {
                return 1;
            }
            if( adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo,
                                                             CFG_DEV,
                                                             PHY_OFF )
                                                             )
            {
                return 1;
            }
            pDevice->eState |= ADF7030_1_STATE_CONFIGURED;
        }
    }
    else {
        return 1;
    }
    return 0;
}

/**
 * @brief Enable the adf7030-1.
 *
 * @note This function initialize the adf7030 HW device by applying a HW Reset
 *       then configure it with the given configuration if any.
 *
 * @param [in] pDevice    Pointer to ADF3030-1 device instance.
 * @param [in] pCfg       Pointer to the reference configuration
 * @param [in] u32CfgSize The size of the given configuration
 *
 * @return      Status
 *  - #0      If PHY instance was successfully setup
 *  - #1      [D] If PHY instance does not exist or
 *            [D] If PHY instance was not previously initialized or
 *            [D] If the Radio PHY failed to transition to new state or
 *            [D] If the communication with Radio PHY failed or
 *            [D] If PHY HW Reset (Host GPIO) failed
*/
uint8_t adf7030_1_Enable(
    adf7030_1_device_t* const pDevice,
    uint8_t const *     pStartupCfg,
    uint32_t            u32_StartupCfgSize
)
{
    if ( pDevice == NULL) { return 1; }
#if (ADF7030_1_PHY_ERROR_REPORT_ENABLE == 0)
    /* Disable automatic PhyError reporting by default */
    pSPIDevInfo->bPhyErrorCheck = false;
#endif
    if(( adf7030_1_PulseReset(pDevice)) )
    {
        return 1;
    }
    /* Configure the phy hw device */
    return adf7030_1_Configure(pDevice, pStartupCfg, u32_StartupCfgSize);
}

/**
 * @brief Disable the adf7030-1.
 *
 * @param [in] pDevice    Pointer to ADF3030-1 device instance.
 *
 * @return      Status
 *  - #0      If PHY instance was successfully setup
 *  - #1      [D] If PHY instance does not exist or if the Radio PHY failed to
 *            transition to new state or if the communication with Radio PHY failed.
*/
uint8_t adf7030_1_Disable(
    adf7030_1_device_t* const pDevice
)
{
    if ( pDevice == NULL) { return 1; }
    /* Pointer to the ADF7030-1 instance SPI information */
    adf7030_1_spi_info_t * pSPIDevInfo = &pDevice->SPIInfo;
    /* ADF7030-1 result code */
    uint8_t u8i;

#if (ADF7030_1_PHY_ERROR_REPORT_ENABLE == 1)
    /* Disable automatic PhyError reporting by default */
    pSPIDevInfo->bPhyErrorCheck = false;
#endif
    /* Place holder for turning off the adf7030-1 PHY */
    for (u8i = 0; u8i < ADF7030_1_NUM_INT_PIN; u8i++)
    {
        /* Clear interrupt enable register for IRQs */
        if(adf7030_1__IRQ_ClrAllStatus(pDevice, u8i) )
        {
            return 1;
        }
    }
    /* Reset the PHY Radio on exit */
    if(( adf7030_1__STATE_PhyShutdown(pSPIDevInfo)) )
    {
        return 1;
    }
    return 0;
}


/******************************************************************************/

/**
 * @brief Initialize the Host GPIO Interrupt pin to interface with the PHY Radio
 *
 * @param [in] pDevice    Pointer to ADF3030-1 device instance.
 * @param [in] u32Port    Host port address used as interrupt.
 * @param [in] u16Pin     Host pin used as interrupt
 * @param [in] eSense     Trigger condition for GPIO interrupt.
 * @param [in] pfCallback Callback function which will be called upon interrupt detection.
 * @param [in] pCbParam   Callback parameter which will be passed to interrupt handler.
 *
 * @return      Status
 *  - #0    If Host GPIO has been configured for trigger functionality
 *  - #1    If Host port or pin are not valid or if Host GPIO configuration failed
 */
uint8_t adf7030_1_HostGPIOIrq_Init(
    adf7030_1_device_t* const pDevice,
    uint32_t            u32Port,
    uint16_t            u16Pin,
    gpio_irq_trg_cond_e eSense,
    pf_cb_t             pfCallback,
    void*               const pCbParam
)
{
    uint8_t bFlag = 1;
    void* p_CBParam = pCbParam;
    if ( (p_CBParam == NULL) && (pfCallback != NULL) ) {
        p_CBParam = pDevice;
    }
    // If pfCallback == NULL or eSense == GPIO_IRQ_NONE_EDGE disable IT line
    if (pfCallback == NULL || eSense == GPIO_IRQ_NONE_EDGE)
    {
        bFlag = 0;
    }
    if (u32Port && u16Pin)
    {
         /* Setup the GPIO pin direction to input */
        if(BSP_Gpio_InputEnable(u32Port, u16Pin, bFlag) != DEV_SUCCESS)
        {
            return 1;
        }
        /* Setup the GPIO pin IT sensitivity  */
        if(BSP_GpioIt_ConfigLine(u32Port, u16Pin, eSense ) != DEV_SUCCESS)
        {
            return 1;
        }
        /* Enable the GPIO pin IT line  */
        if(BSP_GpioIt_SetLine(u32Port, u16Pin, bFlag) != DEV_SUCCESS)
        {
            return 1;
        }
        /* Setup the GPIO pin IT callback */
        if(BSP_GpioIt_SetCallback( u32Port, u16Pin, pfCallback, p_CBParam) != DEV_SUCCESS)
        {
            return 1;
        }
        return 0;
    }
    return 1;
}

/**
 * @brief Initialize the Host GPIO Trigger pin to interface with the PHY Radio
 *
 * @param [in] pDevice Pointer to ADF3030-1 device instance.
 * @param [in] u32Port Host port address used as trigger.
 * @param [in] u16Pin  Host pin used as trigger.
 * @param [in] bEnable PHY Trigger PIN to configure.
 *
 * @return      Status
 *  - #0     If Host GPIO has been configured for trigger functionality
 *  - #1     If Host port or pin are not valid or if Host GPIO configuration failed
*/
uint8_t adf7030_1_HostGPIOTrig_Init(
    adf7030_1_device_t* const pDevice,
    uint32_t            u32Port,
    uint16_t            u16Pin,
    uint8_t             bEnable
)
{
    /* Setup Host GPIO for trigger */
    if (u32Port && u16Pin)
    {
        if(BSP_Gpio_SetLow(u32Port, u16Pin) != DEV_SUCCESS)
        {
            return 1;
        }

        if(BSP_Gpio_OutputEnable(u32Port, u16Pin, bEnable) != DEV_SUCCESS)
        {
            return 1;
        }
        return 0;
    }
    return 1;
}

/******************************************************************************/

/**
 * @brief Initializes SPI device for communication with the adf7030-1 PHY
 *
 * @note This function open and configure the SPI peripheral in "blocking mode",
 *       (no DMA transfer assistance).
 *
 * @param [in] pDevice Pointer to ADF3030-1 device instance.
 *
 * @return      Status
 *  - #0    If SPI peripheral was successfully initialize
 *  - #1    [D] SPI peripheral failed to initialize or if the given device
 *          instance pointer is NULL
 *
 * @sa  adf7030_1_HostSPI_UnInit().
 */
uint8_t adf7030_1_HostSPI_Init(
    adf7030_1_device_t* const pDevice
)
{
    if ( pDevice == NULL) {
        return 1;
    }

    /* Pointer to the ADF7030-1 instance SPI information */
    adf7030_1_spi_info_t *  pSPIDevInfo = &pDevice->SPIInfo;
    /* Setup pointer to parent device information in opened SPI device info*/
    pSPIDevInfo->hDevInfo = (void *)pDevice;

    /* Handle to SPI device */
    p_spi_dev_t hSPIDevice;
//#warning "B: TO REWORK"
    if( BSP_Spi_Open( (pSPIDevInfo->hSPIDevice) ) != DEV_SUCCESS )
    {
        return 1;
    }

    /* Save SPI device handle into ADF7030-1 instance SPI information */
    //pSPIDevInfo->hSPIDevice = hSPIDevice;

    /* FIXME : Set the default SPI clock rate */
    if(adf7030_1__SPI_SetSpeed(pSPIDevInfo, DEFAULT_SPI_RATE))
    {
        return 1;
    }
    return 0;
}

/**
 * @brief Uninitializes SPI device used to communication with the adf7030-1 PHY
 *
 * @note This function release and deallocate memory used by the SPI peripheral
 *
 * @param [in] pDevice Pointer to ADF3030-1 device instance.
 *
 * @return      Status
 *  - #0     If SPI peripheral was successfully released
 *  - #1     [D] SPI peripheral failed to release or if the given device
 *           instance pointer is NULL
 *
 * @sa  adf7030_1_HostSPI_Init().
 */
uint8_t adf7030_1_HostSPI_UnInit(
    adf7030_1_device_t* const pDevice
)
{
    if ( pDevice == NULL) {
        return 1;
    }
    /* Handle to SPI device */
    p_spi_dev_t hSPIDevice = pDevice->SPIInfo.hSPIDevice;
    /* Close the SPI Device */
    if(BSP_Spi_Close(hSPIDevice) != DEV_SUCCESS)
    {
        return 1;
    }
    /* Setup pointer to parent device information in opened SPI device info*/
    pDevice->SPIInfo.hDevInfo = NULL;
    return 0;
}


#ifdef __cplusplus
}
#endif
