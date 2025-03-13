/**
  * @file at_phy_cmd.c
  * @brief // TODO This file ...
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
  * @par 1.0.0 : 2024/02/20 [TODO: your name]
  * Initial version
  *
  */

/*!
 *  @addtogroup atci
 *  @ingroup app
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "at_phy_cmd.h"
#include "atci_resp.h"
#include "atci_get_cmd.h"

#include "phy_layer_private.h"
#include "rtos_macro.h"

/******************************************************************************/

static
int32_t _phy_noise_loop_(
		char * pOut,
		phy_chan_e eChannel,
		phy_mod_e eModulation,
		uint8_t u8Period,
		uint8_t u8Duration,
		int16_t i16Offset,
		uint16_t u16Delay);

/******************************************************************************/
#ifdef USE_PHY_CCA_CFG_VALUE
extern uint32_t u32CCACfgValue;
#endif

#ifdef USE_PHY_CCA_DELAY_VALUE
extern uint16_t u16CCADelay;
#endif

atci_error_e Exec_ATPHY_Cmd(atci_cmd_t *atciCmdData)
{
	// PHY TX, PHY RX : CH, MOD, Pwr(tx), Tmo(rx), It(Rx)

	atci_error_e status = ATCI_ERR_NONE;

	if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
	{
		Atci_Cmd_Param_Init(atciCmdData);

		// Get channel
		status |= Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
		// Get modulation
		status |= Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
		// Get duration
		status |= Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
		// Get period
		status |= Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT8);
		// Get offset
		status |= Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT16);

		// Get delay
		status |= Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT16);

#ifdef USE_PHY_CCA_CFG_VALUE
		if (atciCmdData->cmdType == AT_CMD_WITH_PARAM_TO_GET)
		{
			// Get CCA cfg register value
			status |= Atci_Buf_Get_Cmd_Param(atciCmdData, PARAM_INT32);
		}
#endif
		if (status == ATCI_ERR_NONE)
		{
			phy_chan_e eChannel   = *(atciCmdData->params[0].val8); // decimal or hex
			phy_mod_e eModulation = *(atciCmdData->params[1].val8); // decimal or hex
			uint8_t u8Duration    = *(atciCmdData->params[2].val8); // second
			uint8_t u8Period      = *(atciCmdData->params[3].val8);
		    //uint8_t u8Period      = 50; // 10, 20, 50, 100, 200, 500 ms
		    int16_t i16Offset     = __ntohs( *(atciCmdData->params[4].val16) );
			uint16_t u16Delay     = __ntohs( *(atciCmdData->params[5].val16) );

#ifdef USE_PHY_CCA_CFG_VALUE
			if (atciCmdData->nbParams > 6)
			{
				u32CCACfgValue = __ntohl( *(atciCmdData->params[6].val32) );
			}
#endif
			if (_phy_noise_loop_((char *)atciCmdData->paramsMem, eChannel, eModulation, u8Period, u8Duration, i16Offset, u16Delay))
			{
				status = ATCI_ERR_UNK;
			}
		}
	}
	else
	{
		status = ATCI_ERR_PARAM_NB;
	}
	return status;
}

/******************************************************************************/
extern phydev_t sPhyDev;

typedef enum { vMin, vAvg, vMax, vNB} eStat;

static int32_t _phy_init_(uint8_t bForceClear);
static int __str_format__(char *pStr, int16_t i16_RawValue, uint8_t bUseFmtExtended);
static void __format_stats__(char *pStr, int16_t ai16_RawValue[], uint32_t u32NbMeas);

#define EXPECTED_STATE (ADF7030_1_STATE_OPENED | ADF7030_1_STATE_INITIALIZED | ADF7030_1_STATE_CONFIGURED | ADF7030_1_STATE_READY)
#define NOISE_STR_FMT() "Noise dBm: "
#define NOISE_STR_FMT_ELEMENT(int_part, dec_part) "%d.%d", int_part, dec_part
#define NOISE_STR_FMT_EXTENDED(raw) " (0x%x; %d)", raw, raw

static int32_t _phy_init_(uint8_t bForceClear)
{
	uint32_t state = 0;
	int32_t eStatus;


	sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CTL_GET_ERR, (uint32_t)&state);
	if (state != ADF7030_1_SUCCESS)
	{
		bForceClear = 1;
	}
	eStatus = sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CTL_GET_STATE, (uint32_t)&state);

	if (bForceClear)
	{
		state = ~EXPECTED_STATE;
	}

	if ( (adf7030_1_state_e)( state & EXPECTED_STATE) != EXPECTED_STATE)
	{
		eStatus |= sPhyDev.pIf->pfUnInit(&sPhyDev);
		eStatus |= sPhyDev.pIf->pfInit(&sPhyDev);
	}

	if (eStatus != PHY_STATUS_OK)
	{
		return -1;
	}
	return 0;
}

static int32_t _phy_noise_loop_(
	char * pOut,
	phy_chan_e eChannel,
	phy_mod_e eModulation,
	uint8_t u8Period,
	uint8_t u8Duration,
	int16_t i16Offset,
	uint16_t u16Delay)
{
	int16_t i16RawNoise[3] =
	{
		[vMin] = 32767,
		[vAvg] = 0,
		[vMax] = -32768
	};

	char* pBase;

	uint32_t ulPeriodTick;
	uint32_t ulEvent;

	uint32_t u32NbMeas;
	uint32_t u32NbMeasNotValid;
	uint32_t u32Count;

	int32_t i32Sum;
	int16_t i16_RawValue;

	if (u8Period == 0)
	{
		u32NbMeas = 100;
		ulPeriodTick = 0;
	}
	else
	{
		u32NbMeas = (u8Duration * 100) / u8Period;
		ulPeriodTick = pdMS_TO_TICKS(u8Period * 10);
	}

	// ---
	if ( _phy_init_(0) )
	{
		return -1;
	}

	// ---
	sPhyDev.i16RssiOffset = i16Offset;
	sPhyDev.eChannel = eChannel;
	sPhyDev.eModulation = eModulation;

	if (sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CTL_CMD_READY, 0) != PHY_STATUS_OK)
	{
		return -1;
	}
	if (sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CMD_CCA, 0) != PHY_STATUS_OK)
	{
		return -1;
	}

	usleep((uint32_t)u16Delay);

	i32Sum = 0;
	u32Count = 0;
	u32NbMeasNotValid = 0;

	pBase = pOut;
	pBase += snprintf(pBase, AT_CMD_DATA_MAX_LEN, "%s", NOISE_STR_FMT() );
	do
	{
		//sPhyDev.pIf->pfNoise(&sPhyDev, eChannel, eModulation);
		////sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CTL_GET_NOISE, (uint32_t)(&u8Noise));

		sPhyDev.u16_Noise = adf7030_1__GetRawCCA(&( ((adf7030_1_device_t*)sPhyDev.pCxt)->SPIInfo));
		//PHY_CONV_Signed11ToIhm(sPhyDev.u16_Noise, &i16_NoiseIntPart, &u8_NoiseDecPart);
		if (sPhyDev.u16_Noise == 0)
		{
			u32NbMeasNotValid++;
		}
		else
		{
			i16_RawValue = PHY_CONV_Signed11ToSigned16(sPhyDev.u16_Noise);
			i32Sum += i16_RawValue;
			if (i16_RawValue < i16RawNoise[vMin])
			{
				i16RawNoise[vMin] = i16_RawValue;
			}
			if (i16_RawValue > i16RawNoise[vMax])
			{
				i16RawNoise[vMax] = i16_RawValue;
			}

	#ifdef USE_NOISE_STR_FMT_EXTENDED
			__str_format__(pBase, i16_RawValue, 1);
	#else
			__str_format__(pBase, i16_RawValue, 0);
	#endif
			Atci_Debug_Param_Data(pOut, NULL);
		}
		if ( ulPeriodTick != 0 )
		{
			if ( sys_flag_wait(&ulEvent, ulPeriodTick) )
			{
				// event has been received, so take it as "CTRL-C"
				break;
			}
		}
		//else // TMO aka period to measure

		u32Count++;
	} while (u32Count < u32NbMeas);

	u32NbMeas -= u32NbMeasNotValid;
	i16RawNoise[vAvg] = i32Sum / (u32NbMeas);

	__format_stats__(pBase, i16RawNoise, u32NbMeas);

	//Atci_Debug_Param_Data(atciCmdData->paramsMem, NULL);
	Atci_Resp_Data(pOut, NULL);
	//sPhyDev.pIf->pfIoctl(&sPhyDev, PHY_CTL_CMD_READY, NULL);
	sPhyDev.pIf->pfUnInit(&sPhyDev);
	return 0;
}

/******************************************************************************/

static int __str_format__(char *pStr, int16_t i16_RawValue, uint8_t bUseFmtExtended)
{
	int nb;
	int16_t i16_NoiseIntPart;
	uint8_t u8_NoiseDecPart;

	PHY_CONV_Signed16toIhm(i16_RawValue, &i16_NoiseIntPart, &u8_NoiseDecPart);

	nb = snprintf( pStr, AT_CMD_DATA_MAX_LEN,
		NOISE_STR_FMT_ELEMENT(i16_NoiseIntPart, u8_NoiseDecPart)
		);
	if (bUseFmtExtended)
	{
		nb += snprintf( pStr + nb, AT_CMD_DATA_MAX_LEN,
			NOISE_STR_FMT_EXTENDED(sPhyDev.u16_Noise)
			);
	}
	return nb;
}

static void __format_stats__(char *pStr, int16_t ai16_RawValue[], uint32_t u32NbMeas)
{
	const char *str[vNB] =
	{
		[vMin] = "(min), ",
		[vAvg] = "(avg), ",
		[vMax] = "(max)"
	};
	int i;
	for (i = vMin; i < vNB; i++)
	{
		pStr += __str_format__(pStr, ai16_RawValue[i], 0);
		pStr += snprintf(pStr, AT_CMD_DATA_MAX_LEN, " %s", str[i]);
	}
	pStr += snprintf(pStr, AT_CMD_DATA_MAX_LEN, " (nb meas. : %lu)", u32NbMeas);
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
