/**
  * @file local_frm.c
  * @brief This file implement functions to build and extract download frames
  *        to/from the local interface.
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
  * @par 1.0.0 : 2023/04/28 [GBI]
  * Initial version
  *
  */

/*!
 *  @addtogroup itf
 *  @ingroup app
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "local_frm.h"
#include <string.h>

#include "crypto.h"
#include "wize_app.h"

/******************************************************************************/

/*!
 * @cond INTERNAL
 * @{
 */

static void _ctr_preset_(uint8_t *pCtr);
static inline void _ctr_next_(uint8_t *pCtr, uint8_t BlockId[BLK_ID_SZ]);

/*!
 * @}
 * @endcond
 */

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
uint8_t LocalFrm_Extract(uint8_t *pData, local_cmd_writeblock_t *pFrame, uint8_t u8KeyId)
{
	uint8_t pCtr[CTR_SIZE];
    uint8_t aHash[CTR_SIZE];
	/*------------------------------------------------------------------------*/
	// Set CTR
    _ctr_preset_(pCtr);
	_ctr_next_(pCtr, pFrame->BlockId);

	/*------------------------------------------------------------------------*/
	if(u8KeyId)
	{
		// Compute HKmob
		if ( Crypto_AES128_CMAC( aHash, pFrame->SwBlock, BLK_SZ, pCtr, u8KeyId ) != CRYPTO_OK)
		{
			return LO_DWN_ERR_UNK;
		}
		// check Hash Kmob
		if ( memcmp( pFrame->HashKmob, aHash, HASH_KMOB_SZ) )
		{
			return LO_DWN_ERR_AUTH;
		}

		/*------------------------------------------------------------------------*/
		// uncipher
		if (Crypto_Decrypt(pData, pFrame->SwBlock, BLK_SZ, pCtr, u8KeyId) != CRYPTO_OK)
		{
			return LO_DWN_ERR_UNK;
		}
	}
	else
	{
		// set Data
		memcpy(pData, pFrame->SwBlock, BLK_SZ);
	}
    return LO_DWN_ERR_NONE;
}

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
uint8_t LocalFrm_Build(local_cmd_writeblock_t *pFrame, uint8_t *pData, uint8_t u8KeyId)
{
	uint8_t pCtr[CTR_SIZE];
    uint8_t aHash[CTR_SIZE];

	/*------------------------------------------------------------------------*/
	// Set CTR
    _ctr_preset_(pCtr);
	_ctr_next_(pCtr, pFrame->BlockId);
	/*------------------------------------------------------------------------*/

	if(u8KeyId)
	{
		// cipher
		if (Crypto_Encrypt(pFrame->SwBlock, pData, BLK_SZ, pCtr, u8KeyId) != CRYPTO_OK)
		{
			return LO_DWN_ERR_UNK;
		}

		// Compute HKmob
		if ( Crypto_AES128_CMAC( aHash, pFrame->SwBlock, BLK_SZ, pCtr, u8KeyId ) != CRYPTO_OK)
		{
			return LO_DWN_ERR_UNK;
		}
	    // set HKmob
	    memcpy( pFrame->HashKmob, aHash, HASH_KMOB_SZ );
	}
	else
	{
		// set Data
		memcpy( pFrame->SwBlock, pData, BLK_SZ );
	    // set HKmob
	    memset( pFrame->HashKmob, 0, HASH_KMOB_SZ );
	}
    return LO_DWN_ERR_NONE;
}

/******************************************************************************/
/*!
 * @cond INTERNAL
 * @{
 */

/*!
 * @static
 * @brief  This function compute the initial CTR
 *
 * @param [in,out] pCtr Pointer on CTR output
 *
 */
static
void _ctr_preset_(uint8_t *pCtr)
{
	uint8_t *p = pCtr;
	WizeApi_GetDeviceId((device_id_t*)p);
    p += 8;
	memset(p, 0x00, CTR_SIZE - MFIELD_SZ - AFIELD_SZ);
}

/*!
 * @static
 * @brief  This function compute the final CTR
 *
 * @param [in,out] pCtr    Pointer on CTR output
 * @param [in]     BlockId The block id to add
 *
 */
static inline
void _ctr_next_(uint8_t *pCtr, uint8_t BlockId[BLK_ID_SZ])
{
	memcpy(&pCtr[MFIELD_SZ + AFIELD_SZ ], BlockId, BLK_ID_SZ);
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
