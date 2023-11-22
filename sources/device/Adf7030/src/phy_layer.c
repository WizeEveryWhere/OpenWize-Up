/**
  * @file phy_layer.c
  * @brief This file implement the phy device driver
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
  * @par 1.0.0 : 2020/05/15[GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup phy_layer
 * @ingroup device
 * @{
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <bsp.h>
#include <bsp_pwrlines.h>
#include "phy_layer_private.h"
#include "adf7030-1_phy.h"

#include "adf7030-1__state.h"
#include "adf7030-1__mem.h"
#include "adf7030-1__irq.h"
#include "adf7030-1__spi.h"
#include "adf7030-1__cfg.h"
#include "adf7030-1__patch.h"
#include "adf7030-1__gpio.h"

#include "adf7030-1_phy_fw.h"
#include "adf7030-1_phy_hw.h"

#include "adf7030-1_reg.h"

#include "adf7030-1_phy_conv.h"
#include "adf7030-1_phy_log.h"

/*!
 * @cond INTERNAL
 * @{
 */

/*!
 * @}
 * @endcond
 */

// The WM6400 modulation is not natively supported by the ADF7030 and must works in Raw Mode

/*!
 * @brief This convenient table hold the human channel representation
 */
const char * const aChanStr[PHY_NB_CH] = {
    [PHY_CH100] = "100", /*!< */
    [PHY_CH110] = "110", /*!< */
    [PHY_CH120] = "120", /*!< */
    [PHY_CH130] = "130", /*!< */
    [PHY_CH140] = "140", /*!< */
    [PHY_CH150] = "150", /*!< */
};

/*!
 * @brief This convenient table hold the human modulation representation
 */
const char * const aModulationStr[PHY_NB_MOD] = {
    [PHY_WM2400] = "WM2400", /*!< */
    [PHY_WM4800] = "WM4800", /*!< */
    [PHY_WM6400] = "WM6400", /*!< */
};

/*!
 * @brief This convenient table hold the human test mode representation
 */
const char * const aTestModeStr[PHY_NB_TST_MODE] = {
    [PHY_TST_MODE_NONE]   = "Disable",
    [PHY_TST_MODE_RX]     = "RX",
    [PHY_TST_MODE_PER_RX] = "RX PER",
	[PHY_TST_MODE_TX]     = "TX",
};

/*!
 * @brief This convenient table hold the human TX test mode representation
 */
const char * const aTestModeTXStr[TMODE_TX_NB] = {
    [TMODE_TX_NONE]        = "Disable",
    [TMODE_TX_CARRIER]     = "CARRIER",
    [TMODE_TX_FDEV_NEG]    = "-FDEV",
	[TMODE_TX_FDEV_MAXNEG] = "-FDEV max",
	[TMODE_TX_FDEV_POS]    = "+FDEV",
	[TMODE_TX_FDEV_MAXPOS] = "+FDEV max",
	[TMODE_TX_ZERO_ONE]    = "0-1",
	[TMODE_TX_PN9]         = "PN9",
};

/*!
 * @brief This convenient table hold the human test mode representation
 */
const char * const aPhyPwrStr[PHY_NB_PWR] = {
    [PHY_PMAX_minus_0db]  = "Pmax -0db",
    [PHY_PMAX_minus_6db]  = "Pmax -6db",
    [PHY_PMAX_minus_12db] = "Pmax -12db",
};

/*!
 * @brief This table hold the Transmission Power setup
 */
phy_power_t aPhyPower[PHY_NB_PWR] __attribute__(( weak )) =
{
	[PHY_PMAX_minus_0db]  = {.coarse = 6, .fine = 20, .micro = 0}, //   0 dBm
	[PHY_PMAX_minus_6db]  = {.coarse = 6, .fine =  6, .micro = 0}, //  -6 dBm
	[PHY_PMAX_minus_12db] = {.coarse = 6, .fine =  3, .micro = 0}, // -12 dBm
};

#ifdef PHY_USE_POWER_RAMP
pa_ramp_rate_e ePaRampRate __attribute__(( weak )) = RAMP_OFF;
#endif

/*!
 * @brief This variable hold the PA state
 */
uint8_t bPaState __attribute__(( weak )) = 0;

/*!
 * @brief This variable hold the RSSI offset after its calibration
 */
int16_t i16RssiOffsetCal __attribute__(( weak )) = 0;

/*!
 * @cond INTERNAL
 * @{
 */
#define DEFAULT_CAL_CFG  ( 0 \
		| ANAFILT_RC_CAL \
		| ADC_NOTCH_CAL \
		| ANA_QEC_CAL \
		| DIG_QEC_CAL \
		| ANCPLL_CAL \
		| LF_RC_CAL \
		| HF_RC_CAL \
		| VCO_CAL \
		| VCO_KV_CAL \
		| TEMP_XTAL_CAL \
		| HF_XTAL \
		)
/*!
 * @}
 * @endcond
 */

/*!
 * @brief This table hold the WM2400 modulation configuration
 */
static const uint8_t RF_CFG_WM2400[] = {
    #include "WM2400_small.cfg"
};

/*!
 * @brief This table hold the WM4800 modulation configuration
 */
static const uint8_t RF_CFG_WM4800[] = {
    #include "WM4800_small.cfg"
};

/*!
 * @brief This table hold the WM6400 modulation configuration
 */
static const uint8_t RF_CFG_WM6400[] = {
    #include "WM6400_small.cfg"
};

/*!
 * @brief This table hold the calibration configuration
 */
static const uint8_t RF_CAL_CFG[] = {
	#include "OfflineCalibrations.cfg"
};

/*!
 * @brief This table hold the basis configuration
 */
static const uint8_t RF_BASE_CFG[] = {
    #include "WM_base.cfg"
};

/*!
 * @brief This table hold the radio calibration result
 */
static uint8_t RF_RADIO_CAL[RADIO_CAL_SZ];

/*!
 * @brief This table hold the vco calibration result
 */
static uint8_t RF_VCO_CAL[VCO_CAL_SZ];

/*!
 * @brief This table hidden rf config
 */
static const uint8_t RF_HIDDEN[] = {
/* @40003E04 */
0x00, 0x00, 0x18, 0x38, 0x40, 0x00, 0x3E, 0x04,
0x00, 0x00, 0x00, 0xC0,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x3A, 0x9D,
};
/*!
 * @brief This structure describe one modulation configuration
 */
typedef struct {
    uint8_t const*  cf;
    uint16_t size;
}cfg_t;

/*!
 * @cond INTERNAL
 * @{
 */

#define PHY_BASE_CFG (PHY_NB_MOD)

#define PHY_RADIO_CAL (PHY_BASE_CFG +1)
#define PHY_VCO_CAL   (PHY_RADIO_CAL +1)
#define PHY_CAL_CFG   (PHY_VCO_CAL +1)
#define PHY_HIDDEN    (PHY_CAL_CFG +1)
#define PHY_NB_CFG    (PHY_HIDDEN +1)

/*!
 * @}
 * @endcond
 */

/*!
 * @brief Convenient macro to setup the table of modulation configuration
 */
#define RF_CFG_SETUP(name) { .cf = name, .size = sizeof(name) }

/*!
 * @brief This table hold the modulation configuration for the Phy device
 */
static const cfg_t RF_CFG[PHY_NB_CFG] = {
    [PHY_WM2400]    = RF_CFG_SETUP( RF_CFG_WM2400 ),
    [PHY_WM4800]    = RF_CFG_SETUP( RF_CFG_WM4800 ),
    [PHY_WM6400]    = RF_CFG_SETUP( RF_CFG_WM6400 ),
	// hidden config
	[PHY_BASE_CFG]  = RF_CFG_SETUP( RF_BASE_CFG ),
	//
	[PHY_RADIO_CAL] = RF_CFG_SETUP( RF_RADIO_CAL ),
	[PHY_VCO_CAL]   = RF_CFG_SETUP( RF_VCO_CAL ),
	[PHY_CAL_CFG]   = RF_CFG_SETUP( RF_CAL_CFG ),
	[PHY_HIDDEN]    = RF_CFG_SETUP( RF_HIDDEN ),
};
#undef RF_CFG_SETUP

//#define PHY_DEBUG_SPE
#ifdef PHY_DEBUG_SPE
profile_t       sProfile;
generic_pkt_t   sPacket;
mem_cfg_desc_t sConfig =
{
	.pNext = NULL,
	.nbBlock = 2,
	.BLOCKS[0] = {
			.Addr = PROFILE_BASE,
			.Size = sizeof(profile_t),
			.pData = (uint8_t*)(&sProfile)
	},
	.BLOCKS[1] = {
			.Addr = GENERIC_PKT_BASE,
			.Size = sizeof(generic_pkt_t),
			.pData = (uint8_t*)(&sPacket)
	},
};
#endif

// Private function (mapped to interface)
static int32_t _init(phydev_t *pPhydev);
static int32_t _uninit(phydev_t *pPhydev);

//static int32_t _do_command(phydev_t *pPhydev, phy_ctl_e eCmd);
static int32_t _do_TX(phydev_t *pPhydev, phy_chan_e eChannel, phy_mod_e eModulation);
static int32_t _do_RX(phydev_t *pPhydev, phy_chan_e eChannel, phy_mod_e eModulation);
static int32_t _do_CCA(phydev_t *pPhydev, phy_chan_e eChannel, phy_mod_e eModulation);

static int32_t _set_send(phydev_t *pPhydev, uint8_t *pBuf, uint8_t u8Len);
static int32_t _get_recv(phydev_t *pPhydev, uint8_t *pBuf, uint8_t *u8Len);

static int32_t _ioctl(phydev_t *pPhydev, uint32_t eCtl, uint32_t args);

/*!
 * @brief This structure hold the Phy device interface
 */
static const phy_if_t _phy_if = {
    .pfInit          = _init,
    .pfUnInit        = _uninit,

	.pfTx            = _do_TX,
    .pfRx            = _do_RX,
	.pfNoise         = _do_CCA,

	.pfSetSend       = _set_send,
	.pfGetRecv       = _get_recv,

	.pfIoctl         = _ioctl
};

extern spi_dev_t spi_ADF7030;

/*!
 * @brief  This function prepare the Phy device with constant configuration
 *
 * @param [in] pPhydev       Pointer on the Phy device instance
 * @param [in] pCtx          Pointer on the adf7030 context
 * @param [in] pINTDevInfo   Pointer on the adf7030 interrupt configuration
 * @param [in] pTRIGDevInfo  Pointer on the adf7030 trigger configuration
 * @param [in] pRESETDevInfo Pointer on the adf7030 reset configuration
 * @param [in] eExtPaPin     Gpio pin use as external PA control (if any)
 * @param [in] eExtLnaPin    Gpio pin use as external LNA control (if any)
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
int32_t Phy_adf7030_setup(
    phydev_t *pPhydev,
    adf7030_1_device_t *pCtx,
    adf7030_1_gpio_int_info_t*   const pINTDevInfo,
    adf7030_1_gpio_trig_info_t*  const pTRIGDevInfo,
    adf7030_1_gpio_reset_info_t* const pRESETDevInfo,
    adf7030_1_gpio_pin_e         eExtPaPin,
    adf7030_1_gpio_pin_e         eExtLnaPin
    )
{
    int32_t i32Ret = PHY_STATUS_ERROR;
    if (pPhydev && pCtx)
    {
        pPhydev->pIf = &_phy_if;
        pPhydev->pCxt = pCtx;
        if ( !(adf7030_1_Setup(
                pCtx,
                pINTDevInfo,
                pTRIGDevInfo,
                pRESETDevInfo,
                eExtPaPin,
                eExtLnaPin
                )))
        {

            /* Set to default the SPI struct */
        	//BSP_Spi_SetDefault(pCtx->SPIInfo.hSPIDevice);
        	BSP_Spi_SetDefault(&spi_ADF7030);
        	/* Init the SPI peripheral */
        	if ( !(BSP_Spi_Init(&spi_ADF7030)) )
        	//if ( !(BSP_Spi_Init(pCtx->SPIInfo.hSPIDevice)) )
            {
            	i32Ret = PHY_STATUS_OK;
            }
        }
    }
    return i32Ret;
}

/*!
 * @brief  This function Get the radio and vco calibration data
 *
 * @param [in]  pBuf Pointer to write in the calibration data
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
int32_t Phy_GetCal(uint8_t *pBuf)
{
	int32_t eStatus = PHY_STATUS_ERROR;
    uint8_t *p = pBuf;
    if (p)
    {
    	if ( *(uint64_t*)(RF_CFG[PHY_RADIO_CAL].cf) == RADIO_CAL_HEADER_BE)
    	{
			// Get RADIO Calibration from local buffer
			memcpy(p, RF_CFG[PHY_RADIO_CAL].cf, RF_CFG[PHY_RADIO_CAL].size );
			p += RF_CFG[PHY_RADIO_CAL].size;

	    	if ( *(uint64_t*)(RF_CFG[PHY_RADIO_CAL].cf) == VCO_CAL_HEADER_BE)
	    	{
				// Get VCO Calibration from local buffer
				memcpy(p, RF_CFG[PHY_VCO_CAL].cf, RF_CFG[PHY_VCO_CAL].size );
				eStatus = PHY_STATUS_OK;
	    	}
    	}
    }
	return eStatus;
}

/*!
 * @brief  This function Set the radio and vco calibration data
 *
 * @param [in]  pBuf Pointer on the calibration data to set
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
int32_t Phy_SetCal(uint8_t *pBuf)
{
	int32_t eStatus = PHY_STATUS_ERROR;
    uint8_t *p = pBuf;
    if (p)
    {
    	// check if radio calibration data is (seems) valid
    	if ( *(uint64_t*)(p) == RADIO_CAL_HEADER_BE )
    	{
    		// Set RADIO Calibration to local buffer
    		memcpy(RF_CFG[PHY_RADIO_CAL].cf, p, RF_CFG[PHY_RADIO_CAL].size );

    		p += RF_CFG[PHY_RADIO_CAL].size;

    		// check if vco calibration data is (seems) valid
        	if ( *(uint64_t*)(p) == VCO_CAL_HEADER_BE )
        	{
        		// Set VCO Calibration to local buffer
        		memcpy(RF_CFG[PHY_VCO_CAL].cf, p, RF_CFG[PHY_VCO_CAL].size );
        		eStatus = PHY_STATUS_OK;
        	}
    	}
    }
	return eStatus;
}

/*!
 * @brief  This function clear the radio and vco calibration data
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 *
 */
inline int32_t Phy_ClrCal(void)
{
	*(uint64_t*)(RF_CFG[PHY_RADIO_CAL].cf) = 0x0;
	*(uint64_t*)(RF_CFG[PHY_VCO_CAL].cf) = 0x0;
	return PHY_STATUS_OK;
}

/******************************************************************************/
/******************************************************************************/

/*!
 * @cond INTERNAL
 * @{
 */

#define PHY_PCK_TX_BUFF_BASE_OFFSET 0x2BC          // (x4) 0xAF0
#define PHY_PCK_TX_BUFF_OFFSET (PHY_PCK_TX_BUFF_BASE_OFFSET + 0x04) // (x4) 0xB00
#define PHY_PCK_TX_BUFF_ADDR PARAM_ADF7030_1_SRAM_BASE | ( PHY_PCK_TX_BUFF_OFFSET << 2 )

/*!
 * @}
 * @endcond
 */

// Internal private function
static int32_t _ready_seq(phydev_t *pPhydev);
static int32_t _sleep_seq(phydev_t *pPhydev);
static int32_t _trx_seq(phydev_t *pPhydev);
static int32_t _test_seq(phydev_t *pPhydev, test_modes_tx_e eTxMode);
static int32_t _auto_calibrate_seq(phydev_t *pPhydev);
static int32_t _rssi_calibrate_seq(phydev_t *pPhydev, int8_t i8RssiRefLevel);
static int32_t _do_cmd(phydev_t *pPhydev, uint8_t eCmd);
static void _frame_it(void *p_CbParam, void *p_Arg);
static void _instrum_it(void *p_CbParam, void *p_Arg);

/*!
 * @static
 * @brief  This function initialize the Phy device
 *
 * @param [in]  pPhydev Pointer on the Phy device instance
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
static int32_t _init(phydev_t *pPhydev)
{
    int32_t i32Ret = PHY_STATUS_ERROR;
    uint8_t u8i;
    adf7030_1_device_t* pDevice = pPhydev->pCxt;
    adf7030_1_gpio_int_info_t *pIntGPIOInfo = pDevice->IntGPIOInfo;
    if(pPhydev)
    {
		// private parameters
    	pDevice->bCfgDone = 0;
    	pDevice->bCrcOn = 0;
    	pDevice->bTxPwrDone = 0;
    	pDevice->u8PendTXBuffSize = 0;
		if( !(adf7030_1_Init( pDevice, &spi_ADF7030 )) )
		{
			// set default parameters
			pPhydev->i16TxFreqOffset = DEFAULT_TX_FREQ_OFFSET;
			pPhydev->eModulation = DEFAULT_MOD;
			pPhydev->eTxPower = DEFAULT_TX_POWER;
			pPhydev->eChannel = DEFAULT_CH;
			pPhydev->pfEvtCb = NULL;
			pPhydev->pCbParam = NULL;
			pPhydev->bPreSyncOn = 0;
			pPhydev->bCrcOn = 0;

			pPhydev->eTestMode = PHY_TST_MODE_NONE;

			pDevice->CalCfg.RADIO_CAL_CFG0 = DEFAULT_CAL_CFG;
			// FIXME : set the CAL headers
			*(uint64_t*)(RF_CFG[PHY_RADIO_CAL].cf) = 0x0;
			*(uint64_t*)(RF_CFG[PHY_VCO_CAL].cf) = 0x0;

			pIntGPIOInfo[ADF7030_1_INTPIN0].pfIntCb = &_frame_it;
			pIntGPIOInfo[ADF7030_1_INTPIN1].pfIntCb = NULL; //&_instrum_it;
			i32Ret = PHY_STATUS_OK;
			for (u8i =0; u8i < 2; u8i++)
			{
				pIntGPIOInfo[u8i].pIntCbParam = (void*)pPhydev;
				if (adf7030_1_HostGPIOIrq_Init( pDevice,
							pIntGPIOInfo[u8i].u32Port,
							pIntGPIOInfo[u8i].u16Pin,
							GPIO_IRQ_RISING_EDGE,
							pIntGPIOInfo[u8i].pfIntCb,
							pIntGPIOInfo[u8i].pIntCbParam ) )
				{
					i32Ret = PHY_STATUS_ERROR;
					break;
				}
			}
			if (i32Ret != PHY_STATUS_ERROR) {
				//i32Ret = _do_cmd(pPhydev, PHY_CTL_CMD_READY);
				i32Ret = _do_cmd(pPhydev, PHY_CTL_CMD_PWR_ON);

				adf7030_1__IRQ_ClrStatus(pDevice, ADF7030_1_INTPIN0, 0xFFFFFFFF);
				adf7030_1__IRQ_ClrStatus(pDevice, ADF7030_1_INTPIN1, 0xFFFFFFFF);

				adf7030_1__IRQ_GetMap(pDevice, ADF7030_1_INTPIN0);
				adf7030_1__IRQ_GetMap(pDevice, ADF7030_1_INTPIN1);
			}
		}
    }
    return i32Ret;
}

/*!
 * @static
 * @brief  This function un-initialize the Phy device. (Power OFF and Reset released)
 *
 * @param [in]  pPhydev Pointer on the Phy device instance
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
static int32_t _uninit(phydev_t *pPhydev)
{
    int32_t i32Ret = PHY_STATUS_ERROR;
    adf7030_1_device_t* pDevice = pPhydev->pCxt;
    uint8_t u8i;
    if(pPhydev)
    {
    	i32Ret = PHY_STATUS_OK;
		 /* Clear and disable adf7030 interrupt */
		for (u8i = 0; u8i < ADF7030_1_NUM_INT_PIN; u8i++)
		{
			i32Ret |= adf7030_1_SetupInt( pDevice, u8i, 0);
		}
		if (!i32Ret) {
			i32Ret = adf7030_1_UnInit( pPhydev->pCxt );
		}

		// Power off the PHY
		i32Ret = _do_cmd(pPhydev, PHY_CTL_CMD_PWR_OFF);
		// Release the Reset pin
		BSP_Gpio_SetLow(pDevice->ResetGPIOInfo.u32Port, pDevice->ResetGPIOInfo.u16Pin);

		if (!i32Ret) {
			pDevice->bCfgDone = 0;
			pDevice->bCrcOn = 0;
			pDevice->bTxPwrDone = 0;
			pDevice->u8PendTXBuffSize = 0;
		}
    }
    return i32Ret;
}

/******************************************************************************/
/******************************************************************************/
/*!
 * @static
 * @brief  This function implement the ready sequence
 *
 * @param [in]  pPhydev Pointer on the Phy device instance
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
static int32_t _ready_seq(phydev_t *pPhydev)
{
	int32_t eStatus = PHY_STATUS_OK;
	uint8_t eRet = 0;
    adf7030_1_device_t* pDevice = pPhydev->pCxt;
    adf7030_1_spi_info_t* pSPIDevInfo = &(pDevice->SPIInfo);

	pDevice->eState &= ~ADF7030_1_STATE_READY;
	pDevice->eState &= ~(ADF7030_1_STATE_BUSY);
	switch (pSPIDevInfo->nPhyState)
	{
		// abort the current state, if required
		case PHY_TX:
		case PHY_RX:
			// abort --> EOF interrupt
			// So: disable interrupt
			eRet |= adf7030_1__IRQ_SetMap(pDevice, ADF7030_1_INTPIN0, (uint32_t)0x0);
		case CCA:
		case DO_CAL:
		case MON:
			eRet |= adf7030_1__STATE_PhyCMD_WaitReady(pSPIDevInfo, PHY_ON, PHY_ON);
			if(eRet)
			{
				break;
			}
		// still in ready state
		case PHY_ON:
			// does it need full configuration ?
			if ( pDevice->bCfgDone == 0 )
			{
				// yes, switch to PHY_OFF state
				eRet |= adf7030_1__STATE_PhyCMD_WaitReady(pSPIDevInfo, PHY_OFF, PHY_OFF);
			}
			break;
		case PHY_OFF:
		case CFG_DEV :
			break;
		default: // unexpected state
			break;
	}

	switch (pSPIDevInfo->nPhyState)
	{
		case PHY_SLEEP:
			// will wake-up, so need to CFG_DEV
			pDevice->eState &= ~ADF7030_1_STATE_CONFIGURED;
			// Wake up
			eRet |= adf7030_1_PulseWakup(pDevice);
			// reinitialize the PNTR pointers
			eRet |= adf7030_1__SPI_GetMMapPointers(pSPIDevInfo);
			// switch to PHY_OFF
			eRet |= adf7030_1__STATE_PhyCMD_WaitReady(pSPIDevInfo, PHY_OFF, PHY_OFF);
			//eRet |= adf7030_1__SendConfiguration( pSPIDevInfo, RF_CFG[PHY_BASE_CFG].cf, RF_CFG[PHY_BASE_CFG].size);
			eRet |= adf7030_1__SendConfiguration( pSPIDevInfo, RF_CFG[PHY_HIDDEN].cf, RF_CFG[PHY_HIDDEN].size);
			if(eRet)
			{
				break;
			}
		case PHY_OFF:
			// Does it need a full configuration ?
			if ( pDevice->bCfgDone == 0 )
			{
				// Yes, load the configuration file
				eRet |= adf7030_1__SendConfiguration( pSPIDevInfo, RF_CFG[pPhydev->eModulation].cf, RF_CFG[pPhydev->eModulation].size);
				if (!eRet)
				{
					pDevice->eState &= ~ADF7030_1_STATE_CONFIGURED;
				}

				// Check if calibration data are set or not
				if (*(uint64_t*)(RF_CFG[PHY_RADIO_CAL].cf) != 0x0 )
				{
					// send calibration RADIO and VCO
					if ( !(adf7030_1__SendConfiguration( pSPIDevInfo, RF_CFG[PHY_RADIO_CAL].cf, RF_CFG[PHY_RADIO_CAL].size)) )
					{
						if ( !(adf7030_1__SendConfiguration( pSPIDevInfo, RF_CFG[PHY_VCO_CAL].cf, RF_CFG[PHY_VCO_CAL].size)) )
						{
							pDevice->eState |= ADF7030_1_STATE_CALIBRATED;
						}
					}
				}
				//else { /* Default radio calibration are send with base_cfg*/ }
/*
				// Write offset value to NB_OFFSET in rssi_cfg_t
				rssi_cfg_t rssi_cfg;
				rssi_cfg = (rssi_cfg_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, PROFILE_RSSI_CFG_Addr));
				// Calculate error (dbm) = Average (dbm) - Power input (dbm)
				//u16Avg = u32Sum/20 - ( ( (i8RssiRefLevel >>2) ^0x7FF) +1 );
				//rssi_cfg.RSSI_CFG_b.NB_OFFSET = pPhydev->i16RssiOffset;
				rssi_cfg.RSSI_CFG_b.NB_OFFSET = i16RssiOffsetCal;
				adf7030_1__SPI_SetMem32(pSPIDevInfo, PROFILE_RSSI_CFG_Addr, rssi_cfg.RSSI_CFG);
*/
			}
			// Does it need a CFG_DEV state, case of : full configuration or from wake-up
			if ( !(pDevice->eState & ADF7030_1_STATE_CONFIGURED) )
			{
				// yes, switch to CFG_DEV, then PHY_OFF (automatic goes back)
				eRet |= adf7030_1__STATE_PhyCMD_WaitReady(pSPIDevInfo, CFG_DEV, PHY_OFF);
				if (!eRet)
				{
					pDevice->bCfgDone = 1;
					pDevice->eState |= ADF7030_1_STATE_CONFIGURED;
				}
			}
			// switch to PHY_ON, ...it is ready
			eRet |= adf7030_1__STATE_PhyCMD_WaitReady(pSPIDevInfo, PHY_ON, PHY_ON);
		case PHY_ON : // CFG_DEV or PHY_ON without requiring full configuration
		case CFG_DEV :
			break;
		default: // unexpected state
			eRet = 1;
			break;
	}

	if(!eRet)
	{
		pDevice->eState |= ADF7030_1_STATE_READY;
	}
	else
	{
		// enable to communicate with the PHY
		eStatus = PHY_STATUS_ERROR;
		/* Notice that, at this point :
		 *  - pDevice->eState has ADF7030_1_STATE_READY not set
		 *  - pDevice->eState could has ADF7030_1_STATE_CONFIGURED not set
		 *  - pDevice->bCfgDone could be 0
		 */
	}
	return eStatus;
}

/*!
 * @static
 * @brief  This function implement the sleep sequence
 *
 * @param [in]  pPhydev Pointer on the Phy device instance
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
static int32_t _sleep_seq(phydev_t *pPhydev)
{
	int32_t eStatus = PHY_STATUS_OK;
	uint8_t eRet = 0;
    adf7030_1_device_t* pDevice = pPhydev->pCxt;
    adf7030_1_spi_info_t* pSPIDevInfo = &(pDevice->SPIInfo);

    eRet = adf7030_1__IRQ_SetMap(pDevice, ADF7030_1_INTPIN0, (uint32_t)0x0);
	switch (pSPIDevInfo->nPhyState)
	{
		// stop the current state, if required
		case PHY_TX:
		case PHY_RX:
		case CCA:
		case DO_CAL:
		case MON:
			eRet |= adf7030_1__STATE_PhyCMD_WaitReady(pSPIDevInfo, PHY_ON, PHY_ON);
		case PHY_OFF:
		case PHY_ON:
			// of course, not ready anymore
			pDevice->eState &= ~ADF7030_1_STATE_READY;
			// nor configured
			pDevice->eState &= ~ADF7030_1_STATE_CONFIGURED;
			pSPIDevInfo->nPhyNextState = PHY_SLEEP;
			eRet |= adf7030_1__STATE_PhyCMD( pSPIDevInfo, pSPIDevInfo->nPhyNextState );
			if (!eRet)
			{
				pSPIDevInfo->nPhyState = PHY_SLEEP;
			}
		default:
			break;
	}
	if(eRet)
	{
		eStatus = PHY_STATUS_ERROR;
	}
	return eStatus;
}

/*!
 * @static
 * @brief  This function implement the transmission/reception sequence
 *
 * @param [in]  pPhydev Pointer on the Phy device instance
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
static int32_t _trx_seq(phydev_t *pPhydev)
{
	int32_t eStatus = PHY_STATUS_OK;
	uint8_t eRet = 0;
    adf7030_1_device_t* pDevice = pPhydev->pCxt;
    adf7030_1_spi_info_t* pSPIDevInfo = &(pDevice->SPIInfo);
	// It must be set to READY before
	if(pDevice->eState & ADF7030_1_STATE_READY)
	{
		if( ! (pDevice->eState & ADF7030_1_STATE_BUSY) )
		{
			// Need to update CRC ?
			if ( pDevice->bCrcOn != pPhydev->bCrcOn )
			{
				// Configure CRC
				frame_cfg0_t frame_cfg0;
				frame_cfg0 = (frame_cfg0_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, GENERIC_PKT_FRAME_CFG0_Addr));
				frame_cfg0.FRAME_CFG0_b.CRC_LEN = pPhydev->bCrcOn*16;
				adf7030_1__SPI_SetMem32(pSPIDevInfo, GENERIC_PKT_FRAME_CFG0_Addr, frame_cfg0.FRAME_CFG0);
				pDevice->bCrcOn = pPhydev->bCrcOn;
			}

			// Need to update TX Power ?
			if ( !(pDevice->bTxPwrDone) )
			{
				// Change TX Power
				// PA_Coarse in {1 to 6}
				// PA_Fine in {0, 3 to 127}
				// PA_Micro in {1 to 31}
				radio_dig_tx_cfg0_t tx_cfg0;
				tx_cfg0 = (radio_dig_tx_cfg0_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, PROFILE_RADIO_DIG_TX_CFG0_Addr));
				// Setup PA1 power
				tx_cfg0.RADIO_DIG_TX_CFG0_b.PA_COARSE = aPhyPower[pPhydev->eTxPower].coarse;
				tx_cfg0.RADIO_DIG_TX_CFG0_b.PA_FINE = aPhyPower[pPhydev->eTxPower].fine;
				tx_cfg0.RADIO_DIG_TX_CFG0_b.PA_MICRO = aPhyPower[pPhydev->eTxPower].micro;
				adf7030_1__SPI_SetMem32(pSPIDevInfo, PROFILE_RADIO_DIG_TX_CFG0_Addr, tx_cfg0.RADIO_DIG_TX_CFG0);
				// Change TX power ramp
#ifdef PHY_USE_POWER_RAMP
				radio_dig_tx_cfg1_t tx_cfg1;
				tx_cfg1 = (radio_dig_tx_cfg1_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, PROFILE_RADIO_DIG_TX_CFG1_Addr));
				tx_cfg1.RADIO_DIG_TX_CFG1_b.PA_RAMP_RATE = ePaRampRate;
				adf7030_1__SPI_SetMem32(pSPIDevInfo, PROFILE_RADIO_DIG_TX_CFG1_Addr, tx_cfg1.RADIO_DIG_TX_CFG1);
#endif
				pDevice->bTxPwrDone = 1;
			}

			// Write offset value to NB_OFFSET in rssi_cfg_t
			rssi_cfg_t rssi_cfg;
			rssi_cfg = (rssi_cfg_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, PROFILE_RSSI_CFG_Addr));
			// Calculate error (dbm) = Average (dbm) - Power input (dbm)
			//u16Avg = u32Sum/20 - ( ( (i8RssiRefLevel >>2) ^0x7FF) +1 );
			//rssi_cfg.RSSI_CFG_b.NB_OFFSET = pPhydev->i16RssiOffset;
			rssi_cfg.RSSI_CFG_b.NB_OFFSET = i16RssiOffsetCal;
			adf7030_1__SPI_SetMem32(pSPIDevInfo, PROFILE_RSSI_CFG_Addr, rssi_cfg.RSSI_CFG);

			// Change frequency
			uint32_t u32_Freq = PHY_FREQUENCY_CH(pPhydev->eChannel);
			// only for TX
			u32_Freq += pPhydev->i16TxFreqOffset;
			adf7030_1__SPI_SetMem32( pSPIDevInfo, PROFILE_CH_FREQ_Addr, u32_Freq);

			// Enable / Disable interrupt
			if (pPhydev->eTestMode == PHY_TST_MODE_NONE)
			{
				// Need to update interrupt on PREMBLE and SYNC ?
				if(pPhydev->bPreSyncOn)
				{
					// if not set
					//if( !(pDevice->IntGPIOInfo[ADF7030_1_INTPIN0].nIntMap & (PREAMBLE_IRQn_Msk | SYNCWORD_IRQn_Msk)) )
					{
						eRet |= adf7030_1__IRQ_SetMap(pDevice, ADF7030_1_INTPIN0, (uint32_t)(PREAMBLE_IRQn_Msk | SYNCWORD_IRQn_Msk | EOF_IRQn_Msk));
						eRet |= adf7030_1__IRQ_ClrStatus(pDevice, ADF7030_1_INTPIN0, 0xFFFFFFFF);
					}
					// else, already set
				}
				else {
					// if set
					//if( (pDevice->IntGPIOInfo[ADF7030_1_INTPIN0].nIntMap & (PREAMBLE_IRQn_Msk | SYNCWORD_IRQn_Msk)) )
					{
						eRet |= adf7030_1__IRQ_SetMap(pDevice, ADF7030_1_INTPIN0, (uint32_t)EOF_IRQn_Msk);
						eRet |= adf7030_1__IRQ_ClrStatus(pDevice, ADF7030_1_INTPIN0, 0xFFFFFFFF);
					}
					// else, already unset
				}
			}
			else
			{
				// disable interrupt
				eRet |= adf7030_1__IRQ_SetMap(pDevice, ADF7030_1_INTPIN0, (uint32_t)0x0);
			}
#ifdef PHY_DEBUG_SPE
			eRet = adf7030_1__ReadDataBlock(pSPIDevInfo, &(sConfig.BLOCKS[0]));
			eRet = adf7030_1__ReadDataBlock(pSPIDevInfo, &(sConfig.BLOCKS[1]));
#endif
			if(eRet)
			{
				eStatus = PHY_STATUS_ERROR;
			}
		}
		else {
			eStatus = PHY_STATUS_BUSY;
		}
	}
	else {
		eStatus = PHY_STATUS_ERROR;
		pSPIDevInfo->eXferResult = ADF7030_1_INVALID_OPERATION;
	}
	return eStatus;
}

/*!
 * @brief  This function change the PHY to test mode
 *
 * @param [in]  pPhydev       Pointer on the Phy device instance
 * @param [in]  eTxMode       TX test mode (see test_modes_tx_e)
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
static int32_t _test_seq(phydev_t *pPhydev, test_modes_tx_e eTxMode)
{
    int32_t eStatus = PHY_STATUS_ERROR;
	phy_ctl_e eCmd;

    adf7030_1_device_t* pDevice = pPhydev->pCxt;
    adf7030_1_spi_info_t* pSPIDevInfo = &(pDevice->SPIInfo);

    // Set to ready
    eStatus = _do_cmd(pPhydev, PHY_CTL_CMD_READY);

	// It must be set to READY before
	if(eStatus == PHY_STATUS_OK )
	{
		test_modes1_t test_modes1;
		test_modes0_t test_modes0;
		test_modes0 = (test_modes0_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, GENERIC_PKT_TEST_MODES0_Addr));
	    //
		if (pPhydev->eTestMode == PHY_TST_MODE_NONE)
		{
			eCmd = PHY_CTL_CMD_READY;
			// Unset test mode
			test_modes0.TEST_MODES0_b.PER_EN = 0;
			test_modes0.TEST_MODES0_b.PER_IRQ_SELF_CLEAR = 0;
			test_modes0.TEST_MODES0_b.TX_TEST = 0;
			pDevice->bTxPwrDone = 0;
			pDevice->bCfgDone = 0;
		}
		else
		{
			if (pPhydev->eTestMode < PHY_TST_MODE_TX )
			{
				// RX test mode
				eCmd = PHY_CMD_RX;
				test_modes1.TEST_MODES1_b.PACKET_CNT = 0xFFFF;
				test_modes1.TEST_MODES1_b.CURRENT_CNT = 0;
				adf7030_1__SPI_SetMem32(pSPIDevInfo, GENERIC_PKT_TEST_MODES1_Addr, test_modes1.TEST_MODES1);

				if (pPhydev->eTestMode == PHY_TST_MODE_PER_RX)
				{
					// Set to test mode
					test_modes0.TEST_MODES0_b.PER_EN = 1;
					test_modes0.TEST_MODES0_b.PER_IRQ_SELF_CLEAR = 1;

					eCmd = PHY_CMD_CCA;
					// Disable AFC
					config_t afc_config;
					afc_config = (config_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, AFC_CONFIG_Addr));
					afc_config.CONFIG_b.MODE = 0;
					adf7030_1__SPI_SetMem32(pSPIDevInfo, AFC_CONFIG_Addr, afc_config.CONFIG);

					// Set TICK_RATE to 1 and DETECTION_TIME to 0
					cca_cfg_t cca_cfg;
					cca_cfg = (cca_cfg_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, PROFILE_CCA_CFG_Addr));
					cca_cfg.CCA_CFG_b.TICK_RATE = 1;
					cca_cfg.CCA_CFG_b.DETECTION_TIME = 0;
					adf7030_1__SPI_SetMem32(pSPIDevInfo, PROFILE_CCA_CFG_Addr, cca_cfg.CCA_CFG);
				}
			}
			else
			{
				// TX test mode
				eCmd = PHY_CMD_TX;
				// check if require deviation of +/- 2 fdev
				if (eTxMode == TMODE_TX_FDEV_MAXNEG || eTxMode == TMODE_TX_FDEV_MAXPOS)
				{
					radio_modes_t radio_modes;
					radio_modes = (radio_modes_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, PROFILE_RADIO_MODES_Addr));
					// check if current modulation is 4FSK (required for +/- 2fdev test
					if (radio_modes.RADIO_MODES_b.TX_MOD_TYPE != MOD_4FSK)
					{
						eTxMode--;
					}
				}
				test_modes0.TEST_MODES0_b.TX_TEST = eTxMode;
			}
		}
#if defined (USE_PHY_LAYER_TRACE)
		if(pPhydev->eTestMode != PHY_TST_MODE_NONE)
		{
			TRACE_PHY_LAYER("Phy Enable Test mode\n");
			TRACE_PHY_LAYER("- Mode : %s", aTestModeStr[pPhydev->eTestMode]);
			if (pPhydev->eTestMode == PHY_TST_MODE_TX)
			{
				TRACE_PHY_LAYER(" (%s)\n", aTestModeTXStr[eTxMode]);
			}
			else
			{
				TRACE_PHY_LAYER("\n");
			}
			TRACE_PHY_LAYER("- Channel    : %s\n", aChanStr[pPhydev->eChannel]);
			TRACE_PHY_LAYER("- Modulation : %s\n", aModulationStr[pPhydev->eModulation]);
			TRACE_PHY_LAYER("- Power      : %s (%02hx, %02hx, %02hx)\n",
					aPhyPwrStr[pPhydev->eTxPower],
					aPhyPower[pPhydev->eTxPower].coarse,
					aPhyPower[pPhydev->eTxPower].fine,
					aPhyPower[pPhydev->eTxPower].micro);
		}
		else
		{
			TRACE_PHY_LAYER("Phy Disable Test mode\n");
		}
#endif
		adf7030_1__SPI_SetMem32(pSPIDevInfo, GENERIC_PKT_TEST_MODES0_Addr, test_modes0.TEST_MODES0);
		eStatus = _do_cmd(pPhydev, eCmd);
	}
    return eStatus;
}

/*!
 * @brief  This function implement the calibration sequence
 *
 * @param [in]  pPhydev Pointer on the Phy device instance
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
static int32_t _auto_calibrate_seq(phydev_t *pPhydev)
{
	int32_t eStatus = PHY_STATUS_ERROR;
	uint8_t eRet = 0;
    adf7030_1_device_t* pDevice = pPhydev->pCxt;
    adf7030_1_spi_info_t* pSPIDevInfo = &(pDevice->SPIInfo);
    data_blck_desc_t sBlock;

    eStatus = _do_cmd(pPhydev, PHY_CTL_CMD_RESET);
    if (eStatus == PHY_STATUS_OK )
    {
    	eStatus = _do_cmd(pPhydev, PHY_CTL_CMD_READY);
    }

	// It must be set to READY before
	if(pDevice->eState & ADF7030_1_STATE_READY)
	{
		// device must be in PHY_OFF state
		eRet |= adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo, PHY_OFF, PHY_OFF );

		// Transfers Offline calibration patch to the PHY Radio
	    eRet |= adf7030_1_Configure(pDevice, RF_CFG[PHY_CAL_CFG].cf, RF_CFG[PHY_CAL_CFG].size);

		// Change frequency to mid of the band
		adf7030_1__SPI_SetMem32( pSPIDevInfo, PROFILE_CH_FREQ_Addr, (uint32_t)(PHY_FREQUENCY_CH(PHY_CH120) + PHY_CHANNEL_WIDTH/2));

		// Enable the calibration
		eRet |= adf7030_1__SetupPatch(pSPIDevInfo, SM_DATA_CAL_ENABLE_key, 1);
		eRet |= adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo, CFG_DEV, PHY_OFF );
	    eRet |= adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo, PHY_ON, PHY_ON );

		// Setup "module" to calibrate
	    adf7030_1__SPI_SetMem32(pSPIDevInfo, PROFILE_RADIO_CAL_CFG0_Addr, pDevice->CalCfg.RADIO_CAL_CFG0);
		// Start the calibration
	    eRet |= adf7030_1__STATE_PhyCMD( pSPIDevInfo, DO_CAL );
	    if (!eRet)
	    {
			// Wait for calibration done
			eRet = adf7030_1__STATE_WaitStateReady(pSPIDevInfo, PHY_ON, 0);
			// ---> Calibration finished, should be PHY_ON idle now
			if ( adf7030_1__READ_FIELD(PROFILE_RADIO_CAL_CFG1_CAL_SUCCESS) )
			{
				TRACE_PHY_LAYER("Phy Auto-Calibration success\n");
				pDevice->eState |= ADF7030_1_STATE_CALIBRATED;

				sBlock.WordXfer = 0;
				// Get Radio Calibration result
				sBlock.Addr = PROFILE_RADIO_CAL_RESULTS0_Addr;
				sBlock.pData = &(RF_CFG[PHY_RADIO_CAL].cf[8]);
				sBlock.Size  = sizeof(radio_cal_results_t);
				eRet = adf7030_1__ReadDataBlock( pSPIDevInfo, &(sBlock) );

				// Get VCO Calibration result
				sBlock.Addr = VCO_CAL_RESULTS_DATA0_Addr;
				sBlock.pData = &(RF_CFG[PHY_VCO_CAL].cf[8]);
				sBlock.Size  = sizeof(vco_cal_results_t);
				eRet |= adf7030_1__ReadDataBlock( pSPIDevInfo, &(sBlock) );

				if(!eRet)
				{
					*(uint64_t*)(RF_CFG[PHY_RADIO_CAL].cf) = RADIO_CAL_HEADER_BE;
					*(uint64_t*)(RF_CFG[PHY_VCO_CAL].cf) = VCO_CAL_HEADER_BE;
					eStatus = PHY_STATUS_OK;

#if defined (USE_PHY_LAYER_TRACE)
					uint32_t i;
					uint32_t *p;

					p = (uint32_t*)(RF_CFG[PHY_RADIO_CAL].cf);
					TRACE_PHY_LAYER("Phy Radio Calibration result :\n");
					for (i = 0; i < (RF_CFG[PHY_RADIO_CAL].size/4); i++)
					{
						TRACE_PHY_LAYER("0x%08x\n", p[i]);
					}

					p = (uint32_t*)(RF_CFG[PHY_VCO_CAL].cf);
					TRACE_PHY_LAYER("Phy VCO Calibration result :\n");
					for (i = 0; i < (RF_CFG[PHY_VCO_CAL].size/4); i++)
					{
						TRACE_PHY_LAYER("0x%08x\n", p[i]);
					}
#endif
				}
			}
			else
			{
				TRACE_PHY_LAYER("Phy Auto-Calibration failed\n");
			}
	    }
	}
	else {
		eStatus = PHY_STATUS_ERROR;
		pSPIDevInfo->eXferResult = ADF7030_1_INVALID_OPERATION;
	}
	return eStatus;
}

/*!
 * @brief  This function implement the RSSI offset calibration sequence. Note,
 *         that a carrier at mid band frequency with -77dbm level must externally
 *         be applied during calibration.
 *
 * @param [in]  pPhydev        Pointer on the Phy device instance
 * @param [in]  i8RssiRefLevel RSSI reference input level (dbm) during calibration.
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
static int32_t _rssi_calibrate_seq(phydev_t *pPhydev, int8_t i8RssiRefLevel)
{
	// Power On
	// Configuration
	int32_t eStatus = PHY_STATUS_ERROR;
    adf7030_1_device_t* pDevice = pPhydev->pCxt;
    adf7030_1_spi_info_t* pSPIDevInfo = &(pDevice->SPIInfo);
    cca_cfg_t cca_cfg;
    uint16_t u16Avg;
    uint32_t u32Sum;
    uint8_t i;
	// Auto-Calibrate
    if ( _auto_calibrate_seq(pPhydev) == PHY_STATUS_OK )
    {
		// Clear NB_OFFSET in rssi_cfg_t
		adf7030_1__WRITE_FIELD(PROFILE_RSSI_CFG_NB_OFFSET, 0);
		// Set to PHY_ON
		if ( adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo, PHY_ON, PHY_ON ) )
		{
			return eStatus;
		}
		// Apply Carrier at mid band frequency with i8RssiRefLevel level (e.g.: -77dbm)
		// TODO :

		// clear DETECTION_TIME in cca_cfg_t
		cca_cfg.CCA_CFG = adf7030_1__SPI_GetMem32(pSPIDevInfo, PROFILE_CCA_CFG_Addr);
		adf7030_1__WRITE_FIELD(PROFILE_CCA_CFG_DETECTION_TIME, 0);
		// Issue CCA command
		if (  adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo, CCA, CCA ) )
		{
			// Revert DETECTION_TIME
			adf7030_1__SPI_SetMem32(pSPIDevInfo, PROFILE_CCA_CFG_Addr, cca_cfg.CCA_CFG);
			return eStatus;
		}

		// Wait for at least 64 time the bit transition time
		// assume 2400 bit/s, so 416.67µs per bit, then wait t > 26.67ms
		msleep(30);
		// Read 20 RSSI samples, convert in dbm then average it
		u32Sum = 0;
		for (i = 0; i < 20; i ++)
		{
			u32Sum += adf7030_1__GetRawRSSI( pSPIDevInfo );
		}
		// Calculate error (dbm) = Average (dbm) - Power input (dbm)
		u16Avg = u32Sum/20 - ( ( (i8RssiRefLevel >>2) ^0x7FF) +1 );
		// Write offset value to NB_OFFSET in rssi_cfg_t
		adf7030_1__WRITE_FIELD(PROFILE_RSSI_CFG_NB_OFFSET, u16Avg);

		// Exit from CCA mode
		if (!adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo, PHY_ON, PHY_ON ) )
		{
			// Store the offset value for use at run-time
			i16RssiOffsetCal = u16Avg;
			eStatus = PHY_STATUS_OK;
			TRACE_PHY_LAYER("Phy RSSI-Calibration success\n");
			TRACE_PHY_LAYER("RSSI offset %d\n", i16RssiOffsetCal);
		}
		// Revert DETECTION_TIME
		adf7030_1__SPI_SetMem32(pSPIDevInfo, PROFILE_CCA_CFG_Addr, cca_cfg.CCA_CFG);
    }
    return eStatus;
}

/*!
 * @brief  This is the main FSM.
 *
 * @details This function ensure that transition between state are respected.
 * ... :
 * - Wake-up, (re)configuration,
 *
 * @param [in] pPhydev    Pointer on the Phy device instance
 * @param [in] eCmd       Requested command to execute
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 */
static int32_t _do_cmd(phydev_t *pPhydev, uint8_t eCmd)
{
	int32_t eStatus = PHY_STATUS_OK;
	uint8_t eRet = 0;
    adf7030_1_device_t* pDevice = pPhydev->pCxt;
    adf7030_1_spi_info_t* pSPIDevInfo = &(pDevice->SPIInfo);

	if ( eCmd < PHY_CTL_CMD_READY)
	{
		// Power supply command
		pDevice->eState &= ADF7030_1_STATE_OPENED; // clear all except OPEN
        pSPIDevInfo->eXferResult = ADF7030_1_SUCCESS;
        pSPIDevInfo->nStatus.VALUE = 0;
        pSPIDevInfo->nPhyState = PHY_SLEEP;
        pSPIDevInfo->nPhyNextState = PHY_SLEEP;

		pPhydev->bCrcOn = 0;
		pPhydev->bPreSyncOn = 0;

		pDevice->bCfgDone = 0;
		pDevice->bCrcOn = 0;
		pDevice->bTxPwrDone = 0;
		pDevice->u8PendTXBuffSize = 0;

		pPhydev->u16_Noise = 0;
		pPhydev->u16_Rssi  = 0;
		pPhydev->u16_Ferr  = 0;
		pPhydev->eTestMode = PHY_TST_MODE_NONE;

		switch(eCmd)
		{
			case PHY_CTL_CMD_PWR_OFF:
				BSP_PwrLine_Clr(PA_EN_MSK);
				BSP_PwrLine_Clr(RF_EN_MSK);
				break;
			case PHY_CTL_CMD_PWR_ON:
				PHY_TMR_CAPTURE_POWER_ON();
				// sleep for x µS or mS
				BSP_PwrLine_Set(RF_EN_MSK);
				// TODO : add micro-sleep to ensure power "propagating"
				(bPaState)?(BSP_PwrLine_Set(PA_EN_MSK)):(BSP_PwrLine_Clr(PA_EN_MSK));
			case PHY_CTL_CMD_RESET:
			default:
				adf7030_1_PulseReset(pDevice);
				eRet |= adf7030_1__SPI_GetMMapPointers(pSPIDevInfo);
				eRet |= adf7030_1__SendConfiguration( pSPIDevInfo, RF_CFG[PHY_BASE_CFG].cf, RF_CFG[PHY_BASE_CFG].size);
				eRet |= adf7030_1__STATE_PhyCMD_WaitReady(pSPIDevInfo, CFG_DEV, PHY_OFF);
				if (eRet)
				{
					eStatus = PHY_STATUS_ERROR;
				}
				else
				{
					pDevice->eState |= ADF7030_1_STATE_INITIALIZED;
				}
				break;
		}
	}
    else
    {
    	// It must be Opened and Initialized
		if (pDevice->eState & (ADF7030_1_STATE_OPENED | ADF7030_1_STATE_INITIALIZED) )
		{
		    misc_fw_t misc_fw;

		    if (pSPIDevInfo->nPhyState != PHY_SLEEP)
		    {
		    	misc_fw.FW = adf7030_1__GetMiscFwStatus(pSPIDevInfo);
		    	pSPIDevInfo->nPhyState =  misc_fw.FW_b.CURR_STATE;
		    	pSPIDevInfo->ePhyError =  misc_fw.FW_b.ERR_CODE;
		    }

			switch (eCmd)
			{
				case PHY_CTL_CMD_READY:
					eStatus = _ready_seq(pPhydev);
					PHY_TMR_CAPTURE_READY();
					break;
				case PHY_CTL_CMD_SLEEP:
					eStatus = _sleep_seq(pPhydev);
					break;
				case PHY_CMD_RX:
				case PHY_CMD_CCA:
				case PHY_CMD_TX:
					eStatus = _trx_seq(pPhydev);
					if (eStatus == PHY_STATUS_OK)
					{
						if ( eCmd == PHY_CMD_RX)
						{
							pSPIDevInfo->nPhyNextState = PHY_RX;
							pDevice->eState |= ADF7030_1_STATE_RECEIVING;
						}
						else if (eCmd == PHY_CMD_CCA)
						{
							pSPIDevInfo->nPhyNextState = CCA;
							pDevice->eState |= ADF7030_1_STATE_NOISE_MEAS;
						}
						else { // PHY_CMD_TX
							pSPIDevInfo->nPhyNextState = PHY_TX;
							pDevice->eState |= ADF7030_1_STATE_TRANSMITTING;
						}
#ifdef USE_PHY_TRIG
						eRet = adf7030_1_SetupTrig(pDevice, ADF7030_1_TRIGPIN0, pSPIDevInfo->nPhyNextState, 1);
						eRet |= adf7030_1_PulseTrigger(pDevice, ADF7030_1_TRIGPIN0);
#else
						//eRet = adf7030_1__STATE_PhyCMD( pSPIDevInfo, pSPIDevInfo->nPhyNextState );
						eRet = adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo, pSPIDevInfo->nPhyNextState, pSPIDevInfo->nPhyNextState );
#endif
						if(eRet)
						{
							eStatus = PHY_STATUS_ERROR;
						}
					}
					break;
				default:
					break;
			}
		}
		else {
			eStatus = PHY_STATUS_ERROR;
			pSPIDevInfo->eXferResult = ADF7030_1_INVALID_OPERATION;
		}
    }
    return eStatus;
}
/*!
 * @brief  Interruption handler to treat the frame event
 *
 * @param [in] p_CbParam Pointer on call-back parameter
 * @param [in] p_Arg     Pointer on call-back argument
 *
 * @return None
 */
static void _frame_it(void *p_CbParam, void *p_Arg)
{
	PHY_TMR_CAPTURE_ENTERING_IT();
	phydev_t *pPhydev = (phydev_t *) p_CbParam;
    adf7030_1_device_t* pDevice = (adf7030_1_device_t*)pPhydev->pCxt;
    adf7030_1_spi_info_t* pSPIDevInfo = &(pDevice->SPIInfo);
    misc_fw_t misc_fw;
    uint32_t eEvt = PHYDEV_EVT_NONE;
    uint32_t u32IrqStatus;

    u32IrqStatus = adf7030_1__GetIrqStatus(pSPIDevInfo, ADF7030_1_INTPIN0);
	misc_fw.FW = adf7030_1__GetMiscFwStatus(pSPIDevInfo);
	pSPIDevInfo->nPhyState =  misc_fw.FW_b.CURR_STATE;
	pSPIDevInfo->ePhyError =  misc_fw.FW_b.ERR_CODE;

	if( pDevice->IntGPIOInfo[ADF7030_1_INTPIN0].nIntMap & (PREAMBLE_IRQn_Msk | SYNCWORD_IRQn_Msk ) )
	{
		if(u32IrqStatus & PREAMBLE_IRQn_Msk )
		{
			PHY_TMR_CAPTURE_PREAMBLE_DETECTED();
		}
		if (pDevice->eState & ADF7030_1_STATE_RECEIVING)
		{
			if(u32IrqStatus & PREAMBLE_IRQn_Msk )
			{
				pDevice->bDetected = 1;
			}
			if(u32IrqStatus & SYNCWORD_IRQn_Msk )
			{
				PHY_TMR_CAPTURE_SYNCH_DETECTED();
				if ( pDevice->bDetected )
				{
					pDevice->bDetected = 0;
					pPhydev->u16_Ferr = adf7030_1__GetRawAfcFreqErr(&(pDevice->SPIInfo));
					eEvt = PHYDEV_EVT_RX_STARTED;
				}
			}
		}
	}
	if(u32IrqStatus & EOF_IRQn_Msk )
	{
		if (pDevice->eState & ADF7030_1_STATE_TRANSMITTING)
		{
			PHY_TMR_CAPTURE_TX_COMPLETE();
			eEvt = PHYDEV_EVT_TX_COMPLETE;
			pDevice->eState &= ~ADF7030_1_STATE_TRANSMITTING;
		}
		else if (pDevice->eState & ADF7030_1_STATE_RECEIVING)
		{
			PHY_TMR_CAPTURE_RX_COMPLETE();
			eEvt = PHYDEV_EVT_RX_COMPLETE;
			pDevice->eState &= ~ADF7030_1_STATE_RECEIVING;
		}
		else if (pDevice->eState & ADF7030_1_STATE_NOISE_MEAS)
		{
			pDevice->eState &= ~ADF7030_1_STATE_NOISE_MEAS;
		}
		else // abort TX or RX ?
		{}
	}

	if(u32IrqStatus == 0 )
	{
		eEvt = PHYDEV_EVT_ERROR;
	}

    // just to help debug
    pDevice->IntGPIOInfo[ADF7030_1_INTPIN0].nIntStatus = u32IrqStatus;
    // clear interrupt status
    adf7030_1__ClrIrqStatus(pSPIDevInfo, ADF7030_1_INTPIN0);
    // event notification
    if( (eEvt != PHYDEV_EVT_NONE) && pPhydev->pfEvtCb ) {
		pPhydev->pfEvtCb(pPhydev->pCbParam, eEvt);
	}
    PHY_TMR_CAPTURE_LEAVING_IT();
}

/*!
 * @brief  Interruption handler as an instrumentation
 *
 * @param [in] p_CbParam Pointer on call-back parameter
 * @param [in] p_Arg     Pointer on call-back argument
 *
 * @return None
 */
static void _instrum_it(void *p_CbParam, void *p_Arg)
{
	phydev_t *pPhydev = (phydev_t *) p_CbParam;
    adf7030_1_device_t* pDevice = (adf7030_1_device_t*)pPhydev->pCxt;
    adf7030_1_spi_info_t* pSPIDevInfo = &(pDevice->SPIInfo);
    uint32_t u32IrqStatus;

    u32IrqStatus = adf7030_1__GetIrqStatus(pSPIDevInfo, ADF7030_1_INTPIN1);
    // just to help debug
    pDevice->IntGPIOInfo[ADF7030_1_INTPIN1].nIntStatus = u32IrqStatus;
    // clear interrupt status
    adf7030_1__ClrIrqStatus(pSPIDevInfo, ADF7030_1_INTPIN1);
}

/******************************************************************************/
/******************************************************************************/

/*!
 * @static
 * @brief  This function execute a TX sequence
 *
 * @param [in]  pPhydev     Pointer on the Phy device instance
 * @param [in]  eChannel    Channel use to TX
 * @param [in]  eModulation Modulation use to TX
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
static int32_t _do_TX(phydev_t *pPhydev, phy_chan_e eChannel, phy_mod_e eModulation)
{
    int32_t i32Ret = PHY_STATUS_OK;
    adf7030_1_device_t* pDevice = pPhydev->pCxt;
	if ( !(pDevice->eState & ADF7030_1_STATE_BUSY) )
	{
		// set modulation
		if ( eModulation != pPhydev->eModulation)
		{
			pPhydev->eModulation = eModulation;
			// full reconfiguration is required
			pDevice->bCfgDone = 0;
		}
		// set the Channel
		pPhydev->eChannel = eChannel;

		i32Ret = _do_cmd(pPhydev, PHY_CTL_CMD_READY);
		if (i32Ret == PHY_STATUS_OK)
		{
			uint8_t u8Sz = 0;
			if (eModulation == PHY_WM6400)
			{
				// from here, configuration for WM6400 has been done (set in CFG file)
				// - raw mode is already selected
				// - CRC is disable
				// - PREAMBLE and SYNC word are set in the TX packet buffer
				// - adjust the TX buffer address (offset)
				// So, to take into account PREAMBLE and SYNCHRO words :
				// - adjust the payload length
				u8Sz = (PHY_WM6400_PREAMBLE_SIZE/8) + (PHY_WM6400_SYNC_WORD_SIZE/8);
			}
			// set payload length
			adf7030_1__SPI_SetField(&(pDevice->SPIInfo),
					GENERIC_PKT_FRAME_CFG1_PAYLOAD_SIZE_Addr,
					GENERIC_PKT_FRAME_CFG1_PAYLOAD_SIZE_Pos,
					GENERIC_PKT_FRAME_CFG1_PAYLOAD_SIZE_Size,
					(uint32_t)(pDevice->u8PendTXBuffSize + u8Sz)
					);
			pDevice->u8PendTXBuffSize = 0;
			i32Ret = _do_cmd(pPhydev, PHY_CMD_TX);
		}
	}
	else
	{
		i32Ret = PHY_STATUS_BUSY;
	}
    return i32Ret;
}

/*!
 * @static
 * @brief  This function execute an RX sequence
 *
 * @param [in]  pPhydev     Pointer on the Phy device instance
 * @param [in]  eChannel    Channel use to RX
 * @param [in]  eModulation Modulation use to RX
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
static int32_t _do_RX(phydev_t *pPhydev, phy_chan_e eChannel, phy_mod_e eModulation)
{
    int32_t i32Ret = PHY_STATUS_OK;
    adf7030_1_device_t* pDevice = pPhydev->pCxt;
	if ( !(pDevice->eState & ADF7030_1_STATE_BUSY) )
	{
		if (eModulation > PHY_WM6400)
		{
			// modulation other than PHY_WM2400 and PHY_WM4800 are forbidden
			pDevice->SPIInfo.eXferResult = ADF7030_1_INVALID_PHY_CONFIGURATION;
			i32Ret = PHY_STATUS_ERROR;
		}
		else
		{
			// set modulation
			if ( eModulation != pPhydev->eModulation)
			{
				pPhydev->eModulation = eModulation;
				// full reconfiguration is required
				pDevice->bCfgDone = 0;
			}
			// set the Channel
			pPhydev->eChannel = eChannel;
			i32Ret = _do_cmd(pPhydev, PHY_CTL_CMD_READY);
			if (i32Ret == PHY_STATUS_OK)
			{
				i32Ret = _do_cmd(pPhydev, PHY_CMD_RX);
			}
		}
	}
	else
	{
		i32Ret = PHY_STATUS_BUSY;
	}
    return i32Ret;
}

/*!
 * @static
 * @brief  This function execute a Noise Measurement sequence
 *
 * @param [in]  pPhydev     Pointer on the Phy device instance
 * @param [in]  eChannel    Channel on which the Noise must be measured
 * @param [in]  eModulation Modulation on which the Noise must be measured
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
static int32_t _do_CCA(phydev_t *pPhydev, phy_chan_e eChannel, phy_mod_e eModulation)
{
    int32_t i32Ret = PHY_STATUS_OK;
    adf7030_1_device_t* pDevice = pPhydev->pCxt;
	if ( !(pDevice->eState & ADF7030_1_STATE_BUSY) )
	{
		// set modulation
		if ( eModulation != pPhydev->eModulation)
		{
			pPhydev->eModulation = eModulation;
			// full reconfiguration is required
			pDevice->bCfgDone = 0;
		}
		// set the Channel
		pPhydev->eChannel = eChannel;

		i32Ret = _do_cmd(pPhydev, PHY_CTL_CMD_READY);
		if (i32Ret == PHY_STATUS_OK)
		{
			i32Ret = _do_cmd(pPhydev, PHY_CMD_CCA);
			if ( i32Ret == PHY_STATUS_OK)
			{
				pPhydev->u16_Noise = adf7030_1__GetRawNoise( &(((adf7030_1_device_t*)pPhydev->pCxt)->SPIInfo), NOISE_MEAS_AVG_NB );
				i32Ret = _do_cmd(pPhydev, PHY_CTL_CMD_READY);
				pDevice->eState &= ~ADF7030_1_STATE_NOISE_MEAS;
			}
		}
	}
	else
	{
		i32Ret = PHY_STATUS_BUSY;
	}
    return i32Ret;
}

/*!
 * @static
 * @brief  This function set the packet to send
 *
 * @param [in]  pPhydev Pointer on the Phy device instance
 * @param [in]  pBuf    Pointer to get data to send
 * @param [in]  u8Len   Reference on the data length to send
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
static int32_t _set_send(phydev_t *pPhydev, uint8_t *pBuf, uint8_t u8Len)
{
	int32_t i32Ret = PHY_STATUS_ERROR;
    adf7030_1_device_t* pDevice = pPhydev->pCxt;
    if(pBuf && u8Len )
    {
    	if (!(pDevice->eState & ADF7030_1_STATE_TRANSMITTING ))
    	{
			if ( !(pDevice->eState & ADF7030_1_STATE_READY ) )
			{
				i32Ret = _do_cmd(pPhydev, PHY_CTL_CMD_READY);
			}
			else
			{
				i32Ret = PHY_STATUS_OK;
			}

			if (i32Ret == PHY_STATUS_OK)
			{
				data_blck_desc_t sBlock;
				sBlock.pData = pBuf;
				sBlock.Size  = u8Len;
				sBlock.WordXfer = 0;
				sBlock.Addr = PHY_PCK_TX_BUFF_ADDR;

				if( !( adf7030_1__WriteDataBlock( &(pDevice->SPIInfo), &sBlock) ) )
				{
					// set packet buffer len
					pDevice->u8PendTXBuffSize = u8Len;
				}
				else
				{
					i32Ret = PHY_STATUS_ERROR;
				}
			}
    	}
    	else {
    		i32Ret = PHY_STATUS_BUSY;
    	}
    }
    return i32Ret;
}

/*!
 * @static
 * @brief  This function get the received packet
 *
 * @param [in]  pPhydev Pointer on the Phy device instance
 * @param [in]  pBuf    Pointer on buffer to get received data
 * @param [in]  u8Len   Reference on received number of bytes
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 *
 */
static int32_t _get_recv(phydev_t *pPhydev, uint8_t *pBuf, uint8_t* u8Len)
{
	int32_t i32Ret = PHY_STATUS_ERROR;
    adf7030_1_device_t* pDevice = pPhydev->pCxt;
    if(pBuf && u8Len )
    {
    	if (!(pDevice->eState & ADF7030_1_STATE_RECEIVING ) )
		{
			if ( !(pDevice->eState & ADF7030_1_STATE_READY ) )
			{
				i32Ret = _do_cmd(pPhydev, PHY_CTL_CMD_READY);
			}
			else
			{
				i32Ret = PHY_STATUS_OK;
			}

			if (i32Ret == PHY_STATUS_OK)
			{
				if ( !(adf7030_1__GetRxPacket( &(pDevice->SPIInfo), pBuf, u8Len )) )
				{
					pPhydev->u16_Rssi = adf7030_1__GetRawRSSI( &(pDevice->SPIInfo));
				}
				else
				{
					i32Ret = PHY_STATUS_ERROR;
				}
			}
    	}
    	else {
    		i32Ret = PHY_STATUS_BUSY;
    	}
    }
    return i32Ret;
}

/******************************************************************************/
/*!
 * @static
 * @brief  This function Get/Set internal configuration variable
 *
 * @param [in]     pPhydev Pointer on the Phy device instance
 * @param [in]     eCtl    Id of configuration variable to get/set (see phy_ctl_e)
 * @param [in, out] args    scalar or pointer that hold the value to set/get
 *
 * @retval PHY_STATUS_OK (see phy_status_e::PHY_STATUS_OK)
 * @retval PHY_STATUS_BUSY (see phy_status_e::PHY_STATUS_BUSY)
 * @retval PHY_STATUS_ERROR (see phy_status_e::PHY_STATUS_ERROR)
 */
static int32_t _ioctl(phydev_t *pPhydev, uint32_t eCtl, uint32_t args)
{
	int32_t i32Ret = PHY_STATUS_OK;
	adf7030_1_device_t* pDevice = pPhydev->pCxt;
	adf7030_1_spi_info_t* pSPIDevInfo = &(pDevice->SPIInfo);

	if( (!pPhydev) || (!pDevice) || (!pSPIDevInfo))
	{
		return PHY_STATUS_ERROR;
	}

	if(eCtl > PHY_CTL_CMD)
	{
		if (eCtl == PHY_CMD_TEMP)
		{
			if (adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo, MON, PHY_ON ))
			{
				i32Ret = PHY_STATUS_ERROR;
			}
			else
			{
				int32_t temp = (int32_t)(adf7030_1__READ_FIELD(PROFILE_MONITOR1_TEMP_OUTPUT));
				*(float*)args = PHY_CONV_TempToFloat((int16_t)temp);
			}
		}
		else if (eCtl == PHY_CMD_CLKOUT)
		{
			adf7030_1_gpio_cfg_e eCfg;


			// The clock frequency of the generated clock signal is selected by GPIO_CLK_FREQ_SEL in the PROFILE_RADIO_MODES register.
			// These profile fields must be written by the host in the PHY_OFF state before issuing a CMD_CFG_DEV command.
			// The CMD_GPCLK command, shown in Table 5, must be issued only from the PHY_ON state.
//#define HAS_PHY_CLKOUT_FREQ
#ifdef HAS_PHY_CLKOUT_FREQ
			// device must be in PHY_OFF state
			if (adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo, PHY_OFF, PHY_OFF ) == 0)
			{
				radio_modes_t radio_modes;
				radio_modes = (radio_modes_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, PROFILE_RADIO_MODES_Addr));
				radio_modes.RADIO_MODES_b.GPIO_CLK_FREQ_SEL = GPIO_CLK_FREQ_D1;
				//radio_modes.RADIO_MODES_b.GPIO_CLK_FREQ_SEL = GPIO_CLK_FREQ_D8;
				adf7030_1__SPI_SetMem32(pSPIDevInfo, PROFILE_RADIO_MODES_Addr, radio_modes.RADIO_MODES);
				if (adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo, CFG_DEV, PHY_OFF ) == 0)
				{
					if (adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo, PHY_ON, PHY_ON ))
					{
						i32Ret = PHY_STATUS_ERROR;
					}
				}
				else
				{
					i32Ret = PHY_STATUS_ERROR;
				}
			}
			else
			{
				i32Ret = PHY_STATUS_ERROR;
			}
#endif

			if (i32Ret == PHY_STATUS_OK)
			{
				// Set ADF7030_GPIOx to output 6.5 Mhz clock frequency (default from .cfg files)
				eCfg = (((test_sport_t)args).bGpioClk)?(GPCLK_OUT):(0);
				if (adf7030_1__GPIO_SetCfg(pSPIDevInfo, ((test_sport_t)args).eGpioClk, eCfg, 1) )
				{
					i32Ret = PHY_STATUS_ERROR;
				}
				else
				{
					if (eCfg)
					{
						if (adf7030_1__STATE_PhyCMD_WaitReady( pSPIDevInfo, GPCLK, GPCLK ))
						{
							i32Ret = PHY_STATUS_ERROR;
						}
					}
				}
			}
		}
		else if (eCtl == PHY_CMD_SPORT)
		{
			adf7030_1_gpio_cfg_e eCfg;
			eCfg = (((test_sport_t)args).bGpioClk)?(SPORT_TRX_CLOCK):(0);
			if (adf7030_1__GPIO_SetCfg(pSPIDevInfo, ((test_sport_t)args).eGpioClk, eCfg, 1) )
			{
				i32Ret = PHY_STATUS_ERROR;
			}
			eCfg = (((test_sport_t)args).bGpioData)?(SPORT_RX_DATA):(0);
			if (adf7030_1__GPIO_SetCfg(pSPIDevInfo, ((test_sport_t)args).eGpioData, eCfg, 1) )
			{
				i32Ret = PHY_STATUS_ERROR;
			}
		}
		else if (eCtl == PHY_CMD_TEST)
		{
			pPhydev->eTestMode = ((test_mode_info_t)args).eTestMode;
			pPhydev->eChannel = ((test_mode_info_t)args).eChannel;
			pPhydev->eModulation = ((test_mode_info_t)args).eModulation;
			i32Ret = _test_seq(pPhydev, ((test_mode_info_t)args).eTxMode);
		}
		else if (eCtl == PHY_CMD_AUTO_CAL)
		{
			i32Ret = _auto_calibrate_seq(pPhydev);
		}
		else if (eCtl == PHY_CMD_RSSI_CAL)
		{
			i32Ret = _rssi_calibrate_seq(pPhydev, (int8_t)(args & 0xFF));
		}
		else
		{
			i32Ret = _do_cmd(pPhydev, eCtl);
		}
	}
	else
	{
		if(eCtl == PHY_CTL_GET_STR_ERR)
		{
			if(pSPIDevInfo->eXferResult)
			{
				//*((char*)args) = getErrMsg(pDevice);
				uint32_t tt = (uint32_t)getErrMsg(pDevice);
				*((uint32_t*)args) = tt;
			}
			pSPIDevInfo->eXferResult = 0;
		}
		else {
			//uint8_t i;
			switch(eCtl)
			{
				case PHY_CTL_SET_PA:
					//(args)?(BSP_PwrLine_Set(PA_EN_MSK)):(BSP_PwrLine_Clr(PA_EN_MSK));
					bPaState = (args)?(1):(0);
					if(pDevice->eState & ADF7030_1_STATE_READY)
					{
						(bPaState)?(BSP_PwrLine_Set(PA_EN_MSK)):(BSP_PwrLine_Clr(PA_EN_MSK));
					}
					break;
				case PHY_CTL_SET_TX_FREQ_OFF:
					pPhydev->i16TxFreqOffset = (int16_t)args;
					break;
				case PHY_CTL_SET_TX_POWER:
					if ( (phy_power_e)args != pPhydev->eTxPower)
					{
						pPhydev->eTxPower = (phy_power_e)args;
						// run-time configure TX power is required
						((adf7030_1_device_t*)pPhydev->pCxt)->bTxPwrDone = 0;
					}
					break;
				case PHY_CTL_SET_PWR_ENTRY:
					if ( ((phy_power_entry_t*)args)->eEntryId < PHY_NB_PWR)
					{
						aPhyPower[((phy_power_entry_t*)args)->eEntryId].coarse = ((phy_power_entry_t*)args)->sEntryValue.coarse;
						aPhyPower[((phy_power_entry_t*)args)->eEntryId].fine = ((phy_power_entry_t*)args)->sEntryValue.fine;
						aPhyPower[((phy_power_entry_t*)args)->eEntryId].micro = ((phy_power_entry_t*)args)->sEntryValue.micro;
						if ( ((phy_power_entry_t*)args)->eEntryId == pPhydev->eTxPower)
						{
							// run-time configure TX power is required
							pDevice->bTxPwrDone = 0;
						}
					}
					else
					{
						i32Ret = PHY_STATUS_ERROR;
					}
					break;
				case PHY_CTL_GET_PWR_ENTRY:
					if ( ((phy_power_entry_t*)args)->eEntryId < PHY_NB_PWR)
					{
						((phy_power_entry_t*)args)->sEntryValue.coarse = aPhyPower[((phy_power_entry_t*)args)->eEntryId].coarse;
						((phy_power_entry_t*)args)->sEntryValue.fine = aPhyPower[((phy_power_entry_t*)args)->eEntryId].fine;
						((phy_power_entry_t*)args)->sEntryValue.micro = aPhyPower[((phy_power_entry_t*)args)->eEntryId].micro;
					}
					else
					{
						i32Ret = PHY_STATUS_ERROR;
					}
					break;
				case PHY_CTL_GET_PA:
					//*(uint8_t*)args = (BSP_PwrLine_Get(PA_EN_MSK))?(1):(0);
					*(uint8_t*)args = bPaState;
					break;
				case PHY_CTL_GET_TX_FREQ_OFF:
					*(uint8_t*)args = pPhydev->i16TxFreqOffset;
					break;
				case PHY_CTL_GET_TX_POWER:
					*(uint8_t*)args = pPhydev->eTxPower;
					break;
				case PHY_CTL_GET_FREQ_ERR:
					*(float*)args = PHY_CONV_AfcFreqErrToFloat( pPhydev->u16_Ferr );
					break;
				case PHY_CTL_GET_RSSI:
					*(uint8_t*)args = PHY_CONV_Signed11ToRssi( pPhydev->u16_Rssi );
#if 0
#if defined (USE_PHY_LAYER_TRACE)
					live_link_qual_t link_qual;
					link_qual = (live_link_qual_t)(adf7030_1__SPI_GetMem32(pSPIDevInfo, GENERIC_PKT_LIVE_LINK_QUAL_Addr));

				    uint32_t signed_rssi = (adf7030_1__READ_FIELD(GENERIC_PKT_LIVE_LINK_QUAL_RSSI)) << (32 -11);
				    signed_rssi >>= (32 -11);

				    int32_t sign = signed_rssi >> 31;
				    uint32_t abs_rssi = (sign + signed_rssi) ^ sign;

				     /* Output received packet RSSI */
				    TRACE_PHY_LAYER("%+03d\.%02d dBm", (signed_rssi >> 2), (abs_rssi & 3) * 25);
#endif
#endif
					break;
				case PHY_CTL_GET_NOISE:
					*(uint8_t*)args = PHY_CONV_Signed11ToRssi( pPhydev->u16_Noise );
					break;
				case PHY_CTL_GET_ERR:
					*(uint8_t*)args = ((adf7030_1_device_t*)pPhydev->pCxt)->SPIInfo.eXferResult;
					break;
				case PHY_CTL_GET_STATE:
					*(uint8_t*)args = ((adf7030_1_device_t*)pPhydev->pCxt)->eState;
					break;
				default:
					break;
			}
		}
	}
	return i32Ret;
}

#ifdef __cplusplus
}
#endif

/*! @} */
