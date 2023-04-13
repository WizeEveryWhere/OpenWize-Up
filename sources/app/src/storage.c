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
 * @addtogroup app
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
#include "parameters_cfg.h"
#include "parameters.h"

extern const uint8_t a_ParamDefault[];

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
PERM_SECTION(".param") uint8_t a_ParamValue[PARAM_DEFAULT_SZ];

/******************************************************************************/
#include "phy_layer_private.h"
#include "wize_api.h"

extern phy_power_t aPhyPower[PHY_NB_PWR];
extern int16_t i16RssiOffsetCal;

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

/*!
 * @brief This hold (hard-coded) the default phy power settings
 */
const phy_power_t aDefaultPhyPower[PHY_NB_PWR] =
{
	[PHY_PMAX_minus_0db]  = {.coarse = 6, .fine = 20, .micro = 0}, //   0 dBm
	[PHY_PMAX_minus_6db]  = {.coarse = 6, .fine =  6, .micro = 0}, //  -6 dBm
	[PHY_PMAX_minus_12db] = {.coarse = 6, .fine =  3, .micro = 0}, // -12 dBm
};

/*!
 * @brief This hold (hard-coded) the default phy rssi offset
 */
const int16_t i16DefaultRssiOffsetCal = 0;

/*!
 * @brief This hold (hard-coded) the default PA state
 */
const uint8_t bDefaultPaState = 1;
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
  * @brief Table of keys
  */
KEY_SECTION(".data.keys") key_s _a_Key_[KEY_MAX_NB];

/******************************************************************************/
#include "platform.h"
#include "bsp_boot.h"

/*!
  * @brief Variable to hold the number of reboot on "crash"
  */
PERM_SECTION(".param") uint8_t boot_count;

/******************************************************************************/
#include "flash_storage.h"

/*!
  * @brief Define the hard-coded flash address for the storage area
  */
extern unsigned int __nvm_org__;
extern unsigned int _nvm_size;
#define STORAGE_FLASH_ADDRESS (&__nvm_org__)

/*!
  * @brief Pointer on the storage area n flash
  */
const struct flash_store_s * pStorage_FlashArea;

/******************************************************************************/
/******************************************************************************/
struct _store_special_s
{
	device_id_t sDeviceInfo;
	uint8_t     bPaState;
	uint8_t     ND1;
	int16_t     i16PhyRssiOffset;
	phy_power_t aPhyPower[PHY_NB_PWR];
	uint8_t     ND2[3];
	uint8_t     aPhyCalRes[CAL_RES_SZ] __attribute__ ((aligned(8)));
	uint8_t     ND3[4];
};

/*!
  * @brief  This initialize the storage area
  *
  * @param [in] bForce Force to defaults.
  *
  * @retval  None
  *
  */
void Storage_Init(uint8_t bForce)
{
	pStorage_FlashArea = (const struct flash_store_s *) STORAGE_FLASH_ADDRESS;
	if(bForce || pStorage_FlashArea->sHeader.u16Status == 0xFFFF)
	{
		Storage_SetDefault();
		if ( Storage_Store() == 1)
		{
			// error
			printf("Flash : Failed to store ");
		}
	}
	if ( Storage_Get() == 1)
	{
		// error
		printf("Flash : Failed to read ");
	}
}

/*!
  * @brief  Set to defaults device id, all parameters and all keys
  *
  * @retval  None
  *
  */
void Storage_SetDefault(void)
{
	WizeApi_SetDeviceId(&sDefaultDevId);
	memcpy(aPhyPower, aDefaultPhyPower, sizeof(phy_power_t)*PHY_NB_PWR);
	EX_PHY_SetPa(bDefaultPaState);
	i16RssiOffsetCal = i16DefaultRssiOffsetCal;
	Phy_ClrCal();
	Param_Init(a_ParamDefault);
	memcpy(_a_Key_, sDefaultKey, sizeof(_a_Key_));
}

/*!
  * @brief  Store current into the flash memory
  *
  * @retval  0 Success
  * @retval  1 Failed
  *
  */
uint8_t Storage_Store(void)
{
	struct _store_special_s store_special;
	struct storage_area_s sStorageArea;
	// Prepare first part with device ID, phy power and rssi cal. values
	WizeApi_GetDeviceId(&(store_special.sDeviceInfo));
	memcpy(&(store_special.aPhyPower), aPhyPower, sizeof(phy_power_t)*PHY_NB_PWR);
	store_special.bPaState = EX_PHY_GetPa();
	store_special.i16PhyRssiOffset = i16RssiOffsetCal;
	Phy_GetCal(store_special.aPhyCalRes);

	sStorageArea.u32SrcAddr[0] = (uint32_t)(_a_Key_);
	sStorageArea.u32SrcAddr[1] = (uint32_t)(&store_special);
	sStorageArea.u32SrcAddr[2] = (uint32_t)(a_ParamValue);

	sStorageArea.u32Size[0] = sizeof(_a_Key_);
	sStorageArea.u32Size[1] = sizeof(struct _store_special_s);
	sStorageArea.u32Size[2] = PARAM_DEFAULT_SZ;

	sStorageArea.pFlashArea = (const struct flash_store_s *) STORAGE_FLASH_ADDRESS;;
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

/*!
  * @brief  Get from flash memory o current
  *
  * @retval  0 Success
  * @retval  1 Failed
  *
  */
uint8_t Storage_Get(void)
{
	struct _store_special_s store_special;
	struct storage_area_s sStorageArea;

	sStorageArea.u32SrcAddr[0] = (uint32_t)(_a_Key_);
	sStorageArea.u32SrcAddr[1] = (uint32_t)(&store_special);
	sStorageArea.u32SrcAddr[2] = (uint32_t)(a_ParamValue);

	sStorageArea.u32Size[0] = sizeof(_a_Key_);
	sStorageArea.u32Size[1] = sizeof(struct _store_special_s);
	sStorageArea.u32Size[2] = PARAM_DEFAULT_SZ;

	sStorageArea.pFlashArea = (const struct flash_store_s *) STORAGE_FLASH_ADDRESS;;
	if ( FlashStorage_StoreRead(&sStorageArea) != DEV_SUCCESS)
	{
		return 1;
	}

	// Init special
	WizeApi_SetDeviceId( &(store_special.sDeviceInfo) );
	memcpy(aPhyPower, store_special.aPhyPower, sizeof(phy_power_t)*PHY_NB_PWR);
	EX_PHY_SetPa(store_special.bPaState);
	i16RssiOffsetCal = store_special.i16PhyRssiOffset;
	Phy_SetCal(store_special.aPhyCalRes);
	return 0;
}

#ifdef __cplusplus
}
#endif

/*! @} */
