/**
  * @file storage.c
  * @brief This file implement storage functions
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
  * @par 1.0.0 : 2021/02/07 [GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup default
 * @ingroup storage
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <string.h>
#include "storage.h"

/*!
 * @cond INTERNAL
 * @{
 */

#ifndef PERM_SECTION
#define PERM_SECTION(psection) __attribute__(( section(psection) )) __attribute__((used))
#endif

#ifndef KEY_SECTION
#define KEY_SECTION(ksection) __attribute__(( section(ksection) )) __attribute__((used))  __attribute__(( aligned (2048) ))
#endif

/*!
 * @}
 * @endcond
 */
/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */
PERM_SECTION(".roinfo.fw") DECLARE_FWINFO();
PERM_SECTION(".roinfo.hw") DECLARE_HWINFO();
/*!
 * @}
 * @endcond
 */
/******************************************************************************/
#include "phy_layer_private.h"

/*!
 * @brief This hold (hard-coded) the default PA state
 */
const uint8_t bDefaultPaState = 1;

#ifdef USE_PHY_POWER_RAMP
/*!
 * @brief This hold (hard-coded) the default PA ramp state
 */
	const pa_ramp_rate_e eDefaultPaRampRate = RAMP_OFF;
#endif

/*!
 * @brief This hold (hard-coded) the default phy power settings
 */
const phy_power_t aDefaultPhyPower[PHY_NB_PWR] =
{

	//[PHY_PMAX_minus_0db]  = {.coarse = 6, .fine = 20, .micro = 0}, /*!<   0 dBm */
	//[PHY_PMAX_minus_6db]  = {.coarse = 6, .fine =  6, .micro = 0}, /*!<  -6 dBm */
	//[PHY_PMAX_minus_12db] = {.coarse = 6, .fine =  3, .micro = 0}, /*!< -12 dBm */

	// Correction values from Alciom measurement (2023/07/25)
	[PHY_PMAX_minus_0db]  = {.coarse = 1, .fine = 0x25, .micro = 1}, /*!   0 dBm */
	[PHY_PMAX_minus_6db]  = {.coarse = 1, .fine = 0x11, .micro = 1}, /*!  -6 dBm */
	[PHY_PMAX_minus_12db] = {.coarse = 1, .fine = 0x09, .micro = 1}, /*! -12 dBm */
};

/*!
 * @brief This hold (hard-coded) the default phy rssi offset
 */
const int16_t i16DefaultRssiOffsetCal = 0x3C7;

/*! @cond INTERNAL @{ */
PERM_SECTION(".noinit") uint8_t bPaState;
#ifdef USE_PHY_POWER_RAMP
	PERM_SECTION(".noinit") pa_ramp_rate_e ePaRampRate;
#endif
PERM_SECTION(".noinit") int16_t i16RssiOffsetCal;
PERM_SECTION(".noinit") phy_power_t aPhyPower[PHY_NB_PWR];
/*! @} @endcond */
/******************************************************************************/
#include "parameters_cfg.h"
#include "parameters.h"

extern const uint8_t a_ParamDefault[];

/*!
 * @cond INTERNAL
 * @{
 */
/*!
  * @brief The parameters values table size
  */
const uint16_t u16_ParamValueSz = PARAM_DEFAULT_SZ;

/*!
  * @brief The parameters access table size
  */
const uint8_t u8_ParamAccessCfgSz = PARAM_ACCESS_CFG_SZ;

/*!
  * @brief The restriction table size
  */
const uint8_t u8_ParamRestrCfgSz = PARAM_RESTR_CFG_SZ;

/*!
  * @brief Table of parameters values
  */
PERM_SECTION(".noinit") uint8_t a_ParamValue[PARAM_DEFAULT_SZ];
/*!
 * @}
 * @endcond
 */
/******************************************************************************/
#include "crypto.h"
#include "key_priv.h"

/*!
 * @brief This define some hard-coded default keys
 */
const key_s sDefaultKey[KEY_MAX_NB] =
{
	[0] = {
	.key = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	}},
	[KEY_ENC_MIN] = {
	.key = {
		0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08,
		0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	}},
	[KEY_ENC_MIN+1] = {
	.key = {
		0x55, 0x22, 0x19, 0xe2, 0x65, 0xeb, 0xb4, 0x8c,
		0x8a, 0xdf, 0x58, 0x71, 0x79, 0xd9, 0xc6, 0xb0,
		0x63, 0xd5, 0x7c, 0xa8, 0xd3, 0x7e, 0xd6, 0xcb,
		0x11, 0x51, 0xa7, 0x59, 0xcc, 0xad, 0xba, 0x40
	}},
	[KEY_MAC_ID] = {
	.key = {
		0x88, 0xe3, 0x35, 0x63, 0x8f, 0x52, 0x19, 0x46,
		0xc3, 0x8e, 0x32, 0xee, 0xba, 0xa3, 0xc9, 0x9f,
		0x4a, 0xe7, 0x0b, 0xfb, 0x2b, 0xb2, 0x53, 0x40,
		0x25, 0x04, 0x85, 0x76, 0xe3, 0x81, 0xfe, 0xad
	}},
	[KEY_CHG_ID] = {
	.key = {
		0x51, 0x15, 0x1a, 0xb6, 0xa7, 0x47, 0x6b, 0xb1,
		0x53, 0x44, 0x61, 0xdf, 0x67, 0xa0, 0x8b, 0x04,
		0x38, 0x8c, 0xb2, 0x83, 0x96, 0x9c, 0xae, 0x27,
		0x68, 0xe1, 0x2d, 0x0d, 0x83, 0xa4, 0x89, 0xbe
	}}
};

/*!
 * @cond INTERNAL
 * @{
 */
/*!
  * @brief Table of keys
  */
//KEY_SECTION(".data.keys") key_s _a_Key_[KEY_MAX_NB];
KEY_SECTION(".noinit") key_s _a_Key_[KEY_MAX_NB];
/*!
 * @}
 * @endcond
 */
/******************************************************************************/
#include "wize_api.h"
/*!
 * @brief This define hard-coded default device id
 */
const device_id_t sDefaultDevId =
{
//==========================================================================
/* How does it work ?
 *
 * ( The Manufacturer ID shall be registered with the Flag association 30
 * http://www.dlms.com/organization/flagmanufacturesids/index.html)
 *
 * Device number : SET 00 02 82 22 30 03
 *
 * Position in alphabet
 * SET => 'S' : 19; 'E' : 5; 'T' : 20
 * Manufacturer ID => ( pos('S') << 5 + pos('E')) << 5 + pos('T') = (19 << 5 + 5) << 5 + 20
 *
 * So, :
 * Manufacturer = 0xB44C
 * TRx Number   = 0x00028222
 * Version      = 0x30
 * Type         = 0x03
 *
 * Then :
 * sDeviceInfo =
   {
		.aManuf = { 0x4C, 0xB4 },
		.aNum = {0x22, 0x82, 0x02, 0x00},
		.u8Ver = 0x30,
		.u8Type = 0x03
	}
 *
 */
//==========================================================================
	.aManuf = { 0xFF, 0xFF },
	.aNum = {0x00, 0x00, 0x00, 0x00},
	.u8Ver = 0x00,
	.u8Type = 0x00
};

/******************************************************************************/
#include "platform.h"
#include "bsp_boot.h"


/******************************************************************************/
#include "flash_storage.h"
#include "nvm_area.h"

/*!
 * @cond INTERNAL
 * @{
 */

/*!
  * @brief Pointer in flash to the NVM area
  */
const struct flash_store_s * pNvmArea;

/******************************************************************************/

void Storage_Init(uint8_t bForce)
{
	pNvmArea = (const struct flash_store_s *) NVM_ORG;
	if(bForce || pNvmArea->sHeader.u16Status == 0xFFFF)
	{
		Storage_SetDefault(ALL_AREA_ID);
	}
	else
	{
		if ( Storage_Get(ALL_AREA_ID) == 1)
		{
			// error
			printf("NVM : Failed to read ");
		}
	}

	// Get some immutable parameters from default table
	// VERS_HW_TRX
	// VERS_FW_TRX
	memcpy(&a_ParamValue[0], &sHwInfo.version[1], 2);
	memcpy(&a_ParamValue[2], &sFwInfo.version[1], 2);
}

/******************************************************************************/

void Storage_SetDefault(uint8_t eArea)
{
	if (eArea & KEY_AREA_ID)
	{
		memcpy(_a_Key_, sDefaultKey, sizeof(_a_Key_));
	}
	if (eArea & SPE_AREA_ID)
	{
		WizeApi_SetDeviceId(&sDefaultDevId);
		memcpy(aPhyPower, aDefaultPhyPower, sizeof(phy_power_t)*PHY_NB_PWR);
		bPaState = bDefaultPaState;
	#ifdef USE_PHY_POWER_RAMP
		ePaRampRate = eDefaultPaRampRate;
	#endif
		i16RssiOffsetCal = i16DefaultRssiOffsetCal;
		Phy_ClrCal();
	}
	if (eArea & PAR_AREA_ID)
	{
		Param_Init(a_ParamDefault);
	}
}

/******************************************************************************/

uint8_t Storage_Store(void)
{
	struct _store_special_s store_special;
	struct storage_area_s sStorageArea;

	uint8_t u8ExtFlags = EXT_FLAGS_PHYCAL_WRITE_EN_MSK | EXT_FLAGS_IDENT_WRITE_EN_MSK | EXT_FLAGS_KEYS_WRITE_EN_MSK;

	sStorageArea.pFlashArea = (const struct flash_store_s *) NVM_ORG;

	// Prepare first part with device ID, phy power and rssi cal. values
	if(pNvmArea->sHeader.u16Status != 0xFFFF)
	{
#ifdef HAS_EXTEND_PARAMETER
		Param_Access(EXTEND_FLAGS, &u8ExtFlags, 0);
#endif

		struct _store_special_s* p = ((struct _store_special_s*)sStorageArea.pFlashArea->sHeader.u32PartAddr[1]);
		memcpy(&store_special,	p, sizeof(struct _store_special_s));

		// Write key in Flash is forbidden
		if( !(u8ExtFlags & EXT_FLAGS_KEYS_WRITE_EN_MSK))
		{
			// Get keys from storage area
			memcpy(_a_Key_, (void*)sStorageArea.pFlashArea->sHeader.u32PartAddr[0], sizeof(_a_Key_));
		}
	}

	// Write phy calibration in Flash is enable
	if( (u8ExtFlags & EXT_FLAGS_PHYCAL_WRITE_EN_MSK))
	{
		memcpy(&(store_special.aPhyPower), aPhyPower, sizeof(phy_power_t)*PHY_NB_PWR);
		store_special.i16PhyRssiOffset = i16RssiOffsetCal;
		Phy_GetCal(store_special.aPhyCalRes);
	}
	// Write ident in Flash is enable
	if( (u8ExtFlags & EXT_FLAGS_IDENT_WRITE_EN_MSK))
	{
		WizeApi_GetDeviceId(&(store_special.sDeviceInfo));
	}
	store_special.bPaState = bPaState;
#ifdef USE_PHY_POWER_RAMP
	store_special.ePaRampRate = (uint8_t)ePaRampRate;
#endif

	sStorageArea.u32SrcAddr[0] = (uint32_t)(_a_Key_);
	sStorageArea.u32SrcAddr[1] = (uint32_t)(&store_special);
	sStorageArea.u32SrcAddr[2] = (uint32_t)(a_ParamValue);

	sStorageArea.u32Size[0] = sizeof(_a_Key_);
	sStorageArea.u32Size[1] = sizeof(struct _store_special_s);
	sStorageArea.u32Size[2] = PARAM_DEFAULT_SZ;

	if ( FlashStorage_StoreInit(&sStorageArea) != DEV_SUCCESS)
	{
		return 1;
	}
	if ( FlashStorage_StoreWrite(&sStorageArea) != DEV_SUCCESS)
	{
		return 1;
	}
	if ( FlashStorage_StoreFini(&sStorageArea) != DEV_SUCCESS)
	{
		return 1;
	}
	return 0;
}

/******************************************************************************/

uint8_t Storage_Get(uint8_t eArea)
{
	struct _store_special_s store_special;
	struct storage_area_s sStorageArea;

	uint8_t i;
	for (i = 0; i < NB_STORE_PART; i++)
	{
		sStorageArea.u32SrcAddr[i] = 0;
		sStorageArea.u32Size[i] = 0;
	}

	if (eArea & KEY_AREA_ID)
	{
		sStorageArea.u32SrcAddr[0] = (uint32_t)(_a_Key_);
		sStorageArea.u32Size[0] = sizeof(_a_Key_);
	}
	if (eArea & SPE_AREA_ID)
	{
		sStorageArea.u32SrcAddr[1] = (uint32_t)(&store_special);
		sStorageArea.u32Size[1] = sizeof(struct _store_special_s);
	}
	if (eArea & PAR_AREA_ID)
	{
		sStorageArea.u32SrcAddr[2] = (uint32_t)(a_ParamValue);
		sStorageArea.u32Size[2] = PARAM_DEFAULT_SZ;
	}

	sStorageArea.pFlashArea = (const struct flash_store_s *) NVM_ORG;;
	if ( FlashStorage_StoreRead(&sStorageArea) != DEV_SUCCESS)
	{
		return 1;
	}

	if (eArea & SPE_AREA_ID)
	{
		// Init special
		WizeApi_SetDeviceId( &(store_special.sDeviceInfo) );
		memcpy(aPhyPower, store_special.aPhyPower, sizeof(phy_power_t)*PHY_NB_PWR);
		bPaState = store_special.bPaState;
	#ifdef USE_PHY_POWER_RAMP
		ePaRampRate = (pa_ramp_rate_e)store_special.ePaRampRate;
	#endif
		i16RssiOffsetCal = store_special.i16PhyRssiOffset;
		Phy_SetCal(store_special.aPhyCalRes);
	}
	return 0;
}

/*!
 * @}
 * @endcond
 */
/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
