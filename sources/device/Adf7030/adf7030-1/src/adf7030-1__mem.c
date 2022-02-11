/*!
 *****************************************************************************
  @file:    adf7030-1_mem.c
  
  @brief:    adf7030-1 Memory Accress Functions.
                - SPI communication with between Host and Radio.
                - Shadow Memory MAP
  
  @details:    ...
  
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


/** \addtogroup adf7030-1 adf7030-1 Driver
 *  @{
 */

/** \addtogroup adf7030-1__mem Memory Interface
 *  @{
 */
#ifndef _ADF7030_1__MEM_C_
/*! \cond PRIVATE */
#define _ADF7030_1__MEM_C_

#include "adf7030-1__mem.h"

#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "adf7030-1_reg.h"

#include "adf7030-1__common.h"
#include "adf7030-1__spi.h"

#ifdef __ICCARM__
/*
* IAR MISRA C 2004 error suppressions.
*
* Pm140 (rule 11.3): a cast should not be performed between a pointer type and an integral type
*   The rule makes an exception for memory-mapped register accesses.
*
* Pm073 (rule 14.7): a function should have a single point of exit.
* Pm143 (rule 14.7): a function should have a single point of exit at the end of the function.
*   Multiple returns are used for error handling.
*/
#pragma diag_suppress=Pm073,Pm143
#endif /* __ICCARM__ */

/*! \endcond */

/**
 * @brief       Function call to convert a configuration file into data block
 *              description structure.
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pCONFIG      Pointer to Host configuration structure.
 * @param [in]  Size         The pCONFIG content size.
 * @param [in]  pDataBlock   Pointer to the output data block descriptor.
 * @param [in]  BlkId        The requested block id.
 *
 *
 * @return      Status
 *  - #0  If the Configuration was written successfully in the Host.
 *  - #1  If the memory transfer is not allowed or if the Configuration transfert failed.
 */
uint8_t adf7030_1__ConvConfig(
    const uint8_t*    pCONFIG,
    uint32_t          Size,
    data_blck_desc_t* pDataBlock,
    uint8_t           BlkId
)
{

    uint32_t array_position = 0;
    uint8_t u8i;
    uint32_t length;

    if (pCONFIG == NULL || pDataBlock == NULL || Size == 0)
    {
    	return 1;
    }

    u8i = 0;
    while(array_position < Size) // Continue operation until full data file has been written
    {
      // Calculate the number of bytes to write
      length =  (*(pCONFIG + array_position ) << 16) |
                (*(pCONFIG + array_position + 1) << 8) |
                (*(pCONFIG + array_position + 2));
      if ( u8i == BlkId )
      {
    	  break;
      }
      // Update the array position to point to the next block
      array_position += length;
      u8i++;
    }

    if ( u8i == BlkId )
    {
		if(length > 0x2E4)
		{
		   return 1;
		}

		length -= 8;
		pDataBlock->Addr =  (uint32_t)( *(pCONFIG + array_position + 4) << 24 );
		pDataBlock->Addr |= (uint32_t)( *(pCONFIG + array_position + 5) << 16 );
		pDataBlock->Addr |= (uint32_t)( *(pCONFIG + array_position + 6) << 8 );
		pDataBlock->Addr |= (uint32_t)( *(pCONFIG + array_position + 7) );
		pDataBlock->Size = length;


		uint32_t i, nbWord, nbShort, nbByte;

		nbWord = length/4;
		nbShort = (length%4)/2;
		nbByte = length - (nbWord*4) - (nbShort*2);

		uint8_t *pDest = (uint8_t*)(pDataBlock->pData);
		const uint8_t *pSrc  = (pCONFIG + array_position + 8);

		for ( i = 0; i < nbWord; i++) {
			*((uint32_t*)pDest) = __REV( *((uint32_t*)pSrc) );
			pDest += 4;
			pSrc += 4;
		}

		for ( i = 0; i < nbShort; i++) {
			*((uint16_t*)pDest) = __REV16( *((uint16_t*)pSrc) );
			pDest += 2;
			pSrc += 2;
		}

		for ( i = 0; i < nbByte; i++) {
			*(pDest) = *(pSrc);
			pDest++;
			pSrc++;
		}
		return 0;
    }
    else {
    	return 0;
    }
}


/**
 * @brief       Function call to write Configuration to Radio PHY
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pCONFIG         Pointer to Host configuration structure.
 *
 * @note                        Configuration data is provided by ADI RF Setting Calculator.
 *                              @sa USER_CONFIG_20.h for an example.
 *                              This function modifies SPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0  If the Configuration was written successfully in the Host.
 *  - #1  If the memory transfer is not allowed or if the Configuration transfert failed.
 */
inline uint8_t adf7030_1__WriteConfig(
    adf7030_1_spi_info_t* pSPIDevInfo,
    mem_cfg_desc_t*       pCONFIG
)
{
    return(adf7030_1__XferConfig( pSPIDevInfo,
                                  pCONFIG,
                                  PHY_MEM_WRITE,
                                  0));
}


/**
 * @brief       Function call to read Configuration from Radio PHY
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in|out]  pCONFIG     Pointer to Host configuration structure.
 *
 * @note                        Configuration data is provided by ADI RF Setting Calculator.
 *                              @sa USER_CONFIG_20.h for an example.
 *                              This function modifies SPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0  If the Configuration was readback successfully in the Host.
 *  - #1  If the memory transfer is not allowed or if the Configuration transfert failed.
 */
inline uint8_t adf7030_1__ReadConfig(
    adf7030_1_spi_info_t* pSPIDevInfo,
    mem_cfg_desc_t*       pCONFIG
)
{
    return(adf7030_1__XferConfig( pSPIDevInfo,
                                  pCONFIG,
                                  PHY_MEM_READ,
                                  0));
}


/**
 * @brief       Function call to verify the integrity of the patch
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pCONFIG         Pointer to Host configuration structure.
 *
 * @note                        Configuration data is provided by ADI RF Setting Calculator.
 *                              @sa USER_CONFIG_20.h for an example.
 *                              This function modifies SPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0  If the Configuration in the PHY matches the reference in the Host.
 *  - #1  If the memory transfer is not allowed or if the Configuration is different.
 */
inline uint8_t adf7030_1__VerifyConfig(
    adf7030_1_spi_info_t* pSPIDevInfo,
    mem_cfg_desc_t*       pCONFIG
)
{
    return(adf7030_1__XferConfig( pSPIDevInfo,
                                  pCONFIG,
                                  PHY_MEM_READ,
                                  1));
}


/**
 * @brief       Generic Configuration transfers function between Host and the Radio PHY
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pCONFIG         Pointer to Host configuration structure.
 *
 * @param [in]  bREAD_nWRITE    Boolean  -TRUE to write Config from Host to the Radio PHY
 *                                       -FALSE to read Config from Radio PHY back to Host
 *
 * @param [in]  bCHECK_ONLY     Boolean  -TRUE to check PHY Config versus Host Reference
 *                                       -FALSE [D] to perform READ_nWRITE operation
 *
 * @note                        This function can modify pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0  If the transfert was succesfull to the adf7030-1.
 *  - #1  If the memory transfer is not allowed or if the transfert failed.
 */
uint8_t adf7030_1__XferConfig(
    adf7030_1_spi_info_t* pSPIDevInfo,
    mem_cfg_desc_t*       pCONFIG,
    uint8_t                  bREAD_nWRITE,
    uint8_t                  bCHECK_ONLY
)
{
    uint8_t bEXIT;
    do{
  
        for(uint32_t i = 0; i < pCONFIG->nbBlock; i++)
        {
          if(( adf7030_1__XferDataBlock( pSPIDevInfo,
                                         &pCONFIG->BLOCKS[i],
                                         0,
                                         0,
                                         bREAD_nWRITE,
                                         bCHECK_ONLY )) )
          {
              return 1;
          }
        }

        /* Check if nested MEM Configuration transfert is present */
        if(pCONFIG->pNext)
        {
            /* Move pointer to MEM structure to the following one */
            pCONFIG = pCONFIG->pNext;
            bEXIT = 0;
        }
        else
        {
            bEXIT = 1;
        }

    }while(bEXIT == 0);

    return 0;
}


/**
 * @brief       Function call to write the Data block
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pBLOCK          Pointer to Host data block structure.
 *
 * @note                        This function modifies SPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0            If the Data block was written successfully in the Host.
 *  - #1  If the memory transfer is not allowed or if the Data block transfert failed.
 */
inline uint8_t adf7030_1__WriteDataBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBLOCK
)
{
    return(adf7030_1__XferDataBlock( pSPIDevInfo,
                                     pBLOCK,
                                     0,
                                     0,
                                     PHY_MEM_WRITE,
                                     0));
}


/**
 * @brief       Function call to read the Data block
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pBLOCK          Pointer to Host data block structure.
 *
 * @note                        This function modifies SPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0  If the Data block was readback successfully in the Host.
 *  - #1  If the memory transfer is not allowed or if the Data block transfert failed.
 */
inline uint8_t adf7030_1__ReadDataBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBLOCK
)
{
    return(adf7030_1__XferDataBlock( pSPIDevInfo,
                                     pBLOCK,
                                     0,
                                     0,
                                     PHY_MEM_READ,
                                     0));
}


/**
 * @brief       Function call to verify the integrity of Data block
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pBLOCK          Pointer to Host data block structure.
 *
 * @note                        This function modifies SPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0  If the Configuration in the PHY matches the reference in the Host.
 *  - #1  If the memory transfer is not allowed or if the Configuration is different.
 */
inline uint8_t adf7030_1__VerifyDataBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBLOCK
)
{
    return(adf7030_1__XferDataBlock( pSPIDevInfo,
                                     pBLOCK,
                                     0,
                                     0,
                                     PHY_MEM_READ,
                                     1));
}


/**
 * @brief       Function call to partially write a section of the Host Data block
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pBLOCK          Pointer to Host data block structure.
 *
 * @param [in]  Offset          Offset relative to the Host data block structure starting address.
 *
 * @param [in]  Size            Size in byte to be transfered from Host data block structure.
 *
 * @note                        This function modifies SPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0  If the Data block was written successfully in the Host.
 *  - #1  If the memory transfer is not allowed or if the Data block transfert failed.
 */
inline uint8_t adf7030_1__WritePartialDataBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBLOCK,
    uint16_t              Offset,
    uint16_t              Size
)
{
    return(adf7030_1__XferDataBlock( pSPIDevInfo,
                                     pBLOCK,
                                     Offset,
                                     Size,
                                     PHY_MEM_WRITE,
                                     0));
}


/**
 * @brief       Function call to partially read a section of the Host Data block
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pBLOCK          Pointer to Host data block structure.
 *
 * @param [in]  Offset          Offset relative to the Host data block structure starting address.
 *
 * @param [in]  Size            Size in byte to be transfered to Host data block structure.
 *
 * @note                        This function modifies SPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0  If the Data block was readback successfully in the Host.
 *  - #1  If the memory transfer is not allowed or if the Data block transfert failed.
 */
inline uint8_t adf7030_1__ReadPartiaDataBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBLOCK,
    uint16_t              Offset,
    uint16_t              Size
)
{
    return(adf7030_1__XferDataBlock( pSPIDevInfo,
                                     pBLOCK,
                                     Offset,
                                     Size,
                                     PHY_MEM_READ,
                                     0));
}


/**
 * @brief       Function call to verify the integrity of a portion of a Host Data block
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pBLOCK          Pointer to Host data block structure.
 *
 * @note                        This function modifies SPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0  If the Configuration in the PHY matches the reference in the Host.
 *  - #1  If the memory transfer is not allowed. or if the Configuration is different.
 */
inline uint8_t adf7030_1__VerifyPartialDataBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBLOCK,
    uint16_t              Offset,
    uint16_t              Size
)
{
    return(adf7030_1__XferDataBlock( pSPIDevInfo,
                                     pBLOCK,
                                     Offset,
                                     Size,
                                     PHY_MEM_READ,
                                     1));
}


/**
 * @brief       Generic Data block transfers function between Host and the Radio PHY
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pBLOCK          Pointer to Host configuration structure sub data block.
 *
 * @param [in]  Offset          Offset relative to the Host data block structure starting address.
 *
 * @param [in]  Size            Size in byte to be transfered from Host data block structure.
 *
 * @param [in]  bREAD_nWRITE    Boolean  -TRUE to write data block from Host to the Radio PHY
 *                                       -FALSE to read data block from Radio PHY back to Host
 *
 * @param [in]  bCHECK_ONLY     Boolean  -TRUE to check PHY data block versus Host Reference
 *                                       -FALSE [D] to perform READ_nWRITE operation
 *
 * @note                        This function can modify pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0   If the transfer was successful to the adf7030-1.
 *  - #1   If the memory transfer is not allowed or if the transfer failed.
 */
uint8_t adf7030_1__XferDataBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBLOCK,
    uint16_t              Byte_Offset,
    uint16_t              Byte_Size,
    uint8_t               bREAD_nWRITE,
    uint8_t               bCHECK_ONLY
)
{
    /* 8bits Mode SPI transfers mode by default */
    if(pBLOCK->WordXfer == 0)
    {
        /* Get Memory Block Address */
        uint32_t PHYAddr = pBLOCK->Addr + Byte_Offset;

        uint32_t size = (Byte_Size == 0) ? pBLOCK->Size : Byte_Size;

        adf7030_1_spi_pntr_t PNTR_id;
        int32_t PNTR_offset;
    
        /* Get good PNTRid from pSPIDevInfo->PHY_PNTR[] */
        adf7030_1__SPI_FindMMapPointer( pSPIDevInfo, PHYAddr, &PNTR_id, &PNTR_offset);
        
        /* Setup pointer to Host start of data */
        uint8_t * pHostMem = (uint8_t *)(pBLOCK->pData + Byte_Offset);
        uint8_t * pHostRef = NULL;
        
        if(bCHECK_ONLY)
        {
            pHostRef = pHostMem;
            pHostMem = NULL;
        }
        
        /* No valid PHY SPI pointer available, use 8bits access with no offset */
        if(PNTR_id == 0)
        {
            /* Change the SPI custom pointer 0 */
            if(adf7030_1__SPI_SetMMapCustomPntr0( pSPIDevInfo, PHYAddr ) )
            {
                return 1;
            }
            
            if(bREAD_nWRITE == 1)
            {
                /* Read from radio PHY */
                if(adf7030_1__SPI_rd_cmp_byte_p_a( pSPIDevInfo,
                                                   PNTR_CUSTOM0_ADDR,
                                                   size,
                                                   pHostMem,
                                                   pHostRef) )
                {
                    if(pBLOCK->Volatile)
                    {
                        /* In case block contains volatile data, force return SUCCESS for debug */ 
                        //\todo Return different value
                        return 0;
                    }
                    else
                    {
                      return 1;
                    }
                }                    
            }else{
                /* Write to radio PHY */
                if(adf7030_1__SPI_wr_byte_p_a( pSPIDevInfo, PNTR_CUSTOM0_ADDR, size, pHostMem) )
                {
                    return 1;
                }  
            }
        }
        /* Valid PHY SPI pointer available, use 8bits access with pointer offset */
        else
        {
            if(bREAD_nWRITE == 1)
            {
                /* Read from radio PHY */
                if(adf7030_1__SPI_rd_cmp_byte_b_a( pSPIDevInfo,
                                                   PNTR_id,
                                                   PNTR_offset,
                                                   size,
                                                   pHostMem,
                                                   pHostRef) )
                {
                    if(pBLOCK->Volatile)
                    {
                        /* In case block contains volatile data, force return SUCCESS for debug */ 
                        //\todo Return different value
                        return 0;
                    }
                    else
                    {
                      return 1;
                    }
                }                    
            }else{
                /* Write to radio PHY */
                if(adf7030_1__SPI_wr_byte_b_a( pSPIDevInfo, PNTR_id, PNTR_offset, size, pHostMem) )
                {
                    return 1;
                }  
            }
        }
    }
    /* This block only support 32bits Mode SPI transfers, Let us just use block modes */
    else
    {
        /* Intercept non-word transfer error due to Byte_Offset */
        if(Byte_Offset & 0x3)
        {
        	pSPIDevInfo->eXferResult = ADF7030_1_INVALID_OPERATION;
        	return 1;
        }

        /* Get Memory Block Address */
        uint32_t PHYAddr = pBLOCK->Addr + Byte_Offset;

        uint32_t size = (Byte_Size == 0) ? (pBLOCK->Size >> 2) : (Byte_Size >> 2);

        /* Setup pointer to Host start of data */
        uint32_t * pHostMem = (uint32_t *)(pBLOCK->pData + Byte_Offset);
        uint32_t * pHostRef = NULL;

        if(bCHECK_ONLY)
        {
            pHostRef = pHostMem;
            pHostMem = NULL;
        }
        
        if(bREAD_nWRITE == 1)
        {
            /* Read from radio PHY */
            if(adf7030_1__SPI_rd_cmp_word_b_a( pSPIDevInfo,
                                               PHYAddr,
                                               size,
                                               pHostMem,
                                               pHostRef) )
            {
                if(pBLOCK->Volatile)
                {
                      /* In case block contains volatile data, force return SUCCESS for debug */ 
                      //\todo Return different value
                      return 0;
                }
                else
                {
                      return 1;
                }
            }                    
        }else{
            /* Write to radio PHY */
            if(adf7030_1__SPI_wr_word_b_a( pSPIDevInfo, PHYAddr, size, pHostMem) )
            {
                return 1;
            }  
        } 
    }
    
    return 0;
}

/**
 * @brief       Generic memory transfers function between Host shadow and the Radio PHY
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pMEM            Descriptor to the paired Host/PHY memory region.
 *
 * @param [in]  bREAD_nWRITE    Boolean  -TRUE to write from Host shadow to the Radio PHY
 *                                       -FALSE to read from Radio PHY back to Host shadow
 *
 * @note                        This function assume the following:
 *                              - Both Host shadow block (&(pMEM->pBuff)) and 
 *                              PHY memory blocks (pMEM->PhyAddr) are starting at 
 *                              a word alligned location.
 *                              - pSPIDevInfo->PHY_PNTR[] is in sync with PHY value.
 *
 * @return      Status
 *  - #0    If the transfert was succesfull to the adf7030-1.
 *  - #1    [D] If the transfert failed.
 */
uint8_t adf7030_1__SPI_Xfer_Mem(
    adf7030_1_spi_info_t* pSPIDevInfo,
    mem_desc_t*           pMEM,
    uint8_t                  bREAD_nWRITE
)
{
    uint8_t bEXIT;
    do{
  
        if(!pMEM->XferMode)
        {
          /* Block Transfers Mode */
                
            uint32_t PHYAddr = pMEM->PhyAddr + pMEM->BlockXfer.Offset;
            
            adf7030_1_spi_pntr_t PNTR_id;
            int32_t PNTR_offset;
            
            /* Get good PNTRid from pSPIDevInfo->PHY_PNTR[] */
            adf7030_1__SPI_FindMMapPointer( pSPIDevInfo, PHYAddr, &PNTR_id, &PNTR_offset);
                
            /* 32bits Mode SPI transfers */
            if(!(PHYAddr & 3) && !(pMEM->BlockXfer.Size & 3))
            { 
                /* Setup pointer to Host start of data */
                uint32_t * pHostMem;
                pHostMem = (uint32_t *)pMEM->pBuff + (pMEM->BlockXfer.Offset >> 2);
    
                /* Valid PHY SPI pointer available, use 32bits bloc pointer access */
                if((PNTR_id) && !(PNTR_offset >> 2))
                {     
                    if(bREAD_nWRITE == 1)
                    {
                        /* Read from radio PHY */
                        if(adf7030_1__SPI_rd_word_b_p( pSPIDevInfo, PNTR_id, pMEM->BlockXfer.Size >> 2, pHostMem) )
                        {
                            return 1;
                        }                    
                    }else{
                        /* Write to radio PHY */
                        if(adf7030_1__SPI_wr_word_b_p( pSPIDevInfo, PNTR_id, pMEM->BlockXfer.Size >> 2, pHostMem) )
                        {
                            return 1;
                        }  
                    }
                }
                /* No valid PHY SPI pointer available, use 32bits bloc address access */
                else
                {
                    if(bREAD_nWRITE == 1)
                    {
                        /* Read from radio PHY */
                        if(adf7030_1__SPI_rd_word_b_a( pSPIDevInfo, PHYAddr, pMEM->BlockXfer.Size >> 2, pHostMem) )
                        {
                            return 1;
                        }                    
                    }else{
                        /* Write to radio PHY */
                        if(adf7030_1__SPI_wr_word_b_a( pSPIDevInfo, PHYAddr, pMEM->BlockXfer.Size >> 2, pHostMem) )
                        {
                            return 1;
                        }  
                    }
                }
            }
            /* 8bits Mode SPI transfers */
            else
            {
                /* Setup pointer to Host start of data */
                uint8_t * pHostMem;
                pHostMem = (uint8_t *)pMEM->pBuff + pMEM->BlockXfer.Offset;
    
                /* No valid PHY SPI pointer available, use 8bits access with no offset */
                if(PNTR_id == 0)
                {
                    /* Change the SPI custom pointer 0 */
                    if(adf7030_1__SPI_SetMMapCustomPntr0( pSPIDevInfo, PHYAddr ) )
                    {
                        return 1;
                    }
                    
                    /* Copy Host MEM memory block to PHY memory block via the SPI */
                    if(bREAD_nWRITE == 1)
                    {
                        /* Read from radio PHY */
                        if(adf7030_1__SPI_rd_byte_p_a( pSPIDevInfo, PNTR_CUSTOM0_ADDR, pMEM->BlockXfer.Size, pHostMem) )
                        {
                            return 1;
                        }
                    }else{
                        /* Write to radio PHY */
                        if(adf7030_1__SPI_wr_byte_p_a( pSPIDevInfo, PNTR_CUSTOM0_ADDR, pMEM->BlockXfer.Size, pHostMem) )
                        {
                            return 1;
                        }  
                    }
                }
                /* Valid PHY SPI pointer available, use 8bits access with pointer offset */
                else
                {
                    /* Copy Host MEM memory block to PHY memory block via the SPI */
                    if(bREAD_nWRITE == 1)
                    {
                        /* Read from radio PHY */
                        if(adf7030_1__SPI_rd_byte_b_a( pSPIDevInfo, PNTR_id, PNTR_offset, pMEM->BlockXfer.Size, pHostMem) )
                        {
                            return 1;
                        }
                    }else{
                        /* Write to radio PHY */
                        if(adf7030_1__SPI_wr_byte_b_a( pSPIDevInfo, PNTR_id, PNTR_offset, pMEM->BlockXfer.Size, pHostMem) )
                        {
                            return 1;
                        }  
                    }
                }
            }
        }
        else
        {
          /* Random Transfers Mode */
          
        }
        
        
        /* Check if nested MEM transfert is present */
        if(pMEM->pNextBloc)
        {
            /* Move pointer to MEM structure to the following one */
            pMEM = pMEM->pNextBloc;
            bEXIT = 0;
        }
        else
        {
            bEXIT = 1;
        } 
    
    }while(bEXIT == 0);
  
    return 0;
}


/**
 * @brief       Write a single 32bits memory location via the SPI
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  Addr            PHY Address location at which the SPI will write.
 *
 * @param [in]  Value           32bits Value to write.
 *
 * @note                        This function does not support "SPI Error Status"
 *
 * @return      None
 */
inline void adf7030_1__SPI_SetMem32(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint32_t              Value
)
{
    adf7030_1__SPI_SetBytes( pSPIDevInfo, Addr, Value, 4, NULL);
}

/**
 * @brief       Read a single 32bits memory location via the SPI
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  Addr            PHY Address location at which the SPI will write.
 *
 * @note                        This function does not support "SPI Error Status"
 *
 * @return      Value           32bits readback value.
 */
inline uint32_t adf7030_1__SPI_GetMem32(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr
)
{  
    return(adf7030_1__SPI_GetBytes( pSPIDevInfo, Addr, 4, NULL)); 
}

/**
 * @brief       Write a single 24bits memory location via the SPI
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  Addr            PHY Address location at which the SPI will write.
 *
 * @param [in]  Value           24bits Value to write.
 *
 * @note                        This function is changing SPI custom pointer0, 
 *                              pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR] and the PHY
 *                              SPI pntr are overwritten. It also does not support
 *                              "SPI Error Status".
 *
 * @return      None
 */
inline void adf7030_1__SPI_SetMem24(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint32_t              Value
)
{
    adf7030_1__SPI_SetBytes( pSPIDevInfo, Addr, (0xFFFFFF & Value), 3, NULL);
}

/**
 * @brief       Read a 24bits memory location via the SPI
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  Addr            PHY Address location at which the SPI will write.
 *
 * @note                        This function is changing SPI custom pointer0, 
 *                              pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR] and the PHY
 *                              SPI pntr are overwritten. It also does not support
 *                              "SPI Error Status".
 *
 * @return      Value           24bits readback value.
 */
inline uint32_t adf7030_1__SPI_GetMem24(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr
)
{
    return((0xFFFFFF & adf7030_1__SPI_GetBytes( pSPIDevInfo, Addr, 3, NULL)));
}

/**
 * @brief       Write a single 16bits memory location via the SPI
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  Addr            PHY Address location at which the SPI will write.
 *
 * @param [in]  Value           16bits Value to write.
 *
 * @note                        This function is changing SPI custom pointer0, 
 *                              pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR] and the PHY
 *                              SPI pntr are overwritten. It also does not support
 *                              "SPI Error Status".
 *
 * @return      None
 */
inline void adf7030_1__SPI_SetMem16(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint16_t              Value
)
{
    adf7030_1__SPI_SetBytes( pSPIDevInfo, Addr, Value, 2, NULL);
}

/**
 * @brief       Read a 16bits memory location via the SPI
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  Addr            PHY Address location at which the SPI will write.
 *
 * @note                        This function is changing SPI custom pointer0, 
 *                              pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR] and the PHY
 *                              SPI pntr are overwritten. It also does not support
 *                              "SPI Error Status".
 *
 * @return      Value           16bits readback value.
 */
inline uint16_t adf7030_1__SPI_GetMem16(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr
)
{
    return((uint16_t)adf7030_1__SPI_GetBytes( pSPIDevInfo, Addr, 2, NULL));
}

/**
 * @brief       Write a single 8bits memory location via the SPI
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  Addr            PHY Address location at which the SPI will write.
 *
 * @param [in]  Value           8bits Value to write.
 *
 * @note                        This function is changing SPI custom pointer0, 
 *                              pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR] and the PHY
 *                              SPI pntr are overwritten. It also does not support
 *                              "SPI Error Status".
 *
 * @return      None
 */
inline void adf7030_1__SPI_SetMem8(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint8_t               Value
)
{
    adf7030_1__SPI_SetBytes( pSPIDevInfo, Addr, Value, 1, NULL);
}

/**
 * @brief       Read a 8bits memory location via the SPI
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  Addr            PHY Address location at which the SPI will write.
 *
 * @note                        This function is changing SPI custom pointer0, 
 *                              pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR] and the PHY
 *                              SPI pntr are overwritten. It also does not support
 *                              "SPI Error Status".
 *
 * @return      Value           8bits readback value.
 */
inline uint8_t adf7030_1__SPI_GetMem8(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr
)
{
    return((uint8_t)adf7030_1__SPI_GetBytes( pSPIDevInfo, Addr, 1, NULL));
}


/**
 * @brief       Writes "nbBytes" memory location via the SPI
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  Addr            PHY (Byte)Address location at which the SPI will write.
 *
 * @param [in]  Value           Value to write.
 *
 * @param [in]  nbBytes         Number of Bytes to writes (1, 2, 3 or 4);
 *
 * @param [in|out]  pRegVal     Pointer to "last full 32bits value" of Addr location
 *                              in the event of Addr not being within byte accessible
 *                              range of the memory map.
 *
 * @note                        This function is changing SPI custom pointer0, 
 *                              pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR] and the PHY
 *                              SPI pntr are overwritten. It also does not support
 *                              "SPI Error Status".
 *
 * @return      None
 * 
 * @sa  adf7030_1__SPI_GetBytes()
 */
void adf7030_1__SPI_SetBytes(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint32_t              Value,
    uint32_t              nbBytes,
    uint32_t*             pRegVal
)
{   
    /* Get current SPI custom pntr 0 value */ 
    int32_t AddrDiff = (int32_t)Addr - (int32_t)(pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR]);
    
    uint8_t Offset;
    
    /* Check if byte access is allowed at this address */
    uint8_t bByteAccess = adf7030_1__MEM_CheckByteAccess(Addr);
    uint8_t bInrange = ((AddrDiff >= 0) & (AddrDiff < (255 + nbBytes))) ? 1 : 0;
    
    if((bInrange == 1) && (bByteAccess == 1))
    {
      /* Here PNTR_CUSTOM0_ADDR is within range, just change the offset */
        Offset = (uint8_t)AddrDiff; 
    }else{
      /* Here PNTR_CUSTOM0_ADDR is not within range, changing it, we shall */
            
        if(((Addr >> 2) << 2) != pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR]) //Fix for double setup of pntr0
        {
            /* Setup the PHY Radio SPI pointer 0 address to Addr */
            if ( adf7030_1__SPI_SetMMapCustomPntr0( pSPIDevInfo, ((Addr >> 2) << 2)) )
            {
            /* Exit if error */
                return ;
            }
        }
          
        Offset = Addr & 0x03;
    }
    
    if(bByteAccess == 0)
    {
        /* Construct the byte mask depending on nbBytes */
        uint32_t msk = (0x80 << ((nbBytes - 1) << 3));
        msk |= ~msk;
        msk <<= (Offset << 3);
        
        uint32_t tmp_reg = 0;
        
        /* Clear masked bytes from pRegVal */
        if(pRegVal)
        {
            tmp_reg = *pRegVal & ~msk;
        }
        
        /* Shift Value by the Offset */
        Value <<= (Offset << 3);
        
        /* Mix value in full reg */
        Value |= tmp_reg;
      
        /* Byte access is not permitted, do word access instead */
        adf7030_1__SPI_wr_word_b_p( pSPIDevInfo, PNTR_CUSTOM0_ADDR, 1, &Value);

    }
    else
    {
        if((Offset == 0) && (nbBytes == 4))
        {
          /* Write the 32bit alligned data */ 
            adf7030_1__SPI_wr_word_b_p( pSPIDevInfo, PNTR_CUSTOM0_ADDR, 1, &Value);
        }else{
          /* Write the nbBytes data */
            adf7030_1__SPI_wr_byte_b_a( pSPIDevInfo, PNTR_CUSTOM0_ADDR, Offset, nbBytes, (uint8_t *)&Value);
        }
    }
}

/**
 * @brief       Read "nbBytes" memory location via the SPI
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  Addr            PHY Address location at which the SPI will write.
 *
 * @param [in]  nbBytes         Number of Bytes to read (1, 2, 3 or 4);
 *
 * @param [in|out]  pRegVal     Pointer to full 32bits register writeback location
 *                              in the event of Addr not being within byte accessible
 *                              range of the memory map.
 *
 * @note                        This function is changing SPI custom pointer0, 
 *                              pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR] and the PHY
 *                              SPI pntr are overwritten. It also does not support
 *                              "SPI Error Status".
 *
 * @return      Value           32bits readback value.
 * 
 * @sa  adf7030_1__SPI_SetBytes()
 */
uint32_t adf7030_1__SPI_GetBytes(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint32_t              nbBytes,
    uint32_t*             pRegVal
)
{
    /* SPI transfert result */
	uint8_t Result = 0;

      /* Get current SPI custom pntr 0 value */ 
    int32_t AddrDiff = (int32_t)Addr - (int32_t)(pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR]);
    
    uint8_t Offset;
    
    /* Check if byte access is allowed at this address */
    uint8_t bByteAccess = adf7030_1__MEM_CheckByteAccess(Addr);
    uint8_t bInrange = ((AddrDiff >= 0) & (AddrDiff < (255 + nbBytes))) ? 1 : 0;
    
    if((bInrange == 1) && (bByteAccess == 1))
    {
      /* Here PNTR_CUSTOM0_ADDR is within range, just change the offset */
        Offset = (uint8_t)AddrDiff;
      
    }else{
      /* Here PNTR_CUSTOM0_ADDR is not within range, changing it, we shall */
            
        if(((Addr >> 2) << 2) != pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR]) //Fix for double setup of pntr0
        {
            /* Setup the PHY Radio SPI pointer 0 address to Addr */
            Result = adf7030_1__SPI_SetMMapCustomPntr0( pSPIDevInfo, ((Addr >> 2) << 2));
            
            /* Exit if error */
            if(Result)
            {
                return(0);
            }
        }

        Offset = Addr & 0x03;
    }
    
    uint32_t RetVal;
    
    if(bByteAccess == 0)
    {
        /* Byte access is not permitted, do word access instead */
        Result = adf7030_1__SPI_rd_word_b_p( pSPIDevInfo, PNTR_CUSTOM0_ADDR, 1, &RetVal);
        
        if(pRegVal)
        {
            /* Return the value of full 32bit register*/
            *pRegVal = RetVal;
        }
        
        /* Construct the byte mask depending on nbBytes */
        uint32_t msk = (0x80 << ((nbBytes - 1) << 3));
        msk |= ~msk;
        
        /* Realign value for bit 0 */
        RetVal >>= (Offset << 3);
        
        /* Apply msak to readback value */
        RetVal &= msk;
    }
    else
    {
      /* Byte access is permitted */
        if((Offset == 0) && (nbBytes == 4))
        {
            /* Read the 32bit alligned data */  
            Result = adf7030_1__SPI_rd_word_b_p( pSPIDevInfo, PNTR_CUSTOM0_ADDR, 1, &RetVal);
            
            if(pRegVal)
            {
                /* Return the value of full 32bit register*/
                *pRegVal = RetVal;
            }
        }else{
            /* Read the nbBytes data */
            Result = adf7030_1__SPI_rd_byte_b_a( pSPIDevInfo, PNTR_CUSTOM0_ADDR, Offset, nbBytes, (uint8_t *)&RetVal);
        }
    }
    
    /* Exit if error */
    pSPIDevInfo->eXferResult = ADF7030_1_SUCCESS;
    
    return(RetVal);
}

/**
 * @brief       Generic setting of a Radio PHY bitfield to a value via the SPI
 *
 * @note                        This function does not support "SPI Error Status".
 *                              Call adf7030_1__SPI_GetBytes and adf7030_1__SPI_GetBytes
 *                              and apply appropriate masking to the SPI value readback.
 *
 * @sa  adf7030_1__SPI_GetBytes().
 * @sa  adf7030_1__SPI_SetBytes().
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  Addr            32bit alligned PHY Address location of the bitfield.
 *
 * @param [in]  Pos             Starting bit position withing the 32bits word.
 *
 * @param [in]  Size            Bit lenght of the bitfield.
 *
 * @param [in]  Val             Bitfield value.
 *
 * @return      None            
 */
void adf7030_1__SPI_SetField(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint32_t              Pos,
    uint32_t              Size,
    uint32_t              Val
)
{
    uint32_t RegVal = 0;
      
    /* Compute Bitfields parameters */
    uint32_t fStartByte = Pos >> 3;
    uint32_t fPos_fromStartByte = Pos - (fStartByte << 3);
    uint32_t fLenBytes = ((Size - 1) >> 3) + 1;
    
    /* Check for special cases where we can skip adf7030_1__SPI_GetBytes() */
    uint32_t fMsk = ((1 << Size) - 1);
    
    uint32_t fVal;
    
    if(((fMsk % 255) == 0) && ((Pos % 8) == 0) && (adf7030_1__MEM_CheckByteAccess(Addr) == 1))
    {
        /* This should cover the following bitfield mask values 
           0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
           0xFFFF0000, 0x00FFFF00, 0x0000FFFF
           0xFFFFFF00, 0x00FFFFFF,
           0xFFFFFFFF... 0 is an invalid value. */
        fVal = 0;
    }
    else
    {
        /* Read Bitfield vis SPI */
        fVal = adf7030_1__SPI_GetBytes( pSPIDevInfo, Addr + fStartByte, fLenBytes, &RegVal);
    }

    /* Compute the Mask */ 
    fMsk <<= fPos_fromStartByte;
    
    /* Bound Val to the Byte Position */
    Val <<= fPos_fromStartByte;
    Val &= fMsk;
    
    /* And SPI returned value with inverted Mask */
    fVal &= (~fMsk);
    
    /* Or SPI returned value with Val*/
    fVal |= Val;
    
    /* Write back Bitfield vis SPI */
    adf7030_1__SPI_SetBytes( pSPIDevInfo, Addr + fStartByte, fVal, fLenBytes, &RegVal);
}


/**
 * @brief       Generic readback of a bitfield value from the Radio PHY via the SPI
 *
 * @note                        This function does not support "SPI Error Status".
 *                              Call adf7030_1__SPI_GetBytes and apply appropriate
 *                              masking to the SPI value readback.
 *
 * @sa  adf7030_1__SPI_GetBytes().
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  Addr            32bit alligned PHY Address location of the bitfield.
 *
 * @param [in]  Pos             Starting bit position withing the 32bits word.
 *
 * @param [in]  Size            Bit lenght of the bitfield.
 *
 * @return      fVal            Bitfield value
 */
uint32_t adf7030_1__SPI_GetField(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint32_t              Addr,
    uint32_t              Pos,
    uint32_t              Size
)
{
    /* Compute Bitfields parameters */
    uint32_t fStartByte = Pos >> 3; // eq /8
    uint32_t fPos_fromStartByte = Pos - (fStartByte << 3);

	uint32_t fEndBit = Pos + Size;
    uint32_t fEndByte = fEndBit >> 3; // eq /8
    uint8_t fRest = (fEndBit - ( fEndByte << 3))?(1):(0);

    //uint32_t fLenBytes = ((Size - 1) >> 3) + 1; // Bug when fields are between two bytes
    uint32_t fLenBytes = fEndByte - fStartByte + fRest;

    /* Read Bitfield vis SPI */
    uint32_t fVal = adf7030_1__SPI_GetBytes( pSPIDevInfo, Addr + fStartByte, fLenBytes, NULL);
    
    /* Compute the Mask */ 
    uint32_t fMsk = ((1 << Size) - 1) << fPos_fromStartByte;
    
    /* And SPI returned value with Mask */
    fVal &= fMsk;
    
    /* Shift right by fPos_fromStartByte */
    fVal >>= fPos_fromStartByte;
    
    /* Return Bitfield Value */
    return(fVal);
}


/**
 * @brief       Generic Function to check if byte rw operation is permitted
 *
 * @note        By default, all internal memory of the PHY Radio can be accessed
 *              at byte level. This includes ROM, RAM and Radio Peripheral Registers.
 *              APB Peripherals only support 32bits word access.
 *
 * @param [in]  Addr            32bit byte address location.
 *
 * @return      None
 */
inline uint8_t adf7030_1__MEM_CheckByteAccess(
    uint32_t nAddr
)
{
    /*  */ 
    if(nAddr > 0x40000000UL){
        if((nAddr >= 0x40004000UL) && (nAddr <= 0x400042E4UL))
        {
            /* Baseband modem configuration registers */ 
            return(1);
        }
        else
        {
            /* Radio auxillary peripherals connected via APBs bus */ 
            return(0);
        }
    }
    else
    {
        /* ROM, RAM etc */
        return(1);
    }
}


#endif /* _ADF7030_1__MEM_C_ */

/** @} */ /* End of group adf7030-1__mem Memory Interface */
/** @} */ /* End of group adf7030-1 adf7030-1 Driver */
