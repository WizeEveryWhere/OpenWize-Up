/*!
 *****************************************************************************
  @file:    adf7030-1__patch.c
  
  @brief:    adf7030-1 Patch Memory Access Functions.
                - SPI Xfer with between Host and Radio.
                - Inject, Eject
  
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

/** \addtogroup adf7030-1__patch Patch Interface
 *  @{
 */
#ifndef _ADF7030_1__PATCH_C_
/*! \cond PRIVATE */
#define _ADF7030_1__PATCH_C_

#include "adf7030-1__patch.h"

#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "adf7030-1_reg.h"
#include "adf7030-1__common.h"
#include "adf7030-1__irq.h"
#include "adf7030-1__mem.h"
#include "adf7030-1__spi.h"
#include "adf7030-1__state.h"

#include "adf7030-1_phy.h"

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
 * @brief       Function call to write patch to Radio PHY
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pPatch          Pointer to Host "patch" descriptor.
 *
 * @note                        This function only transfert patch data to the PHY. 
 *                              Patch must be enabled at a later stage by calling the 
 *                              adf7030_1__InjectPatch() function.
 *                              This function modifies pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0    If the patch was written successfully in the Host.
 *  - #1    [D] If the patch transfert failed.
 */
uint8_t adf7030_1__WritePatch(
    adf7030_1_spi_info_t* pSPIDevInfo,
    patch_desc_t*         pPATCH
)
{
    return(adf7030_1__XferPatch( pSPIDevInfo,
                                 pPATCH,
                                 PHY_MEM_WRITE,
                                 0));
}

/**
 * @brief       Function call to readback the patch from Radio PHY
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in|out]  pPatch      Pointer to Host "patch" descriptor.
 *
 * @note                        This function only readback patch data from the PHY. 
 *                              pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR] could be modified.
 *
 * @return      Status
 *  - #0     If the patch was readback successfully in the Host.
 *  - #1    [D] If the patch transfert failed.
 */
uint8_t adf7030_1__ReadPatch(
    adf7030_1_spi_info_t* pSPIDevInfo,
    patch_desc_t*         pPATCH
)
{
    return(adf7030_1__XferPatch( pSPIDevInfo,
                                 pPATCH,
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
 * @param [in]  pPatch          Pointer to Host "patch" descriptor.
 *
 * @note                        This function only disable the patch, it can be 
 *                              re-enable at a later stage by calling the 
 *                              adf7030_1__InjectPatch() function.
 *                              This function modifies pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0    If the patch in the PHY matches the reference in the Host.
 *  - #1    [D] If the patch is different.
 */
uint8_t adf7030_1__VerifyPatch(
    adf7030_1_spi_info_t* pSPIDevInfo,
    patch_desc_t*         pPATCH
)
{
    return(adf7030_1__XferPatch( pSPIDevInfo,
                                 pPATCH,
                                 PHY_MEM_READ,
                                 1));
}

/**
 * @brief       Function call to perform PHY Radio patch self-checking
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pPatch          Pointer to Host "patch" descriptor.
 *
 * @param [in]  bCHECK          Boolean  -TRUE to verify the Inject sequence in the PHY
 *                                       -FALSE to discard any verifications
 *
 * @param [in]  eCmd            PHY Radio Command to execute to trigger the start
 *                              of the PHY Radio selftest internal routine.
 *                              See adf7030_1_radio_state_e enum for available commands.
 *
 * @param [in]  pCHECKSUM_CFG   Pointer to Host "patch" checksum configuration descriptor.
 *                              If NULL, the adf7030_1__SelfCheckPatch() will use the
 *                              default provided with the "patch" header file.
 *
 * @return      Status
 *  - #0       If the patch intergrity is correct.
 *  - #1       If the patch intergrity is incorrect or if the patch is different.
 */
uint8_t adf7030_1__SelfCheckPatch(
    adf7030_1_spi_info_t*   pSPIDevInfo,
    patch_desc_t*           pPATCH,
    uint8_t                 bCHECK,
    adf7030_1_radio_state_e eCmd,
    patch_checksum_desc_t*  pCHECKSUM_CFG
)
{
    data_blck_desc_t* pPATCH_SelfcheckBlock = &pPATCH->BLOCKS[pPATCH->CheckBlockId];

    /* Download the patch selfcheck code */
    if( adf7030_1__WritePatchBlock( pSPIDevInfo, pPATCH_SelfcheckBlock) )
    {
        return 1;
    }

    if(bCHECK == 1)
    {
        /* Verify the patch selfcheck code integrity */
        if( adf7030_1__VerifyPatchBlock( pSPIDevInfo, pPATCH_SelfcheckBlock ) )
        {
            return 1;
        }
    }

    /* Setup patch selfcheck checksum descriptor */
    if(pCHECKSUM_CFG == NULL) pCHECKSUM_CFG = pPATCH->pChecksum;
    
    /* Configure patch selfcheck */
    if( adf7030_1__WritePatchCheckumInfo( pSPIDevInfo, pPATCH, pCHECKSUM_CFG ) )
    {
        return 1;
    }

    /* Enabling the patch selfcheck function */
    if( adf7030_1__SetupPatch( pSPIDevInfo, pPATCH->CheckSeq, 1 ) )
    {
        return 1;
    }
    /* Trigger the Offline calibration selfcheck execution */
    return adf7030_1__STATE_PhyCMD( pSPIDevInfo, eCmd );
}

/**
 * @brief       Function call to setup the patch
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
   * @param [in]  SetupSeq        Setup patch sequence desired:
 *                               - InjectSeq to enable the patch
 *                               - EjectSeq to disable the patch
 *                               - CheckSeq to enable patch selfcheck and auto inject
 *
 * @param [in]  bCHECK          Boolean  -TRUE to verify the Inject sequence in the PHY
 *                                       -FALSE to discard any verifications
 *
 * @note                        This function assume the following the patch has
 *                              already been transfered and that its integrity is
 *                              valid. User can check the Ram for the patch presence
 *                              by calling the function adf7030_1__VerifyPatch().
 *
 * @return      Status
 *  - #0     If the Inject sequence was succesfull.
 *  - #1     If the Inject sequence was not correct or if the Inject failed.
 */
uint8_t adf7030_1__SetupPatch(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint16_t              SetupSeq,
    uint8_t               bCHECK
)
{
    if(SetupSeq == 0)
    {
        return 1;
    }
    
    /* Write Inject sequence to radio PHY */
    uint32_t Seq = BRAM_BASE + SetupSeq;

    if(( adf7030_1__SPI_wr_word_b_a( pSPIDevInfo,
    								 SM_DATA_CALIBRATION_Addr,
                                     1,
                                     &Seq )))
    {
        return 1;
    }
    
    /* Readback Inject Sequence number if requested */
    if(bCHECK == 1)
    {
        uint32_t Seq_rb;
        
        if(( adf7030_1__SPI_rd_word_b_a( pSPIDevInfo,
        								 SM_DATA_CALIBRATION_Addr,
                                         1,
                                         &Seq_rb )) )
        {
            return 1;
        }
        
        /* Compare Setup patch sequence with the readback sequence */
        if(Seq != Seq_rb)
        {
            return 1;
        }
    }
      
    return 0;
}


/**
 * @brief       Function to write a single Patch block to the PHY Radio
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pBlock          Pointer to single Host patch memory block.
 *
 * @note                        This function modifies pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0    If the Patch block has been written to the PHY.
 *  - #1    [D] If the Patch block could not be written.
 */
uint8_t adf7030_1__WritePatchBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBlock
)
{
    return(adf7030_1__XferPatchBlock( pSPIDevInfo,
                                      pBlock,
                                      PHY_MEM_WRITE,
                                      0 ));
}


/**
 * @brief       Function to read a single Patch block to the PHY Radio
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in|out] pBlock       Pointer to single Host patch memory block.
 *
 * @note                        This function modifies pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0    If the Patch block has been read back into pBlock->pdata.
 *  - #1    [D] If the Patch block could not be read.
 */
uint8_t adf7030_1__ReadPatchBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBlock
)
{    
    return(adf7030_1__XferPatchBlock( pSPIDevInfo,
                                      pBlock,
                                      PHY_MEM_READ,
                                      0 ));
}


/**
 * @brief       Function to verify content of a single Patch block in the PHY.
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pBlock          Pointer to single Host patch memory block.
 *
 * @note                        This function modifies pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0    If the Patch block in the PHY matches the reference in the Host.
 *  - #1    [D] If the Patch block of the PHY is different than the reference in the Host.
 */
uint8_t adf7030_1__VerifyPatchBlock(
    adf7030_1_spi_info_t* pSPIDevInfo,
    data_blck_desc_t*     pBlock
)
{
    return(adf7030_1__XferPatchBlock( pSPIDevInfo,
                                      pBlock,
                                      PHY_MEM_READ,
                                      1 ));
}


/**
 * @brief       Generic Patch transfers function between Host and the Radio PHY
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in|out]  pPatch      Pointer to Host "patch" descriptor.
 *
 * @param [in]  bREAD_nWRITE    Boolean  -TRUE to write Patch from Host to the Radio PHY
 *                                       -FALSE to read Patch from Radio PHY back to Host
 *
 * @param [in]  bCHECK_ONLY     Boolean  -TRUE to check PHY patch versus Host Reference
 *                                       -FALSE [D] to perform READ_nWRITE operation
 *
 * @note                        This function modifies pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0    If the transfert was succesfull to the adf7030-1.
 *  - #1    [D] If the transfert failed.
 */
uint8_t adf7030_1__XferPatch(
    adf7030_1_spi_info_t* pSPIDevInfo, 
    patch_desc_t*         pPATCH,
    uint8_t               bREAD_nWRITE,
    uint8_t               bCHECK_ONLY
)
{   
    for(uint32_t idx = 0; idx < pPATCH->nbBlock; idx++)
    {
        /* Discard transfert of Patch Informations or Selfcheck section */
        if((idx != pPATCH->InfoBlockId) && (idx != pPATCH->CheckBlockId))
        {
            if(adf7030_1__XferPatchBlock( pSPIDevInfo, 
                                          (data_blck_desc_t*)&pPATCH->BLOCKS[idx],
                                          bREAD_nWRITE,
                                          bCHECK_ONLY ) )
            {
                return 1;
            }
        }    
    }  
    
    return 0;
}


/**
 * @brief       Generic Patch Section transfer function between Host and the Radio PHY
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in|out]  pBLOCK      Pointer to Host "patch" sub-block descriptor.
 *
 * @param [in]  bREAD_nWRITE    Boolean  -TRUE to write Patch from Host to the Radio PHY
 *                                       -FALSE to read Patch from Radio PHY back to Host
 *
 * @param [in]  bCHECK_ONLY     Boolean  -TRUE to check PHY patch versus Host Reference
 *                                       -FALSE [D] to perform READ_nWRITE operation
 *
 * @note                        This function modifies pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0    If the transfert was succesfull to the adf7030-1.
 *  - #1    [D] If the transfert failed.
 */
uint8_t adf7030_1__XferPatchBlock(
    adf7030_1_spi_info_t* pSPIDevInfo, 
    data_blck_desc_t*     pBLOCK,
    uint8_t               bREAD_nWRITE,
    uint8_t               bCHECK_ONLY
)
{     
    /* Block Transfers Mode */     
    uint32_t PHYAddr = pBLOCK->Addr;
      
    adf7030_1_spi_pntr_t PNTR_id;
    int32_t PNTR_offset;
    
    /* Get good PNTRid from pSPIDevInfo->PHY_PNTR[] */
    adf7030_1__SPI_FindMMapPointer( pSPIDevInfo, PHYAddr, &PNTR_id, &PNTR_offset);
        
    /* Lets just use 8bits Mode SPI transfers for patch*/
    {
        /* Setup pointer to Host start of data */
        uint8_t * pHostMem = (uint8_t *)pBLOCK->pData;
        uint8_t * pHostRef = NULL;
        
        if(bCHECK_ONLY)
        {
            pHostRef = pHostMem;
            pHostMem = NULL;
        }
        
        uint32_t size = pBLOCK->Size;
    
        /* No valid PHY SPI pointer available, use 8bits access with no offset */
        if(PNTR_id == 0)
        {
            /* Change the SPI custom pointer 0 */
            if(adf7030_1__SPI_SetMMapCustomPntr0( pSPIDevInfo,
                                                  PHYAddr ) )
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
                    return 1;
                }
            }
            else
            {
                /* Write to radio PHY */
                if(adf7030_1__SPI_wr_byte_p_a( pSPIDevInfo,
                                               PNTR_CUSTOM0_ADDR,
                                               size,
                                               pHostMem) )
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
                    return 1;
                }                    
            }
            else
            {
                /* Write to radio PHY */
                if(adf7030_1__SPI_wr_byte_b_a( pSPIDevInfo,
                                               PNTR_id,
                                               PNTR_offset,
                                               size,
                                               pHostMem) )
                {
                    return 1;
                }  
            }
        }
    }
    
    return 0;
}


/**
 * @brief       Generic Patch checksum configuration transfers function between Host and the Radio PHY
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  pPatch      Pointer to Host "patch" descriptor.
 *
 * @param [in|out]  pPATCH_CHECKSUM      Pointer to Host "patch" checksum configuration descriptor.
 *
 * @param [in]  bREAD_nWRITE    Boolean  -TRUE to write Patch from Host to the Radio PHY
 *                                       -FALSE to read Patch from Radio PHY back to Host
 *
 * @param [in]  bCHECK_ONLY     Boolean  -TRUE to check PHY patch versus Host Reference
 *                                       -FALSE [D] to perform READ_nWRITE operation
 *
 * @note                        This function modifies pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM0_ADDR].
 *
 * @return      Status
 *  - #0    If the transfert was succesfull to the adf7030-1.
 *  - #1    [D] If the transfert failed.
 */
uint8_t adf7030_1__XferChecksum(
    adf7030_1_spi_info_t*  pSPIDevInfo,
    patch_desc_t*          pPATCH,
    patch_checksum_desc_t* pPATCH_CHECKSUM,
    uint8_t                bREAD_nWRITE,
    uint8_t                bCHECK_ONLY
)
{
    data_blck_desc_t PATCH_CHECKSUM_BLOCK;
    PATCH_CHECKSUM_BLOCK.Addr = pPATCH->BLOCKS[pPATCH->InfoBlockId].Addr;
    PATCH_CHECKSUM_BLOCK.Size = (uint16_t)pPATCH_CHECKSUM->Size;
    PATCH_CHECKSUM_BLOCK.pData = (uint8_t *)pPATCH_CHECKSUM;

    return(adf7030_1__XferPatchBlock( pSPIDevInfo,
                                      &PATCH_CHECKSUM_BLOCK,
                                      bREAD_nWRITE,
                                      bCHECK_ONLY ));
}                                         

#endif /* _ADF7030_1__PATCH_C_ */

/** @} */ /* End of group adf7030-1__patch Patch Interface */
/** @} */ /* End of group adf7030-1 adf7030-1 Driver */
