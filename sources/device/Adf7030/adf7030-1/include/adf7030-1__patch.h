/*!
 *****************************************************************************
  @file:	adf7030-1__patch.h 
  @brief:	...  
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


#ifndef _ADF7030_1__PATCH_H_
#define _ADF7030_1__PATCH_H_

#include "adf7030-1__common.h"
#include "adf7030-1__mem.h"


/** \addtogroup adf7030-1 adf7030-1 Driver
 *  @{
 */

/** \addtogroup adf7030-1__patch Patch Interface
 *  @{
 */


/** Generic Type to describes the Radio PHY Patch Information data.
*/
typedef struct {
    char     patch_name[73];
    uint16_t build_number;
    char     family_str[4];
    uint16_t adiid;
    uint16_t chipid;
    uint8_t  full_layer_num;
    uint8_t  eco_num;
    uint16_t svn_version;
} patch_info_t ;

typedef struct patch_section_s patch_section_t;
typedef struct patch_checksum_desc_s patch_checksum_desc_t;
typedef patch_info_t version_info_t; //\todo: Remove the legacy typedef declaration

struct patch_section_s {
    uint16_t Size;
    uint16_t Addr;
};

typedef union {
    uint16_t     u16_val;  
    struct{
        uint16_t enable         : 1; //!< 0->Disabled, 1->Enabled
        uint16_t mode           : 1; //!< 0->Non-Direct, 1->Direct
        uint16_t type           : 2; //!< see "patch_selfcheck.c" line 182
        uint16_t bitdir         : 1; //!< 0->LSB first, 1->MSB first
        uint16_t calculate_only : 1; //!< Only calculate CRC
        uint16_t set_irq        : 1; //!< Set IRQs once finished
        uint16_t inject_ok      : 1; //!< Automatic Inject "patch" on valid CRC
        uint16_t register_cmd   : 1; //!< 0->Unregister CRC special command, 1->Register CRC special command
        uint16_t crc_len        : 7; //!< lenght of the CRC polynomial
    };
} crc_config_t;

struct patch_checksum_desc_s {
    uint8_t         Size;
    uint8_t         nbBlock;
    crc_config_t    crc_config;
    uint32_t        crc_poly;
    uint32_t        crc_seed;
    uint32_t        crc_output;
    uint32_t        GoldenCRC;
    patch_section_t Sections[];
};

struct patch_desc_s {
    char                  Name[16];     //!< String with name of Patch
    uint16_t              Size;         //!< Total size of the Patch
    uint16_t              nbBlock;      //!< Number of data blocks
    uint16_t              InfoBlockId;  //!< Index of Patch info structure in the PHY
    uint16_t              CheckBlockId; //!< Index of of the SelfCheck section in the PHY
    uint16_t              CheckSeq;     //!< Selfchecking patch sequence
    uint16_t              InjectSeq;    //!< Enable patch sequence
    uint16_t              EjectSeq;     //!< Disable patch sequence
    patch_checksum_desc_t *pChecksum;   //!< Pointer to patch checksum configuration
    data_blck_desc_t      BLOCKS[];
};
  
/* Write patch data to Radio PHY */
uint8_t adf7030_1__WritePatch(
    adf7030_1_spi_info_t* pSPIDevInfo,
    patch_desc_t*         pPATCH
);

/* Read patch data from Radio PHY */
uint8_t adf7030_1__ReadPatch(
    adf7030_1_spi_info_t* pSPIDevInfo,
    patch_desc_t*         pPATCH
);

/* Verify patch integrity */
uint8_t adf7030_1__VerifyPatch(
    adf7030_1_spi_info_t* pSPIDevInfo,
    patch_desc_t*         pPATCH
);
    
/* Selfchecking patch integrity directly on the PHY Radio */
uint8_t adf7030_1__SelfCheckPatch(
    adf7030_1_spi_info_t*   pSPIDevInfo,
    patch_desc_t*           pPATCH,
    uint8_t                 bCHECK,
    adf7030_1_radio_state_e eCmd,
    patch_checksum_desc_t*  pCHECKSUM_CFG
);

/* Setup Patch sequence */
uint8_t adf7030_1__SetupPatch(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint16_t              SetupSeq,
    uint8_t               bCHECK
);

/* Write a single Patch block to the PHY */
uint8_t adf7030_1__WritePatchBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBlock
);

/* Read a single Patch block to the PHY */
uint8_t adf7030_1__ReadPatchBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBlock
);

/* Verify content of a single Patch block in the PHY */
uint8_t adf7030_1__VerifyPatchBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBlock
);

/* Transfers Patch between Host shadow and the radio PHY */
uint8_t adf7030_1__XferPatch(
    adf7030_1_spi_info_t* pSPIDevInfo,
    patch_desc_t*         pPATCH,
    uint8_t               bREAD_nWRITE,
    uint8_t               bCHECK_ONLY
);

/* Transfers Patch subblock between Host and the Radio PHY */
uint8_t adf7030_1__XferPatchBlock(
    adf7030_1_spi_info_t* pSPIDevInfo, 
    data_blck_desc_t*     pBLOCK,
    uint8_t               bREAD_nWRITE,
    uint8_t               bCHECK_ONLY
);

/* Transfers Patch checksum configuration */
uint8_t adf7030_1__XferChecksum(
    adf7030_1_spi_info_t*  pSPIDevInfo,
    patch_desc_t*          pPATCH,
    patch_checksum_desc_t* pPATCH_CHECKSUM,
    uint8_t                bREAD_nWRITE,
    uint8_t                bCHECK_ONLY
);

#define adf7030_1__WritePatchCheckumInfo( pSPIDevInfo, pPATCH, pPATCH_CHECKSUM) adf7030_1__XferChecksum( pSPIDevInfo, \
                                                                                                         (patch_desc_t*)pPATCH, \
                                                                                                         (patch_checksum_desc_t*)pPATCH_CHECKSUM, \
                                                                                                         (uint8_t)PHY_MEM_WRITE, \
                                                                                                         0 )
#define adf7030_1__ReadPatchCheckumInfo( pSPIDevInfo, pPATCH, pPATCH_CHECKSUM) adf7030_1__XferChecksum( pSPIDevInfo, \
                                                                                                         (patch_desc_t*)pPATCH, \
                                                                                                         (patch_checksum_desc_t*)pPATCH_CHECKSUM, \
                                                                                                         (uint8_t)PHY_MEM_READ, \
                                                                                                         0 )
#define adf7030_1__VerifyPatchCheckumInfo( pSPIDevInfo, pPATCH, pPATCH_CHECKSUM) adf7030_1__XferChecksum( pSPIDevInfo,  \
                                                                                                         (patch_desc_t*)pPATCH, \
                                                                                                         (patch_checksum_desc_t*)pPATCH_CHECKSUM, \
                                                                                                         (uint8_t)PHY_MEM_READ, \
                                                                                                         0 )
/** @} */ /* End of group adf7030-1__patch Patch Interface */
/** @} */ /* End of group adf7030-1 adf7030-1 Driver */

#endif /* _ADF7030_1__PATCH_H_ */
