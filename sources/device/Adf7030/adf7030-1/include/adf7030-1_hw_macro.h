/*!
 *****************************************************************************
  @file:    adf7030-1_hw_macro.h
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


#ifndef ADF7030_HW_MACRO_H
#define ADF7030_HW_MACRO_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup adf7030-1 ADF7030-1 Driver
  * @{
  */

/** @addtogroup adf7030-1_headers Headers
 *  @{
 */
    
/** @addtogroup PHY_headers PHY
  * @{
  */

/** @addtogroup HW_Macro Hardware Macro Definition
  * @{
  */

/* ========================================================================== */
/* ================              Peripheral memory map       ================ */
/* ========================================================================== */

#define VCO_CAL_RESULTS_BASE  0x20000844UL

#define PMU_BASE              0x40000C00UL
#define SPI_HOST_BASE         0x40001800UL
#define IRQ_CTRL_BASE         0x40003800UL
#define MCR_BASE              0x40004000UL

#define PLL_CP_BASE           0x40004020UL // PLL Charge Pump Register Base address
#define PLL_LF_BASE           0x40004024UL // PLL Loop Filter Register Base address
#define AFERX_FILT_STG1_BASE  0x40004058UL // Analog Filter Receiver Stage 1 base address
#define AFC_BASE              0x400041F8UL
#define CRMGT_BASE            0x40004270UL
#define MISC_BASE             0x400042B4UL

#define GPIO_BASE             0x40000800UL
#define GPIO_CON0_3_REG       0x40000800UL
#define GPIO_CON4_7_REG       0x40000804UL
#define GPIO_SET_REG          0x4000081CUL
#define GPIO_CLR_REG          0x40000820UL
/* ========================================================================== */
/* ================           Peripheral declaration         ================ */
/* ========================================================================== */
/*
#define VCO_CAL_RESULTS       ((VCO_CAL_RESULTS_Type*) VCO_CAL_RESULTS_BASE)
#define PMU                   ((PMU_Type            *) PMU_BASE)
#define SPI_HOST              ((SPI_HOST_Type       *) SPI_HOST_BASE)
#define IRQ_CTRL              ((IRQ_CTRL_Type       *) IRQ_CTRL_BASE)
#define AFC                   ((AFC_Type            *) AFC_BASE)
#define CRMGT                 ((CRMGT_Type          *) CRMGT_BASE)
#define MISC                  ((MISC_Type           *) MISC_BASE)
*/
/* ======================================================================================== */
/* ================          struct 'PMU'                                  ================ */
/* ======================================================================================== */

/* ----------------------------  PMU_KEY  ---------------------------- */
#define PMU_KEY_Addr                                                 0x40000C08UL                                                 /*!< PMU KEY: Address                                             */
#define pPMU_KEY                                                     (*(volatile uint32_t *) PMU_KEY_Addr)                        /*!< PMU KEY: Pointer                                             */
#define PMU_KEY_Msk                                                  0x0000003FUL                                                 /*!< PMU KEY: Mask                                                */
#define PMU_KEY_Rst                                                  0x00000000UL                                                 /*!< PMU KEY: Reset                                               */

#define PMU_KEY_SW_KEY_Size                                          6                                                            /*!< PMU KEY: SW_KEY Size                                         */
#define PMU_KEY_SW_KEY_Pos                                           0                                                            /*!< PMU KEY: SW_KEY Position                                     */
#define PMU_KEY_SW_KEY_Msk                                           (0x003fUL << PMU_KEY_SW_KEY_Pos)                             /*!< PMU KEY: SW_KEY Mask                                         */
#define PMU_KEY_SW_KEY_Rst                                           0x0000UL                                                     /*!< PMU KEY: SW_KEY Reset                                        */
#define PMU_KEY_SW_KEY_Addr                                          0x40000c08UL                                                 /*!< PMU KEY: SW_KEY Address                                      */

/* ======================================================================================== */
/* ================          struct 'SPI_HOST'                             ================ */
/* ======================================================================================== */

/* ----------------------------  SPI_HOST_PNTR0  ---------------------------- */
#define SPI_HOST_PNTR0_Addr                                          0x40001800UL                                                 /*!< SPI_HOST PNTR0: Address                                      */
#define pSPI_HOST_PNTR0                                              (*(volatile uint32_t *) SPI_HOST_PNTR0_Addr)                 /*!< SPI_HOST PNTR0: Pointer                                      */
#define SPI_HOST_PNTR0_Msk                                           0xFFFFFFFFUL                                                 /*!< SPI_HOST PNTR0: Mask                                         */
#define SPI_HOST_PNTR0_Rst                                           0x40001800UL                                                 /*!< SPI_HOST PNTR0: Reset                                        */

#define SPI_HOST_PNTR0_SPIS_PNTR0_Size                               32                                                           /*!< SPI_HOST PNTR0: SPIS_PNTR0 Size                              */
#define SPI_HOST_PNTR0_SPIS_PNTR0_Pos                                0                                                            /*!< SPI_HOST PNTR0: SPIS_PNTR0 Position                          */
#define SPI_HOST_PNTR0_SPIS_PNTR0_Msk                                (0xffffffffUL << SPI_HOST_PNTR0_SPIS_PNTR0_Pos)              /*!< SPI_HOST PNTR0: SPIS_PNTR0 Mask                              */
#define SPI_HOST_PNTR0_SPIS_PNTR0_Rst                                0x40001800UL                                                 /*!< SPI_HOST PNTR0: SPIS_PNTR0 Reset                             */
#define SPI_HOST_PNTR0_SPIS_PNTR0_Addr                               0x40001800UL                                                 /*!< SPI_HOST PNTR0: SPIS_PNTR0 Address                           */

/* ----------------------------  SPI_HOST_PNTR1  ---------------------------- */
#define SPI_HOST_PNTR1_Addr                                          0x40001804UL                                                 /*!< SPI_HOST PNTR1: Address                                      */
#define pSPI_HOST_PNTR1                                              (*(volatile uint32_t *) SPI_HOST_PNTR1_Addr)                 /*!< SPI_HOST PNTR1: Pointer                                      */
#define SPI_HOST_PNTR1_Msk                                           0xFFFFFFFFUL                                                 /*!< SPI_HOST PNTR1: Mask                                         */
#define SPI_HOST_PNTR1_Rst                                           0x40001804UL                                                 /*!< SPI_HOST PNTR1: Reset                                        */

#define SPI_HOST_PNTR1_SPIS_PNTR1_Size                               32                                                           /*!< SPI_HOST PNTR1: SPIS_PNTR1 Size                              */
#define SPI_HOST_PNTR1_SPIS_PNTR1_Pos                                0                                                            /*!< SPI_HOST PNTR1: SPIS_PNTR1 Position                          */
#define SPI_HOST_PNTR1_SPIS_PNTR1_Msk                                (0xffffffffUL << SPI_HOST_PNTR1_SPIS_PNTR1_Pos)              /*!< SPI_HOST PNTR1: SPIS_PNTR1 Mask                              */
#define SPI_HOST_PNTR1_SPIS_PNTR1_Rst                                0x40001804UL                                                 /*!< SPI_HOST PNTR1: SPIS_PNTR1 Reset                             */
#define SPI_HOST_PNTR1_SPIS_PNTR1_Addr                               0x40001804UL                                                 /*!< SPI_HOST PNTR1: SPIS_PNTR1 Address                           */

/* ----------------------------  SPI_HOST_PNTR2  ---------------------------- */
#define SPI_HOST_PNTR2_Addr                                          0x40001808UL                                                 /*!< SPI_HOST PNTR2: Address                                      */
#define pSPI_HOST_PNTR2                                              (*(volatile uint32_t *) SPI_HOST_PNTR2_Addr)                 /*!< SPI_HOST PNTR2: Pointer                                      */
#define SPI_HOST_PNTR2_Msk                                           0xFFFFFFFFUL                                                 /*!< SPI_HOST PNTR2: Mask                                         */
#define SPI_HOST_PNTR2_Rst                                           0x40001808UL                                                 /*!< SPI_HOST PNTR2: Reset                                        */

#define SPI_HOST_PNTR2_SPIS_PNTR2_Size                               32                                                           /*!< SPI_HOST PNTR2: SPIS_PNTR2 Size                              */
#define SPI_HOST_PNTR2_SPIS_PNTR2_Pos                                0                                                            /*!< SPI_HOST PNTR2: SPIS_PNTR2 Position                          */
#define SPI_HOST_PNTR2_SPIS_PNTR2_Msk                                (0xffffffffUL << SPI_HOST_PNTR2_SPIS_PNTR2_Pos)              /*!< SPI_HOST PNTR2: SPIS_PNTR2 Mask                              */
#define SPI_HOST_PNTR2_SPIS_PNTR2_Rst                                0x40001808UL                                                 /*!< SPI_HOST PNTR2: SPIS_PNTR2 Reset                             */
#define SPI_HOST_PNTR2_SPIS_PNTR2_Addr                               0x40001808UL                                                 /*!< SPI_HOST PNTR2: SPIS_PNTR2 Address                           */

/* ======================================================================================== */
/* ================          struct 'IRQ_CTRL'                             ================ */
/* ======================================================================================== */

/* ----------------------------  IRQ_CTRL_MASK0  ---------------------------- */
#define IRQ_CTRL_MASK0_Addr                                          0x40003800UL                                                 /*!< IRQ_CTRL MASK0: Address                                      */
#define pIRQ_CTRL_MASK0                                              (*(volatile uint32_t *) IRQ_CTRL_MASK0_Addr)                 /*!< IRQ_CTRL MASK0: Pointer                                      */
#define IRQ_CTRL_MASK0_Msk                                           0x00000F00UL                                                 /*!< IRQ_CTRL MASK0: Mask                                         */
#define IRQ_CTRL_MASK0_Rst                                           0x00000000UL                                                 /*!< IRQ_CTRL MASK0: Reset                                        */

#define IRQ_CTRL_MASK0_BUFF_HALF_IRQN_Size                           1                                                            /*!< IRQ_CTRL MASK0: BUFF_HALF_IRQN Size                          */
#define IRQ_CTRL_MASK0_BUFF_HALF_IRQN_Pos                            8                                                            /*!< IRQ_CTRL MASK0: BUFF_HALF_IRQN Position                      */
#define IRQ_CTRL_MASK0_BUFF_HALF_IRQN_Msk                            (0x0001UL << IRQ_CTRL_MASK0_BUFF_HALF_IRQN_Pos)              /*!< IRQ_CTRL MASK0: BUFF_HALF_IRQN Mask                          */
#define IRQ_CTRL_MASK0_BUFF_HALF_IRQN_Rst                            0x0000UL                                                     /*!< IRQ_CTRL MASK0: BUFF_HALF_IRQN Reset                         */
#define IRQ_CTRL_MASK0_BUFF_HALF_IRQN_Addr                           0x40003800UL                                                 /*!< IRQ_CTRL MASK0: BUFF_HALF_IRQN Address                       */

#define IRQ_CTRL_MASK0_BUFF_FULL_IRQN_Size                           1                                                            /*!< IRQ_CTRL MASK0: BUFF_FULL_IRQN Size                          */
#define IRQ_CTRL_MASK0_BUFF_FULL_IRQN_Pos                            9                                                            /*!< IRQ_CTRL MASK0: BUFF_FULL_IRQN Position                      */
#define IRQ_CTRL_MASK0_BUFF_FULL_IRQN_Msk                            (0x0001UL << IRQ_CTRL_MASK0_BUFF_FULL_IRQN_Pos)              /*!< IRQ_CTRL MASK0: BUFF_FULL_IRQN Mask                          */
#define IRQ_CTRL_MASK0_BUFF_FULL_IRQN_Rst                            0x0000UL                                                     /*!< IRQ_CTRL MASK0: BUFF_FULL_IRQN Reset                         */
#define IRQ_CTRL_MASK0_BUFF_FULL_IRQN_Addr                           0x40003800UL                                                 /*!< IRQ_CTRL MASK0: BUFF_FULL_IRQN Address                       */

#define IRQ_CTRL_MASK0_SM_READY_IRQN_Size                            1                                                            /*!< IRQ_CTRL MASK0: SM_READY_IRQN Size                           */
#define IRQ_CTRL_MASK0_SM_READY_IRQN_Pos                             10                                                           /*!< IRQ_CTRL MASK0: SM_READY_IRQN Position                       */
#define IRQ_CTRL_MASK0_SM_READY_IRQN_Msk                             (0x0001UL << IRQ_CTRL_MASK0_SM_READY_IRQN_Pos)               /*!< IRQ_CTRL MASK0: SM_READY_IRQN Mask                           */
#define IRQ_CTRL_MASK0_SM_READY_IRQN_Rst                             0x0000UL                                                     /*!< IRQ_CTRL MASK0: SM_READY_IRQN Reset                          */
#define IRQ_CTRL_MASK0_SM_READY_IRQN_Addr                            0x40003800UL                                                 /*!< IRQ_CTRL MASK0: SM_READY_IRQN Address                        */

#define IRQ_CTRL_MASK0_SM_IDLE_IRQN_Size                             1                                                            /*!< IRQ_CTRL MASK0: SM_IDLE_IRQN Size                            */
#define IRQ_CTRL_MASK0_SM_IDLE_IRQN_Pos                              11                                                           /*!< IRQ_CTRL MASK0: SM_IDLE_IRQN Position                        */
#define IRQ_CTRL_MASK0_SM_IDLE_IRQN_Msk                              (0x0001UL << IRQ_CTRL_MASK0_SM_IDLE_IRQN_Pos)                /*!< IRQ_CTRL MASK0: SM_IDLE_IRQN Mask                            */
#define IRQ_CTRL_MASK0_SM_IDLE_IRQN_Rst                              0x0000UL                                                     /*!< IRQ_CTRL MASK0: SM_IDLE_IRQN Reset                           */
#define IRQ_CTRL_MASK0_SM_IDLE_IRQN_Addr                             0x40003800UL                                                 /*!< IRQ_CTRL MASK0: SM_IDLE_IRQN Address                         */

/* ----------------------------  IRQ_CTRL_MASK1  ---------------------------- */
#define IRQ_CTRL_MASK1_Addr                                          0x40003804UL                                                 /*!< IRQ_CTRL MASK1: Address                                      */
#define pIRQ_CTRL_MASK1                                              (*(volatile uint32_t *) IRQ_CTRL_MASK1_Addr)                 /*!< IRQ_CTRL MASK1: Pointer                                      */
#define IRQ_CTRL_MASK1_Msk                                           0x00000F00UL                                                 /*!< IRQ_CTRL MASK1: Mask                                         */
#define IRQ_CTRL_MASK1_Rst                                           0x00000000UL                                                 /*!< IRQ_CTRL MASK1: Reset                                        */

#define IRQ_CTRL_MASK1_BUFF_HALF_IRQN_Size                           1                                                            /*!< IRQ_CTRL MASK1: BUFF_HALF_IRQN Size                          */
#define IRQ_CTRL_MASK1_BUFF_HALF_IRQN_Pos                            8                                                            /*!< IRQ_CTRL MASK1: BUFF_HALF_IRQN Position                      */
#define IRQ_CTRL_MASK1_BUFF_HALF_IRQN_Msk                            (0x0001UL << IRQ_CTRL_MASK1_BUFF_HALF_IRQN_Pos)              /*!< IRQ_CTRL MASK1: BUFF_HALF_IRQN Mask                          */
#define IRQ_CTRL_MASK1_BUFF_HALF_IRQN_Rst                            0x0000UL                                                     /*!< IRQ_CTRL MASK1: BUFF_HALF_IRQN Reset                         */
#define IRQ_CTRL_MASK1_BUFF_HALF_IRQN_Addr                           0x40003804UL                                                 /*!< IRQ_CTRL MASK1: BUFF_HALF_IRQN Address                       */

#define IRQ_CTRL_MASK1_BUFF_FULL_IRQN_Size                           1                                                            /*!< IRQ_CTRL MASK1: BUFF_FULL_IRQN Size                          */
#define IRQ_CTRL_MASK1_BUFF_FULL_IRQN_Pos                            9                                                            /*!< IRQ_CTRL MASK1: BUFF_FULL_IRQN Position                      */
#define IRQ_CTRL_MASK1_BUFF_FULL_IRQN_Msk                            (0x0001UL << IRQ_CTRL_MASK1_BUFF_FULL_IRQN_Pos)              /*!< IRQ_CTRL MASK1: BUFF_FULL_IRQN Mask                          */
#define IRQ_CTRL_MASK1_BUFF_FULL_IRQN_Rst                            0x0000UL                                                     /*!< IRQ_CTRL MASK1: BUFF_FULL_IRQN Reset                         */
#define IRQ_CTRL_MASK1_BUFF_FULL_IRQN_Addr                           0x40003804UL                                                 /*!< IRQ_CTRL MASK1: BUFF_FULL_IRQN Address                       */

#define IRQ_CTRL_MASK1_SM_READY_IRQN_Size                            1                                                            /*!< IRQ_CTRL MASK1: SM_READY_IRQN Size                           */
#define IRQ_CTRL_MASK1_SM_READY_IRQN_Pos                             10                                                           /*!< IRQ_CTRL MASK1: SM_READY_IRQN Position                       */
#define IRQ_CTRL_MASK1_SM_READY_IRQN_Msk                             (0x0001UL << IRQ_CTRL_MASK1_SM_READY_IRQN_Pos)               /*!< IRQ_CTRL MASK1: SM_READY_IRQN Mask                           */
#define IRQ_CTRL_MASK1_SM_READY_IRQN_Rst                             0x0000UL                                                     /*!< IRQ_CTRL MASK1: SM_READY_IRQN Reset                          */
#define IRQ_CTRL_MASK1_SM_READY_IRQN_Addr                            0x40003804UL                                                 /*!< IRQ_CTRL MASK1: SM_READY_IRQN Address                        */

#define IRQ_CTRL_MASK1_SM_IDLE_IRQN_Size                             1                                                            /*!< IRQ_CTRL MASK1: SM_IDLE_IRQN Size                            */
#define IRQ_CTRL_MASK1_SM_IDLE_IRQN_Pos                              11                                                           /*!< IRQ_CTRL MASK1: SM_IDLE_IRQN Position                        */
#define IRQ_CTRL_MASK1_SM_IDLE_IRQN_Msk                              (0x0001UL << IRQ_CTRL_MASK1_SM_IDLE_IRQN_Pos)                /*!< IRQ_CTRL MASK1: SM_IDLE_IRQN Mask                            */
#define IRQ_CTRL_MASK1_SM_IDLE_IRQN_Rst                              0x0000UL                                                     /*!< IRQ_CTRL MASK1: SM_IDLE_IRQN Reset                           */
#define IRQ_CTRL_MASK1_SM_IDLE_IRQN_Addr                             0x40003804UL                                                 /*!< IRQ_CTRL MASK1: SM_IDLE_IRQN Address                         */

/* ----------------------------  IRQ_CTRL_STATUS0  ---------------------------- */
#define IRQ_CTRL_STATUS0_Addr                                        0x40003808UL                                                 /*!< IRQ_CTRL STATUS0: Address                                    */
#define pIRQ_CTRL_STATUS0                                            (*(volatile uint32_t *) IRQ_CTRL_STATUS0_Addr)               /*!< IRQ_CTRL STATUS0: Pointer                                    */
#define IRQ_CTRL_STATUS0_Msk                                         0x00000FFFUL                                                 /*!< IRQ_CTRL STATUS0: Mask                                       */
#define IRQ_CTRL_STATUS0_Rst                                         0x00000000UL                                                 /*!< IRQ_CTRL STATUS0: Reset                                      */

#define IRQ_CTRL_STATUS0_PREAMBLE_IRQN_Size                          1                                                            /*!< IRQ_CTRL STATUS0: PREAMBLE_IRQN Size                         */
#define IRQ_CTRL_STATUS0_PREAMBLE_IRQN_Pos                           0                                                            /*!< IRQ_CTRL STATUS0: PREAMBLE_IRQN Position                     */
#define IRQ_CTRL_STATUS0_PREAMBLE_IRQN_Msk                           (0x0001UL << IRQ_CTRL_STATUS0_PREAMBLE_IRQN_Pos)             /*!< IRQ_CTRL STATUS0: PREAMBLE_IRQN Mask                         */
#define IRQ_CTRL_STATUS0_PREAMBLE_IRQN_Rst                           0x0000UL                                                     /*!< IRQ_CTRL STATUS0: PREAMBLE_IRQN Reset                        */
#define IRQ_CTRL_STATUS0_PREAMBLE_IRQN_Addr                          0x40003808UL                                                 /*!< IRQ_CTRL STATUS0: PREAMBLE_IRQN Address                      */

#define IRQ_CTRL_STATUS0_PREAMBLE_GONE_IRQN_Size                     1                                                            /*!< IRQ_CTRL STATUS0: PREAMBLE_GONE_IRQN Size                    */
#define IRQ_CTRL_STATUS0_PREAMBLE_GONE_IRQN_Pos                      1                                                            /*!< IRQ_CTRL STATUS0: PREAMBLE_GONE_IRQN Position                */
#define IRQ_CTRL_STATUS0_PREAMBLE_GONE_IRQN_Msk                      (0x0001UL << IRQ_CTRL_STATUS0_PREAMBLE_GONE_IRQN_Pos)        /*!< IRQ_CTRL STATUS0: PREAMBLE_GONE_IRQN Mask                    */
#define IRQ_CTRL_STATUS0_PREAMBLE_GONE_IRQN_Rst                      0x0000UL                                                     /*!< IRQ_CTRL STATUS0: PREAMBLE_GONE_IRQN Reset                   */
#define IRQ_CTRL_STATUS0_PREAMBLE_GONE_IRQN_Addr                     0x40003808UL                                                 /*!< IRQ_CTRL STATUS0: PREAMBLE_GONE_IRQN Address                 */

#define IRQ_CTRL_STATUS0_SYNCWORD_IRQN_Size                          1                                                            /*!< IRQ_CTRL STATUS0: SYNCWORD_IRQN Size                         */
#define IRQ_CTRL_STATUS0_SYNCWORD_IRQN_Pos                           2                                                            /*!< IRQ_CTRL STATUS0: SYNCWORD_IRQN Position                     */
#define IRQ_CTRL_STATUS0_SYNCWORD_IRQN_Msk                           (0x0001UL << IRQ_CTRL_STATUS0_SYNCWORD_IRQN_Pos)             /*!< IRQ_CTRL STATUS0: SYNCWORD_IRQN Mask                         */
#define IRQ_CTRL_STATUS0_SYNCWORD_IRQN_Rst                           0x0000UL                                                     /*!< IRQ_CTRL STATUS0: SYNCWORD_IRQN Reset                        */
#define IRQ_CTRL_STATUS0_SYNCWORD_IRQN_Addr                          0x40003808UL                                                 /*!< IRQ_CTRL STATUS0: SYNCWORD_IRQN Address                      */

#define IRQ_CTRL_STATUS0_LENGTH_IRQN_Size                            1                                                            /*!< IRQ_CTRL STATUS0: LENGTH_IRQN Size                           */
#define IRQ_CTRL_STATUS0_LENGTH_IRQN_Pos                             3                                                            /*!< IRQ_CTRL STATUS0: LENGTH_IRQN Position                       */
#define IRQ_CTRL_STATUS0_LENGTH_IRQN_Msk                             (0x0001UL << IRQ_CTRL_STATUS0_LENGTH_IRQN_Pos)               /*!< IRQ_CTRL STATUS0: LENGTH_IRQN Mask                           */
#define IRQ_CTRL_STATUS0_LENGTH_IRQN_Rst                             0x0000UL                                                     /*!< IRQ_CTRL STATUS0: LENGTH_IRQN Reset                          */
#define IRQ_CTRL_STATUS0_LENGTH_IRQN_Addr                            0x40003808UL                                                 /*!< IRQ_CTRL STATUS0: LENGTH_IRQN Address                        */

#define IRQ_CTRL_STATUS0_PAYLOAD_IRQN_Size                           1                                                            /*!< IRQ_CTRL STATUS0: PAYLOAD_IRQN Size                          */
#define IRQ_CTRL_STATUS0_PAYLOAD_IRQN_Pos                            4                                                            /*!< IRQ_CTRL STATUS0: PAYLOAD_IRQN Position                      */
#define IRQ_CTRL_STATUS0_PAYLOAD_IRQN_Msk                            (0x0001UL << IRQ_CTRL_STATUS0_PAYLOAD_IRQN_Pos)              /*!< IRQ_CTRL STATUS0: PAYLOAD_IRQN Mask                          */
#define IRQ_CTRL_STATUS0_PAYLOAD_IRQN_Rst                            0x0000UL                                                     /*!< IRQ_CTRL STATUS0: PAYLOAD_IRQN Reset                         */
#define IRQ_CTRL_STATUS0_PAYLOAD_IRQN_Addr                           0x40003808UL                                                 /*!< IRQ_CTRL STATUS0: PAYLOAD_IRQN Address                       */

#define IRQ_CTRL_STATUS0_PAYLOAD_BLOC_IRQN_Size                      1                                                            /*!< IRQ_CTRL STATUS0: PAYLOAD_BLOC_IRQN Size                     */
#define IRQ_CTRL_STATUS0_PAYLOAD_BLOC_IRQN_Pos                       5                                                            /*!< IRQ_CTRL STATUS0: PAYLOAD_BLOC_IRQN Position                 */
#define IRQ_CTRL_STATUS0_PAYLOAD_BLOC_IRQN_Msk                       (0x0001UL << IRQ_CTRL_STATUS0_PAYLOAD_BLOC_IRQN_Pos)         /*!< IRQ_CTRL STATUS0: PAYLOAD_BLOC_IRQN Mask                     */
#define IRQ_CTRL_STATUS0_PAYLOAD_BLOC_IRQN_Rst                       0x0000UL                                                     /*!< IRQ_CTRL STATUS0: PAYLOAD_BLOC_IRQN Reset                    */
#define IRQ_CTRL_STATUS0_PAYLOAD_BLOC_IRQN_Addr                      0x40003808UL                                                 /*!< IRQ_CTRL STATUS0: PAYLOAD_BLOC_IRQN Address                  */

#define IRQ_CTRL_STATUS0_CRC_CHK_IRQN_Size                           1                                                            /*!< IRQ_CTRL STATUS0: CRC_CHK_IRQN Size                          */
#define IRQ_CTRL_STATUS0_CRC_CHK_IRQN_Pos                            6                                                            /*!< IRQ_CTRL STATUS0: CRC_CHK_IRQN Position                      */
#define IRQ_CTRL_STATUS0_CRC_CHK_IRQN_Msk                            (0x0001UL << IRQ_CTRL_STATUS0_CRC_CHK_IRQN_Pos)              /*!< IRQ_CTRL STATUS0: CRC_CHK_IRQN Mask                          */
#define IRQ_CTRL_STATUS0_CRC_CHK_IRQN_Rst                            0x0000UL                                                     /*!< IRQ_CTRL STATUS0: CRC_CHK_IRQN Reset                         */
#define IRQ_CTRL_STATUS0_CRC_CHK_IRQN_Addr                           0x40003808UL                                                 /*!< IRQ_CTRL STATUS0: CRC_CHK_IRQN Address                       */

#define IRQ_CTRL_STATUS0_EOF_IRQN_Size                               1                                                            /*!< IRQ_CTRL STATUS0: EOF_IRQN Size                              */
#define IRQ_CTRL_STATUS0_EOF_IRQN_Pos                                7                                                            /*!< IRQ_CTRL STATUS0: EOF_IRQN Position                          */
#define IRQ_CTRL_STATUS0_EOF_IRQN_Msk                                (0x0001UL << IRQ_CTRL_STATUS0_EOF_IRQN_Pos)                  /*!< IRQ_CTRL STATUS0: EOF_IRQN Mask                              */
#define IRQ_CTRL_STATUS0_EOF_IRQN_Rst                                0x0000UL                                                     /*!< IRQ_CTRL STATUS0: EOF_IRQN Reset                             */
#define IRQ_CTRL_STATUS0_EOF_IRQN_Addr                               0x40003808UL                                                 /*!< IRQ_CTRL STATUS0: EOF_IRQN Address                           */

#define IRQ_CTRL_STATUS0_BUFF_HALF_IRQN_Size                         1                                                            /*!< IRQ_CTRL STATUS0: BUFF_HALF_IRQN Size                        */
#define IRQ_CTRL_STATUS0_BUFF_HALF_IRQN_Pos                          8                                                            /*!< IRQ_CTRL STATUS0: BUFF_HALF_IRQN Position                    */
#define IRQ_CTRL_STATUS0_BUFF_HALF_IRQN_Msk                          (0x0001UL << IRQ_CTRL_STATUS0_BUFF_HALF_IRQN_Pos)            /*!< IRQ_CTRL STATUS0: BUFF_HALF_IRQN Mask                        */
#define IRQ_CTRL_STATUS0_BUFF_HALF_IRQN_Rst                          0x0000UL                                                     /*!< IRQ_CTRL STATUS0: BUFF_HALF_IRQN Reset                       */
#define IRQ_CTRL_STATUS0_BUFF_HALF_IRQN_Addr                         0x40003808UL                                                 /*!< IRQ_CTRL STATUS0: BUFF_HALF_IRQN Address                     */

#define IRQ_CTRL_STATUS0_BUFF_FULL_IRQN_Size                         1                                                            /*!< IRQ_CTRL STATUS0: BUFF_FULL_IRQN Size                        */
#define IRQ_CTRL_STATUS0_BUFF_FULL_IRQN_Pos                          9                                                            /*!< IRQ_CTRL STATUS0: BUFF_FULL_IRQN Position                    */
#define IRQ_CTRL_STATUS0_BUFF_FULL_IRQN_Msk                          (0x0001UL << IRQ_CTRL_STATUS0_BUFF_FULL_IRQN_Pos)            /*!< IRQ_CTRL STATUS0: BUFF_FULL_IRQN Mask                        */
#define IRQ_CTRL_STATUS0_BUFF_FULL_IRQN_Rst                          0x0000UL                                                     /*!< IRQ_CTRL STATUS0: BUFF_FULL_IRQN Reset                       */
#define IRQ_CTRL_STATUS0_BUFF_FULL_IRQN_Addr                         0x40003808UL                                                 /*!< IRQ_CTRL STATUS0: BUFF_FULL_IRQN Address                     */

#define IRQ_CTRL_STATUS0_SM_READY_IRQN_Size                          1                                                            /*!< IRQ_CTRL STATUS0: SM_READY_IRQN Size                         */
#define IRQ_CTRL_STATUS0_SM_READY_IRQN_Pos                           10                                                           /*!< IRQ_CTRL STATUS0: SM_READY_IRQN Position                     */
#define IRQ_CTRL_STATUS0_SM_READY_IRQN_Msk                           (0x0001UL << IRQ_CTRL_STATUS0_SM_READY_IRQN_Pos)             /*!< IRQ_CTRL STATUS0: SM_READY_IRQN Mask                         */
#define IRQ_CTRL_STATUS0_SM_READY_IRQN_Rst                           0x0000UL                                                     /*!< IRQ_CTRL STATUS0: SM_READY_IRQN Reset                        */
#define IRQ_CTRL_STATUS0_SM_READY_IRQN_Addr                          0x40003808UL                                                 /*!< IRQ_CTRL STATUS0: SM_READY_IRQN Address                      */

#define IRQ_CTRL_STATUS0_SM_IDLE_IRQN_Size                           1                                                            /*!< IRQ_CTRL STATUS0: SM_IDLE_IRQN Size                          */
#define IRQ_CTRL_STATUS0_SM_IDLE_IRQN_Pos                            11                                                           /*!< IRQ_CTRL STATUS0: SM_IDLE_IRQN Position                      */
#define IRQ_CTRL_STATUS0_SM_IDLE_IRQN_Msk                            (0x0001UL << IRQ_CTRL_STATUS0_SM_IDLE_IRQN_Pos)              /*!< IRQ_CTRL STATUS0: SM_IDLE_IRQN Mask                          */
#define IRQ_CTRL_STATUS0_SM_IDLE_IRQN_Rst                            0x0000UL                                                     /*!< IRQ_CTRL STATUS0: SM_IDLE_IRQN Reset                         */
#define IRQ_CTRL_STATUS0_SM_IDLE_IRQN_Addr                           0x40003808UL                                                 /*!< IRQ_CTRL STATUS0: SM_IDLE_IRQN Address                       */

/* ----------------------------  IRQ_CTRL_STATUS1  ---------------------------- */
#define IRQ_CTRL_STATUS1_Addr                                        0x4000380CUL                                                 /*!< IRQ_CTRL STATUS1: Address                                    */
#define pIRQ_CTRL_STATUS1                                            (*(volatile uint32_t *) IRQ_CTRL_STATUS1_Addr)               /*!< IRQ_CTRL STATUS1: Pointer                                    */
#define IRQ_CTRL_STATUS1_Msk                                         0x00000FFFUL                                                 /*!< IRQ_CTRL STATUS1: Mask                                       */
#define IRQ_CTRL_STATUS1_Rst                                         0x00000000UL                                                 /*!< IRQ_CTRL STATUS1: Reset                                      */

#define IRQ_CTRL_STATUS1_PREAMBLE_IRQN_Size                          1                                                            /*!< IRQ_CTRL STATUS1: PREAMBLE_IRQN Size                         */
#define IRQ_CTRL_STATUS1_PREAMBLE_IRQN_Pos                           0                                                            /*!< IRQ_CTRL STATUS1: PREAMBLE_IRQN Position                     */
#define IRQ_CTRL_STATUS1_PREAMBLE_IRQN_Msk                           (0x0001UL << IRQ_CTRL_STATUS1_PREAMBLE_IRQN_Pos)             /*!< IRQ_CTRL STATUS1: PREAMBLE_IRQN Mask                         */
#define IRQ_CTRL_STATUS1_PREAMBLE_IRQN_Rst                           0x0000UL                                                     /*!< IRQ_CTRL STATUS1: PREAMBLE_IRQN Reset                        */
#define IRQ_CTRL_STATUS1_PREAMBLE_IRQN_Addr                          0x4000380cUL                                                 /*!< IRQ_CTRL STATUS1: PREAMBLE_IRQN Address                      */

#define IRQ_CTRL_STATUS1_PREAMBLE_GONE_IRQN_Size                     1                                                            /*!< IRQ_CTRL STATUS1: PREAMBLE_GONE_IRQN Size                    */
#define IRQ_CTRL_STATUS1_PREAMBLE_GONE_IRQN_Pos                      1                                                            /*!< IRQ_CTRL STATUS1: PREAMBLE_GONE_IRQN Position                */
#define IRQ_CTRL_STATUS1_PREAMBLE_GONE_IRQN_Msk                      (0x0001UL << IRQ_CTRL_STATUS1_PREAMBLE_GONE_IRQN_Pos)        /*!< IRQ_CTRL STATUS1: PREAMBLE_GONE_IRQN Mask                    */
#define IRQ_CTRL_STATUS1_PREAMBLE_GONE_IRQN_Rst                      0x0000UL                                                     /*!< IRQ_CTRL STATUS1: PREAMBLE_GONE_IRQN Reset                   */
#define IRQ_CTRL_STATUS1_PREAMBLE_GONE_IRQN_Addr                     0x4000380cUL                                                 /*!< IRQ_CTRL STATUS1: PREAMBLE_GONE_IRQN Address                 */

#define IRQ_CTRL_STATUS1_SYNCWORD_IRQN_Size                          1                                                            /*!< IRQ_CTRL STATUS1: SYNCWORD_IRQN Size                         */
#define IRQ_CTRL_STATUS1_SYNCWORD_IRQN_Pos                           2                                                            /*!< IRQ_CTRL STATUS1: SYNCWORD_IRQN Position                     */
#define IRQ_CTRL_STATUS1_SYNCWORD_IRQN_Msk                           (0x0001UL << IRQ_CTRL_STATUS1_SYNCWORD_IRQN_Pos)             /*!< IRQ_CTRL STATUS1: SYNCWORD_IRQN Mask                         */
#define IRQ_CTRL_STATUS1_SYNCWORD_IRQN_Rst                           0x0000UL                                                     /*!< IRQ_CTRL STATUS1: SYNCWORD_IRQN Reset                        */
#define IRQ_CTRL_STATUS1_SYNCWORD_IRQN_Addr                          0x4000380cUL                                                 /*!< IRQ_CTRL STATUS1: SYNCWORD_IRQN Address                      */

#define IRQ_CTRL_STATUS1_LENGTH_IRQN_Size                            1                                                            /*!< IRQ_CTRL STATUS1: LENGTH_IRQN Size                           */
#define IRQ_CTRL_STATUS1_LENGTH_IRQN_Pos                             3                                                            /*!< IRQ_CTRL STATUS1: LENGTH_IRQN Position                       */
#define IRQ_CTRL_STATUS1_LENGTH_IRQN_Msk                             (0x0001UL << IRQ_CTRL_STATUS1_LENGTH_IRQN_Pos)               /*!< IRQ_CTRL STATUS1: LENGTH_IRQN Mask                           */
#define IRQ_CTRL_STATUS1_LENGTH_IRQN_Rst                             0x0000UL                                                     /*!< IRQ_CTRL STATUS1: LENGTH_IRQN Reset                          */
#define IRQ_CTRL_STATUS1_LENGTH_IRQN_Addr                            0x4000380cUL                                                 /*!< IRQ_CTRL STATUS1: LENGTH_IRQN Address                        */

#define IRQ_CTRL_STATUS1_PAYLOAD_IRQN_Size                           1                                                            /*!< IRQ_CTRL STATUS1: PAYLOAD_IRQN Size                          */
#define IRQ_CTRL_STATUS1_PAYLOAD_IRQN_Pos                            4                                                            /*!< IRQ_CTRL STATUS1: PAYLOAD_IRQN Position                      */
#define IRQ_CTRL_STATUS1_PAYLOAD_IRQN_Msk                            (0x0001UL << IRQ_CTRL_STATUS1_PAYLOAD_IRQN_Pos)              /*!< IRQ_CTRL STATUS1: PAYLOAD_IRQN Mask                          */
#define IRQ_CTRL_STATUS1_PAYLOAD_IRQN_Rst                            0x0000UL                                                     /*!< IRQ_CTRL STATUS1: PAYLOAD_IRQN Reset                         */
#define IRQ_CTRL_STATUS1_PAYLOAD_IRQN_Addr                           0x4000380cUL                                                 /*!< IRQ_CTRL STATUS1: PAYLOAD_IRQN Address                       */

#define IRQ_CTRL_STATUS1_PAYLOAD_BLOC_IRQN_Size                      1                                                            /*!< IRQ_CTRL STATUS1: PAYLOAD_BLOC_IRQN Size                     */
#define IRQ_CTRL_STATUS1_PAYLOAD_BLOC_IRQN_Pos                       5                                                            /*!< IRQ_CTRL STATUS1: PAYLOAD_BLOC_IRQN Position                 */
#define IRQ_CTRL_STATUS1_PAYLOAD_BLOC_IRQN_Msk                       (0x0001UL << IRQ_CTRL_STATUS1_PAYLOAD_BLOC_IRQN_Pos)         /*!< IRQ_CTRL STATUS1: PAYLOAD_BLOC_IRQN Mask                     */
#define IRQ_CTRL_STATUS1_PAYLOAD_BLOC_IRQN_Rst                       0x0000UL                                                     /*!< IRQ_CTRL STATUS1: PAYLOAD_BLOC_IRQN Reset                    */
#define IRQ_CTRL_STATUS1_PAYLOAD_BLOC_IRQN_Addr                      0x4000380cUL                                                 /*!< IRQ_CTRL STATUS1: PAYLOAD_BLOC_IRQN Address                  */

#define IRQ_CTRL_STATUS1_CRC_CHK_IRQN_Size                           1                                                            /*!< IRQ_CTRL STATUS1: CRC_CHK_IRQN Size                          */
#define IRQ_CTRL_STATUS1_CRC_CHK_IRQN_Pos                            6                                                            /*!< IRQ_CTRL STATUS1: CRC_CHK_IRQN Position                      */
#define IRQ_CTRL_STATUS1_CRC_CHK_IRQN_Msk                            (0x0001UL << IRQ_CTRL_STATUS1_CRC_CHK_IRQN_Pos)              /*!< IRQ_CTRL STATUS1: CRC_CHK_IRQN Mask                          */
#define IRQ_CTRL_STATUS1_CRC_CHK_IRQN_Rst                            0x0000UL                                                     /*!< IRQ_CTRL STATUS1: CRC_CHK_IRQN Reset                         */
#define IRQ_CTRL_STATUS1_CRC_CHK_IRQN_Addr                           0x4000380cUL                                                 /*!< IRQ_CTRL STATUS1: CRC_CHK_IRQN Address                       */

#define IRQ_CTRL_STATUS1_EOF_IRQN_Size                               1                                                            /*!< IRQ_CTRL STATUS1: EOF_IRQN Size                              */
#define IRQ_CTRL_STATUS1_EOF_IRQN_Pos                                7                                                            /*!< IRQ_CTRL STATUS1: EOF_IRQN Position                          */
#define IRQ_CTRL_STATUS1_EOF_IRQN_Msk                                (0x0001UL << IRQ_CTRL_STATUS1_EOF_IRQN_Pos)                  /*!< IRQ_CTRL STATUS1: EOF_IRQN Mask                              */
#define IRQ_CTRL_STATUS1_EOF_IRQN_Rst                                0x0000UL                                                     /*!< IRQ_CTRL STATUS1: EOF_IRQN Reset                             */
#define IRQ_CTRL_STATUS1_EOF_IRQN_Addr                               0x4000380cUL                                                 /*!< IRQ_CTRL STATUS1: EOF_IRQN Address                           */

#define IRQ_CTRL_STATUS1_BUFF_HALF_IRQN_Size                         1                                                            /*!< IRQ_CTRL STATUS1: BUFF_HALF_IRQN Size                        */
#define IRQ_CTRL_STATUS1_BUFF_HALF_IRQN_Pos                          8                                                            /*!< IRQ_CTRL STATUS1: BUFF_HALF_IRQN Position                    */
#define IRQ_CTRL_STATUS1_BUFF_HALF_IRQN_Msk                          (0x0001UL << IRQ_CTRL_STATUS1_BUFF_HALF_IRQN_Pos)            /*!< IRQ_CTRL STATUS1: BUFF_HALF_IRQN Mask                        */
#define IRQ_CTRL_STATUS1_BUFF_HALF_IRQN_Rst                          0x0000UL                                                     /*!< IRQ_CTRL STATUS1: BUFF_HALF_IRQN Reset                       */
#define IRQ_CTRL_STATUS1_BUFF_HALF_IRQN_Addr                         0x4000380cUL                                                 /*!< IRQ_CTRL STATUS1: BUFF_HALF_IRQN Address                     */

#define IRQ_CTRL_STATUS1_BUFF_FULL_IRQN_Size                         1                                                            /*!< IRQ_CTRL STATUS1: BUFF_FULL_IRQN Size                        */
#define IRQ_CTRL_STATUS1_BUFF_FULL_IRQN_Pos                          9                                                            /*!< IRQ_CTRL STATUS1: BUFF_FULL_IRQN Position                    */
#define IRQ_CTRL_STATUS1_BUFF_FULL_IRQN_Msk                          (0x0001UL << IRQ_CTRL_STATUS1_BUFF_FULL_IRQN_Pos)            /*!< IRQ_CTRL STATUS1: BUFF_FULL_IRQN Mask                        */
#define IRQ_CTRL_STATUS1_BUFF_FULL_IRQN_Rst                          0x0000UL                                                     /*!< IRQ_CTRL STATUS1: BUFF_FULL_IRQN Reset                       */
#define IRQ_CTRL_STATUS1_BUFF_FULL_IRQN_Addr                         0x4000380cUL                                                 /*!< IRQ_CTRL STATUS1: BUFF_FULL_IRQN Address                     */

#define IRQ_CTRL_STATUS1_SM_READY_IRQN_Size                          1                                                            /*!< IRQ_CTRL STATUS1: SM_READY_IRQN Size                         */
#define IRQ_CTRL_STATUS1_SM_READY_IRQN_Pos                           10                                                           /*!< IRQ_CTRL STATUS1: SM_READY_IRQN Position                     */
#define IRQ_CTRL_STATUS1_SM_READY_IRQN_Msk                           (0x0001UL << IRQ_CTRL_STATUS1_SM_READY_IRQN_Pos)             /*!< IRQ_CTRL STATUS1: SM_READY_IRQN Mask                         */
#define IRQ_CTRL_STATUS1_SM_READY_IRQN_Rst                           0x0000UL                                                     /*!< IRQ_CTRL STATUS1: SM_READY_IRQN Reset                        */
#define IRQ_CTRL_STATUS1_SM_READY_IRQN_Addr                          0x4000380cUL                                                 /*!< IRQ_CTRL STATUS1: SM_READY_IRQN Address                      */

#define IRQ_CTRL_STATUS1_SM_IDLE_IRQN_Size                           1                                                            /*!< IRQ_CTRL STATUS1: SM_IDLE_IRQN Size                          */
#define IRQ_CTRL_STATUS1_SM_IDLE_IRQN_Pos                            11                                                           /*!< IRQ_CTRL STATUS1: SM_IDLE_IRQN Position                      */
#define IRQ_CTRL_STATUS1_SM_IDLE_IRQN_Msk                            (0x0001UL << IRQ_CTRL_STATUS1_SM_IDLE_IRQN_Pos)              /*!< IRQ_CTRL STATUS1: SM_IDLE_IRQN Mask                          */
#define IRQ_CTRL_STATUS1_SM_IDLE_IRQN_Rst                            0x0000UL                                                     /*!< IRQ_CTRL STATUS1: SM_IDLE_IRQN Reset                         */
#define IRQ_CTRL_STATUS1_SM_IDLE_IRQN_Addr                           0x4000380cUL                                                 /*!< IRQ_CTRL STATUS1: SM_IDLE_IRQN Address                       */

/* ======================================================================================== */
/* ================          struct 'AFC'                                  ================ */
/* ======================================================================================== */

#define SERDES_PM0_CFG_Size                                          32
#define SERDES_PM0_CFG_Pos                                           0
#define SERDES_PM0_CFG_Addr                                          0x40004128UL

#define SERDES_PM0_CFG_CORR0_EN_Msk
#define SERDES_PM0_CFG_CORR1_EN_Msk
#define SERDES_PM0_CFG_INPUT_SEL_TX_OUTPUT_Eval
#define SERDES_PM0_CFG_INPUT_SEL_Pos

#define SERDES_PM0_PATTERN1_Size                                     32
#define SERDES_PM0_PATTERN1_Pos                                      0
#define SERDES_PM0_PATTERN1_Addr                                     0x40004130UL

#define SERDES_PM0_STATUS1_Size                                      ()
#define SERDES_PM0_STATUS1_Pos                                       ()
#define SERDES_PM0_STATUS1_Addr                                      0x40004138UL

#define SERDES_PM0_STATUS1_PATTERN_SIZE_Size                         6
#define SERDES_PM0_STATUS1_PATTERN_SIZE_Pos                          20
#define SERDES_PM0_STATUS1_PATTERN_SIZE_Addr                         0x40004138UL


/* ----------------------------  AFC_CONFIG  ---------------------------- */
#define AFC_CONFIG_Addr                                              0x400041F8UL                                                 /*!< AFC CONFIG: Address                                          */
#define pAFC_CONFIG                                                  (*(volatile uint32_t *) AFC_CONFIG_Addr)                     /*!< AFC CONFIG: Pointer                                          */
#define AFC_CONFIG_Msk                                               0x00000007UL                                                 /*!< AFC CONFIG: Mask                                             */
#define AFC_CONFIG_Rst                                               0x00000000UL                                                 /*!< AFC CONFIG: Reset                                            */

#define AFC_CONFIG_MODE_Size                                         3                                                            /*!< AFC CONFIG: MODE Size                                        */
#define AFC_CONFIG_MODE_Pos                                          0                                                            /*!< AFC CONFIG: MODE Position                                    */
#define AFC_CONFIG_MODE_Msk                                          (0x0007UL << AFC_CONFIG_MODE_Pos)                            /*!< AFC CONFIG: MODE Mask                                        */
#define AFC_CONFIG_MODE_Rst                                          0x0000UL                                                     /*!< AFC CONFIG: MODE Reset                                       */
#define AFC_CONFIG_MODE_Addr                                         0x400041f8UL                                                 /*!< AFC CONFIG: MODE Address                                     */
#define AFC_CONFIG_MODE_OFF_Eval                                     0

/* ----------------------------  AFC_FREQUENCY_ERROR  ---------------------------- */
#define AFC_FREQUENCY_ERROR_Addr                                     0x40004208UL                                                 /*!< AFC FREQUENCY_ERROR: Address                                 */
#define pAFC_FREQUENCY_ERROR                                         (*(volatile uint32_t *) AFC_FREQUENCY_ERROR_Addr)            /*!< AFC FREQUENCY_ERROR: Pointer                                 */
#define AFC_FREQUENCY_ERROR_Msk                                      0x0000FFFFUL                                                 /*!< AFC FREQUENCY_ERROR: Mask                                    */
#define AFC_FREQUENCY_ERROR_Rst                                      0x00000000UL                                                 /*!< AFC FREQUENCY_ERROR: Reset                                   */

#define AFC_FREQUENCY_ERROR_READBACK_Size                            16                                                           /*!< AFC FREQUENCY_ERROR: READBACK Size                           */
#define AFC_FREQUENCY_ERROR_READBACK_Pos                             0                                                            /*!< AFC FREQUENCY_ERROR: READBACK Position                       */
#define AFC_FREQUENCY_ERROR_READBACK_Msk                             (0xffffUL << AFC_FREQUENCY_ERROR_READBACK_Pos)               /*!< AFC FREQUENCY_ERROR: READBACK Mask                           */
#define AFC_FREQUENCY_ERROR_READBACK_Rst                             0x0000UL                                                     /*!< AFC FREQUENCY_ERROR: READBACK Reset                          */
#define AFC_FREQUENCY_ERROR_READBACK_Addr                            0x40004208UL                                                 /*!< AFC FREQUENCY_ERROR: READBACK Address                        */

/* ======================================================================================== */
/* ================          struct 'CRMGT'                                ================ */
/* ======================================================================================== */

/* ----------------------------  CRMGT_PROC_CLK_EN  ---------------------------- */
#define CRMGT_PROC_CLK_EN_Addr                                       0x40004278UL                                                 /*!< CRMGT PROC_CLK_EN: Address                                   */
#define pCRMGT_PROC_CLK_EN                                           (*(volatile uint32_t *) CRMGT_PROC_CLK_EN_Addr)              /*!< CRMGT PROC_CLK_EN: Pointer                                   */
#define CRMGT_PROC_CLK_EN_Msk                                        0xFFFFFFFFUL                                                 /*!< CRMGT PROC_CLK_EN: Mask                                      */
#define CRMGT_PROC_CLK_EN_Rst                                        0x00000000UL                                                 /*!< CRMGT PROC_CLK_EN: Reset                                     */

#define CRMGT_PROC_CLK_EN_CONFIGURATION_Size                         32                                                           /*!< CRMGT PROC_CLK_EN: CONFIGURATION Size                        */
#define CRMGT_PROC_CLK_EN_CONFIGURATION_Pos                          0                                                            /*!< CRMGT PROC_CLK_EN: CONFIGURATION Position                    */
#define CRMGT_PROC_CLK_EN_CONFIGURATION_Msk                          (0xffffffffUL << CRMGT_PROC_CLK_EN_CONFIGURATION_Pos)        /*!< CRMGT PROC_CLK_EN: CONFIGURATION Mask                        */
#define CRMGT_PROC_CLK_EN_CONFIGURATION_Rst                          0x0000UL                                                     /*!< CRMGT PROC_CLK_EN: CONFIGURATION Reset                       */
#define CRMGT_PROC_CLK_EN_CONFIGURATION_Addr                         0x40004278UL                                                 /*!< CRMGT PROC_CLK_EN: CONFIGURATION Address                     */

/* ======================================================================================== */
/* ================          struct 'MISC'                                 ================ */
/* ======================================================================================== */

/* ----------------------------  MISC_FW  ---------------------------- */
#define MISC_FW_Addr                                                 0x400042B4UL                                                 /*!< MISC FW: Address                                             */
#define pMISC_FW                                                     (*(volatile uint32_t *) MISC_FW_Addr)                        /*!< MISC FW: Pointer                                             */
#define MISC_FW_Msk                                                  0x00003F03UL                                                 /*!< MISC FW: Mask                                                */
#define MISC_FW_Rst                                                  0x00000000UL                                                 /*!< MISC FW: Reset                                               */

#define MISC_FW_STATUS_Size                                          2                                                            /*!< MISC FW: STATUS Size                                         */
#define MISC_FW_STATUS_Pos                                           0                                                            /*!< MISC FW: STATUS Position                                     */
#define MISC_FW_STATUS_Msk                                           (0x0003UL << MISC_FW_STATUS_Pos)                             /*!< MISC FW: STATUS Mask                                         */
#define MISC_FW_STATUS_Rst                                           0x0000UL                                                     /*!< MISC FW: STATUS Reset                                        */
#define MISC_FW_STATUS_Addr                                          0x400042b4UL                                                 /*!< MISC FW: STATUS Address                                      */
#define MISC_FW_STATUS_TRANS_Eval                                    0
#define MISC_FW_STATUS_STATE_Eval                                    1
#define MISC_FW_STATUS_IDLE_Eval                                     2
#define MISC_FW_STATUS_CAL_Eval                                      3

#define MISC_FW_CURR_STATE_Size                                      6                                                            /*!< MISC FW: CURR_STATE Size                                     */
#define MISC_FW_CURR_STATE_Pos                                       8                                                            /*!< MISC FW: CURR_STATE Position                                 */
#define MISC_FW_CURR_STATE_Msk                                       (0x003fUL << MISC_FW_CURR_STATE_Pos)                         /*!< MISC FW: CURR_STATE Mask                                     */
#define MISC_FW_CURR_STATE_Rst                                       0x0000UL                                                     /*!< MISC FW: CURR_STATE Reset                                    */
#define MISC_FW_CURR_STATE_Addr                                      0x400042b4UL                                                 /*!< MISC FW: CURR_STATE Address                                  */
#define MISC_FW_CURR_STATE_PHY_SLEEP_Eval                            0
#define MISC_FW_CURR_STATE_PHY_OFF_Eval                              1
#define MISC_FW_CURR_STATE_PHY_ON_Eval                               2
#define MISC_FW_CURR_STATE_PHY_RX_Eval                               3
#define MISC_FW_CURR_STATE_PHY_TX_Eval                               4
#define MISC_FW_CURR_STATE_CONFIGURING_Eval                          5
#define MISC_FW_CURR_STATE_CCA_Eval                                  6
#define MISC_FW_CURR_STATE_CALIBRATING_Eval                          9
#define MISC_FW_CURR_STATE_MONITORING_Eval                           10

#define MISC_FW_ERROR_Size                                          8                                                            /*!< MISC FW: STATUS Size                                         */
#define MISC_FW_ERROR_Pos                                           24                                                            /*!< MISC FW: STATUS Position                                     */
#define MISC_FW_ERROR_Msk                                           (0x00FFUL << MISC_FW_ERROR_Pos)                             /*!< MISC FW: STATUS Mask                                         */
#define MISC_FW_ERROR_Rst                                           0x0000UL                                                     /*!< MISC FW: STATUS Reset                                        */
#define MISC_FW_ERROR_Addr                                          0x400042b4UL                                                 /*!< MISC FW: STATUS Address                                      */

/* ======================================================================================== */

#define MISC_MCR_SNOOP_CFG_EN_Size                                  1
#define MISC_MCR_SNOOP_CFG_EN_Pos                                   0
#define MISC_MCR_SNOOP_CFG_EN_Addr                                  0x400042bcUL

#define MISC_MCR_SNOOP_CFG_ADDRESS_Size                             8
#define MISC_MCR_SNOOP_CFG_ADDRESS_Pos                              24
#define MISC_MCR_SNOOP_CFG_ADDRESS_Addr                             0x400042bcUL


/** @} */ /* End of group HW_Macro Hardware Macro Definition */
/** @} */ /* End of group PHY_headers PHY */
/** @} */ /* End of group adf7030-1_headers Headers */
/** @} */ /* End of group adf7030-1 ADF7030-1 Driver */

#ifdef __cplusplus
}
#endif


#endif  /* ADF7030_HW_MACRO_H */

