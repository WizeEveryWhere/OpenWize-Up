/**
  * @file: update_area.c
  * @brief: This file implement the functions to treat the partition header
  *         and/or the image FW.
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
  * @par 1.0.0 : 2023/07/11[GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup update
 * @ingroup app
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "update_area.h"
#include "img_storage.h"
#include "bsp.h"

/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

#ifdef BUILD_STANDALONE_APP
	#define MAGIC_WORD_0 0xDEADC0DEUL // Dead Code
	#define MAGIC_WORD_8 0x0DAC0DACUL // o dec o dac
#endif

/*!
 * @brief This internal structure hold information on update partition
 */
struct update_area_s
{
	uint32_t u32MagicHeader;  /**< The magic header word to set */
	uint32_t u32MagicTrailer; /**< The magic footer word to set   */
	uint32_t u32ImgAdd;       /**< The image address */
	uint32_t u32ImgMaxSz;     /**< The maximum size allowed */
	uint32_t u32HeaderSz;     /**< The header size (512 bytes) */
	uint32_t bIsInit;         /**< Set if update_area_s is initialized */
};

/******************************************************************************/
extern unsigned int __header_size__;

/*!
 * @brief Pointer on exchange info area (update FW)
 */
struct __exch_info_s * const p = (struct __exch_info_s * const)&(__exchange_area_org__);

/*!
 * @brief Pointer on partition info area (current running FW)
 */
struct __img_info_s * p_part_info;

/*!
 * @brief This hold information on update partition
 */
struct update_area_s sUpdateArea;

/*!
 * @}
 * @endcond
 */


/******************************************************************************/

static int32_t _update_area_erase_header_(void);

/******************************************************************************/

/*!
 * @brief This function setup the update area
 *
 * @return see the @link update_status_e @endlink
 */
update_status_e UpdateArea_Setup(void)
{
	uint32_t crc;
	crc = p->crc + 1;
	sUpdateArea.bIsInit = 0;
	// check the CRC
	BSP_CRC_Init();
	crc = BSP_CRC_Compute( (uint32_t*)p, (uint32_t)(sizeof(struct __exch_info_s) - 4) / 4);
	BSP_CRC_Deinit();

	if ( p->crc == crc )
	{
		sUpdateArea.u32HeaderSz     = p->header_sz;
		sUpdateArea.u32MagicHeader  = p->magic;
		sUpdateArea.u32ImgAdd       = p->dest + p->header_sz;
		sUpdateArea.u32ImgMaxSz     = p->dest_sz - p->header_sz;
		p_part_info = (struct __img_info_s * const)(p->src);
	}
	else
	{
		sUpdateArea.u32HeaderSz     = (uint32_t)&(__header_size__);
		sUpdateArea.u32MagicHeader  = MAGIC_WORD_8;
		//sUpdateArea.u32ImgAdd       = 0x0802C200 + (uint32_t)&(__header_size__);
		sUpdateArea.u32ImgAdd       = 0x0802C000 + (uint32_t)&(__header_size__);
		sUpdateArea.u32ImgMaxSz     = 0x2A000 - (uint32_t)&(__header_size__);
		p_part_info = NULL;
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

/*!
 * @brief This function initialize the update area
 *
 * @param [in] eType     The type of update (see update_type_e)
 * @param [in] u16BlkCnt The expected number of update block
 *
 * @return see the @link update_status_e @endlink
 */
update_status_e UpdateArea_Initialize(uint8_t eType, uint16_t u16BlkCnt)
{
	if (eType < UPD_TYPE_NB)
	{
		if (eType != UPD_TYPE_EXTERNAL)
		{
			if (_update_area_erase_header_())
			{
				// Failed
				return UPD_STATUS_STORE_FAILED;
			}
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

/*!
 * @brief This function store an update FW block
 *
 * @param [in] eType The type of update (see update_type_e)
 * @param [in] u16Id The update block id
 * @param [in] pData The update block data to store
 *
 * @return see the @link update_status_e @endlink
 */
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

/*!
 * @brief This function finalize the update area
 *
 * @param [in] eType     The type of update (see update_type_e)
 * @param [in] u32HashSW The hash number to validate the FW image
 * @param [in] img_sz    The size of FW image
 *
 * @return see the @link update_status_e @endlink
 */
update_status_e UpdateArea_Finalize(uint8_t eType, uint32_t u32HashSW, uint32_t img_sz)
{
	update_status_e status = UPD_STATUS_UNK;

	if (eType < UPD_TYPE_NB)
	{
		if (eType != UPD_TYPE_EXTERNAL)
		{
			status = UpdateArea_CheckImg(u32HashSW);

			if (status == UPD_STATUS_VALID)
			{
				struct __img_info_s s_img_info;
				time_t t;

				s_img_info.magic = sUpdateArea.u32MagicHeader;
				s_img_info.hash = u32HashSW;
				//s_img_info.size = img_sz + sUpdateArea.u32HeaderSz + 4;
				s_img_info.size = img_sz + sUpdateArea.u32HeaderSz + 8;
				if (s_img_info.size % 8)
				{
					s_img_info.size += 8 - (s_img_info.size % 8);
				}

				time(&t);
				s_img_info.epoch = (uint32_t)t;
				s_img_info.reserved[0] = 0xFFFFFFFF;
				s_img_info.reserved[1] = 0xFFFFFFFF;

				// image is valid : Finalize with Header
				status = UpdateArea_WriteHeader(&s_img_info);
			}
		}
	}
	return status;
}

/*!
 * @brief This function validate the FW image
 *
 * @param [in] u32HashSW The hash number to validate the FW image
 *
 * @retval @link update_status_e::UPD_STATUS_VALID @endlink If image is valid
 * @retval @link update_status_e::UPD_STATUS_INPROGRESS @endlink If a FW block is missing
 * @retval @link update_status_e::UPD_STATUS_CORRUPTED @endlink If image validation failed
 */
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

/*!
 * @brief This function write the partition header
 *
 * @param [in] p_img_info Pointer on internal partition image info
 *
 * @retval @link update_status_e::UPD_STATUS_READY @endlink If success
 * @retval @link update_status_e::UPD_STATUS_STORE_FAILED @endlink If fail to write header
 */
update_status_e UpdateArea_WriteHeader(struct __img_info_s *p_img_info)
{
	// Finalize with Header
	uint32_t temp[2];
	uint32_t u32TgtAdd = sUpdateArea.u32ImgAdd + p_img_info->size - sUpdateArea.u32HeaderSz - 8;

	if ( (p_img_info->size > sUpdateArea.u32ImgMaxSz) || (sUpdateArea.bIsInit != MAGIC_WORD_8))
	{
		return UPD_STATUS_STORE_FAILED;
	}

	// magic_dead
	//temp[0] = *(uint32_t*)u32TgtAdd;
	temp[0] = 0xFFFFFFFF;
	temp[1] = sUpdateArea.u32MagicTrailer;
	if ( BSP_Flash_Write(u32TgtAdd, (uint64_t*)temp, 1) != DEV_SUCCESS )
	{
		return UPD_STATUS_STORE_FAILED;
	}
	// header
	if ( BSP_Flash_Write(
		(sUpdateArea.u32ImgAdd - sUpdateArea.u32HeaderSz), (uint64_t*)p_img_info, sizeof(struct __img_info_s)/8)
			!= DEV_SUCCESS)
	{
		return UPD_STATUS_STORE_FAILED;
	}
	UpdateArea_SetBootUpdate();
	return UPD_STATUS_READY;
}

/*!
 * @brief This function set the boot request for an Update
 *
 */
inline void UpdateArea_SetBootUpdate(void)
{
	p->request = BOOT_REQ_UPDATE;
}

/*!
 * @brief This function set the boot request for an Roll-Back
 *
 */
inline void UpdateArea_SetBootBack(void)
{
	p->request = BOOT_REQ_BACK;
}

/*!
 * @brief This function set the application as "bootable"
 *
 */
inline void UpdateArea_SetBootable(void)
{
	p->bootable = 0x1;
}
/******************************************************************************/
/*!
 * @static
 * @brief This function erase the image partition header
 *
 * @return 0 if success, -1 otherwise
 *
 */
static int32_t _update_area_erase_header_(void)
{
	int32_t ret = -1;
	uint16_t page_id = BSP_Flash_GetPage(sUpdateArea.u32ImgAdd - sUpdateArea.u32HeaderSz);
	uint8_t retry = 3;
	do {
		if ( BSP_Flash_Erase(page_id) == DEV_SUCCESS )
		{
			retry = 0;
			ret = 0;
			break;
		}
		retry--;
	} while (retry);

	return ret;
}

/******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
