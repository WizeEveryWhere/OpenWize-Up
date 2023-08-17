/**
  * @file: update_area.c
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
  * 1.0.0 : 2023/07/11[GBI]
  * Initial version
  *
  *
  */
#ifdef __cplusplus
extern "C" {
#endif

#include "update_area.h"
#include "img_storage.h"
#include "bsp.h"

#ifndef BUILD_STANDALAONE_APP
	#include "img.h"
#else
	#define MAGIC_WORD_0 0xDEADC0DEUL // Dead Code
	#define MAGIC_WORD_8 0x0DAC0DACUL // o dec o dac
#endif

/******************************************************************************/
/*!
 * @brief This
 */
struct update_area_s
{
	uint32_t u32MagicHeader;  /**<   */
	uint32_t u32MagicTrailer; /**<   */
	uint32_t u32ImgAdd;       /**<   */
	uint32_t u32ImgMaxSz;     /**<   */
	uint32_t u32HeaderSz;     /**<   */
	uint32_t bIsInit;         /**<   */
};

/******************************************************************************/
extern unsigned int __header_size__;

#ifndef BUILD_STANDALAONE_APP
	const struct __exch_info_s *p = (const struct __exch_info_s *)&(__exchange_area_org__);
#endif

struct update_area_s sUpdateArea;
/******************************************************************************/

update_status_e UpdateArea_Setup(void)
{
#ifndef BUILD_STANDALAONE_APP
	uint32_t crc;
	crc = p->crc + 1;
#warning "*** Ensure that CRC computation is enable in the bootstrap ***"
#ifdef HAS_CRC_COMPUTE
	// check the CRC
	BSP_CRC_Init();
	crc = BSP_CRC_Compute( (uint32_t*)p, (uint32_t)(sizeof(struct __exch_info_s) - 4) / 4);
	BSP_CRC_Deinit();
#endif
	if ( p->crc == crc )
	{
		sUpdateArea.u32HeaderSz     = p->header_sz;
		sUpdateArea.u32MagicHeader  = p->magic;
		sUpdateArea.u32ImgAdd       = p->dest + p->header_sz;
		sUpdateArea.u32ImgMaxSz     = p->dest_sz - p->header_sz;
	}
	else
#endif
	{
		sUpdateArea.u32HeaderSz     = (uint32_t)&(__header_size__);
		sUpdateArea.u32MagicHeader  = MAGIC_WORD_8;
		sUpdateArea.u32ImgAdd       = 0x0802C200 + (uint32_t)&(__header_size__);
		sUpdateArea.u32ImgMaxSz     = 0x2A000 - (uint32_t)&(__header_size__);
	}
	sUpdateArea.u32MagicTrailer = MAGIC_WORD_0;

	if( ImgStore_Setup(
			sUpdateArea.u32ImgAdd,
			sUpdateArea.u32ImgMaxSz,
			BSP_Flash_Write,
			BSP_Flash_EraseArea) )
	{
		// if Image Storage initialization failed then UPSATE is not possible
		return UPD_STATUS_STORE_FAILED;
	}
	sUpdateArea.bIsInit = MAGIC_WORD_8;
	return UPD_STATUS_READY;
}

update_status_e UpdateArea_Initialize(uint8_t eType, uint16_t u16BlkCnt)
{
	if (eType < UPD_TYPE_NB)
	{
		if (eType != UPD_TYPE_EXTERNAL)
		{
			// Init dwn storage
			if ( ImgStore_Init(u16BlkCnt) )
			{
				// Failed
				return UPD_STATUS_STORE_FAILED;
			}
		}
		return UPD_STATUS_UNK;
	}
	return UPD_STATUS_STORE_FAILED;
}

update_status_e UpdateArea_Proceed(uint8_t eType, uint16_t u16Id, const uint8_t *pData)
{
	if (eType < UPD_TYPE_NB)
	{
		if (eType != UPD_TYPE_EXTERNAL)
		{
			ImgStore_StoreBlock(u16Id - 1, (uint8_t *)pData);
		}
		return UPD_STATUS_UNK;
	}
	return UPD_STATUS_STORE_FAILED;
}

update_status_e UpdateArea_Finalize(uint32_t u32HashSW, uint32_t img_sz)
{
	update_status_e status;
	status = UpdateArea_CheckImg(u32HashSW);
	if (status == UPD_STATUS_VALID)
	{
		// image is valid : Finalize with Header
		status = UpdateArea_WriteHeader(img_sz);
	}
	return status;
}

update_status_e UpdateArea_CheckImg(uint32_t u32HashSW)
{
	if ( ImgStore_IsComplete() )
	{
		if ( ImgStore_Verify((uint8_t*)&(u32HashSW), 4) )
		{
			// image is corrupted
			return UPD_STATUS_CORRUPTED;
		}
		else
		{
			// image is valid
			return UPD_STATUS_VALID;
		}
	}
	else
	{
		return UPD_STATUS_INPROGRESS;
	}
}

update_status_e UpdateArea_WriteHeader(uint32_t img_sz)
{
	// Finalize with Header
	uint32_t temp[2];

	// magic_dead
	temp[0] = sUpdateArea.u32MagicTrailer;
	temp[1] = 0xFFFFFFFF;

	if ( (img_sz > sUpdateArea.u32ImgMaxSz) || (sUpdateArea.bIsInit != MAGIC_WORD_8))
	{
		return UPD_STATUS_STORE_FAILED;
	}

	uint32_t u32TgtAdd = sUpdateArea.u32ImgAdd + img_sz;

	img_sz += (u32TgtAdd % 8);
	u32TgtAdd += (u32TgtAdd % 8);

	if ( *(uint32_t*)u32TgtAdd != temp[0] )
	{
		if ( BSP_Flash_Write(u32TgtAdd, (uint64_t*)temp, 1) != DEV_SUCCESS )
		{
			return UPD_STATUS_STORE_FAILED;
		}
	}

	// magic_header , img_sz
	temp[0] = sUpdateArea.u32MagicHeader;
	temp[1] = img_sz;

	if ( BSP_Flash_Write(
		(sUpdateArea.u32ImgAdd - sUpdateArea.u32HeaderSz), (uint64_t*)temp, 1)
			!= DEV_SUCCESS)
	{
		return UPD_STATUS_STORE_FAILED;
	}
	return UPD_STATUS_READY;
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif
