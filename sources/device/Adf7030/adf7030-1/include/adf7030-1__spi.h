/*!
* ****************************************************************************
  @file:    adf7030-1__spi.h
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


#ifndef _ADF7030_1__SPI_H_
#define _ADF7030_1__SPI_H_

#include "adf7030-1__common.h"

/** \addtogroup adf7030-1 adf7030-1 Driver
*   @{
*/

/** \addtogroup adf7030-1__spi SPI Communication Interface
*   @{
*/

/*! Enumeration of different SPI pointer type*/
typedef enum {
    PNTR_SETUP_ADDR     = 0,
    PNTR_SRAM_ADDR      = 1,
    PNTR_MCR_LOW        = 2,
    PNTR_MCR_HIGH       = 3,
    PNTR_IRQ_CTRL_ADDR  = 4,
    PNTR_CUSTOM0_ADDR   = 5,    //Used for generic byte Access
    PNTR_CUSTOM1_ADDR   = 6,    //Used for TX
    PNTR_CUSTOM2_ADDR   = 7 
} adf7030_1_spi_pntr_t;

/*! Enumeration of Host SPI transaction type*/
typedef enum {
    SPI_WRITE = 0,
    SPI_READ  = 1
} adf7030_1_spi_trx_t;

/*! Enumeration of Host SPI Rate*/
typedef enum {
    DEFAULT_SPI_RATE = 0,
    FAST_SPI_RATE    = 1
} adf7030_1_spi_rate_t;

typedef adf7030_1_res_e (* ADF7030_1_SPI_Xfer)(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              cmdOffset,
    adf7030_1_spi_pntr_t  PNTR_ID,
    uint32_t              Addr,
    uint32_t              nbBytes,
    void*                 pDataIO,
    void*                 pDataRef
);

/*! \cond PRIVATE*/

/* memory command */
#define ADF703x_SPI_MEM_WRITE       (0 << 6)
#define ADF703x_SPI_MEM_READ        (1 << 6)
#define ADF703x_SPI_MEM_RANDOM      (0 << 5)
#define ADF703x_SPI_MEM_BLOCK       (1 << 5)
#define ADF703x_SPI_MEM_POINTER     (0 << 4)
#define ADF703x_SPI_MEM_ADDRESS     (1 << 4)
#define ADF703x_SPI_MEM_SHORT       (0 << 3)
#define ADF703x_SPI_MEM_LONG        (1 << 3)

/*! \endcond */

uint8_t adf7030_1__SPI_SetSpeed(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_spi_rate_t  eSPIRate
);

uint8_t adf7030_1__SPI_SetMMapPointers(
    adf7030_1_spi_info_t* pSPIDevInfo
);

uint8_t adf7030_1__SPI_GetMMapPointers(
    adf7030_1_spi_info_t* pSPIDevInfo
);

void adf7030_1__SPI_FindMMapPointer(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    adf7030_1_spi_pntr_t* pPNTR_ID,
    int32_t*              ByteOffset
);
    
uint8_t adf7030_1__SPI_SetMMapCustomPntr0(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr
);

uint8_t adf7030_1__SPI_wr_word_b_a(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              AddrIn,
    uint32_t              num_xfrs,
    uint32_t*             pDataIn
);

uint8_t adf7030_1__SPI_rd_cmp_word_b_a(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              AddrIn,
    uint32_t              num_xfrs,
    uint32_t*             pDataOut,
    uint32_t*             pDataRef
);

#define adf7030_1__SPI_rd_word_b_a( pSPIDevInfo, AddrIn, num_xfrs, pDataOut ) \
        adf7030_1__SPI_rd_cmp_word_b_a( pSPIDevInfo, AddrIn, num_xfrs, pDataOut, NULL)

uint8_t adf7030_1__SPI_wr_word_b_p(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_spi_pntr_t  pntrID,
    uint32_t              num_xfrs,
    uint32_t*             pDataIn
);

uint8_t adf7030_1__SPI_rd_cmp_word_b_p(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_spi_pntr_t  pntrID,
    uint32_t              num_xfrs,
    uint32_t*             pDataOut,
    uint32_t*             pDataRef
);

#define adf7030_1__SPI_rd_word_b_p( pSPIDevInfo, pntrID, num_xfrs, pDataOut ) \
        adf7030_1__SPI_rd_cmp_word_b_p( pSPIDevInfo, pntrID, num_xfrs, pDataOut, NULL)

uint8_t adf7030_1__SPI_wr_byte_b_a(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_spi_pntr_t  pntrID,
    uint32_t              AddrIn,
    uint32_t              num_xfrs,
    uint8_t*              pDataIn
);

uint8_t adf7030_1__SPI_rd_cmp_byte_b_a(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_spi_pntr_t  pntrID,
    uint32_t              AddrIn,
    uint32_t              num_xfrs,
    uint8_t*              pDataOut,
    uint8_t*              pDataRef
);

#define adf7030_1__SPI_rd_byte_b_a( pSPIDevInfo, pntrID, AddrIn, num_xfrs, pDataOut ) \
        adf7030_1__SPI_rd_cmp_byte_b_a( pSPIDevInfo, pntrID, AddrIn, num_xfrs, pDataOut, NULL )

uint8_t adf7030_1__SPI_wr_byte_p_a(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_spi_pntr_t  pntrID,
    uint32_t              num_xfrs,
    uint8_t*              pDataIn
);

uint8_t adf7030_1__SPI_rd_cmp_byte_p_a(
    adf7030_1_spi_info_t* pSPIDevInfo,
    adf7030_1_spi_pntr_t  pntrID,
    uint32_t              num_xfrs,
    uint8_t*              pDataOut,
    uint8_t*              pDataRef
);

#define adf7030_1__SPI_rd_byte_p_a( pSPIDevInfo, pntrID, num_xfrs, pDataOut ) \
        adf7030_1__SPI_rd_cmp_byte_p_a( pSPIDevInfo, pntrID, num_xfrs, pDataOut, NULL )

uint8_t adf7030_1__SPI_rd_word_r_a(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t*             pAddrIn,
    uint32_t*             pDataOut,
    uint32_t              num_xfrs
);

uint8_t adf7030_1__SPI_Block_Xfer__fast(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              cmdOffset,
    adf7030_1_spi_pntr_t  PNTR_ID,
    uint32_t              Addr,
    uint32_t              nbBytes,
    void*                 pDataIO,
    void*                 pDataRef
);

uint8_t adf7030_1__SPI_Block_Xfer(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              cmdOffset,
    adf7030_1_spi_pntr_t  PNTR_ID,
    uint32_t              Addr,
    uint32_t              nbBytes,
    void*                 pDataIO,
    void*                 pDataRef
);

uint8_t adf7030_1__SPI_block_transaction(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              cmdOffset,
    uint32_t              nbBytes,
    void*                 pDataIO,
    void*                 pDataRef
);

void adf7030_1__SPI_ReadWrite_Fast(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t*              pTX_DATA,
    uint8_t*              pRX_DATA,
    uint32_t              nSize
);

void adf7030_1__SPI_Xfer_WriteBuff(
    void*    pDest,
    void*    pSrc,
    uint32_t nBytes,
    uint32_t mode,
    uint32_t op
);

uint8_t adf7030_1__SPI_Xfer_ReadBuff(
    void*    pDest,
    void*    pSrc,
    void*    pRef,
    uint32_t nBytes,
    uint32_t mode
);

/** @}*/ /* End of group adf7030-1__spi SPI Communication Interface*/
/** @}*/ /* End of group adf7030-1 adf7030-1 Driver*/

#endif /* _ADF7030_1__SPI_H_*/
