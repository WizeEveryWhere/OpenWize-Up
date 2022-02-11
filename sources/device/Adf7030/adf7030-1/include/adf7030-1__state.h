/*!
*****************************************************************************
  @file:	adf7030-1__state.h 
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


#ifndef _ADF7030_1__STATE_H_
#define _ADF7030_1__STATE_H_

#include "adf7030-1__common.h"

/** \addtogroup adf7030-1 adf7030-1 Driver
*  @{
*/

/** \addtogroup adf7030-1__state SPI Command Interface
*  @{
*/

/*! \cond PRIVATE*/
#ifndef __INTERNAL_DEV__
/* ----------------------------  SPI_HOST_STATUS  ----------------------------*/
#define SPI_HOST_STATUS_Addr                                         0x40001814UL                                                 /*!< SPI_HOST STATUS: Address                                    */
#define pSPI_HOST_STATUS                                             (*(volatile uint32_t*) SPI_HOST_STATUS_Addr)                /*!< SPI_HOST STATUS: Pointer                                    */
#define SPI_HOST_STATUS_Msk                                          0x0003FFFFUL                                                 /*!< SPI_HOST STATUS: Mask                                       */
#define SPI_HOST_STATUS_Rst                                          0x0000A0A0UL                                                 /*!< SPI_HOST STATUS: Reset                                      */

#define SPI_HOST_STATUS_MEM_ACC_STATUS_Size                          1                                                            /*!< SPI_HOST STATUS: MEM_ACC_STATUS Size                        */
#define SPI_HOST_STATUS_MEM_ACC_STATUS_Pos                           0                                                            /*!< SPI_HOST STATUS: MEM_ACC_STATUS Position                    */
#define SPI_HOST_STATUS_MEM_ACC_STATUS_Msk                           (0x0001UL << SPI_HOST_STATUS_MEM_ACC_STATUS_Pos)             /*!< SPI_HOST STATUS: MEM_ACC_STATUS Mask                        */
#define SPI_HOST_STATUS_MEM_ACC_STATUS_Rst                           0x0000UL                                                     /*!< SPI_HOST STATUS: MEM_ACC_STATUS Reset                       */
#define SPI_HOST_STATUS_MEM_ACC_STATUS_Addr                          0x40001814UL                                                 /*!< SPI_HOST STATUS: MEM_ACC_STATUS Address                     */

#define SPI_HOST_STATUS_FW_STATUS_Size                               4                                                            /*!< SPI_HOST STATUS: FW_STATUS Size                             */
#define SPI_HOST_STATUS_FW_STATUS_Pos                                1                                                            /*!< SPI_HOST STATUS: FW_STATUS Position                         */
#define SPI_HOST_STATUS_FW_STATUS_Msk                                (0x000fUL << SPI_HOST_STATUS_FW_STATUS_Pos)                  /*!< SPI_HOST STATUS: FW_STATUS Mask                             */
#define SPI_HOST_STATUS_FW_STATUS_Rst                                0x0000UL                                                     /*!< SPI_HOST STATUS: FW_STATUS Reset                            */
#define SPI_HOST_STATUS_FW_STATUS_Addr                               0x40001814UL                                                 /*!< SPI_HOST STATUS: FW_STATUS Address                          */

#define SPI_HOST_STATUS_CMD_READY_Size                               1                                                            /*!< SPI_HOST STATUS: CMD_READY Size                             */
#define SPI_HOST_STATUS_CMD_READY_Pos                                5                                                            /*!< SPI_HOST STATUS: CMD_READY Position                         */
#define SPI_HOST_STATUS_CMD_READY_Msk                                (0x0001UL << SPI_HOST_STATUS_CMD_READY_Pos)                  /*!< SPI_HOST STATUS: CMD_READY Mask                             */
#define SPI_HOST_STATUS_CMD_READY_Rst                                0x0001UL                                                     /*!< SPI_HOST STATUS: CMD_READY Reset                            */
#define SPI_HOST_STATUS_CMD_READY_Addr                               0x40001814UL                                                 /*!< SPI_HOST STATUS: CMD_READY Address                          */

#define SPI_HOST_STATUS_EXT_IRQ_STATUS_Size                          1                                                            /*!< SPI_HOST STATUS: EXT_IRQ_STATUS Size                        */
#define SPI_HOST_STATUS_EXT_IRQ_STATUS_Pos                           6                                                            /*!< SPI_HOST STATUS: EXT_IRQ_STATUS Position                    */
#define SPI_HOST_STATUS_EXT_IRQ_STATUS_Msk                           (0x0001UL << SPI_HOST_STATUS_EXT_IRQ_STATUS_Pos)             /*!< SPI_HOST STATUS: EXT_IRQ_STATUS Mask                        */
#define SPI_HOST_STATUS_EXT_IRQ_STATUS_Rst                           0x0000UL                                                     /*!< SPI_HOST STATUS: EXT_IRQ_STATUS Reset                       */
#define SPI_HOST_STATUS_EXT_IRQ_STATUS_Addr                          0x40001814UL                                                 /*!< SPI_HOST STATUS: EXT_IRQ_STATUS Address                     */
#define SPI_HOST_STATUS_EXT_IRQ_STATUS_ENUM_0_Eval                   0
#define SPI_HOST_STATUS_EXT_IRQ_STATUS_ENUM_1_Eval                   1

#define SPI_HOST_STATUS_SPIS_READY_Size                              1                                                            /*!< SPI_HOST STATUS: SPIS_READY Size                            */
#define SPI_HOST_STATUS_SPIS_READY_Pos                               7                                                            /*!< SPI_HOST STATUS: SPIS_READY Position                        */
#define SPI_HOST_STATUS_SPIS_READY_Msk                               (0x0001UL << SPI_HOST_STATUS_SPIS_READY_Pos)                 /*!< SPI_HOST STATUS: SPIS_READY Mask                            */
#define SPI_HOST_STATUS_SPIS_READY_Rst                               0x0001UL                                                     /*!< SPI_HOST STATUS: SPIS_READY Reset                           */
#define SPI_HOST_STATUS_SPIS_READY_Addr                              0x40001814UL                                                 /*!< SPI_HOST STATUS: SPIS_READY Address                         */

#endif

/* special radio commands*/
#define CMD_RAM_LOAD_DONE       RADIO_COMMAND | SPCNTRN  | 0x08 //Configures the ADF703x to use a downloaded patch by restarting the ADF703x
#define CMD_IRQ1_DIS_IRQ0_DIS   RADIO_COMMAND | SPCNTRN  | 0x0C //Disable IRQ_IN0 and IRQ_IN1 in triggering preloaded radio commands
#define CMD_IRQ1_DIS_IRQ0_EN    RADIO_COMMAND | SPCNTRN  | 0x0D //Disable IRQ_IN1 and enable IRQ_IN0 in triggering preloaded radio commands
#define CMD_IRQ1_EN_IRQ0_DIS    RADIO_COMMAND | SPCNTRN  | 0x0E //Enable IRQ_IN1 and disable IRQ_IN0 in triggering preloaded radio commands
#define CMD_IRQ1_EN_IRQ0_EN     RADIO_COMMAND | SPCNTRN  | 0x0F //Enable IRQ_IN1 and IRQ_IN0 in triggering preloaded radio commands

/*status*/
#define STATUS_SPI_READY(x)             (x & (0x1 << 7))
#define STATUS_IRQ_STATUS(x)            (x & (0x1 << 6))
#define STATUS_CMD_READY(x)             (x & (0x1 << 5))
#define STATUS_ERR(x)                   (x & (0x1 << 3))
#define STATUS_TRANSITION_STATUS(x)     ((x & 0x6) >> 1)

/*! \endcond*/

/*! Enumeration of different radio command type*/
typedef enum {
    RADIO_CMD   = (0x1 << 7), // 0x80
    SPECIAL_CMD = (0x3 << 6), // 0xC0
} adf7030_1_radio_cmd_t;

#define CMD_NOP   0xFF
#define CMD_RESET 0xC7


uint8_t adf7030_1__STATE_PhyWakeUp(
    adf7030_1_spi_info_t* pSPIDevInfo
);

uint8_t adf7030_1__STATE_PhyShutdown(
    adf7030_1_spi_info_t* pSPIDevInfo
);

uint8_t adf7030_1__STATE_PhyCMD(
    adf7030_1_spi_info_t*   pSPIDevInfo, 
    adf7030_1_radio_state_e goState
);

uint8_t adf7030_1__STATE_PhyCMD_Ex(
    adf7030_1_spi_info_t*      pSPIDevInfo, 
    adf7030_1_radio_extended_e ExCmd
);

uint8_t adf7030_1__STATE_PhyCMD_WaitReady(
    adf7030_1_spi_info_t*   pSPIDevInfo, 
    adf7030_1_radio_state_e goState,
    adf7030_1_radio_state_e retState
);

uint8_t adf7030_1__STATE_WaitIdle(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t               nRetry
);

uint8_t adf7030_1__STATE_WaitExec(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t               nRetry
);

uint8_t adf7030_1__STATE_WaitReady(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t               nRetry
);

uint8_t adf7030_1__STATE_WaitExtIRQ(
    adf7030_1_spi_info_t* pSPIDevInfo,
    uint8_t               nRetry
);

uint8_t adf7030_1__STATE_WaitStateReady(
    adf7030_1_spi_info_t*   pSPIDevInfo,
    adf7030_1_radio_state_e nState,
    uint8_t                 nRetry
);

uint8_t adf7030_1__STATE_Poll_SpiStatus(
    adf7030_1_spi_info_t*  pSPIDevInfo, 
    adf7030_1_spi_status_t nSPIStatus,
    uint8_t                nSPIStatusMsk,
    uint8_t                nRetry
);

uint8_t adf7030_1__STATE_Poll_PhyState(
    adf7030_1_spi_info_t*   pSPIDevInfo, 
    adf7030_1_radio_state_e State,
    uint8_t                 nRetry
);

uint8_t adf7030_1__XferCmdPoll(
    adf7030_1_spi_info_t*   pSPIDevInfo,
    uint8_t                 nPhyCmd,
    uint32_t                nStatusPoll,
    adf7030_1_radio_state_e nStatePoll,
    uint8_t                 nRetry
);

#if (ADF7030_1_PHY_ERROR_REPORT_ENABLE == 1)

uint8_t adf7030_1__STATE_ClearPhyError(
    adf7030_1_spi_info_t* pSPIDevInfo
);

#endif


/** @}*/ /* End of group adf7030-1__state SPI Command Interface*/
/** @}*/ /* End of group adf7030-1 adf7030-1 Driver*/

#endif /* _ADF7030_1__STATE_H_*/
