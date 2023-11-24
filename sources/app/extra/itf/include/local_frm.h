/**
  * @file local_frm.h
  * @brief This file define the prototype functions to build and extract download frames
  *        to/from the local interface...
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
  * @par 1.0.0 : 2023/04/26 [GBI]
  * Initial version
  *
  */

/*!
 *  @addtogroup itf
 *  @ingroup app
 *  @{
 */

#ifndef _ITF_LOCAL_FRM_H_
#define _ITF_LOCAL_FRM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*!
 * @cond INTERNAL
 * @{
 */
#define DWN_ID_SZ    4
#define BLK_ID_SZ    2
#define BLK_SZ       210
#define HASH_KMOB_SZ 4
#define CPU_UID_SZ   8

/*!
 * @}
 * @endcond
 */

/******************************************************************************/
/*--- Local anndownload ---*/

/*!
 * @brief This enumeration define error code when command is a ANN_DOWNLOAD
 */
typedef enum {
	LO_DWN_ERR_NONE          = 0x00, /**< No error */
	LO_DWN_ERR_VALUE         = 0x03, /**< One field value is illegal */
	// ---
	LO_DWN_ERR_ILLEGAL_VALUE = 0x81, /**< One field value is illegal */
	LO_DWN_ERR_FRM_LEN       = 0x82, /**< The frame length is incorrect */
	// ---
	LO_DWN_ERR_INI_SW_VER    = 0x83, /**< The initial version is incorrect */
	LO_DWN_ERR_HW_VER        = 0x84, /**< The HW version is incorrect */
	// ---
	LO_DWN_ERR_TGT_SW_VER    = 0x88, /**< The target SW version is incorrect */
	LO_DWN_ERR_TGT_VER_DWL   = 0x89, /**< The target is already download */
	LO_DWN_ERR_OUT_OF_WINDOW = 0x8A, /**< The command is out of window */
	// ---
	LO_DWN_ERR_SES_ID        = 0x91, /**< Mismatch with the ANN session id */
	// ---
	LO_DWN_ERR_BLK_ID        = 0x94, /**< The block ID is out of range */
	LO_DWN_ERR_AUTH          = 0x95, /**< Authentication dosen't match */
	LO_DWN_ERR_WRITE         = 0x96, /**< Write block Error */
	LO_DWN_ERR_CORRUPTED     = 0x97, /**< The image is corrupted or uncompleted */
	// ---
	LO_DWN_ERR_MFIELD        = 0x98, /**< The MField is incorrect */
	LO_DWN_ERR_BLK_CNT       = 0x99, /**< The number of block is incorrect */
	// ---
	LO_DWN_ERR_UNK           = 0xFF, /**< Other error */
} local_dwn_err_code_e;


/*!
 * @brief This enumeration define the filed id in ANN_DOWNLOAD command
 */
typedef enum {
	LO_ANN_FIELD_ID_DwnId,           /**< Download number */
	LO_ANN_FIELD_ID_SwVersionIni,    /**< SW initial version */
	LO_ANN_FIELD_ID_SwVersionTarget, /**< SW target version */
	LO_ANN_FIELD_ID_MField,          /**< MField */
	LO_ANN_FIELD_ID_HwVersion,       /**< HW version */
	LO_ANN_FIELD_ID_BlockCount,      /**< Block number to download */
	LO_ANN_FIELD_ID_HashSW,          /**< Hash on the entire SW to download */
} local_ann_param_id_e;

/*!
 * @brief This struct defines the local command frame in case of ANN_DOWNLOAD command
 */
typedef struct __attribute__((packed))
{
	uint8_t DwnId[DWN_ID_SZ];    /**< The download session identification  */
	uint8_t SwVersionIni[2];     /**< The expected initial SW version  */
	uint8_t SwVersionTarget[2];  /**< The target SW version */
	uint8_t MField[2];           /**< The expected MField */
	uint8_t HwVersion[2];        /**< The expected HW version */
	uint8_t BlockCount[BLK_ID_SZ]; /**< The number of block to download */
	union {
		uint8_t reserved[6];
		struct
		{
			uint8_t DayRepeat;   /**< The number of repeat days */
			uint8_t DeltaSec;    /**< The delta in second between SW block */
			uint8_t DaysProg[4]; /**< Eppch of the programmed first day to download */
		};
	};
	uint8_t HashSW[4];           /**< The Hash computed on the entire downloaded SW */
} local_cmd_anndownload_t;

/******************************************************************************/
/*--- Local writeblock ---*/

/*!
 * @brief This struct defines the local command frame in case of WRITE_BLOCK command
 */
typedef struct
{
	uint8_t DwnId[DWN_ID_SZ];       /**< The download session identification */
	uint8_t BlockId[BLK_ID_SZ];     /**< The block id */
	uint8_t SwBlock[BLK_SZ];        /**< The block SW */
	uint8_t HashKmob[HASH_KMOB_SZ]; /**< The authentication */
} local_cmd_writeblock_t;

/******************************************************************************/
/*--- Local activateupdate ---*/

/*!
 * @brief This struct defines the local command frame in case of ACTIVATE_UPDATE command
 */
typedef struct
{
	uint8_t DwnId[DWN_ID_SZ];    /**< The download session identification  */
} local_cmd_update_t;

/******************************************************************************/
/*--- Local readbitmap ---*/

#if THIS_IS_NOT_IMPLEMENTED_YET
/*!
 * @brief This struct defines the local command frame in case of READ_BITMAP command
 */
typedef struct {
	uint8_t BlockCount[2];    /**<   */
	uint8_t BlockBitmap[];    /**<   */
} local_rsp_update_t;
#endif

/******************************************************************************/
/******************************************************************************/
/*!
  * @brief  This function extract a download frame intended from local interface.
  *
  * @param [out] pData   Pointer on output data
  * @param [in]  pFrame  Pointer on input frame
  * @param [in]  u8KeyId Id of the key used to encrypt and authenticate.
  *
  * @retval  local_dwn_err_code_e::LO_DWN_ERR_NONE If the extract success.
  *          local_dwn_err_code_e::LO_DWN_ERR_AUTH If authenticate failed
  *          local_dwn_err_code_e::LO_DWN_ERR_UNK  Otherwise
  *
  */
uint8_t LocalFrm_Extract(uint8_t *pData, local_cmd_writeblock_t *pFrame, uint8_t u8KeyId);

/*!
  * @brief  This function build a download frame intended to be send on local interface.
  *
  * @details The resulting frame is encrypted and authenticated with AES128. See
  *          local_cmd_writeblock_t for frame structure.
  *
  * @param [out] pFrame  Pointer on output frame
  * @param [in]  pData   Pointer on input data
  * @param [in]  u8KeyId Id of the key used to encrypt and authenticate.
  *
  * @retval  local_dwn_err_code_e::LO_DWN_ERR_NONE If the build success.
  *          local_dwn_err_code_e::LO_DWN_ERR_UNK  Otherwise
  *
  */
uint8_t LocalFrm_Build(local_cmd_writeblock_t *pFrame, uint8_t *pData, uint8_t u8KeyId);

/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _ITF_LOCAL_FRM_H_ */

/*! @} */
