/*!
 *****************************************************************************
  @file:    adf7030-1__state.c
  
  @brief:    adf7030-1 State Machine Control Functions.
                - Sending PHY command.
                - State transitions
  
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

/** \addtogroup adf7030-1__state SPI Command Interface
 *  @{
 */
#ifndef _ADF7030_1__STATE_C_
/*! \cond PRIVATE */
#define _ADF7030_1__STATE_C_

#include "adf7030-1__state.h"

#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "adf7030-1_reg.h"

#include "adf7030-1__common.h"
#include "adf7030-1__mem.h"
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

#define PMU_CLOCKS_Addr                                              0x40000C20UL
#define PMU_CLOCKS_Rst                                               0x00000001UL

/**
 * @brief       Powering off sequence for the PHY Radio
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @return      Status
 *  - #0      If the Radio power off was correctly executed.
 *  - #1      [D] If the communication with Radio PHY failed.
 */
uint8_t adf7030_1__STATE_PhyShutdown(
    adf7030_1_spi_info_t* pSPIDevInfo
)
{
    uint32_t reset_key = 0x00000027;
    uint32_t reset_pmu = PMU_CLOCKS_Rst;
  
    /* Overide clocks enables control register */
    adf7030_1__WRITE_FIELD(CRMGT_PROC_CLK_EN_CONFIGURATION, 0xFFFFFFFF);
      
    /* Set the PMU Key */
    if(adf7030_1__SPI_wr_word_b_a( pSPIDevInfo,
                                   PMU_KEY_Addr,
                                   1,
                                   &reset_key))
    {
        return 1;
    }
    
    /* Turn on PMU clock */
    if(adf7030_1__SPI_wr_word_b_a( pSPIDevInfo,
                                   PMU_CLOCKS_Addr,
                                   1,
                                   &reset_pmu) )
    {
        return 1;
    }

    /* Issue the PHY Reset command */
    return(adf7030_1__XferCmdPoll( pSPIDevInfo,
    							   CMD_RESET,
                                   0,
                                   (adf7030_1_radio_state_e)0,
                                   0));
}


/**
 * @brief       Send Radio PHY Command
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  goState         PHY Radio State to execute. See adf7030_1_radio_state_e
 *                              enum for available States.
 *
 * @note                        This function does return immediately after
 *                              issuing a Radio PHY command. The SPIDevInfo.nStatus
 *                              field get updated with the SPI status returned as
 *                              the command is issued. 
 *
 * @return      Status
 *  - #0      If the Radio PHY command was transfert to the adf7030-1.
 *  - #1      [D] If the communication with Radio PHY failed.
 */
uint8_t adf7030_1__STATE_PhyCMD(
    adf7030_1_spi_info_t*   pSPIDevInfo, 
    adf7030_1_radio_state_e goState
)
{
    return(adf7030_1__XferCmdPoll( pSPIDevInfo,
                                   RADIO_CMD | (uint8_t)goState,
                                   0,
                                   (adf7030_1_radio_state_e)0,
                                   0));
}


/**
 * @brief       Send Radio PHY Special Command
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  ExCmd           PHY Radio State to execute. See adf7030_1_radio_state_e
 *                              enum for available States.
 *
 * @note                        This function does return immediately after
 *                              issuing a Radio PHY Special command. The SPIDevInfo.nStatus
 *                              field get updated with the SPI status returned as
 *                              the command is issued. 
 *
 * @return      Status
 *  - #0      If the Radio PHY Special command was transfert to the adf7030-1.
 *  - #1      [D] If the communication with Radio PHY failed.
 */
uint8_t adf7030_1__STATE_PhyCMD_Ex(
    adf7030_1_spi_info_t*      pSPIDevInfo, 
    adf7030_1_radio_extended_e ExCmd
)
{
    return(adf7030_1__XferCmdPoll( pSPIDevInfo,
                                   SPECIAL_CMD | (uint8_t)ExCmd,
                                   0,
                                   (adf7030_1_radio_state_e)0,
                                   0));
}


/**
 * @brief       Send Radio PHY Command and wait util PHY has transition to new state.
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  goState         PHY Radio State to execute. See adf7030_1_radio_state_e
 *                              enum for available States.
 *
 * @param [in]  retState        PHY Radio State expected to be in before returning.
 *
 * @note                        This function issues a Radio PHY command according
 *                              to the goState, the polls the Radio Firmware state
 *                              for a fix amount of time (up to 255 retries) 
 *                              until retState is entered and the transition status 
 *                              is set to "state" or "idle". 
 *                              The SPIDevInfo.nStatus field get updated with the
 *                              live SPI status returned during SPI communication.
 *
 * @return      Status
 *  - #0  If the Radio PHY transition to new state.
 *  - #1  [D] If the Radio PHY failed to transition to new state or if the
 *            communication with Radio PHY failed.
 */
uint8_t adf7030_1__STATE_PhyCMD_WaitReady(
    adf7030_1_spi_info_t*   pSPIDevInfo, 
    adf7030_1_radio_state_e goState,
    adf7030_1_radio_state_e retState
)
{
    uint32_t StatusPoll = ( ((SPI_HOST_STATUS_FW_STATUS_Msk | SPI_HOST_STATUS_CMD_READY_Msk) << 16 ) |  //Mask
                            (((STATE << 1)                  | SPI_HOST_STATUS_CMD_READY_Msk) << 8  ) |  //Status Condition 2
                             ((IDLE  << 1)                  | SPI_HOST_STATUS_CMD_READY_Msk) );         //Status Condition 1

    return(adf7030_1__XferCmdPoll( pSPIDevInfo,
                                   RADIO_CMD | (uint8_t)goState,
                                   StatusPoll,
                                   retState,
                                   255));
}


/**
 * @brief       Polls Radio PHY SPI Status with NOPs until in Idle mode.
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  nRetry          Number of polling retry. Set to Zero, for continuous
 *                              polling the SPI Status.
 *
 * @note                        This function issues a Radio PHY NOP command polling 
 *                              the SPI status for a fix amount of time (nRetry)
 *                              until Idle flag is set.
 *                              The SPIDevInfo.nStatus field get updated with the
 *                              live SPI status returned during SPI communication.
 *
 * @return      Status
 *  - #0  If the Radio PHY transition to new state.
 *  - #1  [D] If the Radio PHY failed to transition to new state or if the
 *            communication with Radio PHY failed.
 */
uint8_t adf7030_1__STATE_WaitIdle(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t               nRetry
)
{
    uint32_t StatusPoll = ( ((SPI_HOST_STATUS_FW_STATUS_Msk | SPI_HOST_STATUS_CMD_READY_Msk) << 16 ) |  //Mask
                                                                                         ( 0 << 8  ) |  //Status Condition 2
                              ((IDLE << 1)                  | SPI_HOST_STATUS_CMD_READY_Msk) );         //Status Condition 1
    
    return(adf7030_1__XferCmdPoll( pSPIDevInfo,
                                   0,
                                   StatusPoll,
                                   (adf7030_1_radio_state_e)0,
                                   nRetry)); //\todo: enable forever mode?
}

/**
 * @brief       Polls Radio PHY SPI Status with NOPs until in Exec mode.
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  nRetry          Number of polling retry. Set to Zero, for continuous
 *                              polling the SPI Status.
 *
 * @note                        This function issues a Radio PHY NOP command polling 
 *                              the SPI status for a fix amount of time (nRetry)
 *                              until state Execution flag is set.
 *                              The SPIDevInfo.nStatus field get updated with the
 *                              live SPI status returned during SPI communication.
 *
 * @return      Status
 *  - #0  If the Radio PHY transition to new state.
 *  - #1  [D] If the Radio PHY failed to transition to new state or if the
 *            communication with Radio PHY failed.
 */
uint8_t adf7030_1__STATE_WaitExec(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t               nRetry
)
{
    uint32_t StatusPoll = ( ((SPI_HOST_STATUS_FW_STATUS_Msk | SPI_HOST_STATUS_CMD_READY_Msk) << 16 ) |  //Mask
                                                                                         ( 0 << 8  ) |  //Status Condition 2
                             ((STATE << 1)                  | SPI_HOST_STATUS_CMD_READY_Msk) );         //Status Condition 1
    
    return(adf7030_1__XferCmdPoll( pSPIDevInfo,
                                   0,
                                   StatusPoll,
                                   (adf7030_1_radio_state_e)0,
                                   nRetry)); //\todo: enable forever mode?
}


/**
 * @brief       Polls Radio PHY SPI Status with NOPs until ready for operation.
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  nRetry          Number of polling retry. Set to Zero, for continuous
 *                              polling the SPI Status.
 *
 * @note                        This function issues a Radio PHY NOP command polling 
 *                              the SPI status for a fix amount of time (nRetry)
 *                              until either Idle or State flag is set.
 *                              The SPIDevInfo.nStatus field get updated with the
 *                              live SPI status returned during SPI communication.
 *
 * @return      Status
 *  - #0  If the Radio PHY transition to new state.
 *  - #1  [D] If the Radio PHY failed to transition to new state or if the
 *            communication with Radio PHY failed.
 */
uint8_t adf7030_1__STATE_WaitReady(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t               nRetry
)
{
    uint32_t StatusPoll = ( ((SPI_HOST_STATUS_FW_STATUS_Msk | SPI_HOST_STATUS_CMD_READY_Msk) << 16 ) |  //Mask
                            (((STATE << 1)                  | SPI_HOST_STATUS_CMD_READY_Msk) << 8  ) |  //Status Condition 2
                             ((IDLE  << 1)                  | SPI_HOST_STATUS_CMD_READY_Msk) );         //Status Condition 1
    
    return(adf7030_1__XferCmdPoll( pSPIDevInfo,
                                   0,
                                   StatusPoll,
                                   (adf7030_1_radio_state_e)0,
                                   nRetry)); //\todo: enable forever mode?
}


/**
 * @brief       Polls Radio PHY SPI Status with NOPs until External IRQ flag is found.
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  nRetry          Number of polling retry. Set to Zero, for continuous
 *                              polling the SPI Status.
 *
 * @note                        This function issues a Radio PHY NOP command polling 
 *                              the SPI status for a fix amount of time (nRetry)
 *                              until External IRQ flag is set.
 *                              The SPIDevInfo.nStatus field get updated with the
 *                              live SPI status returned during SPI communication.
 *
 * @return      Status
 *  - #0  If the Radio PHY transition to new state.
 *  - #1  [D] If the Radio PHY failed to transition to new state or if the
 *            communication with Radio PHY failed.
 */
uint8_t adf7030_1__STATE_WaitExtIRQ(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t               nRetry
)
{
    uint32_t StatusPoll = ( ((SPI_HOST_STATUS_EXT_IRQ_STATUS_Msk | SPI_HOST_STATUS_CMD_READY_Msk) << 16 ) |  //Mask
                                                                                              ( 0 << 8  ) |  //Status Condition 2
                             (SPI_HOST_STATUS_EXT_IRQ_STATUS_Msk | SPI_HOST_STATUS_CMD_READY_Msk) );         //Status Condition 1

    return(adf7030_1__XferCmdPoll( pSPIDevInfo,
                                   0,
                                   StatusPoll,
                                   (adf7030_1_radio_state_e)0,
                                   nRetry)); //\todo: enable forever mode?
}

                                 
/**
 * @brief       Polls Radio PHY SPI Status with NOPs until ready for operation.
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  nState          PHY Radio State expected expected to match.
 *
 * @param [in]  nRetry          Number of polling retry. Set to Zero, for continuous
 *                              polling the SPI Status.
 *
 * @note                        This function issues a Radio PHY NOP command polling 
 *                              the SPI status for a fix amount of time (nRetry)
 *                              until either Idle or State flag is set.
 *                              The SPIDevInfo.nStatus field get updated with the
 *                              live SPI status returned during SPI communication.
 *
 * @return      Status
 *  - #0  If the Radio PHY transition to new state.
 *  - #1  [D] If the Radio PHY failed to transition to new state or if the
 *            communication with Radio PHY failed.
 */
uint8_t adf7030_1__STATE_WaitStateReady(
    adf7030_1_spi_info_t*   pSPIDevInfo,
    adf7030_1_radio_state_e nState,
    uint8_t                 nRetry
)
{
//SPI_HOST_STATUS_EXT_IRQ_STATUS_Msk
//SPI_HOST_STATUS_SPIS_READY_Msk  

    uint32_t StatusPoll = ( ((SPI_HOST_STATUS_FW_STATUS_Msk | SPI_HOST_STATUS_CMD_READY_Msk) << 16 ) |  //Mask
                            (((STATE << 1)                  | SPI_HOST_STATUS_CMD_READY_Msk) << 8  ) |  //Status Condition 2
                             ((IDLE  << 1)                  | SPI_HOST_STATUS_CMD_READY_Msk) );         //Status Condition 1
    
    return(adf7030_1__XferCmdPoll( pSPIDevInfo,
                                   0,
                                   StatusPoll,
                                   nState,
                                   nRetry)); //\todo: enable forever mode?
}


/**
 * @brief       Polls Radio PHY SPI Status with NOPs until match is found.
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  nSPIStatus      Radio PHY SPI status to match. See adf7030_1_spi_status_t
 *                              structure for available fields.
 *
 * @param [in]  nSPIStatusMsk   Radio PHY SPI status mask to enable.
 *
 * @param [in]  nRetry          Number of polling retry. Set to Zero, for continuous
 *                              polling the SPI Status.
 *
 * @note                        This function issues a Radio PHY NOP command polling 
 *                              the Radio Firmware state for a fix amount of time (nRetry)
 *                              until retState is entered and until the transition
 *                              status is set to ready or idle.
 *                              The SPIDevInfo.nStatus field get updated with the
 *                              live SPI status returned during SPI communication.
 *
 * @return      Status
 *  - #0  If the Radio PHY transition to new state.
 *  - #1  [D] If the Radio PHY failed to transition to new state or if the
 *            communication with Radio PHY failed.
 */
uint8_t adf7030_1__STATE_Poll_SpiStatus(
    adf7030_1_spi_info_t*  pSPIDevInfo, 
    adf7030_1_spi_status_t nSPIStatus,
    uint8_t                nSPIStatusMsk,
    uint8_t                nRetry
)
{
    uint32_t StatusPoll = ( ( nSPIStatusMsk << 16 ) |  //Mask
                                        ( 0 << 8  ) |  //Status Condition 2
                          (uint8_t)nSPIStatus.VALUE ); //Status Condition 1

    return(adf7030_1__XferCmdPoll( pSPIDevInfo,
                                   0,
                                   StatusPoll,
                                   (adf7030_1_radio_state_e)0,
                                   nRetry));
}


/**
 * @brief       Polls Radio PHY SPI Status until match is found.
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  State           PHY Radio State to match. See adf7030_1_radio_state_e
 *                              enum for available States.
 *
 * @param [in]  nRetry          Number of polling retries. Set to Zero, for continuously
 *                              polling the SPI Status.
 *
 * @note                        This function is polling the Radio PHY State  
 *                              for a fix amount of time (nRetry) until State is 
 *                              reach.
 *                              The SPIDevInfo.nStatus field get updated with the
 *                              live SPI status returned during SPI communication.
 *
 * @return      Status
 *  - #0  If the Radio PHY transition to new state.
 *  - #1  [D] If the Radio PHY failed to transition to new state or if the
 *            communication with Radio PHY failed.
 */
uint8_t adf7030_1__STATE_Poll_PhyState(
    adf7030_1_spi_info_t*   pSPIDevInfo, 
    adf7030_1_radio_state_e State,
    uint8_t                 nRetry
)
{
    uint32_t StatusPoll = ( ( 0 << 16 ) |  //Mask
                             ( 0 << 8 ) |  //Status Condition 2
                                    0 );   //Status Condition 1

    return(adf7030_1__XferCmdPoll( pSPIDevInfo,
                                   0,
                                   StatusPoll,
                                   State,
                                   nRetry));
}


/**
 * @brief       Generic Radio PHY Command Transfers
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the 
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @param [in]  PHY_CMD         PHY Command from adf7030_1_radio_state_e.
 *
 * @note                        This function automatically set the pSPIDevInfo->ePhyError
 *                              The "Wait for cmd_ready bit" is not correct
 *
 * @return      Status
 *  - #0  If the Radio PHY Command transfers was successful.
 *  - #1  [D] If the Radio PHY Command failed or if the communication with Radio
 *        PHY failed or if critical Radio PHY Error has occurred.
 */
uint8_t adf7030_1__XferCmdPoll(
    adf7030_1_spi_info_t*   pSPIDevInfo,
    uint8_t                 nPhyCmd,
    uint32_t                nStatusPoll,
    adf7030_1_radio_state_e nStatePoll,
    uint8_t                 nRetry
)
{
    if(nPhyCmd)
    {        
        /* Transmit the sequence */
        adf7030_1__SPI_ReadWrite_Fast( pSPIDevInfo,
                                       &nPhyCmd,
                                       &pSPIDevInfo->nStatus.VALUE,
                                       1 );
    }
    
    /* Get the SPI status polling mask, assume*/
    uint8_t nStatusPoll_Cond1 = *((uint8_t *)&nStatusPoll + 0);
    uint8_t nStatusPoll_Cond2 = *((uint8_t *)&nStatusPoll + 1);
    uint8_t nStatusPoll_msk = *((uint8_t *)&nStatusPoll + 2);
    
    nStatusPoll_Cond1 &= nStatusPoll_msk;
    nStatusPoll_Cond2 &= nStatusPoll_msk;
  
    if((nStatePoll == 0) && (nStatusPoll_msk != 0))
    {
      /* Poll until SPI status match */
        uint32_t nPollcnt = nRetry;
      
        uint8_t NOP_Cmd = 0xFF;
              
        uint8_t EXIT = 0;
        
        /* Polling loop */
        do{
            /* Decrement the polling loop counter if tracking */
            if(nRetry) nPollcnt--;

            /* Transmit the sequence */
            adf7030_1__SPI_ReadWrite_Fast( pSPIDevInfo,
                                           &NOP_Cmd,
                                           &pSPIDevInfo->nStatus.VALUE,
                                           1 );

            /* Set Exit flag if SPI Status word matches nStatusPoll_Cond1 */
            if(nStatusPoll_Cond1 == (pSPIDevInfo->nStatus.VALUE & nStatusPoll_msk)){
                EXIT = 1;
            }
            
            /* Set Exit flag if SPI Status word matches nStatusPoll_Cond2 */
            if(nStatusPoll_Cond2 == (pSPIDevInfo->nStatus.VALUE & nStatusPoll_msk)){
                EXIT = 1;
            }
            
            /* Return if number of retries expired in the polling loop */
            if(nRetry && !nPollcnt)
            {
            	if ( !(pSPIDevInfo->eXferResult) ) {
            		pSPIDevInfo->eXferResult = ADF7030_1_SPI_DEV_POLL_EXPIRE;
            	}
            	return 1;
            }
            
        }while(EXIT == 0);
    }
    else if(nStatePoll)
    {
      /* Poll until PHY state match */
        uint32_t Addr = MISC_FW_Addr;
      
        /* Get current SPI custom pntr 2 value */ 
        int32_t AddrDiff = (int32_t)Addr - (int32_t)(pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM2_ADDR]);
        
        uint8_t Offset;
        
        if((AddrDiff >= 0 ) && (AddrDiff < (255 - 1)))
        {
          /* Here PNTR_CUSTOM2_ADDR is within range, just change the offset */
            Offset = (uint8_t)AddrDiff;
          
        }else{
          /* Here PNTR_CUSTOM2_ADDR is not within range, changing it, we shall */
                
            /* Setup the PHY Radio SPI pointer 0 address to Addr */
            if(adf7030_1__SPI_wr_word_b_a( pSPIDevInfo, pSPIDevInfo->PHY_PNTR[PNTR_SETUP_ADDR] + 8, 1, &Addr))
            {
                return 1;
            }
            
            pSPIDevInfo->PHY_PNTR[PNTR_CUSTOM2_ADDR] = Addr;
            
            Offset = 0;
        }

        /* Setup SPI tx and rx buffer addresses */
        uint8_t * pSPI_TX_BUFF = pSPIDevInfo->pSPI_TX_BUFF;
        uint8_t * pSPI_RX_BUFF = pSPIDevInfo->pSPI_RX_BUFF;
        
        /* Transmit SPI command - use 1bytes offset to keep rest of data aligned on word boundary */
        *(pSPI_TX_BUFF + 0) = ADF703x_SPI_MEM_READ |
                              ADF703x_SPI_MEM_BLOCK |
                              ADF703x_SPI_MEM_ADDRESS |
                              ADF703x_SPI_MEM_SHORT |
                              PNTR_CUSTOM2_ADDR;
    
        /* Set Block offset from pntrID */
        *(pSPI_TX_BUFF + 1) = Offset + 1;
        
        /* Add 8bits of NOPs in spi_txbuf */
        *((uint16_t *)pSPI_TX_BUFF + 1) = 0xFFFF;

        uint8_t EXIT = 0;
        
        uint32_t nPollcnt = nRetry;
        
        /* Polling loop */
        do{
            /* Transmit the sequence */
            adf7030_1__SPI_ReadWrite_Fast( pSPIDevInfo,
                                           (void *)pSPI_TX_BUFF,
                                           (void *)pSPI_RX_BUFF,
                                           4 );

            /* Write back current SPI Status into pSPIDevInfo structure */
            pSPIDevInfo->nStatus.VALUE  = *(pSPI_RX_BUFF + 2);
            
            /* Write back current SPI Status into pSPIDevInfo structure */
            pSPIDevInfo->nPhyState      = (adf7030_1_radio_state_e)*(pSPI_RX_BUFF + 3);

            /* Decrement the polling loop counter if tracking */
            if(nRetry) nPollcnt--;
            
            if(nStatusPoll_msk){
                /* Set Exit flag if SPI Status word matches nStatusPoll_Cond1 and FW State matches nStatePoll */
                if((nStatusPoll_Cond1 == (*(pSPI_RX_BUFF + 2) & nStatusPoll_msk)) && (nStatePoll == *(pSPI_RX_BUFF + 3))){
                    EXIT = 1;
                }
                /* Set Exit flag if SPI Status word matches nStatusPoll_Cond2 and FW State matches nStatePoll */
                if((nStatusPoll_Cond2 == (*(pSPI_RX_BUFF + 2) & nStatusPoll_msk)) && (nStatePoll == *(pSPI_RX_BUFF + 3))){
                    EXIT = 1;
                }
            }else{
                /* Set Exit flag if FW State matches nStatePoll */
                if(nStatePoll == *(pSPI_RX_BUFF + 3)){
                    EXIT = 1;
                }
            }
            
            /* Return if number of retries expired in the polling loop */
            if(nRetry && !nPollcnt)
            {
            	if ( !(pSPIDevInfo->eXferResult) ) {
            		pSPIDevInfo->eXferResult = ADF7030_1_SPI_DEV_POLL_EXPIRE;
            	}
            	return 1;
            }
        }while(EXIT == 0);
    }

#if (ADF7030_1_PHY_ERROR_REPORT_ENABLE == 1) && ( ADF7030_1_PHY_ERROR_REPORT_SCHEME == 0)

    if((nPhyCmd != CMD_RESET) && (pSPIDevInfo->bPhyErrorCheck == 1))
    {
        /*!
         *  The "error" bit inside pSPIDevInfo->nStatus is automatically cleared
         *  by internal state machine. Error bit in the SPI status could be hard 
         *  to catch by polling mechanism.
         *  However the "error code" are kept in the upper byte of the MISC_FW register.
         *  Instead, lets always read its content and update the pSPIDevInfo->ePhyError value */

        /* Read MISC_FW_ERR_CODE register from radio PHY */
        uint32_t fw_status_reg;
        if(adf7030_1__SPI_rd_word_b_a( pSPIDevInfo, MISC_FW_Addr, 1, &fw_status_reg))
        {
        	return 1;
        }

        /* Update the pSPIDevInfo->ePhyError */
        pSPIDevInfo->ePhyError = (adf7030_1_radio_error_e)(fw_status_reg >> 26);

        /* If error code exists, return ADF7030_1_HW_ERROR status */
        if(pSPIDevInfo->ePhyError != SM_NOERROR){

            /* If global Radio driver application callback is defined */
            if(pSPIDevInfo->pfPhyErrCb != NULL)
            {
                /* Indicate that interrupt 0 occurred */
            	pSPIDevInfo->pfPhyErrCb(pSPIDevInfo, (void*)((uint32_t)ADF7030_1_PHY_ERROR));
            }
       		pSPIDevInfo->eXferResult = ADF7030_1_HW_ERROR;
        	return 1;
        }
    }
#endif
    return 0;
}


#if (ADF7030_1_PHY_ERROR_REPORT_ENABLE == 1)

/**
 * @brief       Clear the upper 16bits of the MISC_FW register
 *
 * @param [in]  pSPIDevInfo     Pointer to the SPI device info structure of the
 *                              ADI RF Driver used to communicate with the
 *                              adf7030-1 PHY.
 *
 * @return      Status
 *  - #ADF7030_1_SUCCESS              If the Radio PHY error code has been cleared.
 *  - #ADF7030_1_SPI_COMM_FAILED      [D] If the communication with Radio PHY failed.
 */
adf7030_1_res_e adf7030_1__STATE_ClearPhyError(
    adf7030_1_spi_info_t* pSPIDevInfo
)
{
    /* Clears the upper 16bits of the MISC_FW register */
    adf7030_1__SPI_SetField( pSPIDevInfo, MISC_FW_Addr, 16, 16, 0);

    /* Return SPI transfer status */
    return( (pSPIDevInfo->eXferResult)?(1):(0));
}

#endif


#endif /* _ADF7030_1__PATCH_C_ */

/** @} */ /* End of group adf7030-1__state SPI Command Interface */
/** @} */ /* End of group adf7030-1 adf7030-1 Driver */
