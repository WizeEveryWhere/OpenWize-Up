/*!
 *****************************************************************************
  @file:    adf7030-1__mem.h 
  @brief:    ...  
  @version:    $Revision: 
  @date:    $Date:
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


#ifndef _ADF7030_1__MEM_H_
#define _ADF7030_1__MEM_H_

#include <stdint.h>
#include "adf7030-1__common.h"

/** \addtogroup adf7030-1 adf7030-1 Driver
 *  @{
 */

/** \addtogroup adf7030-1__mem Memory Interface
 *  @{
 */


typedef struct{
    uint8_t OffsetLen;          ///Size of the Offset list
    uint8_t *pOffset;          ///Pointer to the first offset
} random_xfer_desc_t;

typedef struct{
    uint16_t Size;              ///Byte size of the Block to transfer
    uint16_t Offset;            ///Byte offset relative to the base location
} blck_xfer_desc_t ;

/**
* Define a config structure for revision tracking/conpatibility
*/
typedef struct {
    char     config_name[16];
    uint16_t build_number;
    char     family_str[4];
    uint16_t adiid;
    uint16_t chipid;
    uint8_t  full_layer_num;
    uint8_t  eco_num;
    uint16_t svn_version;
    char     unused[66];
} config_info_t;


/** Generic structure to describes the Radio PHY shadow sub sections.
    The application layer is responsible for allocating buffer.
*/
struct data_blck_desc_s {
    uint16_t       Size;           ///Byte size of the Block to transfer
    uint8_t        Volatile;       ///Indicate whether the underlying data is volatile
    uint8_t        WordXfer;       ///If !NULL, force 32bits SPI transfer
    uint32_t       Addr;           ///Address of the memory bloc in the PHY memory space
    const uint8_t* pData;          ///pointer to Data block
};

/** Generic structure to describes the Radio PHY shadow memory.
    The application layer is responsible for allocating buffer.
*/
struct mem_desc_s {
    char               Name[16];  ///String with name of memory bloc
    uint16_t           Size;      ///size of the memory bloc
    uint8_t            ByteMode;  ///0->8bits, 1->32bits SPI transfert mode, 2->8bits Only, 3->32bits only
    uint8_t            XferMode;  ///0->Block, 1->Random SPI transfer mode
    uint32_t           PhyAddr;   ///address of the memory bloc in the PHY memory space
    void *             pBuff;     ///pointer to the start of the shadow memory bloc in the host memory space
    void *             pBuffRef;  ///pointer to the initial value of the shadow memory bloc in the host memory space
    blck_xfer_desc_t   BlockXfer; ///descriptor for block access
    random_xfer_desc_t RandXfer;  ///descriptor for random access
    mem_desc_t*        pNextBloc; ///If !NULL, points to the next mem_desc_t block to be chained
};

/** Generic structure to describes the Radio PHY configuration memory.
    Data is provided by the setting calculator
*/
struct mem_cfg_desc_s {
    char             Name[16]; ///String with name of configuration sequence
    uint16_t         Size;     ///Total size of the configuration
    uint16_t         nbBlock;  ///Number of data blocks
    config_info_t*   pInfo;    ///Pointer to the configuration info structure in the host memory
    mem_cfg_desc_t*  pNext;    ///Pointer to the next configuration sequence if desired, else NULL
    data_blck_desc_t BLOCKS[];
};


#define PHY_MEM_READ 1
#define PHY_MEM_WRITE 0

uint8_t adf7030_1__ConvConfig(
    const uint8_t*    pCONFIG,
    uint32_t          Size,
    data_blck_desc_t* pDataBlock,
    uint8_t           BlkId
);


/* Write Configuration data to Radio PHY */
uint8_t adf7030_1__WriteConfig(
    adf7030_1_spi_info_t* pSPIDevInfo,
    mem_cfg_desc_t*       pCONFIG
);

/* Read Configuration data from Radio PHY */
uint8_t adf7030_1__ReadConfig(
    adf7030_1_spi_info_t* pSPIDevInfo,
    mem_cfg_desc_t*       pCONFIG
);

/* Verify Configuration integrity */
uint8_t adf7030_1__VerifyConfig(
    adf7030_1_spi_info_t* pSPIDevInfo,
    mem_cfg_desc_t*       pCONFIG
);
  
/* Transfers Configuration between Host and the radio PHY */
uint8_t adf7030_1__XferConfig(
    adf7030_1_spi_info_t* pSPIDevInfo,
    mem_cfg_desc_t*       pCONFIG,
    uint8_t               bREAD_nWRITE,
    uint8_t               bCHECK_ONLY
);

/* Write Data block to Radio PHY */
uint8_t adf7030_1__WriteDataBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBLOCK
);

/* Read Data block from Radio PHY */
adf7030_1_res_e adf7030_1__ReadDataBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBLOCK
);

/* Verify Data block integrity */
uint8_t adf7030_1__VerifyDataBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBLOCK
);

/* Write Partial Data block to Radio PHY */
uint8_t adf7030_1__WritePartialDataBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBLOCK,
    uint16_t              Offset,
    uint16_t              Size
);

/* Read Partial Data block from Radio PHY */
uint8_t adf7030_1__ReadPartiaDataBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBLOCK,
    uint16_t              Offset,
    uint16_t              Size
);

/* Verify Partial Data block integrity */
uint8_t adf7030_1__VerifyPartialDataBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBLOCK,
    uint16_t              Offset,
    uint16_t              Size
);

/* Transfers Data block between Host and the radio PHY */
uint8_t adf7030_1__XferDataBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBLOCK,
    uint16_t              Byte_Offset,
    uint16_t              Byte_Size,
    uint8_t               bREAD_nWRITE,
    uint8_t               bCHECK_ONLY
);

/* Transfers generic memory blocks between Host shadow and the radio PHY */
uint8_t adf7030_1__SPI_Xfer_Mem(
    adf7030_1_spi_info_t* pSPIDevInfo,
    mem_desc_t*           pMEM,
    uint8_t               bREAD_nWRITE
);

/* Read 32bits memory location via the SPI */
void adf7030_1__SPI_SetMem32(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint32_t              Value
);

/* Read 32bits memory location via the SPI */
uint32_t adf7030_1__SPI_GetMem32(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr
);

/* Read 24bits memory location via the SPI */
void adf7030_1__SPI_SetMem24(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint32_t              Value
);

/* Read 24bits memory location via the SPI */
uint32_t adf7030_1__SPI_GetMem24(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr
);

/* Read 16bits memory location via the SPI */
void adf7030_1__SPI_SetMem16(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint16_t              Value
);

/* Read 16bits memory location via the SPI */
uint16_t adf7030_1__SPI_GetMem16(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr
);

/* Read 8bits memory location via the SPI */
void adf7030_1__SPI_SetMem8(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint8_t               Value
);

/* Read 8bits memory location via the SPI */
uint8_t adf7030_1__SPI_GetMem8(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr
);

/* Generic small memory write (up to 4 bytes) via the SPI */
void adf7030_1__SPI_SetBytes(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint32_t              Value,
    uint32_t              nbBytes,
    uint32_t*             pRegVal
);

/* Generic small memory read (up to 4 bytes) via the SPI */
uint32_t adf7030_1__SPI_GetBytes(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint32_t              nbBytes,
    uint32_t*             pRegVal
);

/* Generic function to Set Field value via SPI */
void adf7030_1__SPI_SetField(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint32_t              Pos,
    uint32_t              Size,
    uint32_t              Val
);

/* Generic Function to Get Field value via SPI */
uint32_t adf7030_1__SPI_GetField(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint32_t              Pos,
    uint32_t              Size
);

/* Generic Function to check if byte rw operation is permitted */
uint8_t adf7030_1__MEM_CheckByteAccess(
    uint32_t nAddr
);

/* Generic Macro for setting a bitfield to a value (uses adf7030_hw_macro.h & adf7030_fw_macro.h definitions) */
#define adf7030_1__WRITE_FIELD(fName, fValue) adf7030_1__SPI_SetField( pSPIDevInfo, \
                                                                       fName##_Addr, \
                                                                       fName##_Pos, \
                                                                       fName##_Size, \
                                                                       fValue )

/* Generic Macro for reading a bitfield value (uses adf7030_hw_macro.h & adf7030_fw_macro.h definitions) */
#define adf7030_1__READ_FIELD(fName) adf7030_1__SPI_GetField( pSPIDevInfo, \
                                                              fName##_Addr, \
                                                              fName##_Pos, \
                                                              fName##_Size )

/* Generic Macro for setting a register address to a value (uses adf7030_hw_macro.h & adf7030_fw_macro.h definitions) */
#define adf7030_1__WRITE_REG(rName, rValue) adf7030_1__SPI_SetBytes( pSPIDevInfo, \
                                                                     rName##_Addr, \
                                                                     rValue, \
                                                                     4, \
                                                                     NULL )

/* Generic Macro for setting a register address to a value (uses adf7030_hw_macro.h & adf7030_fw_macro.h definitions) */
#define adf7030_1__READ_REG(rName) adf7030_1__SPI_GetBytes( pSPIDevInfo, \
                                                            rName##_Addr, \
                                                            4, \
                                                            NULL )


/** @} */ /* End of group adf7030-1__mem Memory Interface */
/** @} */ /* End of group adf7030-1 adf7030-1 Driver */

#endif /* _ADF7030_1__SPI_H_ */
