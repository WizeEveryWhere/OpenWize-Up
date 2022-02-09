/*!
 *****************************************************************************
  @file:    adf7030-1_fw_macro.h
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


#ifndef ADF7030_FW_MACRO_H
#define ADF7030_FW_MACRO_H

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

/** @addtogroup FW_Macro Firmware Macro Definition
  * @{
  */


#define GET_FIELD(var, reg_name) ( (var >> reg_name##_Pos) & ( ~(0xFFFFFFFF << reg_name##_Size) ) )
#define SET_FIELD(var, reg_name) ( ( ( var & ( ~( 0xFFFFFFFF << reg_name##_Size ) ) ) << reg_name##_Pos ) | (~ reg_name##_Msk ) )

/* ============================================================================================ */
/* =======================             Firmware memory map             ======================== */
/* ============================================================================================ */
#define BRAM_BASE                   0x20000000UL
#define SM_CONFIG_BASE              0x200000D0UL
#define SM_DATA_BASE                0x20000120UL
#define PROFILE_BASE                0x200002E4UL
#define GENERIC_PKT_BASE            0x200004F4UL
#define ANAFILT_LUTS_BASE           0x2000060CUL
#define DIGFILT_LUTS_BASE           0x200006B4UL
#define DIGFILT2_LUTS_BASE          0x20000794UL
#define PLLBW_LUTS_BASE             0x20000820UL
#define RSSICFG_LUTS_BASE           0x20000864UL


/* ============================================================================================ */
/* ================             Firmware RAM structure declaration             ================ */
/* ============================================================================================ */

#define SM_CONFIG                   ((sm_config_t         *) SM_CONFIG_BASE)
#define SM_DATA                     ((sm_data_t           *) SM_DATA_BASE)
#define PROFILE                     ((profile_t           *) PROFILE_BASE)
#define GENERIC_PKT                 ((generic_pkt_t       *) GENERIC_PKT_BASE)
#define ANAFILT_LUTS                ((ANAFILT_LUTS_t      *) ANAFILT_LUTS_BASE)
#define DIGFILT_LUTS                ((DIGFILT_LUTS_t      *) DIGFILT_LUTS_BASE)
#define DIGFILT2_LUTS               ((DIGFILT2_LUTS_t     *) DIGFILT2_LUTS_BASE)
#define PLLBW_LUTS                  ((PLLBW_LUTS_t        *) PLLBW_LUTS_BASE)
#define RSSICFG_LUTS                ((RSSICFG_LUTS_t      *) RSSICFG_LUTS_BASE)


/* ======================================================================================== */
/* ================          struct 'SM_CONFIG'                            ================ */
/* ======================================================================================== */

#define SM_CONFIG_WAKE_SOURCE_Addr (0x200000E4UL)


#define SM_CONFIG_TRANSITION_TIME_VAL_Addr (0x200000F8UL)
#define SM_CONFIG_TRANSITION_TIME_VAL_Pos  (0)
#define SM_CONFIG_TRANSITION_TIME_VAL_Size (16)

/* ----------------------------  SM_CONFIG_GPIO_CMD_0  ---------------------------- */
#define SM_CONFIG_GPIO_CMD_0_Addr                                    0x200000FCUL                                                 /*!< SM_CONFIG GPIO_CMD_0: Address                                */
#define pSM_CONFIG_GPIO_CMD_0                                        (*(volatile uint8_t *) SM_CONFIG_GPIO_CMD_0_Addr)            /*!< SM_CONFIG GPIO_CMD_0: Pointer                                */
#define SM_CONFIG_GPIO_CMD_0_Msk                                     0x000000FFUL                                                 /*!< SM_CONFIG GPIO_CMD_0: Mask                                   */
#define SM_CONFIG_GPIO_CMD_0_Rst                                     0x00000000UL                                                 /*!< SM_CONFIG GPIO_CMD_0: Reset                                  */

#define SM_CONFIG_GPIO_CMD_0_VAL_Size                                8                                                            /*!< SM_CONFIG GPIO_CMD_0: VAL Size                               */
#define SM_CONFIG_GPIO_CMD_0_VAL_Pos                                 0                                                            /*!< SM_CONFIG GPIO_CMD_0: VAL Position                           */
#define SM_CONFIG_GPIO_CMD_0_VAL_Msk                                 (0xffUL << SM_CONFIG_GPIO_CMD_0_VAL_Pos)                     /*!< SM_CONFIG GPIO_CMD_0: VAL Mask                               */
#define SM_CONFIG_GPIO_CMD_0_VAL_Rst                                 0x0UL                                                        /*!< SM_CONFIG GPIO_CMD_0: VAL Reset                              */
#define SM_CONFIG_GPIO_CMD_0_VAL_Addr                                0x200000fcUL                                                 /*!< SM_CONFIG GPIO_CMD_0: VAL Address                            */

/* ----------------------------  SM_CONFIG_GPIO_CMD_1  ---------------------------- */
#define SM_CONFIG_GPIO_CMD_1_Addr                                    0x200000FEUL                                                 /*!< SM_CONFIG GPIO_CMD_1: Address                                */
#define pSM_CONFIG_GPIO_CMD_1                                        (*(volatile uint8_t *) SM_CONFIG_GPIO_CMD_1_Addr)            /*!< SM_CONFIG GPIO_CMD_1: Pointer                                */
#define SM_CONFIG_GPIO_CMD_1_Msk                                     0x000000FFUL                                                 /*!< SM_CONFIG GPIO_CMD_1: Mask                                   */
#define SM_CONFIG_GPIO_CMD_1_Rst                                     0x00000000UL                                                 /*!< SM_CONFIG GPIO_CMD_1: Reset                                  */

#define SM_CONFIG_GPIO_CMD_1_VAL_Size                                8                                                            /*!< SM_CONFIG GPIO_CMD_1: VAL Size                               */
#define SM_CONFIG_GPIO_CMD_1_VAL_Pos                                 0                                                            /*!< SM_CONFIG GPIO_CMD_1: VAL Position                           */
#define SM_CONFIG_GPIO_CMD_1_VAL_Msk                                 (0xffUL << SM_CONFIG_GPIO_CMD_1_VAL_Pos)                     /*!< SM_CONFIG GPIO_CMD_1: VAL Mask                               */
#define SM_CONFIG_GPIO_CMD_1_VAL_Rst                                 0x0UL                                                        /*!< SM_CONFIG GPIO_CMD_1: VAL Reset                              */
#define SM_CONFIG_GPIO_CMD_1_VAL_Addr                                0x200000feUL                                                 /*!< SM_CONFIG GPIO_CMD_1: VAL Address                            */

/* ======================================================================================== */
/* ================          struct 'SM_DATA'                              ================ */
/* ======================================================================================== */


/* ----------------------------  SM_DATA_CALIBRATION  ---------------------------- */
#define SM_DATA_CALIBRATION_Addr                                     0x20000130UL                                                 /*!< SM_DATA CALIBRATION: Address                                 */
#define pSM_DATA_CALIBRATION                                         (*(volatile uint32_t *) SM_DATA_CALIBRATION_Addr)            /*!< SM_DATA CALIBRATION: Pointer                                 */
#define SM_DATA_CALIBRATION_Msk                                      0xFFFFFFFFUL                                                 /*!< SM_DATA CALIBRATION: Mask                                    */
#define SM_DATA_CALIBRATION_Rst                                      0x00000000UL                                                 /*!< SM_DATA CALIBRATION: Reset                                   */

#define SM_DATA_CALIBRATION_VAL_Size                                 32                                                           /*!< SM_DATA CALIBRATION: VAL Size                                */
#define SM_DATA_CALIBRATION_VAL_Pos                                  0                                                            /*!< SM_DATA CALIBRATION: VAL Position                            */
#define SM_DATA_CALIBRATION_VAL_Msk                                  (0xffffffffUL << SM_DATA_CALIBRATION_VAL_Pos)                /*!< SM_DATA CALIBRATION: VAL Mask                                */
#define SM_DATA_CALIBRATION_VAL_Rst                                  0x0000UL                                                     /*!< SM_DATA CALIBRATION: VAL Reset                               */
#define SM_DATA_CALIBRATION_VAL_Addr                                 0x20000130UL                                                 /*!< SM_DATA CALIBRATION: VAL Address                             */

#define SM_DATA_CAL_ENABLE_key                                       0x2971UL
#define SM_DATA_CAL_DISABLE_key                                      0x2A21UL

/* ======================================================================================== */
/* ================          struct 'PROFILE'                              ================ */
/* ======================================================================================== */


/* ----------------------------  PROFILE_REF_CLK_CFG  ---------------------------- */
#define PROFILE_REF_CLK_CFG_Addr                                     0x200002E8UL                                                 /*!< PROFILE REF_CLK_CFG: Address                                 */
#define pPROFILE_REF_CLK_CFG                                         (*(volatile uint32_t *) PROFILE_REF_CLK_CFG_Addr)            /*!< PROFILE REF_CLK_CFG: Pointer                                 */
#define PROFILE_REF_CLK_CFG_Msk                                      0x04000000UL                                                 /*!< PROFILE REF_CLK_CFG: Mask                                    */
#define PROFILE_REF_CLK_CFG_Rst                                      0x00000000UL                                                 /*!< PROFILE REF_CLK_CFG: Reset                                   */

#define PROFILE_REF_CLK_CFG_CLK_TYPE_Size                            1                                                            /*!< PROFILE REF_CLK_CFG: CLK_TYPE Size                           */
#define PROFILE_REF_CLK_CFG_CLK_TYPE_Pos                             26                                                           /*!< PROFILE REF_CLK_CFG: CLK_TYPE Position                       */
#define PROFILE_REF_CLK_CFG_CLK_TYPE_Msk                             (0x0001UL << PROFILE_REF_CLK_CFG_CLK_TYPE_Pos)               /*!< PROFILE REF_CLK_CFG: CLK_TYPE Mask                           */
#define PROFILE_REF_CLK_CFG_CLK_TYPE_Rst                             0x0000UL                                                     /*!< PROFILE REF_CLK_CFG: CLK_TYPE Reset                          */
#define PROFILE_REF_CLK_CFG_CLK_TYPE_Addr                            0x200002e8UL                                                 /*!< PROFILE REF_CLK_CFG: CLK_TYPE Address                        */
#define PROFILE_REF_CLK_CFG_CLK_TYPE_TCXO_Eval                       0
#define PROFILE_REF_CLK_CFG_CLK_TYPE_XTAL_Eval                       1

/* ----------------------------  PROFILE_CH_FREQ  ---------------------------- */
#define PROFILE_CH_FREQ_Addr                                         0x200002ECUL                                                 /*!< PROFILE CH_FREQ: Address                                     */
#define pPROFILE_CH_FREQ                                             (*(volatile uint32_t *) PROFILE_CH_FREQ_Addr)                /*!< PROFILE CH_FREQ: Pointer                                     */
#define PROFILE_CH_FREQ_Msk                                          0xFFFFFFFFUL                                                 /*!< PROFILE CH_FREQ: Mask                                        */
#define PROFILE_CH_FREQ_Rst                                          0x00000000UL                                                 /*!< PROFILE CH_FREQ: Reset                                       */

#define PROFILE_CH_FREQ_VAL_Size                                     32                                                           /*!< PROFILE CH_FREQ: VAL Size                                    */
#define PROFILE_CH_FREQ_VAL_Pos                                      0                                                            /*!< PROFILE CH_FREQ: VAL Position                                */
#define PROFILE_CH_FREQ_VAL_Msk                                      (0xffffffffUL << PROFILE_CH_FREQ_VAL_Pos)                    /*!< PROFILE CH_FREQ: VAL Mask                                    */
#define PROFILE_CH_FREQ_VAL_Rst                                      0x0000UL                                                     /*!< PROFILE CH_FREQ: VAL Reset                                   */
#define PROFILE_CH_FREQ_VAL_Addr                                     0x200002ecUL                                                 /*!< PROFILE CH_FREQ: VAL Address                                 */

/* ----------------------------  PROFILE_PACKET_CFG  ---------------------------- */
#define PROFILE_PACKET_CFG_Addr                                      0x200002F4UL                                                 /*!< PROFILE PACKET_CFG: Address                                  */
#define pPROFILE_PACKET_CFG                                          (*(volatile uint32_t *) PROFILE_PACKET_CFG_Addr)             /*!< PROFILE PACKET_CFG: Pointer                                  */
#define PROFILE_PACKET_CFG_Msk                                       0x0000C000UL                                                 /*!< PROFILE PACKET_CFG: Mask                                     */
#define PROFILE_PACKET_CFG_Rst                                       0x00000000UL                                                 /*!< PROFILE PACKET_CFG: Reset                                    */

#define PROFILE_PACKET_CFG_TYPE_FRAME0_Size                          2                                                            /*!< PROFILE PACKET_CFG: TYPE_FRAME0 Size                         */
#define PROFILE_PACKET_CFG_TYPE_FRAME0_Pos                           14                                                           /*!< PROFILE PACKET_CFG: TYPE_FRAME0 Position                     */
#define PROFILE_PACKET_CFG_TYPE_FRAME0_Msk                           (0x0003UL << PROFILE_PACKET_CFG_TYPE_FRAME0_Pos)             /*!< PROFILE PACKET_CFG: TYPE_FRAME0 Mask                         */
#define PROFILE_PACKET_CFG_TYPE_FRAME0_Rst                           0x0000UL                                                     /*!< PROFILE PACKET_CFG: TYPE_FRAME0 Reset                        */
#define PROFILE_PACKET_CFG_TYPE_FRAME0_Addr                          0x200002f4UL                                                 /*!< PROFILE PACKET_CFG: TYPE_FRAME0 Address                      */
#define PROFILE_PACKET_CFG_TYPE_FRAME0_GENERIC_Eval                  0
#define PROFILE_PACKET_CFG_TYPE_FRAME0_IEEE_15G4_Eval                1

/* ----------------------------  PROFILE_RADIO_MODES  ---------------------------- */
#define PROFILE_RADIO_MODES_Addr                                     0x200002F8UL                                                 /*!< PROFILE RADIO_MODES: Address                                 */
#define pPROFILE_RADIO_MODES                                         (*(volatile uint32_t *) PROFILE_RADIO_MODES_Addr)            /*!< PROFILE RADIO_MODES: Pointer                                 */
#define PROFILE_RADIO_MODES_Msk                                      0x00380060UL                                                 /*!< PROFILE RADIO_MODES: Mask                                    */
#define PROFILE_RADIO_MODES_Rst                                      0x00000000UL                                                 /*!< PROFILE RADIO_MODES: Reset                                   */

#define PROFILE_RADIO_MODES_COMBINED_TRX_MATCH_Size                  2                                                            /*!< PROFILE RADIO_MODES: COMBINED_TRX_MATCH Size                 */
#define PROFILE_RADIO_MODES_COMBINED_TRX_MATCH_Pos                   5                                                            /*!< PROFILE RADIO_MODES: COMBINED_TRX_MATCH Position             */
#define PROFILE_RADIO_MODES_COMBINED_TRX_MATCH_Msk                   (0x0003UL << PROFILE_RADIO_MODES_COMBINED_TRX_MATCH_Pos)     /*!< PROFILE RADIO_MODES: COMBINED_TRX_MATCH Mask                 */
#define PROFILE_RADIO_MODES_COMBINED_TRX_MATCH_Rst                   0x0000UL                                                     /*!< PROFILE RADIO_MODES: COMBINED_TRX_MATCH Reset                */
#define PROFILE_RADIO_MODES_COMBINED_TRX_MATCH_Addr                  0x200002f8UL                                                 /*!< PROFILE RADIO_MODES: COMBINED_TRX_MATCH Address              */
#define PROFILE_RADIO_MODES_COMBINED_TRX_MATCH_DISABLE_Eval          0
#define PROFILE_RADIO_MODES_COMBINED_TRX_MATCH_ENABLE_Eval           1

#define PROFILE_RADIO_MODES_GPIO_CLK_FREQ_SEL_Size                   3                                                            /*!< PROFILE RADIO_MODES: GPIO_CLK_FREQ_SEL Size                  */
#define PROFILE_RADIO_MODES_GPIO_CLK_FREQ_SEL_Pos                    19                                                           /*!< PROFILE RADIO_MODES: GPIO_CLK_FREQ_SEL Position              */
#define PROFILE_RADIO_MODES_GPIO_CLK_FREQ_SEL_Msk                    (0x0007UL << PROFILE_RADIO_MODES_GPIO_CLK_FREQ_SEL_Pos)      /*!< PROFILE RADIO_MODES: GPIO_CLK_FREQ_SEL Mask                  */
#define PROFILE_RADIO_MODES_GPIO_CLK_FREQ_SEL_Rst                    0x0000UL                                                     /*!< PROFILE RADIO_MODES: GPIO_CLK_FREQ_SEL Reset                 */
#define PROFILE_RADIO_MODES_GPIO_CLK_FREQ_SEL_Addr                   0x200002f8UL                                                 /*!< PROFILE RADIO_MODES: GPIO_CLK_FREQ_SEL Address               */
#define PROFILE_RADIO_MODES_GPIO_CLK_FREQ_SEL_6p5_MHZ_Eval           0
#define PROFILE_RADIO_MODES_GPIO_CLK_FREQ_SEL_3p25_MHZ_Eval          1
#define PROFILE_RADIO_MODES_GPIO_CLK_FREQ_SEL_1p625_MHZ_Eval         2
#define PROFILE_RADIO_MODES_GPIO_CLK_FREQ_SEL_0p8125_MHZ_Eval        3
#define PROFILE_RADIO_MODES_GPIO_CLK_FREQ_SEL_0p40625_MHZ_Eval       4
#define PROFILE_RADIO_MODES_GPIO_CLK_FREQ_SEL_0p203125_MHZ_Eval      5
#define PROFILE_RADIO_MODES_GPIO_CLK_FREQ_SEL_0p1015625_MHZ_Eval     6
#define PROFILE_RADIO_MODES_GPIO_CLK_FREQ_SEL_0p05078125_MHZ_Eval    7

/* ----------------------------  PROFILE_RADIO_DIG_TX_CFG0  ---------------------------- */
#define PROFILE_RADIO_DIG_TX_CFG0_Addr                               0x20000304UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG0: Address                           */
#define pPROFILE_RADIO_DIG_TX_CFG0                                   (*(volatile uint32_t *) PROFILE_RADIO_DIG_TX_CFG0_Addr)      /*!< PROFILE RADIO_DIG_TX_CFG0: Pointer                           */
#define PROFILE_RADIO_DIG_TX_CFG0_Msk                                0x7FFFF00EUL                                                 /*!< PROFILE RADIO_DIG_TX_CFG0: Mask                              */
#define PROFILE_RADIO_DIG_TX_CFG0_Rst                                0x00000000UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG0: Reset                             */

#define PROFILE_RADIO_DIG_TX_CFG0_TX_FILTER_ENABLE_Size              1                                                            /*!< PROFILE RADIO_DIG_TX_CFG0: TX_FILTER_ENABLE Size             */
#define PROFILE_RADIO_DIG_TX_CFG0_TX_FILTER_ENABLE_Pos               1                                                            /*!< PROFILE RADIO_DIG_TX_CFG0: TX_FILTER_ENABLE Position         */
#define PROFILE_RADIO_DIG_TX_CFG0_TX_FILTER_ENABLE_Msk               (0x0001UL << PROFILE_RADIO_DIG_TX_CFG0_TX_FILTER_ENABLE_Pos) /*!< PROFILE RADIO_DIG_TX_CFG0: TX_FILTER_ENABLE Mask             */
#define PROFILE_RADIO_DIG_TX_CFG0_TX_FILTER_ENABLE_Rst               0x0000UL                                                     /*!< PROFILE RADIO_DIG_TX_CFG0: TX_FILTER_ENABLE Reset            */
#define PROFILE_RADIO_DIG_TX_CFG0_TX_FILTER_ENABLE_Addr              0x20000304UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG0: TX_FILTER_ENABLE Address          */
#define PROFILE_RADIO_DIG_TX_CFG0_TX_FILTER_ENABLE_DISABLE_Eval      0
#define PROFILE_RADIO_DIG_TX_CFG0_TX_FILTER_ENABLE_ENABLE_Eval       1

#define PROFILE_RADIO_DIG_TX_CFG0_TX_GAUSSIAN_BT_Size                2                                                            /*!< PROFILE RADIO_DIG_TX_CFG0: TX_GAUSSIAN_BT Size               */
#define PROFILE_RADIO_DIG_TX_CFG0_TX_GAUSSIAN_BT_Pos                 2                                                            /*!< PROFILE RADIO_DIG_TX_CFG0: TX_GAUSSIAN_BT Position           */
#define PROFILE_RADIO_DIG_TX_CFG0_TX_GAUSSIAN_BT_Msk                 (0x0003UL << PROFILE_RADIO_DIG_TX_CFG0_TX_GAUSSIAN_BT_Pos)   /*!< PROFILE RADIO_DIG_TX_CFG0: TX_GAUSSIAN_BT Mask               */
#define PROFILE_RADIO_DIG_TX_CFG0_TX_GAUSSIAN_BT_Rst                 0x0000UL                                                     /*!< PROFILE RADIO_DIG_TX_CFG0: TX_GAUSSIAN_BT Reset              */
#define PROFILE_RADIO_DIG_TX_CFG0_TX_GAUSSIAN_BT_Addr                0x20000304UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG0: TX_GAUSSIAN_BT Address            */
#define PROFILE_RADIO_DIG_TX_CFG0_TX_GAUSSIAN_BT_ENUM_0_Eval         0
#define PROFILE_RADIO_DIG_TX_CFG0_TX_GAUSSIAN_BT_ENUM_1_Eval         1
#define PROFILE_RADIO_DIG_TX_CFG0_TX_GAUSSIAN_BT_ENUM_2_Eval         2
#define PROFILE_RADIO_DIG_TX_CFG0_TX_GAUSSIAN_BT_ENUM_3_Eval         3

#define PROFILE_RADIO_DIG_TX_CFG0_PA_COARSE_Size                     4                                                            /*!< PROFILE RADIO_DIG_TX_CFG0: PA_COARSE Size                    */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_COARSE_Pos                      12                                                           /*!< PROFILE RADIO_DIG_TX_CFG0: PA_COARSE Position                */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_COARSE_Msk                      (0x000fUL << PROFILE_RADIO_DIG_TX_CFG0_PA_COARSE_Pos)        /*!< PROFILE RADIO_DIG_TX_CFG0: PA_COARSE Mask                    */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_COARSE_Rst                      0x0000UL                                                     /*!< PROFILE RADIO_DIG_TX_CFG0: PA_COARSE Reset                   */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_COARSE_Addr                     0x20000304UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG0: PA_COARSE Address                 */

#define PROFILE_RADIO_DIG_TX_CFG0_PA_FINE_Size                       7                                                            /*!< PROFILE RADIO_DIG_TX_CFG0: PA_FINE Size                      */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_FINE_Pos                        16                                                           /*!< PROFILE RADIO_DIG_TX_CFG0: PA_FINE Position                  */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_FINE_Msk                        (0x007fUL << PROFILE_RADIO_DIG_TX_CFG0_PA_FINE_Pos)          /*!< PROFILE RADIO_DIG_TX_CFG0: PA_FINE Mask                      */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_FINE_Rst                        0x0000UL                                                     /*!< PROFILE RADIO_DIG_TX_CFG0: PA_FINE Reset                     */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_FINE_Addr                       0x20000304UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG0: PA_FINE Address                   */

#define PROFILE_RADIO_DIG_TX_CFG0_PA_MICRO_Size                      7                                                            /*!< PROFILE RADIO_DIG_TX_CFG0: PA_MICRO Size                     */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_MICRO_Pos                       23                                                           /*!< PROFILE RADIO_DIG_TX_CFG0: PA_MICRO Position                 */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_MICRO_Msk                       (0x007fUL << PROFILE_RADIO_DIG_TX_CFG0_PA_MICRO_Pos)         /*!< PROFILE RADIO_DIG_TX_CFG0: PA_MICRO Mask                     */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_MICRO_Rst                       0x0000UL                                                     /*!< PROFILE RADIO_DIG_TX_CFG0: PA_MICRO Reset                    */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_MICRO_Addr                      0x20000304UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG0: PA_MICRO Address                  */

#define PROFILE_RADIO_DIG_TX_CFG0_PA_SEL_Size                        1                                                            /*!< PROFILE RADIO_DIG_TX_CFG0: PA_SEL Size                       */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_SEL_Pos                         30                                                           /*!< PROFILE RADIO_DIG_TX_CFG0: PA_SEL Position                   */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_SEL_Msk                         (0x0001UL << PROFILE_RADIO_DIG_TX_CFG0_PA_SEL_Pos)           /*!< PROFILE RADIO_DIG_TX_CFG0: PA_SEL Mask                       */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_SEL_Rst                         0x0000UL                                                     /*!< PROFILE RADIO_DIG_TX_CFG0: PA_SEL Reset                      */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_SEL_Addr                        0x20000304UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG0: PA_SEL Address                    */
#define PROFILE_RADIO_DIG_TX_CFG0_PA_SEL_PA1_Eval                    0
#define PROFILE_RADIO_DIG_TX_CFG0_PA_SEL_PA2_Eval                    1

/* ----------------------------  PROFILE_RADIO_DIG_TX_CFG1  ---------------------------- */
#define PROFILE_RADIO_DIG_TX_CFG1_Addr                               0x20000308UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG1: Address                           */
#define pPROFILE_RADIO_DIG_TX_CFG1                                   (*(volatile uint32_t *) PROFILE_RADIO_DIG_TX_CFG1_Addr)      /*!< PROFILE RADIO_DIG_TX_CFG1: Pointer                           */
#define PROFILE_RADIO_DIG_TX_CFG1_Msk                                0x0F0FF000UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG1: Mask                              */
#define PROFILE_RADIO_DIG_TX_CFG1_Rst                                0x00000000UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG1: Reset                             */

#define PROFILE_RADIO_DIG_TX_CFG1_PA_RAMP_RATE_Size                  3                                                            /*!< PROFILE RADIO_DIG_TX_CFG1: PA_RAMP_RATE Size                 */
#define PROFILE_RADIO_DIG_TX_CFG1_PA_RAMP_RATE_Pos                   12                                                           /*!< PROFILE RADIO_DIG_TX_CFG1: PA_RAMP_RATE Position             */
#define PROFILE_RADIO_DIG_TX_CFG1_PA_RAMP_RATE_Msk                   (0x0007UL << PROFILE_RADIO_DIG_TX_CFG1_PA_RAMP_RATE_Pos)     /*!< PROFILE RADIO_DIG_TX_CFG1: PA_RAMP_RATE Mask                 */
#define PROFILE_RADIO_DIG_TX_CFG1_PA_RAMP_RATE_Rst                   0x0000UL                                                     /*!< PROFILE RADIO_DIG_TX_CFG1: PA_RAMP_RATE Reset                */
#define PROFILE_RADIO_DIG_TX_CFG1_PA_RAMP_RATE_Addr                  0x20000308UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG1: PA_RAMP_RATE Address              */

#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_OOK_BIT_FRAMING_EN_Size     1                                                            /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_PA_OOK_BIT_FRAMING_EN Size    */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_OOK_BIT_FRAMING_EN_Pos      15                                                           /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_PA_OOK_BIT_FRAMING_EN Position */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_OOK_BIT_FRAMING_EN_Msk      (0x0001UL << PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_OOK_BIT_FRAMING_EN_Pos) /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_PA_OOK_BIT_FRAMING_EN Mask    */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_OOK_BIT_FRAMING_EN_Rst      0x0000UL                                                     /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_PA_OOK_BIT_FRAMING_EN Reset   */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_OOK_BIT_FRAMING_EN_Addr     0x20000308UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_PA_OOK_BIT_FRAMING_EN Address */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_OOK_BIT_FRAMING_EN_DISABLE_Eval 0
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_OOK_BIT_FRAMING_EN_ENABLE_Eval 1

#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_FRAMING_EN_Size             1                                                            /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_PA_FRAMING_EN Size            */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_FRAMING_EN_Pos              16                                                           /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_PA_FRAMING_EN Position        */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_FRAMING_EN_Msk              (0x0001UL << PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_FRAMING_EN_Pos) /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_PA_FRAMING_EN Mask            */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_FRAMING_EN_Rst              0x0000UL                                                     /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_PA_FRAMING_EN Reset           */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_FRAMING_EN_Addr             0x20000308UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_PA_FRAMING_EN Address         */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_FRAMING_EN_DISABLE_Eval     0
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_FRAMING_EN_ENABLE_Eval      1

#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_PIN_SEL_Size                3                                                            /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_PA_PIN_SEL Size               */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_PIN_SEL_Pos                 17                                                           /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_PA_PIN_SEL Position           */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_PIN_SEL_Msk                 (0x0007UL << PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_PIN_SEL_Pos)   /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_PA_PIN_SEL Mask               */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_PIN_SEL_Rst                 0x0000UL                                                     /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_PA_PIN_SEL Reset              */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_PA_PIN_SEL_Addr                0x20000308UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_PA_PIN_SEL Address            */

#define PROFILE_RADIO_DIG_TX_CFG1_EXT_LNA_FRAMING_EN_Size            1                                                            /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_LNA_FRAMING_EN Size           */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_LNA_FRAMING_EN_Pos             24                                                           /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_LNA_FRAMING_EN Position       */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_LNA_FRAMING_EN_Msk             (0x0001UL << PROFILE_RADIO_DIG_TX_CFG1_EXT_LNA_FRAMING_EN_Pos) /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_LNA_FRAMING_EN Mask           */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_LNA_FRAMING_EN_Rst             0x0000UL                                                     /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_LNA_FRAMING_EN Reset          */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_LNA_FRAMING_EN_Addr            0x20000308UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_LNA_FRAMING_EN Address        */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_LNA_FRAMING_EN_DISABLE_Eval    0
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_LNA_FRAMING_EN_ENABLE_Eval     1

#define PROFILE_RADIO_DIG_TX_CFG1_EXT_LNA_PIN_SEL_Size               3                                                            /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_LNA_PIN_SEL Size              */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_LNA_PIN_SEL_Pos                25                                                           /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_LNA_PIN_SEL Position          */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_LNA_PIN_SEL_Msk                (0x0007UL << PROFILE_RADIO_DIG_TX_CFG1_EXT_LNA_PIN_SEL_Pos)  /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_LNA_PIN_SEL Mask              */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_LNA_PIN_SEL_Rst                0x0000UL                                                     /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_LNA_PIN_SEL Reset             */
#define PROFILE_RADIO_DIG_TX_CFG1_EXT_LNA_PIN_SEL_Addr               0x20000308UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG1: EXT_LNA_PIN_SEL Address           */

/* ----------------------------  PROFILE_RADIO_DIG_TX_CFG2  ---------------------------- */
#define PROFILE_RADIO_DIG_TX_CFG2_Addr                               0x2000030CUL                                                 /*!< PROFILE RADIO_DIG_TX_CFG2: Address                           */
#define pPROFILE_RADIO_DIG_TX_CFG2                                   (*(volatile uint32_t *) PROFILE_RADIO_DIG_TX_CFG2_Addr)      /*!< PROFILE RADIO_DIG_TX_CFG2: Pointer                           */
#define PROFILE_RADIO_DIG_TX_CFG2_Msk                                0x0000000FUL                                                 /*!< PROFILE RADIO_DIG_TX_CFG2: Mask                              */
#define PROFILE_RADIO_DIG_TX_CFG2_Rst                                0x00000000UL                                                 /*!< PROFILE RADIO_DIG_TX_CFG2: Reset                             */

#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_Size               4                                                            /*!< PROFILE RADIO_DIG_TX_CFG2: PAOLDO_VOUT_CON Size              */
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_Pos                0                                                            /*!< PROFILE RADIO_DIG_TX_CFG2: PAOLDO_VOUT_CON Position          */
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_Msk                (0x000fUL << PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_Pos)  /*!< PROFILE RADIO_DIG_TX_CFG2: PAOLDO_VOUT_CON Mask              */
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_Rst                0x0000UL                                                     /*!< PROFILE RADIO_DIG_TX_CFG2: PAOLDO_VOUT_CON Reset             */
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_Addr               0x2000030cUL                                                 /*!< PROFILE RADIO_DIG_TX_CFG2: PAOLDO_VOUT_CON Address           */
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_1P85V_Eval    0
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_1P90V_Eval    1
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_1P95V_Eval    2
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_2P00V_Eval    3
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_2P05V_Eval    4
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_2P10V_Eval    5
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_2P15V_Eval    6
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_2P20V_Eval    7
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_2P25V_Eval    8
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_2P30V_Eval    9
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_2P35V_Eval    10
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_2P40V_Eval    11
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_2P45V_Eval    12
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_2P50V_Eval    13
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_2P55V_Eval    14
#define PROFILE_RADIO_DIG_TX_CFG2_PAOLDO_VOUT_CON_VREG_2P60V_Eval    15


/* ----------------------------  PROFILE_RADIO_AFC_CFG0  ---------------------------- */
#define PROFILE_RADIO_AFC_CFG0_Addr                                  0x20000318UL                                                 /*!< PROFILE RADIO_AFC_CFG2: Address                              */

/* ----------------------------  PROFILE_RADIO_AFC_CFG1  ---------------------------- */
#define PROFILE_RADIO_AFC_CFG1_Addr                                  0x2000031CUL                                                 /*!< PROFILE RADIO_AFC_CFG2: Address                              */

/* ----------------------------  PROFILE_RADIO_AFC_CFG2  ---------------------------- */
#define PROFILE_RADIO_AFC_CFG2_Addr                                  0x20000320UL                                                 /*!< PROFILE RADIO_AFC_CFG2: Address                              */
#define pPROFILE_RADIO_AFC_CFG2                                      (*(volatile uint32_t *) PROFILE_RADIO_AFC_CFG2_Addr)         /*!< PROFILE RADIO_AFC_CFG2: Pointer                              */
#define PROFILE_RADIO_AFC_CFG2_Msk                                   0x00000007UL                                                 /*!< PROFILE RADIO_AFC_CFG2: Mask                                 */
#define PROFILE_RADIO_AFC_CFG2_Rst                                   0x00000003UL                                                 /*!< PROFILE RADIO_AFC_CFG2: Reset                                */

#define PROFILE_RADIO_AFC_CFG2_AFC_MODE_Size                         3                                                            /*!< PROFILE RADIO_AFC_CFG2: AFC_MODE Size                        */
#define PROFILE_RADIO_AFC_CFG2_AFC_MODE_Pos                          0                                                            /*!< PROFILE RADIO_AFC_CFG2: AFC_MODE Position                    */
#define PROFILE_RADIO_AFC_CFG2_AFC_MODE_Msk                          (0x0007UL << PROFILE_RADIO_AFC_CFG2_AFC_MODE_Pos)            /*!< PROFILE RADIO_AFC_CFG2: AFC_MODE Mask                        */
#define PROFILE_RADIO_AFC_CFG2_AFC_MODE_Rst                          0x0003UL                                                     /*!< PROFILE RADIO_AFC_CFG2: AFC_MODE Reset                       */
#define PROFILE_RADIO_AFC_CFG2_AFC_MODE_Addr                         0x20000320UL                                                 /*!< PROFILE RADIO_AFC_CFG2: AFC_MODE Address                     */
#define PROFILE_RADIO_AFC_CFG2_AFC_MODE_DISABLED_Eval                0
#define PROFILE_RADIO_AFC_CFG2_AFC_MODE_FW_AFC_Eval                  1
#define PROFILE_RADIO_AFC_CFG2_AFC_MODE_ACQUIRE_Eval                 2
#define PROFILE_RADIO_AFC_CFG2_AFC_MODE_TRACK_Eval                   3
#define PROFILE_RADIO_AFC_CFG2_AFC_MODE_HOLD_Eval                    4

/* ----------------------------  PROFILE_RADIO_CAL_CFG0  ---------------------------- */
#define PROFILE_RADIO_CAL_CFG0_Addr                                  0x20000368UL                                                 /*!< PROFILE RADIO_CAL_CFG0: Address                              */
#define pPROFILE_RADIO_CAL_CFG0                                      (*(volatile uint32_t *) PROFILE_RADIO_CAL_CFG0_Addr)         /*!< PROFILE RADIO_CAL_CFG0: Pointer                              */
#define PROFILE_RADIO_CAL_CFG0_Msk                                   0xFFFFFFFFUL                                                 /*!< PROFILE RADIO_CAL_CFG0: Mask                                 */
#define PROFILE_RADIO_CAL_CFG0_Rst                                   0x00000000UL                                                 /*!< PROFILE RADIO_CAL_CFG0: Reset                                */

#define PROFILE_RADIO_CAL_CFG0_DATA_Size                             32                                                           /*!< PROFILE RADIO_CAL_CFG0: DATA Size                            */
#define PROFILE_RADIO_CAL_CFG0_DATA_Pos                              0                                                            /*!< PROFILE RADIO_CAL_CFG0: DATA Position                        */
#define PROFILE_RADIO_CAL_CFG0_DATA_Msk                              (0xffffffffUL << PROFILE_RADIO_CAL_CFG0_DATA_Pos)            /*!< PROFILE RADIO_CAL_CFG0: DATA Mask                            */
#define PROFILE_RADIO_CAL_CFG0_DATA_Rst                              0x0000UL                                                     /*!< PROFILE RADIO_CAL_CFG0: DATA Reset                           */
#define PROFILE_RADIO_CAL_CFG0_DATA_Addr                             0x20000368UL                                                 /*!< PROFILE RADIO_CAL_CFG0: DATA Address                         */

/* ----------------------------  PROFILE_RADIO_CAL_CFG1  ---------------------------- */
#define PROFILE_RADIO_CAL_CFG1_Addr                                  0x2000036CUL                                                 /*!< PROFILE RADIO_CAL_CFG1: Address                              */
#define pPROFILE_RADIO_CAL_CFG1                                      (*(volatile uint32_t *) PROFILE_RADIO_CAL_CFG1_Addr)         /*!< PROFILE RADIO_CAL_CFG1: Pointer                              */
#define PROFILE_RADIO_CAL_CFG1_Msk                                   0x20000000UL                                                 /*!< PROFILE RADIO_CAL_CFG1: Mask                                 */
#define PROFILE_RADIO_CAL_CFG1_Rst                                   0x00000000UL                                                 /*!< PROFILE RADIO_CAL_CFG1: Reset                                */

#define PROFILE_RADIO_CAL_CFG1_CAL_SUCCESS_Size                      1                                                            /*!< PROFILE RADIO_CAL_CFG1: CAL_SUCCESS Size                     */
#define PROFILE_RADIO_CAL_CFG1_CAL_SUCCESS_Pos                       29                                                           /*!< PROFILE RADIO_CAL_CFG1: CAL_SUCCESS Position                 */
#define PROFILE_RADIO_CAL_CFG1_CAL_SUCCESS_Msk                       (0x0001UL << PROFILE_RADIO_CAL_CFG1_CAL_SUCCESS_Pos)         /*!< PROFILE RADIO_CAL_CFG1: CAL_SUCCESS Mask                     */
#define PROFILE_RADIO_CAL_CFG1_CAL_SUCCESS_Rst                       0x0000UL                                                     /*!< PROFILE RADIO_CAL_CFG1: CAL_SUCCESS Reset                    */
#define PROFILE_RADIO_CAL_CFG1_CAL_SUCCESS_Addr                      0x2000036cUL                                                 /*!< PROFILE RADIO_CAL_CFG1: CAL_SUCCESS Address                  */
#define PROFILE_RADIO_CAL_CFG1_CAL_SUCCESS_UNSUCCESSFUL_Eval         0
#define PROFILE_RADIO_CAL_CFG1_CAL_SUCCESS_SUCCESSFUL_Eval           1

/* ----------------------------  PROFILE_RADIO_CAL_CFG2  ---------------------------- */
#define PROFILE_RADIO_CAL_CFG2_Addr                                  0x20000370UL                                                 /*!< PROFILE RADIO_CAL_CFG2: Address                              */
#define pPROFILE_RADIO_CAL_CFG2                                      (*(volatile uint32_t *) PROFILE_RADIO_CAL_CFG2_Addr)         /*!< PROFILE RADIO_CAL_CFG2: Pointer                              */
#define PROFILE_RADIO_CAL_CFG2_Msk                                   0xFFFFFFFFUL                                                 /*!< PROFILE RADIO_CAL_CFG2: Mask                                 */
#define PROFILE_RADIO_CAL_CFG2_Rst                                   0x00000000UL                                                 /*!< PROFILE RADIO_CAL_CFG2: Reset                                */

#define PROFILE_RADIO_CAL_CFG2_DATA_Size                             32                                                           /*!< PROFILE RADIO_CAL_CFG2: DATA Size                            */
#define PROFILE_RADIO_CAL_CFG2_DATA_Pos                              0                                                            /*!< PROFILE RADIO_CAL_CFG2: DATA Position                        */
#define PROFILE_RADIO_CAL_CFG2_DATA_Msk                              (0xffffffffUL << PROFILE_RADIO_CAL_CFG2_DATA_Pos)            /*!< PROFILE RADIO_CAL_CFG2: DATA Mask                            */
#define PROFILE_RADIO_CAL_CFG2_DATA_Rst                              0x0000UL                                                     /*!< PROFILE RADIO_CAL_CFG2: DATA Reset                           */
#define PROFILE_RADIO_CAL_CFG2_DATA_Addr                             0x20000370UL                                                 /*!< PROFILE RADIO_CAL_CFG2: DATA Address                         */

/* ----------------------------  PROFILE_RSSI_CFG  ---------------------------- */
#define PROFILE_RSSI_CFG_Addr                                        0x20000374UL                                                 /*!< PROFILE RSSI_CFG: Address                                    */
#define pPROFILE_RSSI_CFG                                            (*(volatile uint32_t *) PROFILE_RSSI_CFG_Addr)               /*!< PROFILE RSSI_CFG: Pointer                                    */
#define PROFILE_RSSI_CFG_Msk                                         0x03FF03FFUL                                                 /*!< PROFILE RSSI_CFG: Mask                                       */
#define PROFILE_RSSI_CFG_Rst                                         0x02960138UL                                                 /*!< PROFILE RSSI_CFG: Reset                                      */

#define PROFILE_RSSI_CFG_WB_OFFSET_Size                              10                                                           /*!< PROFILE RSSI_CFG: WB_OFFSET Size                             */
#define PROFILE_RSSI_CFG_WB_OFFSET_Pos                               0                                                            /*!< PROFILE RSSI_CFG: WB_OFFSET Position                         */
#define PROFILE_RSSI_CFG_WB_OFFSET_Msk                               (0x03ffUL << PROFILE_RSSI_CFG_WB_OFFSET_Pos)                 /*!< PROFILE RSSI_CFG: WB_OFFSET Mask                             */
#define PROFILE_RSSI_CFG_WB_OFFSET_Rst                               0x0138UL                                                     /*!< PROFILE RSSI_CFG: WB_OFFSET Reset                            */
#define PROFILE_RSSI_CFG_WB_OFFSET_Addr                              0x20000374UL                                                 /*!< PROFILE RSSI_CFG: WB_OFFSET Address                          */

#define PROFILE_RSSI_CFG_NB_OFFSET_Size                              10                                                           /*!< PROFILE RSSI_CFG: NB_OFFSET Size                             */
#define PROFILE_RSSI_CFG_NB_OFFSET_Pos                               16                                                           /*!< PROFILE RSSI_CFG: NB_OFFSET Position                         */
#define PROFILE_RSSI_CFG_NB_OFFSET_Msk                               (0x03ffUL << PROFILE_RSSI_CFG_NB_OFFSET_Pos)                 /*!< PROFILE RSSI_CFG: NB_OFFSET Mask                             */
#define PROFILE_RSSI_CFG_NB_OFFSET_Rst                               0x0296UL                                                     /*!< PROFILE RSSI_CFG: NB_OFFSET Reset                            */
#define PROFILE_RSSI_CFG_NB_OFFSET_Addr                              0x20000374UL                                                 /*!< PROFILE RSSI_CFG: NB_OFFSET Address                          */

/* ----------------------------  PROFILE_CCA_CFG  ---------------------------- */
#define PROFILE_CCA_CFG_Addr                                         0x20000378UL                                                 /*!< PROFILE CCA_CFG: Address                                     */
#define pPROFILE_CCA_CFG                                             (*(volatile uint32_t *) PROFILE_CCA_CFG_Addr)                /*!< PROFILE CCA_CFG: Pointer                                     */
#define PROFILE_CCA_CFG_Msk                                          0x07FFFFFFUL                                                 /*!< PROFILE CCA_CFG: Mask                                        */
#define PROFILE_CCA_CFG_Rst                                          0x07B04043UL                                                 /*!< PROFILE CCA_CFG: Reset                                       */

#define PROFILE_CCA_CFG_TICK_RATE_Size                               4                                                            /*!< PROFILE CCA_CFG: TICK_RATE Size                              */
#define PROFILE_CCA_CFG_TICK_RATE_Pos                                0                                                            /*!< PROFILE CCA_CFG: TICK_RATE Position                          */
#define PROFILE_CCA_CFG_TICK_RATE_Msk                                (0x000fUL << PROFILE_CCA_CFG_TICK_RATE_Pos)                  /*!< PROFILE CCA_CFG: TICK_RATE Mask                              */
#define PROFILE_CCA_CFG_TICK_RATE_Rst                                0x0003UL                                                     /*!< PROFILE CCA_CFG: TICK_RATE Reset                             */
#define PROFILE_CCA_CFG_TICK_RATE_Addr                               0x20000378UL                                                 /*!< PROFILE CCA_CFG: TICK_RATE Address                           */
#define PROFILE_CCA_CFG_TICK_RATE_X1_DATARATE_Eval                   0
#define PROFILE_CCA_CFG_TICK_RATE_X2_DATARATE_Eval                   2
#define PROFILE_CCA_CFG_TICK_RATE_X4_DATARATE_Eval                   3
#define PROFILE_CCA_CFG_TICK_RATE_X8_DATARATE_Eval                   4
#define PROFILE_CCA_CFG_TICK_RATE_X16_DATARATE_Eval                  5
#define PROFILE_CCA_CFG_TICK_RATE_X32_DATARATE_Eval                  6
#define PROFILE_CCA_CFG_TICK_RATE_X64_DATARATE_Eval                  7
#define PROFILE_CCA_CFG_TICK_RATE_X128_DATARATE_Eval                 8

#define PROFILE_CCA_CFG_TICK_POSTSCALAR_Size                         4                                                            /*!< PROFILE CCA_CFG: TICK_POSTSCALAR Size                        */
#define PROFILE_CCA_CFG_TICK_POSTSCALAR_Pos                          4                                                            /*!< PROFILE CCA_CFG: TICK_POSTSCALAR Position                    */
#define PROFILE_CCA_CFG_TICK_POSTSCALAR_Msk                          (0x000fUL << PROFILE_CCA_CFG_TICK_POSTSCALAR_Pos)            /*!< PROFILE CCA_CFG: TICK_POSTSCALAR Mask                        */
#define PROFILE_CCA_CFG_TICK_POSTSCALAR_Rst                          0x0004UL                                                     /*!< PROFILE CCA_CFG: TICK_POSTSCALAR Reset                       */
#define PROFILE_CCA_CFG_TICK_POSTSCALAR_Addr                         0x20000378UL                                                 /*!< PROFILE CCA_CFG: TICK_POSTSCALAR Address                     */

#define PROFILE_CCA_CFG_DETECTION_TIME_Size                          8                                                            /*!< PROFILE CCA_CFG: DETECTION_TIME Size                         */
#define PROFILE_CCA_CFG_DETECTION_TIME_Pos                           8                                                            /*!< PROFILE CCA_CFG: DETECTION_TIME Position                     */
#define PROFILE_CCA_CFG_DETECTION_TIME_Msk                           (0x00ffUL << PROFILE_CCA_CFG_DETECTION_TIME_Pos)             /*!< PROFILE CCA_CFG: DETECTION_TIME Mask                         */
#define PROFILE_CCA_CFG_DETECTION_TIME_Rst                           0x0040UL                                                     /*!< PROFILE CCA_CFG: DETECTION_TIME Reset                        */
#define PROFILE_CCA_CFG_DETECTION_TIME_Addr                          0x20000378UL                                                 /*!< PROFILE CCA_CFG: DETECTION_TIME Address                      */

#define PROFILE_CCA_CFG_THRESHOLD_Size                               11                                                           /*!< PROFILE CCA_CFG: THRESHOLD Size                              */
#define PROFILE_CCA_CFG_THRESHOLD_Pos                                16                                                           /*!< PROFILE CCA_CFG: THRESHOLD Position                          */
#define PROFILE_CCA_CFG_THRESHOLD_Msk                                (0x07ffUL << PROFILE_CCA_CFG_THRESHOLD_Pos)                  /*!< PROFILE CCA_CFG: THRESHOLD Mask                              */
#define PROFILE_CCA_CFG_THRESHOLD_Rst                                0x07b0UL                                                     /*!< PROFILE CCA_CFG: THRESHOLD Reset                             */
#define PROFILE_CCA_CFG_THRESHOLD_Addr                               0x20000378UL                                                 /*!< PROFILE CCA_CFG: THRESHOLD Address                           */

/* ----------------------------  PROFILE_CCA_READBACK  ---------------------------- */
#define PROFILE_CCA_READBACK_Addr                                    0x2000037CUL                                                 /*!< PROFILE CCA_READBACK: Address                                */
#define pPROFILE_CCA_READBACK                                        (*(volatile uint32_t *) PROFILE_CCA_READBACK_Addr)           /*!< PROFILE CCA_READBACK: Pointer                                */
#define PROFILE_CCA_READBACK_Msk                                     0x0000C7FFUL                                                 /*!< PROFILE CCA_READBACK: Mask                                   */
#define PROFILE_CCA_READBACK_Rst                                     0x00008000UL                                                 /*!< PROFILE CCA_READBACK: Reset                                  */

#define PROFILE_CCA_READBACK_VALUE_Size                              11                                                           /*!< PROFILE CCA_READBACK: VALUE Size                             */
#define PROFILE_CCA_READBACK_VALUE_Pos                               0                                                            /*!< PROFILE CCA_READBACK: VALUE Position                         */
#define PROFILE_CCA_READBACK_VALUE_Msk                               (0x07ffUL << PROFILE_CCA_READBACK_VALUE_Pos)                 /*!< PROFILE CCA_READBACK: VALUE Mask                             */
#define PROFILE_CCA_READBACK_VALUE_Rst                               0x0000UL                                                     /*!< PROFILE CCA_READBACK: VALUE Reset                            */
#define PROFILE_CCA_READBACK_VALUE_Addr                              0x2000037cUL                                                 /*!< PROFILE CCA_READBACK: VALUE Address                          */

#define PROFILE_CCA_READBACK_LIVE_STATUS_Size                        1                                                            /*!< PROFILE CCA_READBACK: LIVE_STATUS Size                       */
#define PROFILE_CCA_READBACK_LIVE_STATUS_Pos                         14                                                           /*!< PROFILE CCA_READBACK: LIVE_STATUS Position                   */
#define PROFILE_CCA_READBACK_LIVE_STATUS_Msk                         (0x0001UL << PROFILE_CCA_READBACK_LIVE_STATUS_Pos)           /*!< PROFILE CCA_READBACK: LIVE_STATUS Mask                       */
#define PROFILE_CCA_READBACK_LIVE_STATUS_Rst                         0x0000UL                                                     /*!< PROFILE CCA_READBACK: LIVE_STATUS Reset                      */
#define PROFILE_CCA_READBACK_LIVE_STATUS_Addr                        0x2000037cUL                                                 /*!< PROFILE CCA_READBACK: LIVE_STATUS Address                    */
#define PROFILE_CCA_READBACK_LIVE_STATUS_CLEAR_Eval                  0
#define PROFILE_CCA_READBACK_LIVE_STATUS_BUSY_Eval                   1

#define PROFILE_CCA_READBACK_STATUS_Size                             1                                                            /*!< PROFILE CCA_READBACK: STATUS Size                            */
#define PROFILE_CCA_READBACK_STATUS_Pos                              15                                                           /*!< PROFILE CCA_READBACK: STATUS Position                        */
#define PROFILE_CCA_READBACK_STATUS_Msk                              (0x0001UL << PROFILE_CCA_READBACK_STATUS_Pos)                /*!< PROFILE CCA_READBACK: STATUS Mask                            */
#define PROFILE_CCA_READBACK_STATUS_Rst                              0x0001UL                                                     /*!< PROFILE CCA_READBACK: STATUS Reset                           */
#define PROFILE_CCA_READBACK_STATUS_Addr                             0x2000037cUL                                                 /*!< PROFILE CCA_READBACK: STATUS Address                         */
#define PROFILE_CCA_READBACK_STATUS_CLEAR_Eval                       0
#define PROFILE_CCA_READBACK_STATUS_BUSY_Eval                        1

/* ----------------------------  PROFILE_LPM_CFG0  ---------------------------- */
#define PROFILE_LPM_CFG0_Addr                                        0x20000380UL                                                 /*!< PROFILE LPM_CFG0: Address                                    */
#define pPROFILE_LPM_CFG0                                            (*(volatile uint32_t *) PROFILE_LPM_CFG0_Addr)               /*!< PROFILE LPM_CFG0: Pointer                                    */
#define PROFILE_LPM_CFG0_Msk                                         0x8001B400UL                                                 /*!< PROFILE LPM_CFG0: Mask                                       */
#define PROFILE_LPM_CFG0_Rst                                         0x00012000UL                                                 /*!< PROFILE LPM_CFG0: Reset                                      */

#define PROFILE_LPM_CFG0_RTC_EN_Size                                 1                                                            /*!< PROFILE LPM_CFG0: RTC_EN Size                                */
#define PROFILE_LPM_CFG0_RTC_EN_Pos                                  10                                                           /*!< PROFILE LPM_CFG0: RTC_EN Position                            */
#define PROFILE_LPM_CFG0_RTC_EN_Msk                                  (0x0001UL << PROFILE_LPM_CFG0_RTC_EN_Pos)                    /*!< PROFILE LPM_CFG0: RTC_EN Mask                                */
#define PROFILE_LPM_CFG0_RTC_EN_Rst                                  0x0000UL                                                     /*!< PROFILE LPM_CFG0: RTC_EN Reset                               */
#define PROFILE_LPM_CFG0_RTC_EN_Addr                                 0x20000380UL                                                 /*!< PROFILE LPM_CFG0: RTC_EN Address                             */
#define PROFILE_LPM_CFG0_RTC_EN_DISABLED_Eval                        0
#define PROFILE_LPM_CFG0_RTC_EN_ENABLED_Eval                         1

#define PROFILE_LPM_CFG0_RTC_RESYNC_Size                             1                                                            /*!< PROFILE LPM_CFG0: RTC_RESYNC Size                            */
#define PROFILE_LPM_CFG0_RTC_RESYNC_Pos                              12                                                           /*!< PROFILE LPM_CFG0: RTC_RESYNC Position                        */
#define PROFILE_LPM_CFG0_RTC_RESYNC_Msk                              (0x0001UL << PROFILE_LPM_CFG0_RTC_RESYNC_Pos)                /*!< PROFILE LPM_CFG0: RTC_RESYNC Mask                            */
#define PROFILE_LPM_CFG0_RTC_RESYNC_Rst                              0x0000UL                                                     /*!< PROFILE LPM_CFG0: RTC_RESYNC Reset                           */
#define PROFILE_LPM_CFG0_RTC_RESYNC_Addr                             0x20000380UL                                                 /*!< PROFILE LPM_CFG0: RTC_RESYNC Address                         */

#define PROFILE_LPM_CFG0_RTC_RECONFIG_EN_Size                        1                                                            /*!< PROFILE LPM_CFG0: RTC_RECONFIG_EN Size                       */
#define PROFILE_LPM_CFG0_RTC_RECONFIG_EN_Pos                         13                                                           /*!< PROFILE LPM_CFG0: RTC_RECONFIG_EN Position                   */
#define PROFILE_LPM_CFG0_RTC_RECONFIG_EN_Msk                         (0x0001UL << PROFILE_LPM_CFG0_RTC_RECONFIG_EN_Pos)           /*!< PROFILE LPM_CFG0: RTC_RECONFIG_EN Mask                       */
#define PROFILE_LPM_CFG0_RTC_RECONFIG_EN_Rst                         0x0001UL                                                     /*!< PROFILE LPM_CFG0: RTC_RECONFIG_EN Reset                      */
#define PROFILE_LPM_CFG0_RTC_RECONFIG_EN_Addr                        0x20000380UL                                                 /*!< PROFILE LPM_CFG0: RTC_RECONFIG_EN Address                    */
#define PROFILE_LPM_CFG0_RTC_RECONFIG_EN_DISABLED_Eval               0
#define PROFILE_LPM_CFG0_RTC_RECONFIG_EN_ENABLED_Eval                1

#define PROFILE_LPM_CFG0_RTC_LF_SRC_SEL_Size                         1                                                            /*!< PROFILE LPM_CFG0: RTC_LF_SRC_SEL Size                        */
#define PROFILE_LPM_CFG0_RTC_LF_SRC_SEL_Pos                          15                                                           /*!< PROFILE LPM_CFG0: RTC_LF_SRC_SEL Position                    */
#define PROFILE_LPM_CFG0_RTC_LF_SRC_SEL_Msk                          (0x0001UL << PROFILE_LPM_CFG0_RTC_LF_SRC_SEL_Pos)            /*!< PROFILE LPM_CFG0: RTC_LF_SRC_SEL Mask                        */
#define PROFILE_LPM_CFG0_RTC_LF_SRC_SEL_Rst                          0x0000UL                                                     /*!< PROFILE LPM_CFG0: RTC_LF_SRC_SEL Reset                       */
#define PROFILE_LPM_CFG0_RTC_LF_SRC_SEL_Addr                         0x20000380UL                                                 /*!< PROFILE LPM_CFG0: RTC_LF_SRC_SEL Address                     */
#define PROFILE_LPM_CFG0_RTC_LF_SRC_SEL_LFRC_Eval                    0
#define PROFILE_LPM_CFG0_RTC_LF_SRC_SEL_LFXTAL_Eval                  1

#define PROFILE_LPM_CFG0_RETAIN_SRAM_Size                            1                                                            /*!< PROFILE LPM_CFG0: RETAIN_SRAM Size                           */
#define PROFILE_LPM_CFG0_RETAIN_SRAM_Pos                             16                                                           /*!< PROFILE LPM_CFG0: RETAIN_SRAM Position                       */
#define PROFILE_LPM_CFG0_RETAIN_SRAM_Msk                             (0x0001UL << PROFILE_LPM_CFG0_RETAIN_SRAM_Pos)               /*!< PROFILE LPM_CFG0: RETAIN_SRAM Mask                           */
#define PROFILE_LPM_CFG0_RETAIN_SRAM_Rst                             0x0001UL                                                     /*!< PROFILE LPM_CFG0: RETAIN_SRAM Reset                          */
#define PROFILE_LPM_CFG0_RETAIN_SRAM_Addr                            0x20000380UL                                                 /*!< PROFILE LPM_CFG0: RETAIN_SRAM Address                        */
#define PROFILE_LPM_CFG0_RETAIN_SRAM_DISABLED_Eval                   0
#define PROFILE_LPM_CFG0_RETAIN_SRAM_ENABLED_Eval                    1

#define PROFILE_LPM_CFG0_ENABLE_Size                                 1                                                            /*!< PROFILE LPM_CFG0: ENABLE Size                                */
#define PROFILE_LPM_CFG0_ENABLE_Pos                                  31                                                           /*!< PROFILE LPM_CFG0: ENABLE Position                            */
#define PROFILE_LPM_CFG0_ENABLE_Msk                                  (0x0001UL << PROFILE_LPM_CFG0_ENABLE_Pos)                    /*!< PROFILE LPM_CFG0: ENABLE Mask                                */
#define PROFILE_LPM_CFG0_ENABLE_Rst                                  0x0000UL                                                     /*!< PROFILE LPM_CFG0: ENABLE Reset                               */
#define PROFILE_LPM_CFG0_ENABLE_Addr                                 0x20000380UL                                                 /*!< PROFILE LPM_CFG0: ENABLE Address                             */

/* ----------------------------  PROFILE_LPM_CFG1  ---------------------------- */
#define PROFILE_LPM_CFG1_Addr                                        0x20000384UL                                                 /*!< PROFILE LPM_CFG1: Address                                    */
#define pPROFILE_LPM_CFG1                                            (*(volatile uint32_t *) PROFILE_LPM_CFG1_Addr)               /*!< PROFILE LPM_CFG1: Pointer                                    */
#define PROFILE_LPM_CFG1_Msk                                         0xFFFFFFFFUL                                                 /*!< PROFILE LPM_CFG1: Mask                                       */
#define PROFILE_LPM_CFG1_Rst                                         0x00000131UL                                                 /*!< PROFILE LPM_CFG1: Reset                                      */

#define PROFILE_LPM_CFG1_RTC_PERIOD_Size                             32                                                           /*!< PROFILE LPM_CFG1: RTC_PERIOD Size                            */
#define PROFILE_LPM_CFG1_RTC_PERIOD_Pos                              0                                                            /*!< PROFILE LPM_CFG1: RTC_PERIOD Position                        */
#define PROFILE_LPM_CFG1_RTC_PERIOD_Msk                              (0xffffffffUL << PROFILE_LPM_CFG1_RTC_PERIOD_Pos)            /*!< PROFILE LPM_CFG1: RTC_PERIOD Mask                            */
#define PROFILE_LPM_CFG1_RTC_PERIOD_Rst                              0x0131UL                                                     /*!< PROFILE LPM_CFG1: RTC_PERIOD Reset                           */
#define PROFILE_LPM_CFG1_RTC_PERIOD_Addr                             0x20000384UL                                                 /*!< PROFILE LPM_CFG1: RTC_PERIOD Address                         */

/* ----------------------------  PROFILE_MONITOR1  ---------------------------- */
#define PROFILE_MONITOR1_Addr                                        0x2000038CUL                                                 /*!< PROFILE MONITOR1: Address                                    */
#define pPROFILE_MONITOR1                                            (*(volatile uint32_t *) PROFILE_MONITOR1_Addr)               /*!< PROFILE MONITOR1: Pointer                                    */
#define PROFILE_MONITOR1_Msk                                         0x00000FFFUL                                                 /*!< PROFILE MONITOR1: Mask                                       */
#define PROFILE_MONITOR1_Rst                                         0x00000000UL                                                 /*!< PROFILE MONITOR1: Reset                                      */

#define PROFILE_MONITOR1_TEMP_OUTPUT_Size                            12                                                           /*!< PROFILE MONITOR1: TEMP_OUTPUT Size                           */
#define PROFILE_MONITOR1_TEMP_OUTPUT_Pos                             0                                                            /*!< PROFILE MONITOR1: TEMP_OUTPUT Position                       */
#define PROFILE_MONITOR1_TEMP_OUTPUT_Msk                             (0x0fffUL << PROFILE_MONITOR1_TEMP_OUTPUT_Pos)               /*!< PROFILE MONITOR1: TEMP_OUTPUT Mask                           */
#define PROFILE_MONITOR1_TEMP_OUTPUT_Rst                             0x0000UL                                                     /*!< PROFILE MONITOR1: TEMP_OUTPUT Reset                          */
#define PROFILE_MONITOR1_TEMP_OUTPUT_Addr                            0x2000038cUL                                                 /*!< PROFILE MONITOR1: TEMP_OUTPUT Address                        */

/* ----------------------------  PROFILE_GPCON0_3  ---------------------------- */
#define PROFILE_GPCON_Base                                        0x20000394UL                                                 /*!< PROFILE GPCON: Base Address                                    */

#define PROFILE_GPCON0_3_Addr                                        0x20000394UL                                                 /*!< PROFILE GPCON0_3: Address                                    */
#define pPROFILE_GPCON0_3                                            (*(volatile uint32_t *) PROFILE_GPCON0_3_Addr)               /*!< PROFILE GPCON0_3: Pointer                                    */
#define PROFILE_GPCON0_3_Msk                                         0x3F3F3F3FUL                                                 /*!< PROFILE GPCON0_3: Mask                                       */
#define PROFILE_GPCON0_3_Rst                                         0x06042120UL                                                 /*!< PROFILE GPCON0_3: Reset                                      */

#define PROFILE_GPCON0_3_PIN0_CFG_Size                               6                                                            /*!< PROFILE GPCON0_3: PIN0_CFG Size                              */
#define PROFILE_GPCON0_3_PIN0_CFG_Pos                                0                                                            /*!< PROFILE GPCON0_3: PIN0_CFG Position                          */
#define PROFILE_GPCON0_3_PIN0_CFG_Msk                                (0x003fUL << PROFILE_GPCON0_3_PIN0_CFG_Pos)                  /*!< PROFILE GPCON0_3: PIN0_CFG Mask                              */
#define PROFILE_GPCON0_3_PIN0_CFG_Rst                                0x0020UL                                                     /*!< PROFILE GPCON0_3: PIN0_CFG Reset                             */
#define PROFILE_GPCON0_3_PIN0_CFG_Addr                               0x20000394UL                                                 /*!< PROFILE GPCON0_3: PIN0_CFG Address                           */
#define PROFILE_GPCON0_3_PIN0_CFG_IRQ_IN0_Eval                       4
#define PROFILE_GPCON0_3_PIN0_CFG_IRQ_IN1_Eval                       5
#define PROFILE_GPCON0_3_PIN0_CFG_IRQ_OUT0_Eval                      6
#define PROFILE_GPCON0_3_PIN0_CFG_IRQ_OUT1_Eval                      7
#define PROFILE_GPCON0_3_PIN0_CFG_SPORT_TXDATA_Eval                  12
#define PROFILE_GPCON0_3_PIN0_CFG_SPORT_RXDATA_Eval                  13
#define PROFILE_GPCON0_3_PIN0_CFG_SPORT_TRXCLK_Eval                  14
#define PROFILE_GPCON0_3_PIN0_CFG_GPCLK_OUT_Eval                     36

#define PROFILE_GPCON0_3_PIN1_CFG_Size                               6                                                            /*!< PROFILE GPCON0_3: PIN1_CFG Size                              */
#define PROFILE_GPCON0_3_PIN1_CFG_Pos                                8                                                            /*!< PROFILE GPCON0_3: PIN1_CFG Position                          */
#define PROFILE_GPCON0_3_PIN1_CFG_Msk                                (0x003fUL << PROFILE_GPCON0_3_PIN1_CFG_Pos)                  /*!< PROFILE GPCON0_3: PIN1_CFG Mask                              */
#define PROFILE_GPCON0_3_PIN1_CFG_Rst                                0x0021UL                                                     /*!< PROFILE GPCON0_3: PIN1_CFG Reset                             */
#define PROFILE_GPCON0_3_PIN1_CFG_Addr                               0x20000394UL                                                 /*!< PROFILE GPCON0_3: PIN1_CFG Address                           */
#define PROFILE_GPCON0_3_PIN1_CFG_IRQ_IN0_Eval                       4
#define PROFILE_GPCON0_3_PIN1_CFG_IRQ_IN1_Eval                       5
#define PROFILE_GPCON0_3_PIN1_CFG_IRQ_OUT0_Eval                      6
#define PROFILE_GPCON0_3_PIN1_CFG_IRQ_OUT1_Eval                      7
#define PROFILE_GPCON0_3_PIN1_CFG_SPORT_TXDATA_Eval                  12
#define PROFILE_GPCON0_3_PIN1_CFG_SPORT_RXDATA_Eval                  13
#define PROFILE_GPCON0_3_PIN1_CFG_SPORT_TRXCLK_Eval                  14
#define PROFILE_GPCON0_3_PIN1_CFG_GPCLK_OUT_Eval                     36

#define PROFILE_GPCON0_3_PIN2_CFG_Size                               6                                                            /*!< PROFILE GPCON0_3: PIN2_CFG Size                              */
#define PROFILE_GPCON0_3_PIN2_CFG_Pos                                16                                                           /*!< PROFILE GPCON0_3: PIN2_CFG Position                          */
#define PROFILE_GPCON0_3_PIN2_CFG_Msk                                (0x003fUL << PROFILE_GPCON0_3_PIN2_CFG_Pos)                  /*!< PROFILE GPCON0_3: PIN2_CFG Mask                              */
#define PROFILE_GPCON0_3_PIN2_CFG_Rst                                0x0004UL                                                     /*!< PROFILE GPCON0_3: PIN2_CFG Reset                             */
#define PROFILE_GPCON0_3_PIN2_CFG_Addr                               0x20000394UL                                                 /*!< PROFILE GPCON0_3: PIN2_CFG Address                           */
#define PROFILE_GPCON0_3_PIN2_CFG_IRQ_IN0_Eval                       4
#define PROFILE_GPCON0_3_PIN2_CFG_IRQ_IN1_Eval                       5
#define PROFILE_GPCON0_3_PIN2_CFG_IRQ_OUT0_Eval                      6
#define PROFILE_GPCON0_3_PIN2_CFG_IRQ_OUT1_Eval                      7
#define PROFILE_GPCON0_3_PIN2_CFG_SPORT_TXDATA_Eval                  12
#define PROFILE_GPCON0_3_PIN2_CFG_SPORT_RXDATA_Eval                  13
#define PROFILE_GPCON0_3_PIN2_CFG_SPORT_TRXCLK_Eval                  14
#define PROFILE_GPCON0_3_PIN2_CFG_GPCLK_OUT_Eval                     36

#define PROFILE_GPCON0_3_PIN3_CFG_Size                               6                                                            /*!< PROFILE GPCON0_3: PIN3_CFG Size                              */
#define PROFILE_GPCON0_3_PIN3_CFG_Pos                                24                                                           /*!< PROFILE GPCON0_3: PIN3_CFG Position                          */
#define PROFILE_GPCON0_3_PIN3_CFG_Msk                                (0x003fUL << PROFILE_GPCON0_3_PIN3_CFG_Pos)                  /*!< PROFILE GPCON0_3: PIN3_CFG Mask                              */
#define PROFILE_GPCON0_3_PIN3_CFG_Rst                                0x0006UL                                                     /*!< PROFILE GPCON0_3: PIN3_CFG Reset                             */
#define PROFILE_GPCON0_3_PIN3_CFG_Addr                               0x20000394UL                                                 /*!< PROFILE GPCON0_3: PIN3_CFG Address                           */
#define PROFILE_GPCON0_3_PIN3_CFG_IRQ_IN0_Eval                       4
#define PROFILE_GPCON0_3_PIN3_CFG_IRQ_IN1_Eval                       5
#define PROFILE_GPCON0_3_PIN3_CFG_IRQ_OUT0_Eval                      6
#define PROFILE_GPCON0_3_PIN3_CFG_IRQ_OUT1_Eval                      7
#define PROFILE_GPCON0_3_PIN3_CFG_SPORT_TXDATA_Eval                  12
#define PROFILE_GPCON0_3_PIN3_CFG_SPORT_RXDATA_Eval                  13
#define PROFILE_GPCON0_3_PIN3_CFG_SPORT_TRXCLK_Eval                  14
#define PROFILE_GPCON0_3_PIN3_CFG_GPCLK_OUT_Eval                     36

/* ----------------------------  PROFILE_GPCON4_7  ---------------------------- */
#define PROFILE_GPCON4_7_Addr                                        0x20000398UL                                                 /*!< PROFILE GPCON4_7: Address                                    */
#define pPROFILE_GPCON4_7                                            (*(volatile uint32_t *) PROFILE_GPCON4_7_Addr)               /*!< PROFILE GPCON4_7: Pointer                                    */
#define PROFILE_GPCON4_7_Msk                                         0x3F3F3F3FUL                                                 /*!< PROFILE GPCON4_7: Mask                                       */
#define PROFILE_GPCON4_7_Rst                                         0x17160705UL                                                 /*!< PROFILE GPCON4_7: Reset                                      */

#define PROFILE_GPCON4_7_PIN4_CFG_Size                               6                                                            /*!< PROFILE GPCON4_7: PIN4_CFG Size                              */
#define PROFILE_GPCON4_7_PIN4_CFG_Pos                                0                                                            /*!< PROFILE GPCON4_7: PIN4_CFG Position                          */
#define PROFILE_GPCON4_7_PIN4_CFG_Msk                                (0x003fUL << PROFILE_GPCON4_7_PIN4_CFG_Pos)                  /*!< PROFILE GPCON4_7: PIN4_CFG Mask                              */
#define PROFILE_GPCON4_7_PIN4_CFG_Rst                                0x0005UL                                                     /*!< PROFILE GPCON4_7: PIN4_CFG Reset                             */
#define PROFILE_GPCON4_7_PIN4_CFG_Addr                               0x20000398UL                                                 /*!< PROFILE GPCON4_7: PIN4_CFG Address                           */
#define PROFILE_GPCON4_7_PIN4_CFG_IRQ_IN0_Eval                       4
#define PROFILE_GPCON4_7_PIN4_CFG_IRQ_IN1_Eval                       5
#define PROFILE_GPCON4_7_PIN4_CFG_IRQ_OUT0_Eval                      6
#define PROFILE_GPCON4_7_PIN4_CFG_IRQ_OUT1_Eval                      7
#define PROFILE_GPCON4_7_PIN4_CFG_SPORT_TXDATA_Eval                  12
#define PROFILE_GPCON4_7_PIN4_CFG_SPORT_RXDATA_Eval                  13
#define PROFILE_GPCON4_7_PIN4_CFG_SPORT_TRXCLK_Eval                  14
#define PROFILE_GPCON4_7_PIN4_CFG_GPCLK_OUT_Eval                     36

#define PROFILE_GPCON4_7_PIN5_CFG_Size                               6                                                            /*!< PROFILE GPCON4_7: PIN5_CFG Size                              */
#define PROFILE_GPCON4_7_PIN5_CFG_Pos                                8                                                            /*!< PROFILE GPCON4_7: PIN5_CFG Position                          */
#define PROFILE_GPCON4_7_PIN5_CFG_Msk                                (0x003fUL << PROFILE_GPCON4_7_PIN5_CFG_Pos)                  /*!< PROFILE GPCON4_7: PIN5_CFG Mask                              */
#define PROFILE_GPCON4_7_PIN5_CFG_Rst                                0x0007UL                                                     /*!< PROFILE GPCON4_7: PIN5_CFG Reset                             */
#define PROFILE_GPCON4_7_PIN5_CFG_Addr                               0x20000398UL                                                 /*!< PROFILE GPCON4_7: PIN5_CFG Address                           */
#define PROFILE_GPCON4_7_PIN5_CFG_IRQ_IN0_Eval                       4
#define PROFILE_GPCON4_7_PIN5_CFG_IRQ_IN1_Eval                       5
#define PROFILE_GPCON4_7_PIN5_CFG_IRQ_OUT0_Eval                      6
#define PROFILE_GPCON4_7_PIN5_CFG_IRQ_OUT1_Eval                      7
#define PROFILE_GPCON4_7_PIN5_CFG_SPORT_TXDATA_Eval                  12
#define PROFILE_GPCON4_7_PIN5_CFG_SPORT_RXDATA_Eval                  13
#define PROFILE_GPCON4_7_PIN5_CFG_SPORT_TRXCLK_Eval                  14
#define PROFILE_GPCON4_7_PIN5_CFG_GPCLK_OUT_Eval                     36

#define PROFILE_GPCON4_7_PIN6_CFG_Size                               6                                                            /*!< PROFILE GPCON4_7: PIN6_CFG Size                              */
#define PROFILE_GPCON4_7_PIN6_CFG_Pos                                16                                                           /*!< PROFILE GPCON4_7: PIN6_CFG Position                          */
#define PROFILE_GPCON4_7_PIN6_CFG_Msk                                (0x003fUL << PROFILE_GPCON4_7_PIN6_CFG_Pos)                  /*!< PROFILE GPCON4_7: PIN6_CFG Mask                              */
#define PROFILE_GPCON4_7_PIN6_CFG_Rst                                0x0016UL                                                     /*!< PROFILE GPCON4_7: PIN6_CFG Reset                             */
#define PROFILE_GPCON4_7_PIN6_CFG_Addr                               0x20000398UL                                                 /*!< PROFILE GPCON4_7: PIN6_CFG Address                           */
#define PROFILE_GPCON4_7_PIN6_CFG_IRQ_IN0_Eval                       4
#define PROFILE_GPCON4_7_PIN6_CFG_IRQ_IN1_Eval                       5
#define PROFILE_GPCON4_7_PIN6_CFG_IRQ_OUT0_Eval                      6
#define PROFILE_GPCON4_7_PIN6_CFG_IRQ_OUT1_Eval                      7
#define PROFILE_GPCON4_7_PIN6_CFG_SPORT_TXDATA_Eval                  12
#define PROFILE_GPCON4_7_PIN6_CFG_SPORT_RXDATA_Eval                  13
#define PROFILE_GPCON4_7_PIN6_CFG_SPORT_TRXCLK_Eval                  14
#define PROFILE_GPCON4_7_PIN6_CFG_GPCLK_OUT_Eval                     36

#define PROFILE_GPCON4_7_PIN7_CFG_Size                               6                                                            /*!< PROFILE GPCON4_7: PIN7_CFG Size                              */
#define PROFILE_GPCON4_7_PIN7_CFG_Pos                                24                                                           /*!< PROFILE GPCON4_7: PIN7_CFG Position                          */
#define PROFILE_GPCON4_7_PIN7_CFG_Msk                                (0x003fUL << PROFILE_GPCON4_7_PIN7_CFG_Pos)                  /*!< PROFILE GPCON4_7: PIN7_CFG Mask                              */
#define PROFILE_GPCON4_7_PIN7_CFG_Rst                                0x0017UL                                                     /*!< PROFILE GPCON4_7: PIN7_CFG Reset                             */
#define PROFILE_GPCON4_7_PIN7_CFG_Addr                               0x20000398UL                                                 /*!< PROFILE GPCON4_7: PIN7_CFG Address                           */
#define PROFILE_GPCON4_7_PIN7_CFG_IRQ_IN0_Eval                       4
#define PROFILE_GPCON4_7_PIN7_CFG_IRQ_IN1_Eval                       5
#define PROFILE_GPCON4_7_PIN7_CFG_IRQ_OUT0_Eval                      6
#define PROFILE_GPCON4_7_PIN7_CFG_IRQ_OUT1_Eval                      7
#define PROFILE_GPCON4_7_PIN7_CFG_SPORT_TXDATA_Eval                  12
#define PROFILE_GPCON4_7_PIN7_CFG_SPORT_RXDATA_Eval                  13
#define PROFILE_GPCON4_7_PIN7_CFG_SPORT_TRXCLK_Eval                  14
#define PROFILE_GPCON4_7_PIN7_CFG_GPCLK_OUT_Eval                     36


#define PROFILE_GPIO_CFG_Addr                                        0x2000039CUL

#define PROFILE_GPIO_CFG_GP_LOOP_PADS_Addr                           0x2000039CUL
#define PROFILE_GPIO_CFG_GP_LOOP_PADS_Pos                            0
#define PROFILE_GPIO_CFG_GP_LOOP_PADS_Size                           8

#define PROFILE_GPIO_CFG_GP_LOOP_Addr                                0x2000039CUL
#define PROFILE_GPIO_CFG_GP_LOOP_Pos                                 8
#define PROFILE_GPIO_CFG_GP_LOOP_Size                                8

#define PROFILE_GPIO_CFG_CUSTOM_SEL_Addr                             0x2000039CUL
#define PROFILE_GPIO_CFG_CUSTOM_SEL_Pos                              16
#define PROFILE_GPIO_CFG_CUSTOM_SEL_Size                             8

/* ----------------------------  PROFILE_SPARE0  ---------------------------- */
#define PROFILE_SPARE0_Addr                                          0x200003A0UL                                                 /*!< PROFILE SPARE0: Address                                      */
#define pPROFILE_SPARE0                                              (*(volatile uint32_t *) PROFILE_SPARE0_Addr)                 /*!< PROFILE SPARE0: Pointer                                      */
#define PROFILE_SPARE0_Msk                                           0xFFFFFFFFUL                                                 /*!< PROFILE SPARE0: Mask                                         */
#define PROFILE_SPARE0_Rst                                           0x00000000UL                                                 /*!< PROFILE SPARE0: Reset                                        */

#define PROFILE_SPARE0_VAL_Size                                      32                                                           /*!< PROFILE SPARE0: VAL Size                                     */
#define PROFILE_SPARE0_VAL_Pos                                       0                                                            /*!< PROFILE SPARE0: VAL Position                                 */
#define PROFILE_SPARE0_VAL_Msk                                       (0xffffffffUL << PROFILE_SPARE0_VAL_Pos)                     /*!< PROFILE SPARE0: VAL Mask                                     */
#define PROFILE_SPARE0_VAL_Rst                                       0x0000UL                                                     /*!< PROFILE SPARE0: VAL Reset                                    */
#define PROFILE_SPARE0_VAL_Addr                                      0x200003a0UL                                                 /*!< PROFILE SPARE0: VAL Address                                  */

/* ----------------------------  PROFILE_SPARE1  ---------------------------- */
#define PROFILE_SPARE1_Addr                                          0x200003A4UL                                                 /*!< PROFILE SPARE1: Address                                      */
#define pPROFILE_SPARE1                                              (*(volatile uint32_t *) PROFILE_SPARE1_Addr)                 /*!< PROFILE SPARE1: Pointer                                      */
#define PROFILE_SPARE1_Msk                                           0xFFFFFFFFUL                                                 /*!< PROFILE SPARE1: Mask                                         */
#define PROFILE_SPARE1_Rst                                           0x00000000UL                                                 /*!< PROFILE SPARE1: Reset                                        */

#define PROFILE_SPARE1_VAL_Size                                      32                                                           /*!< PROFILE SPARE1: VAL Size                                     */
#define PROFILE_SPARE1_VAL_Pos                                       0                                                            /*!< PROFILE SPARE1: VAL Position                                 */
#define PROFILE_SPARE1_VAL_Msk                                       (0xffffffffUL << PROFILE_SPARE1_VAL_Pos)                     /*!< PROFILE SPARE1: VAL Mask                                     */
#define PROFILE_SPARE1_VAL_Rst                                       0x0000UL                                                     /*!< PROFILE SPARE1: VAL Reset                                    */
#define PROFILE_SPARE1_VAL_Addr                                      0x200003a4UL                                                 /*!< PROFILE SPARE1: VAL Address                                  */

/* ----------------------------  PROFILE_SPARE2  ---------------------------- */
#define PROFILE_SPARE2_Addr                                          0x200003A8UL                                                 /*!< PROFILE SPARE2: Address                                      */
#define pPROFILE_SPARE2                                              (*(volatile uint32_t *) PROFILE_SPARE2_Addr)                 /*!< PROFILE SPARE2: Pointer                                      */
#define PROFILE_SPARE2_Msk                                           0xFFFFFFFFUL                                                 /*!< PROFILE SPARE2: Mask                                         */
#define PROFILE_SPARE2_Rst                                           0x00000000UL                                                 /*!< PROFILE SPARE2: Reset                                        */

#define PROFILE_SPARE2_VAL_Size                                      32                                                           /*!< PROFILE SPARE2: VAL Size                                     */
#define PROFILE_SPARE2_VAL_Pos                                       0                                                            /*!< PROFILE SPARE2: VAL Position                                 */
#define PROFILE_SPARE2_VAL_Msk                                       (0xffffffffUL << PROFILE_SPARE2_VAL_Pos)                     /*!< PROFILE SPARE2: VAL Mask                                     */
#define PROFILE_SPARE2_VAL_Rst                                       0x0000UL                                                     /*!< PROFILE SPARE2: VAL Reset                                    */
#define PROFILE_SPARE2_VAL_Addr                                      0x200003a8UL                                                 /*!< PROFILE SPARE2: VAL Address                                  */

/* ----------------------------  PROFILE_SPARE3  ---------------------------- */
#define PROFILE_SPARE3_Addr                                          0x200003ACUL                                                 /*!< PROFILE SPARE3: Address                                      */
#define pPROFILE_SPARE3                                              (*(volatile uint32_t *) PROFILE_SPARE3_Addr)                 /*!< PROFILE SPARE3: Pointer                                      */
#define PROFILE_SPARE3_Msk                                           0xFFFFFFFFUL                                                 /*!< PROFILE SPARE3: Mask                                         */
#define PROFILE_SPARE3_Rst                                           0x00000000UL                                                 /*!< PROFILE SPARE3: Reset                                        */

#define PROFILE_SPARE3_VAL_Size                                      32                                                           /*!< PROFILE SPARE3: VAL Size                                     */
#define PROFILE_SPARE3_VAL_Pos                                       0                                                            /*!< PROFILE SPARE3: VAL Position                                 */
#define PROFILE_SPARE3_VAL_Msk                                       (0xffffffffUL << PROFILE_SPARE3_VAL_Pos)                     /*!< PROFILE SPARE3: VAL Mask                                     */
#define PROFILE_SPARE3_VAL_Rst                                       0x0000UL                                                     /*!< PROFILE SPARE3: VAL Reset                                    */
#define PROFILE_SPARE3_VAL_Addr                                      0x200003acUL                                                 /*!< PROFILE SPARE3: VAL Address                                  */

/* ----------------------------  PROFILE_SPARE4  ---------------------------- */
#define PROFILE_SPARE4_Addr                                          0x200003B0UL                                                 /*!< PROFILE SPARE4: Address                                      */
#define pPROFILE_SPARE4                                              (*(volatile uint32_t *) PROFILE_SPARE4_Addr)                 /*!< PROFILE SPARE4: Pointer                                      */
#define PROFILE_SPARE4_Msk                                           0xFFFFFFFFUL                                                 /*!< PROFILE SPARE4: Mask                                         */
#define PROFILE_SPARE4_Rst                                           0x00000000UL                                                 /*!< PROFILE SPARE4: Reset                                        */

#define PROFILE_SPARE4_VAL_Size                                      32                                                           /*!< PROFILE SPARE4: VAL Size                                     */
#define PROFILE_SPARE4_VAL_Pos                                       0                                                            /*!< PROFILE SPARE4: VAL Position                                 */
#define PROFILE_SPARE4_VAL_Msk                                       (0xffffffffUL << PROFILE_SPARE4_VAL_Pos)                     /*!< PROFILE SPARE4: VAL Mask                                     */
#define PROFILE_SPARE4_VAL_Rst                                       0x0000UL                                                     /*!< PROFILE SPARE4: VAL Reset                                    */
#define PROFILE_SPARE4_VAL_Addr                                      0x200003b0UL                                                 /*!< PROFILE SPARE4: VAL Address                                  */

/* ----------------------------  PROFILE_SPARE5  ---------------------------- */
#define PROFILE_SPARE5_Addr                                          0x200003B4UL                                                 /*!< PROFILE SPARE5: Address                                      */
#define pPROFILE_SPARE5                                              (*(volatile uint32_t *) PROFILE_SPARE5_Addr)                 /*!< PROFILE SPARE5: Pointer                                      */
#define PROFILE_SPARE5_Msk                                           0xFFFFFFFFUL                                                 /*!< PROFILE SPARE5: Mask                                         */
#define PROFILE_SPARE5_Rst                                           0x00000000UL                                                 /*!< PROFILE SPARE5: Reset                                        */

#define PROFILE_SPARE5_VAL_Size                                      32                                                           /*!< PROFILE SPARE5: VAL Size                                     */
#define PROFILE_SPARE5_VAL_Pos                                       0                                                            /*!< PROFILE SPARE5: VAL Position                                 */
#define PROFILE_SPARE5_VAL_Msk                                       (0xffffffffUL << PROFILE_SPARE5_VAL_Pos)                     /*!< PROFILE SPARE5: VAL Mask                                     */
#define PROFILE_SPARE5_VAL_Rst                                       0x0000UL                                                     /*!< PROFILE SPARE5: VAL Reset                                    */
#define PROFILE_SPARE5_VAL_Addr                                      0x200003b4UL                                                 /*!< PROFILE SPARE5: VAL Address                                  */

/* ----------------------------  PROFILE_SPARE6  ---------------------------- */
#define PROFILE_SPARE6_Addr                                          0x200003B8UL                                                 /*!< PROFILE SPARE6: Address                                      */
#define pPROFILE_SPARE6                                              (*(volatile uint32_t *) PROFILE_SPARE6_Addr)                 /*!< PROFILE SPARE6: Pointer                                      */
#define PROFILE_SPARE6_Msk                                           0xFFFFFFFFUL                                                 /*!< PROFILE SPARE6: Mask                                         */
#define PROFILE_SPARE6_Rst                                           0x00000000UL                                                 /*!< PROFILE SPARE6: Reset                                        */

#define PROFILE_SPARE6_VAL_Size                                      32                                                           /*!< PROFILE SPARE6: VAL Size                                     */
#define PROFILE_SPARE6_VAL_Pos                                       0                                                            /*!< PROFILE SPARE6: VAL Position                                 */
#define PROFILE_SPARE6_VAL_Msk                                       (0xffffffffUL << PROFILE_SPARE6_VAL_Pos)                     /*!< PROFILE SPARE6: VAL Mask                                     */
#define PROFILE_SPARE6_VAL_Rst                                       0x0000UL                                                     /*!< PROFILE SPARE6: VAL Reset                                    */
#define PROFILE_SPARE6_VAL_Addr                                      0x200003b8UL                                                 /*!< PROFILE SPARE6: VAL Address                                  */

/* ----------------------------  PROFILE_SPARE7  ---------------------------- */
#define PROFILE_SPARE7_Addr                                          0x200003BCUL                                                 /*!< PROFILE SPARE7: Address                                      */
#define pPROFILE_SPARE7                                              (*(volatile uint32_t *) PROFILE_SPARE7_Addr)                 /*!< PROFILE SPARE7: Pointer                                      */
#define PROFILE_SPARE7_Msk                                           0xFFFFFFFFUL                                                 /*!< PROFILE SPARE7: Mask                                         */
#define PROFILE_SPARE7_Rst                                           0x00000000UL                                                 /*!< PROFILE SPARE7: Reset                                        */

#define PROFILE_SPARE7_VAL_Size                                      32                                                           /*!< PROFILE SPARE7: VAL Size                                     */
#define PROFILE_SPARE7_VAL_Pos                                       0                                                            /*!< PROFILE SPARE7: VAL Position                                 */
#define PROFILE_SPARE7_VAL_Msk                                       (0xffffffffUL << PROFILE_SPARE7_VAL_Pos)                     /*!< PROFILE SPARE7: VAL Mask                                     */
#define PROFILE_SPARE7_VAL_Rst                                       0x0000UL                                                     /*!< PROFILE SPARE7: VAL Reset                                    */
#define PROFILE_SPARE7_VAL_Addr                                      0x200003bcUL                                                 /*!< PROFILE SPARE7: VAL Address                                  */

/* ----------------------------  PROFILE_SPARE8  ---------------------------- */
#define PROFILE_SPARE8_Addr                                          0x200003C0UL                                                 /*!< PROFILE SPARE8: Address                                      */
#define pPROFILE_SPARE8                                              (*(volatile uint32_t *) PROFILE_SPARE8_Addr)                 /*!< PROFILE SPARE8: Pointer                                      */
#define PROFILE_SPARE8_Msk                                           0xFFFFFFFFUL                                                 /*!< PROFILE SPARE8: Mask                                         */
#define PROFILE_SPARE8_Rst                                           0x00000000UL                                                 /*!< PROFILE SPARE8: Reset                                        */

#define PROFILE_SPARE8_VAL_Size                                      32                                                           /*!< PROFILE SPARE8: VAL Size                                     */
#define PROFILE_SPARE8_VAL_Pos                                       0                                                            /*!< PROFILE SPARE8: VAL Position                                 */
#define PROFILE_SPARE8_VAL_Msk                                       (0xffffffffUL << PROFILE_SPARE8_VAL_Pos)                     /*!< PROFILE SPARE8: VAL Mask                                     */
#define PROFILE_SPARE8_VAL_Rst                                       0x0000UL                                                     /*!< PROFILE SPARE8: VAL Reset                                    */
#define PROFILE_SPARE8_VAL_Addr                                      0x200003c0UL                                                 /*!< PROFILE SPARE8: VAL Address                                  */

/* ----------------------------  PROFILE_SPARE9  ---------------------------- */
#define PROFILE_SPARE9_Addr                                          0x200003C4UL                                                 /*!< PROFILE SPARE9: Address                                      */
#define pPROFILE_SPARE9                                              (*(volatile uint32_t *) PROFILE_SPARE9_Addr)                 /*!< PROFILE SPARE9: Pointer                                      */
#define PROFILE_SPARE9_Msk                                           0xFFFFFFFFUL                                                 /*!< PROFILE SPARE9: Mask                                         */
#define PROFILE_SPARE9_Rst                                           0x00000000UL                                                 /*!< PROFILE SPARE9: Reset                                        */

#define PROFILE_SPARE9_VAL_Size                                      32                                                           /*!< PROFILE SPARE9: VAL Size                                     */
#define PROFILE_SPARE9_VAL_Pos                                       0                                                            /*!< PROFILE SPARE9: VAL Position                                 */
#define PROFILE_SPARE9_VAL_Msk                                       (0xffffffffUL << PROFILE_SPARE9_VAL_Pos)                     /*!< PROFILE SPARE9: VAL Mask                                     */
#define PROFILE_SPARE9_VAL_Rst                                       0x0000UL                                                     /*!< PROFILE SPARE9: VAL Reset                                    */
#define PROFILE_SPARE9_VAL_Addr                                      0x200003c4UL                                                 /*!< PROFILE SPARE9: VAL Address                                  */

/* ----------------------------  PROFILE_RADIO_CAL_RESULTS0  ---------------------------- */
#define PROFILE_RADIO_CAL_RESULTS0_Addr                              0x200003C8UL                                                 /*!< PROFILE RADIO_CAL_RESULTS0: Address                          */
#define pPROFILE_RADIO_CAL_RESULTS0                                  (*(volatile uint32_t *) PROFILE_RADIO_CAL_RESULTS0_Addr)     /*!< PROFILE RADIO_CAL_RESULTS0: Pointer                          */
#define PROFILE_RADIO_CAL_RESULTS0_Msk                               0xFFFFFFFFUL                                                 /*!< PROFILE RADIO_CAL_RESULTS0: Mask                             */
#define PROFILE_RADIO_CAL_RESULTS0_Rst                               0x00000000UL                                                 /*!< PROFILE RADIO_CAL_RESULTS0: Reset                            */

#define PROFILE_RADIO_CAL_RESULTS0_DATA_Size                         32                                                           /*!< PROFILE RADIO_CAL_RESULTS0: DATA Size                        */
#define PROFILE_RADIO_CAL_RESULTS0_DATA_Pos                          0                                                            /*!< PROFILE RADIO_CAL_RESULTS0: DATA Position                    */
#define PROFILE_RADIO_CAL_RESULTS0_DATA_Msk                          (0xffffffffUL << PROFILE_RADIO_CAL_RESULTS0_DATA_Pos)        /*!< PROFILE RADIO_CAL_RESULTS0: DATA Mask                        */
#define PROFILE_RADIO_CAL_RESULTS0_DATA_Rst                          0x0000UL                                                     /*!< PROFILE RADIO_CAL_RESULTS0: DATA Reset                       */
#define PROFILE_RADIO_CAL_RESULTS0_DATA_Addr                         0x200003c8UL                                                 /*!< PROFILE RADIO_CAL_RESULTS0: DATA Address                     */

/* ----------------------------  PROFILE_RADIO_CAL_RESULTS1  ---------------------------- */
#define PROFILE_RADIO_CAL_RESULTS1_Addr                              0x200003CCUL                                                 /*!< PROFILE RADIO_CAL_RESULTS1: Address                          */
#define pPROFILE_RADIO_CAL_RESULTS1                                  (*(volatile uint32_t *) PROFILE_RADIO_CAL_RESULTS1_Addr)     /*!< PROFILE RADIO_CAL_RESULTS1: Pointer                          */
#define PROFILE_RADIO_CAL_RESULTS1_Msk                               0xFFFFFFFFUL                                                 /*!< PROFILE RADIO_CAL_RESULTS1: Mask                             */
#define PROFILE_RADIO_CAL_RESULTS1_Rst                               0x00000000UL                                                 /*!< PROFILE RADIO_CAL_RESULTS1: Reset                            */

#define PROFILE_RADIO_CAL_RESULTS1_DATA_Size                         32                                                           /*!< PROFILE RADIO_CAL_RESULTS1: DATA Size                        */
#define PROFILE_RADIO_CAL_RESULTS1_DATA_Pos                          0                                                            /*!< PROFILE RADIO_CAL_RESULTS1: DATA Position                    */
#define PROFILE_RADIO_CAL_RESULTS1_DATA_Msk                          (0xffffffffUL << PROFILE_RADIO_CAL_RESULTS1_DATA_Pos)        /*!< PROFILE RADIO_CAL_RESULTS1: DATA Mask                        */
#define PROFILE_RADIO_CAL_RESULTS1_DATA_Rst                          0x0000UL                                                     /*!< PROFILE RADIO_CAL_RESULTS1: DATA Reset                       */
#define PROFILE_RADIO_CAL_RESULTS1_DATA_Addr                         0x200003ccUL                                                 /*!< PROFILE RADIO_CAL_RESULTS1: DATA Address                     */

/* ----------------------------  PROFILE_RADIO_CAL_RESULTS2  ---------------------------- */
#define PROFILE_RADIO_CAL_RESULTS2_Addr                              0x200003D0UL                                                 /*!< PROFILE RADIO_CAL_RESULTS2: Address                          */
#define pPROFILE_RADIO_CAL_RESULTS2                                  (*(volatile uint32_t *) PROFILE_RADIO_CAL_RESULTS2_Addr)     /*!< PROFILE RADIO_CAL_RESULTS2: Pointer                          */
#define PROFILE_RADIO_CAL_RESULTS2_Msk                               0xFFFFFFFFUL                                                 /*!< PROFILE RADIO_CAL_RESULTS2: Mask                             */
#define PROFILE_RADIO_CAL_RESULTS2_Rst                               0x00000000UL                                                 /*!< PROFILE RADIO_CAL_RESULTS2: Reset                            */

#define PROFILE_RADIO_CAL_RESULTS2_DATA_Size                         32                                                           /*!< PROFILE RADIO_CAL_RESULTS2: DATA Size                        */
#define PROFILE_RADIO_CAL_RESULTS2_DATA_Pos                          0                                                            /*!< PROFILE RADIO_CAL_RESULTS2: DATA Position                    */
#define PROFILE_RADIO_CAL_RESULTS2_DATA_Msk                          (0xffffffffUL << PROFILE_RADIO_CAL_RESULTS2_DATA_Pos)        /*!< PROFILE RADIO_CAL_RESULTS2: DATA Mask                        */
#define PROFILE_RADIO_CAL_RESULTS2_DATA_Rst                          0x0000UL                                                     /*!< PROFILE RADIO_CAL_RESULTS2: DATA Reset                       */
#define PROFILE_RADIO_CAL_RESULTS2_DATA_Addr                         0x200003d0UL                                                 /*!< PROFILE RADIO_CAL_RESULTS2: DATA Address                     */

/* ----------------------------  PROFILE_RADIO_CAL_RESULTS3  ---------------------------- */
#define PROFILE_RADIO_CAL_RESULTS3_Addr                              0x200003D4UL                                                 /*!< PROFILE RADIO_CAL_RESULTS3: Address                          */
#define pPROFILE_RADIO_CAL_RESULTS3                                  (*(volatile uint32_t *) PROFILE_RADIO_CAL_RESULTS3_Addr)     /*!< PROFILE RADIO_CAL_RESULTS3: Pointer                          */
#define PROFILE_RADIO_CAL_RESULTS3_Msk                               0xFFFFFFFFUL                                                 /*!< PROFILE RADIO_CAL_RESULTS3: Mask                             */
#define PROFILE_RADIO_CAL_RESULTS3_Rst                               0x00000000UL                                                 /*!< PROFILE RADIO_CAL_RESULTS3: Reset                            */

#define PROFILE_RADIO_CAL_RESULTS3_DATA_Size                         32                                                           /*!< PROFILE RADIO_CAL_RESULTS3: DATA Size                        */
#define PROFILE_RADIO_CAL_RESULTS3_DATA_Pos                          0                                                            /*!< PROFILE RADIO_CAL_RESULTS3: DATA Position                    */
#define PROFILE_RADIO_CAL_RESULTS3_DATA_Msk                          (0xffffffffUL << PROFILE_RADIO_CAL_RESULTS3_DATA_Pos)        /*!< PROFILE RADIO_CAL_RESULTS3: DATA Mask                        */
#define PROFILE_RADIO_CAL_RESULTS3_DATA_Rst                          0x0000UL                                                     /*!< PROFILE RADIO_CAL_RESULTS3: DATA Reset                       */
#define PROFILE_RADIO_CAL_RESULTS3_DATA_Addr                         0x200003d4UL                                                 /*!< PROFILE RADIO_CAL_RESULTS3: DATA Address                     */

/* ----------------------------  PROFILE_RADIO_CAL_RESULTS4  ---------------------------- */
#define PROFILE_RADIO_CAL_RESULTS4_Addr                              0x200003D8UL                                                 /*!< PROFILE RADIO_CAL_RESULTS4: Address                          */
#define pPROFILE_RADIO_CAL_RESULTS4                                  (*(volatile uint32_t *) PROFILE_RADIO_CAL_RESULTS4_Addr)     /*!< PROFILE RADIO_CAL_RESULTS4: Pointer                          */
#define PROFILE_RADIO_CAL_RESULTS4_Msk                               0xFFFFFFFFUL                                                 /*!< PROFILE RADIO_CAL_RESULTS4: Mask                             */
#define PROFILE_RADIO_CAL_RESULTS4_Rst                               0x00000000UL                                                 /*!< PROFILE RADIO_CAL_RESULTS4: Reset                            */

#define PROFILE_RADIO_CAL_RESULTS4_DATA_Size                         32                                                           /*!< PROFILE RADIO_CAL_RESULTS4: DATA Size                        */
#define PROFILE_RADIO_CAL_RESULTS4_DATA_Pos                          0                                                            /*!< PROFILE RADIO_CAL_RESULTS4: DATA Position                    */
#define PROFILE_RADIO_CAL_RESULTS4_DATA_Msk                          (0xffffffffUL << PROFILE_RADIO_CAL_RESULTS4_DATA_Pos)        /*!< PROFILE RADIO_CAL_RESULTS4: DATA Mask                        */
#define PROFILE_RADIO_CAL_RESULTS4_DATA_Rst                          0x0000UL                                                     /*!< PROFILE RADIO_CAL_RESULTS4: DATA Reset                       */
#define PROFILE_RADIO_CAL_RESULTS4_DATA_Addr                         0x200003d8UL                                                 /*!< PROFILE RADIO_CAL_RESULTS4: DATA Address                     */

/* ----------------------------  PROFILE_RADIO_CAL_RESULTS5  ---------------------------- */
#define PROFILE_RADIO_CAL_RESULTS5_Addr                              0x200003DCUL                                                 /*!< PROFILE RADIO_CAL_RESULTS5: Address                          */
#define pPROFILE_RADIO_CAL_RESULTS5                                  (*(volatile uint32_t *) PROFILE_RADIO_CAL_RESULTS5_Addr)     /*!< PROFILE RADIO_CAL_RESULTS5: Pointer                          */
#define PROFILE_RADIO_CAL_RESULTS5_Msk                               0xFFFFFFFFUL                                                 /*!< PROFILE RADIO_CAL_RESULTS5: Mask                             */
#define PROFILE_RADIO_CAL_RESULTS5_Rst                               0x00000000UL                                                 /*!< PROFILE RADIO_CAL_RESULTS5: Reset                            */

#define PROFILE_RADIO_CAL_RESULTS5_DATA_Size                         32                                                           /*!< PROFILE RADIO_CAL_RESULTS5: DATA Size                        */
#define PROFILE_RADIO_CAL_RESULTS5_DATA_Pos                          0                                                            /*!< PROFILE RADIO_CAL_RESULTS5: DATA Position                    */
#define PROFILE_RADIO_CAL_RESULTS5_DATA_Msk                          (0xffffffffUL << PROFILE_RADIO_CAL_RESULTS5_DATA_Pos)        /*!< PROFILE RADIO_CAL_RESULTS5: DATA Mask                        */
#define PROFILE_RADIO_CAL_RESULTS5_DATA_Rst                          0x0000UL                                                     /*!< PROFILE RADIO_CAL_RESULTS5: DATA Reset                       */
#define PROFILE_RADIO_CAL_RESULTS5_DATA_Addr                         0x200003dcUL                                                 /*!< PROFILE RADIO_CAL_RESULTS5: DATA Address                     */

/* ----------------------------  PROFILE_RADIO_CAL_RESULTS6  ---------------------------- */
#define PROFILE_RADIO_CAL_RESULTS6_Addr                              0x200003E0UL                                                 /*!< PROFILE RADIO_CAL_RESULTS6: Address                          */
#define pPROFILE_RADIO_CAL_RESULTS6                                  (*(volatile uint32_t *) PROFILE_RADIO_CAL_RESULTS6_Addr)     /*!< PROFILE RADIO_CAL_RESULTS6: Pointer                          */
#define PROFILE_RADIO_CAL_RESULTS6_Msk                               0xFFFFFFFFUL                                                 /*!< PROFILE RADIO_CAL_RESULTS6: Mask                             */
#define PROFILE_RADIO_CAL_RESULTS6_Rst                               0x00000000UL                                                 /*!< PROFILE RADIO_CAL_RESULTS6: Reset                            */

#define PROFILE_RADIO_CAL_RESULTS6_DATA_Size                         32                                                           /*!< PROFILE RADIO_CAL_RESULTS6: DATA Size                        */
#define PROFILE_RADIO_CAL_RESULTS6_DATA_Pos                          0                                                            /*!< PROFILE RADIO_CAL_RESULTS6: DATA Position                    */
#define PROFILE_RADIO_CAL_RESULTS6_DATA_Msk                          (0xffffffffUL << PROFILE_RADIO_CAL_RESULTS6_DATA_Pos)        /*!< PROFILE RADIO_CAL_RESULTS6: DATA Mask                        */
#define PROFILE_RADIO_CAL_RESULTS6_DATA_Rst                          0x0000UL                                                     /*!< PROFILE RADIO_CAL_RESULTS6: DATA Reset                       */
#define PROFILE_RADIO_CAL_RESULTS6_DATA_Addr                         0x200003e0UL                                                 /*!< PROFILE RADIO_CAL_RESULTS6: DATA Address                     */

/* ----------------------------  PROFILE_RADIO_CAL_RESULTS7  ---------------------------- */
#define PROFILE_RADIO_CAL_RESULTS7_Addr                              0x200003E4UL                                                 /*!< PROFILE RADIO_CAL_RESULTS7: Address                          */
#define pPROFILE_RADIO_CAL_RESULTS7                                  (*(volatile uint32_t *) PROFILE_RADIO_CAL_RESULTS7_Addr)     /*!< PROFILE RADIO_CAL_RESULTS7: Pointer                          */
#define PROFILE_RADIO_CAL_RESULTS7_Msk                               0xFFFFFFFFUL                                                 /*!< PROFILE RADIO_CAL_RESULTS7: Mask                             */
#define PROFILE_RADIO_CAL_RESULTS7_Rst                               0x00000000UL                                                 /*!< PROFILE RADIO_CAL_RESULTS7: Reset                            */

#define PROFILE_RADIO_CAL_RESULTS7_DATA_Size                         32                                                           /*!< PROFILE RADIO_CAL_RESULTS7: DATA Size                        */
#define PROFILE_RADIO_CAL_RESULTS7_DATA_Pos                          0                                                            /*!< PROFILE RADIO_CAL_RESULTS7: DATA Position                    */
#define PROFILE_RADIO_CAL_RESULTS7_DATA_Msk                          (0xffffffffUL << PROFILE_RADIO_CAL_RESULTS7_DATA_Pos)        /*!< PROFILE RADIO_CAL_RESULTS7: DATA Mask                        */
#define PROFILE_RADIO_CAL_RESULTS7_DATA_Rst                          0x0000UL                                                     /*!< PROFILE RADIO_CAL_RESULTS7: DATA Reset                       */
#define PROFILE_RADIO_CAL_RESULTS7_DATA_Addr                         0x200003e4UL                                                 /*!< PROFILE RADIO_CAL_RESULTS7: DATA Address                     */

/* ----------------------------  PROFILE_RADIO_CAL_RESULTS8  ---------------------------- */
#define PROFILE_RADIO_CAL_RESULTS8_Addr                              0x200003E8UL                                                 /*!< PROFILE RADIO_CAL_RESULTS8: Address                          */
#define pPROFILE_RADIO_CAL_RESULTS8                                  (*(volatile uint32_t *) PROFILE_RADIO_CAL_RESULTS8_Addr)     /*!< PROFILE RADIO_CAL_RESULTS8: Pointer                          */
#define PROFILE_RADIO_CAL_RESULTS8_Msk                               0xFFFFFFFFUL                                                 /*!< PROFILE RADIO_CAL_RESULTS8: Mask                             */
#define PROFILE_RADIO_CAL_RESULTS8_Rst                               0x00000000UL                                                 /*!< PROFILE RADIO_CAL_RESULTS8: Reset                            */

#define PROFILE_RADIO_CAL_RESULTS8_DATA_Size                         32                                                           /*!< PROFILE RADIO_CAL_RESULTS8: DATA Size                        */
#define PROFILE_RADIO_CAL_RESULTS8_DATA_Pos                          0                                                            /*!< PROFILE RADIO_CAL_RESULTS8: DATA Position                    */
#define PROFILE_RADIO_CAL_RESULTS8_DATA_Msk                          (0xffffffffUL << PROFILE_RADIO_CAL_RESULTS8_DATA_Pos)        /*!< PROFILE RADIO_CAL_RESULTS8: DATA Mask                        */
#define PROFILE_RADIO_CAL_RESULTS8_DATA_Rst                          0x0000UL                                                     /*!< PROFILE RADIO_CAL_RESULTS8: DATA Reset                       */
#define PROFILE_RADIO_CAL_RESULTS8_DATA_Addr                         0x200003e8UL                                                 /*!< PROFILE RADIO_CAL_RESULTS8: DATA Address                     */

/* ======================================================================================== */
/* ================          struct 'GENERIC_PKT'                          ================ */
/* ======================================================================================== */


/* ----------------------------  GENERIC_PKT_BUFF_CFG0  ---------------------------- */
#define GENERIC_PKT_BUFF_CFG0_Addr                                   0x200004F4UL                                                 /*!< GENERIC_PKT BUFF_CFG0: Address                               */
#define pGENERIC_PKT_BUFF_CFG0                                       (*(volatile uint32_t *) GENERIC_PKT_BUFF_CFG0_Addr)          /*!< GENERIC_PKT BUFF_CFG0: Pointer                               */
#define GENERIC_PKT_BUFF_CFG0_Msk                                    0x017FFFFFUL                                                 /*!< GENERIC_PKT BUFF_CFG0: Mask                                  */
#define GENERIC_PKT_BUFF_CFG0_Rst                                    0x0015E2BCUL                                                 /*!< GENERIC_PKT BUFF_CFG0: Reset                                 */

#define GENERIC_PKT_BUFF_CFG0_PTR_RX_BASE_Size                       11                                                           /*!< GENERIC_PKT BUFF_CFG0: PTR_RX_BASE Size                      */
#define GENERIC_PKT_BUFF_CFG0_PTR_RX_BASE_Pos                        0                                                            /*!< GENERIC_PKT BUFF_CFG0: PTR_RX_BASE Position                  */
#define GENERIC_PKT_BUFF_CFG0_PTR_RX_BASE_Msk                        (0x07ffUL << GENERIC_PKT_BUFF_CFG0_PTR_RX_BASE_Pos)          /*!< GENERIC_PKT BUFF_CFG0: PTR_RX_BASE Mask                      */
#define GENERIC_PKT_BUFF_CFG0_PTR_RX_BASE_Rst                        0x02bcUL                                                     /*!< GENERIC_PKT BUFF_CFG0: PTR_RX_BASE Reset                     */
#define GENERIC_PKT_BUFF_CFG0_PTR_RX_BASE_Addr                       0x200004f4UL                                                 /*!< GENERIC_PKT BUFF_CFG0: PTR_RX_BASE Address                   */

#define GENERIC_PKT_BUFF_CFG0_PTR_TX_BASE_Size                       11                                                           /*!< GENERIC_PKT BUFF_CFG0: PTR_TX_BASE Size                      */
#define GENERIC_PKT_BUFF_CFG0_PTR_TX_BASE_Pos                        11                                                           /*!< GENERIC_PKT BUFF_CFG0: PTR_TX_BASE Position                  */
#define GENERIC_PKT_BUFF_CFG0_PTR_TX_BASE_Msk                        (0x07ffUL << GENERIC_PKT_BUFF_CFG0_PTR_TX_BASE_Pos)          /*!< GENERIC_PKT BUFF_CFG0: PTR_TX_BASE Mask                      */
#define GENERIC_PKT_BUFF_CFG0_PTR_TX_BASE_Rst                        0x02bcUL                                                     /*!< GENERIC_PKT BUFF_CFG0: PTR_TX_BASE Reset                     */
#define GENERIC_PKT_BUFF_CFG0_PTR_TX_BASE_Addr                       0x200004f4UL                                                 /*!< GENERIC_PKT BUFF_CFG0: PTR_TX_BASE Address                   */

#define GENERIC_PKT_BUFF_CFG0_BIT2AIR_Size                           1                                                            /*!< GENERIC_PKT BUFF_CFG0: BIT2AIR Size                          */
#define GENERIC_PKT_BUFF_CFG0_BIT2AIR_Pos                            22                                                           /*!< GENERIC_PKT BUFF_CFG0: BIT2AIR Position                      */
#define GENERIC_PKT_BUFF_CFG0_BIT2AIR_Msk                            (0x0001UL << GENERIC_PKT_BUFF_CFG0_BIT2AIR_Pos)              /*!< GENERIC_PKT BUFF_CFG0: BIT2AIR Mask                          */
#define GENERIC_PKT_BUFF_CFG0_BIT2AIR_Rst                            0x0000UL                                                     /*!< GENERIC_PKT BUFF_CFG0: BIT2AIR Reset                         */
#define GENERIC_PKT_BUFF_CFG0_BIT2AIR_Addr                           0x200004f4UL                                                 /*!< GENERIC_PKT BUFF_CFG0: BIT2AIR Address                       */
#define GENERIC_PKT_BUFF_CFG0_BIT2AIR_MSB_FIRST_Eval                 0
#define GENERIC_PKT_BUFF_CFG0_BIT2AIR_LSB_FIRST_Eval                 1

#define GENERIC_PKT_BUFF_CFG0_ROLLING_BUFF_EN_Size                   1                                                            /*!< GENERIC_PKT BUFF_CFG0: ROLLING_BUFF_EN Size                  */
#define GENERIC_PKT_BUFF_CFG0_ROLLING_BUFF_EN_Pos                    24                                                           /*!< GENERIC_PKT BUFF_CFG0: ROLLING_BUFF_EN Position              */
#define GENERIC_PKT_BUFF_CFG0_ROLLING_BUFF_EN_Msk                    (0x0001UL << GENERIC_PKT_BUFF_CFG0_ROLLING_BUFF_EN_Pos)      /*!< GENERIC_PKT BUFF_CFG0: ROLLING_BUFF_EN Mask                  */
#define GENERIC_PKT_BUFF_CFG0_ROLLING_BUFF_EN_Rst                    0x0000UL                                                     /*!< GENERIC_PKT BUFF_CFG0: ROLLING_BUFF_EN Reset                 */
#define GENERIC_PKT_BUFF_CFG0_ROLLING_BUFF_EN_Addr                   0x200004f4UL                                                 /*!< GENERIC_PKT BUFF_CFG0: ROLLING_BUFF_EN Address               */
#define GENERIC_PKT_BUFF_CFG0_ROLLING_BUFF_EN_DISABLED_Eval          0
#define GENERIC_PKT_BUFF_CFG0_ROLLING_BUFF_EN_ENABLED_Eval           1

/* ----------------------------  GENERIC_PKT_BUFF_CFG1  ---------------------------- */
#define GENERIC_PKT_BUFF_CFG1_Addr                                   0x200004F8UL                                                 /*!< GENERIC_PKT BUFF_CFG1: Address                               */
#define pGENERIC_PKT_BUFF_CFG1                                       (*(volatile uint32_t *) GENERIC_PKT_BUFF_CFG1_Addr)          /*!< GENERIC_PKT BUFF_CFG1: Pointer                               */
#define GENERIC_PKT_BUFF_CFG1_Msk                                    0xABFFFFFFUL                                                 /*!< GENERIC_PKT BUFF_CFG1: Mask                                  */
#define GENERIC_PKT_BUFF_CFG1_Rst                                    0x00020100UL                                                 /*!< GENERIC_PKT BUFF_CFG1: Reset                                 */

#define GENERIC_PKT_BUFF_CFG1_RX_SIZE_Size                           9                                                            /*!< GENERIC_PKT BUFF_CFG1: RX_SIZE Size                          */
#define GENERIC_PKT_BUFF_CFG1_RX_SIZE_Pos                            0                                                            /*!< GENERIC_PKT BUFF_CFG1: RX_SIZE Position                      */
#define GENERIC_PKT_BUFF_CFG1_RX_SIZE_Msk                            (0x01ffUL << GENERIC_PKT_BUFF_CFG1_RX_SIZE_Pos)              /*!< GENERIC_PKT BUFF_CFG1: RX_SIZE Mask                          */
#define GENERIC_PKT_BUFF_CFG1_RX_SIZE_Rst                            0x0100UL                                                     /*!< GENERIC_PKT BUFF_CFG1: RX_SIZE Reset                         */
#define GENERIC_PKT_BUFF_CFG1_RX_SIZE_Addr                           0x200004f8UL                                                 /*!< GENERIC_PKT BUFF_CFG1: RX_SIZE Address                       */

#define GENERIC_PKT_BUFF_CFG1_TX_SIZE_Size                           9                                                            /*!< GENERIC_PKT BUFF_CFG1: TX_SIZE Size                          */
#define GENERIC_PKT_BUFF_CFG1_TX_SIZE_Pos                            9                                                            /*!< GENERIC_PKT BUFF_CFG1: TX_SIZE Position                      */
#define GENERIC_PKT_BUFF_CFG1_TX_SIZE_Msk                            (0x01ffUL << GENERIC_PKT_BUFF_CFG1_TX_SIZE_Pos)              /*!< GENERIC_PKT BUFF_CFG1: TX_SIZE Mask                          */
#define GENERIC_PKT_BUFF_CFG1_TX_SIZE_Rst                            0x0100UL                                                     /*!< GENERIC_PKT BUFF_CFG1: TX_SIZE Reset                         */
#define GENERIC_PKT_BUFF_CFG1_TX_SIZE_Addr                           0x200004f8UL                                                 /*!< GENERIC_PKT BUFF_CFG1: TX_SIZE Address                       */

#define GENERIC_PKT_BUFF_CFG1_TRX_BLOCK_SIZE_Size                    8                                                            /*!< GENERIC_PKT BUFF_CFG1: TRX_BLOCK_SIZE Size                   */
#define GENERIC_PKT_BUFF_CFG1_TRX_BLOCK_SIZE_Pos                     18                                                           /*!< GENERIC_PKT BUFF_CFG1: TRX_BLOCK_SIZE Position               */
#define GENERIC_PKT_BUFF_CFG1_TRX_BLOCK_SIZE_Msk                     (0x00ffUL << GENERIC_PKT_BUFF_CFG1_TRX_BLOCK_SIZE_Pos)       /*!< GENERIC_PKT BUFF_CFG1: TRX_BLOCK_SIZE Mask                   */
#define GENERIC_PKT_BUFF_CFG1_TRX_BLOCK_SIZE_Rst                     0x0000UL                                                     /*!< GENERIC_PKT BUFF_CFG1: TRX_BLOCK_SIZE Reset                  */
#define GENERIC_PKT_BUFF_CFG1_TRX_BLOCK_SIZE_Addr                    0x200004f8UL                                                 /*!< GENERIC_PKT BUFF_CFG1: TRX_BLOCK_SIZE Address                */

#define GENERIC_PKT_BUFF_CFG1_TX_BUFF_RAWDATA_Size                   1                                                            /*!< GENERIC_PKT BUFF_CFG1: TX_BUFF_RAWDATA Size                  */
#define GENERIC_PKT_BUFF_CFG1_TX_BUFF_RAWDATA_Pos                    27                                                           /*!< GENERIC_PKT BUFF_CFG1: TX_BUFF_RAWDATA Position              */
#define GENERIC_PKT_BUFF_CFG1_TX_BUFF_RAWDATA_Msk                    (0x0001UL << GENERIC_PKT_BUFF_CFG1_TX_BUFF_RAWDATA_Pos)      /*!< GENERIC_PKT BUFF_CFG1: TX_BUFF_RAWDATA Mask                  */
#define GENERIC_PKT_BUFF_CFG1_TX_BUFF_RAWDATA_Rst                    0x0000UL                                                     /*!< GENERIC_PKT BUFF_CFG1: TX_BUFF_RAWDATA Reset                 */
#define GENERIC_PKT_BUFF_CFG1_TX_BUFF_RAWDATA_Addr                   0x200004f8UL                                                 /*!< GENERIC_PKT BUFF_CFG1: TX_BUFF_RAWDATA Address               */
#define GENERIC_PKT_BUFF_CFG1_TX_BUFF_RAWDATA_DISABLED_Eval          0
#define GENERIC_PKT_BUFF_CFG1_TX_BUFF_RAWDATA_ENABLED_Eval           1

#define GENERIC_PKT_BUFF_CFG1_TURNAROUND_RX_Size                     1                                                            /*!< GENERIC_PKT BUFF_CFG1: TURNAROUND_RX Size                    */
#define GENERIC_PKT_BUFF_CFG1_TURNAROUND_RX_Pos                      29                                                           /*!< GENERIC_PKT BUFF_CFG1: TURNAROUND_RX Position                */
#define GENERIC_PKT_BUFF_CFG1_TURNAROUND_RX_Msk                      (0x0001UL << GENERIC_PKT_BUFF_CFG1_TURNAROUND_RX_Pos)        /*!< GENERIC_PKT BUFF_CFG1: TURNAROUND_RX Mask                    */
#define GENERIC_PKT_BUFF_CFG1_TURNAROUND_RX_Rst                      0x0000UL                                                     /*!< GENERIC_PKT BUFF_CFG1: TURNAROUND_RX Reset                   */
#define GENERIC_PKT_BUFF_CFG1_TURNAROUND_RX_Addr                     0x200004f8UL                                                 /*!< GENERIC_PKT BUFF_CFG1: TURNAROUND_RX Address                 */
#define GENERIC_PKT_BUFF_CFG1_TURNAROUND_RX_DISABLE_Eval             0
#define GENERIC_PKT_BUFF_CFG1_TURNAROUND_RX_ENABLE_Eval              1

#define GENERIC_PKT_BUFF_CFG1_TURNAROUND_TX_Size                     1                                                            /*!< GENERIC_PKT BUFF_CFG1: TURNAROUND_TX Size                    */
#define GENERIC_PKT_BUFF_CFG1_TURNAROUND_TX_Pos                      31                                                           /*!< GENERIC_PKT BUFF_CFG1: TURNAROUND_TX Position                */
#define GENERIC_PKT_BUFF_CFG1_TURNAROUND_TX_Msk                      (0x0001UL << GENERIC_PKT_BUFF_CFG1_TURNAROUND_TX_Pos)        /*!< GENERIC_PKT BUFF_CFG1: TURNAROUND_TX Mask                    */
#define GENERIC_PKT_BUFF_CFG1_TURNAROUND_TX_Rst                      0x0000UL                                                     /*!< GENERIC_PKT BUFF_CFG1: TURNAROUND_TX Reset                   */
#define GENERIC_PKT_BUFF_CFG1_TURNAROUND_TX_Addr                     0x200004f8UL                                                 /*!< GENERIC_PKT BUFF_CFG1: TURNAROUND_TX Address                 */
#define GENERIC_PKT_BUFF_CFG1_TURNAROUND_TX_DISABLE_Eval             0
#define GENERIC_PKT_BUFF_CFG1_TURNAROUND_TX_ENABLE_Eval              1

/* ----------------------------  GENERIC_PKT_FRAME_CFG0  ---------------------------- */
#define GENERIC_PKT_FRAME_CFG0_Addr                                  0x200004FCUL                                                 /*!< GENERIC_PKT FRAME_CFG0: Address                              */
#define pGENERIC_PKT_FRAME_CFG0                                      (*(volatile uint32_t *) GENERIC_PKT_FRAME_CFG0_Addr)         /*!< GENERIC_PKT FRAME_CFG0: Pointer                              */
#define GENERIC_PKT_FRAME_CFG0_Msk                                   0x3F3F00FFUL                                                 /*!< GENERIC_PKT FRAME_CFG0: Mask                                 */
#define GENERIC_PKT_FRAME_CFG0_Rst                                   0x10000000UL                                                 /*!< GENERIC_PKT FRAME_CFG0: Reset                                */

#define GENERIC_PKT_FRAME_CFG0_PREAMBLE_LEN_Size                     8                                                            /*!< GENERIC_PKT FRAME_CFG0: PREAMBLE_LEN Size                    */
#define GENERIC_PKT_FRAME_CFG0_PREAMBLE_LEN_Pos                      0                                                            /*!< GENERIC_PKT FRAME_CFG0: PREAMBLE_LEN Position                */
#define GENERIC_PKT_FRAME_CFG0_PREAMBLE_LEN_Msk                      (0x00ffUL << GENERIC_PKT_FRAME_CFG0_PREAMBLE_LEN_Pos)        /*!< GENERIC_PKT FRAME_CFG0: PREAMBLE_LEN Mask                    */
#define GENERIC_PKT_FRAME_CFG0_PREAMBLE_LEN_Rst                      0x0000UL                                                     /*!< GENERIC_PKT FRAME_CFG0: PREAMBLE_LEN Reset                   */
#define GENERIC_PKT_FRAME_CFG0_PREAMBLE_LEN_Addr                     0x200004fcUL                                                 /*!< GENERIC_PKT FRAME_CFG0: PREAMBLE_LEN Address                 */

#define GENERIC_PKT_FRAME_CFG0_SYNC0_LEN_Size                        6                                                            /*!< GENERIC_PKT FRAME_CFG0: SYNC0_LEN Size                       */
#define GENERIC_PKT_FRAME_CFG0_SYNC0_LEN_Pos                         16                                                           /*!< GENERIC_PKT FRAME_CFG0: SYNC0_LEN Position                   */
#define GENERIC_PKT_FRAME_CFG0_SYNC0_LEN_Msk                         (0x003fUL << GENERIC_PKT_FRAME_CFG0_SYNC0_LEN_Pos)           /*!< GENERIC_PKT FRAME_CFG0: SYNC0_LEN Mask                       */
#define GENERIC_PKT_FRAME_CFG0_SYNC0_LEN_Rst                         0x0000UL                                                     /*!< GENERIC_PKT FRAME_CFG0: SYNC0_LEN Reset                      */
#define GENERIC_PKT_FRAME_CFG0_SYNC0_LEN_Addr                        0x200004fcUL                                                 /*!< GENERIC_PKT FRAME_CFG0: SYNC0_LEN Address                    */

#define GENERIC_PKT_FRAME_CFG0_CRC_LEN_Size                          6                                                            /*!< GENERIC_PKT FRAME_CFG0: CRC_LEN Size                         */
#define GENERIC_PKT_FRAME_CFG0_CRC_LEN_Pos                           24                                                           /*!< GENERIC_PKT FRAME_CFG0: CRC_LEN Position                     */
#define GENERIC_PKT_FRAME_CFG0_CRC_LEN_Msk                           (0x003fUL << GENERIC_PKT_FRAME_CFG0_CRC_LEN_Pos)             /*!< GENERIC_PKT FRAME_CFG0: CRC_LEN Mask                         */
#define GENERIC_PKT_FRAME_CFG0_CRC_LEN_Rst                           0x0010UL                                                     /*!< GENERIC_PKT FRAME_CFG0: CRC_LEN Reset                        */
#define GENERIC_PKT_FRAME_CFG0_CRC_LEN_Addr                          0x200004fcUL                                                 /*!< GENERIC_PKT FRAME_CFG0: CRC_LEN Address                      */

/* ----------------------------  GENERIC_PKT_FRAME_CFG1  ---------------------------- */
#define GENERIC_PKT_FRAME_CFG1_Addr                                  0x20000500UL                                                 /*!< GENERIC_PKT FRAME_CFG1: Address                              */
#define pGENERIC_PKT_FRAME_CFG1                                      (*(volatile uint32_t *) GENERIC_PKT_FRAME_CFG1_Addr)         /*!< GENERIC_PKT FRAME_CFG1: Pointer                              */
#define GENERIC_PKT_FRAME_CFG1_Msk                                   0xFFFF1FFFUL                                                 /*!< GENERIC_PKT FRAME_CFG1: Mask                                 */
#define GENERIC_PKT_FRAME_CFG1_Rst                                   0x00000000UL                                                 /*!< GENERIC_PKT FRAME_CFG1: Reset                                */

#define GENERIC_PKT_FRAME_CFG1_PAYLOAD_SIZE_Size                     12                                                           /*!< GENERIC_PKT FRAME_CFG1: PAYLOAD_SIZE Size                    */
#define GENERIC_PKT_FRAME_CFG1_PAYLOAD_SIZE_Pos                      0                                                            /*!< GENERIC_PKT FRAME_CFG1: PAYLOAD_SIZE Position                */
#define GENERIC_PKT_FRAME_CFG1_PAYLOAD_SIZE_Msk                      (0x0fffUL << GENERIC_PKT_FRAME_CFG1_PAYLOAD_SIZE_Pos)        /*!< GENERIC_PKT FRAME_CFG1: PAYLOAD_SIZE Mask                    */
#define GENERIC_PKT_FRAME_CFG1_PAYLOAD_SIZE_Rst                      0x0000UL                                                     /*!< GENERIC_PKT FRAME_CFG1: PAYLOAD_SIZE Reset                   */
#define GENERIC_PKT_FRAME_CFG1_PAYLOAD_SIZE_Addr                     0x20000500UL                                                 /*!< GENERIC_PKT FRAME_CFG1: PAYLOAD_SIZE Address                 */

#define GENERIC_PKT_FRAME_CFG1_PREAMBLE_UNIT_Size                    1                                                            /*!< GENERIC_PKT FRAME_CFG1: PREAMBLE_UNIT Size                   */
#define GENERIC_PKT_FRAME_CFG1_PREAMBLE_UNIT_Pos                     12                                                           /*!< GENERIC_PKT FRAME_CFG1: PREAMBLE_UNIT Position               */
#define GENERIC_PKT_FRAME_CFG1_PREAMBLE_UNIT_Msk                     (0x0001UL << GENERIC_PKT_FRAME_CFG1_PREAMBLE_UNIT_Pos)       /*!< GENERIC_PKT FRAME_CFG1: PREAMBLE_UNIT Mask                   */
#define GENERIC_PKT_FRAME_CFG1_PREAMBLE_UNIT_Rst                     0x0000UL                                                     /*!< GENERIC_PKT FRAME_CFG1: PREAMBLE_UNIT Reset                  */
#define GENERIC_PKT_FRAME_CFG1_PREAMBLE_UNIT_Addr                    0x20000500UL                                                 /*!< GENERIC_PKT FRAME_CFG1: PREAMBLE_UNIT Address                */
#define GENERIC_PKT_FRAME_CFG1_PREAMBLE_UNIT_BIT_PAIR_Eval           0
#define GENERIC_PKT_FRAME_CFG1_PREAMBLE_UNIT_BYTE_Eval               1

#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_Size                    8                                                            /*!< GENERIC_PKT FRAME_CFG1: TRX_IRQ0_TYPE Size                   */
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_Pos                     16                                                           /*!< GENERIC_PKT FRAME_CFG1: TRX_IRQ0_TYPE Position               */
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_Msk                     (0x00ffUL << GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_Pos)       /*!< GENERIC_PKT FRAME_CFG1: TRX_IRQ0_TYPE Mask                   */
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_Rst                     0x0000UL                                                     /*!< GENERIC_PKT FRAME_CFG1: TRX_IRQ0_TYPE Reset                  */
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_Addr                    0x20000500UL                                                 /*!< GENERIC_PKT FRAME_CFG1: TRX_IRQ0_TYPE Address                */
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_NONE_Eval               0
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_PREAMBLE_DETECT_Eval    1
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_PREAMBLE_GONE_Eval      2
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_SYNC_DETECT_Eval        4
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_LEN_PATTERN_Eval        8
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_PAYLOAD_Eval            16
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_PAYLOAD_BLOC_Eval       32
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_CRC_Eval                64
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_EOF_Eval                128
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ0_TYPE_ALL_Eval                255

#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_Size                    8                                                            /*!< GENERIC_PKT FRAME_CFG1: TRX_IRQ1_TYPE Size                   */
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_Pos                     24                                                           /*!< GENERIC_PKT FRAME_CFG1: TRX_IRQ1_TYPE Position               */
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_Msk                     (0x00ffUL << GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_Pos)       /*!< GENERIC_PKT FRAME_CFG1: TRX_IRQ1_TYPE Mask                   */
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_Rst                     0x0000UL                                                     /*!< GENERIC_PKT FRAME_CFG1: TRX_IRQ1_TYPE Reset                  */
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_Addr                    0x20000500UL                                                 /*!< GENERIC_PKT FRAME_CFG1: TRX_IRQ1_TYPE Address                */
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_NONE_Eval               0
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_PREAMBLE_DETECT_Eval    1
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_PREAMBLE_GONE_Eval      2
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_SYNC_DETECT_Eval        4
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_LEN_PATTERN_Eval        8
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_PAYLOAD_Eval            16
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_PAYLOAD_BLOC_Eval       32
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_CRC_Eval                64
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_EOF_Eval                128
#define GENERIC_PKT_FRAME_CFG1_TRX_IRQ1_TYPE_ALL_Eval                255

/* ----------------------------  GENERIC_PKT_FRAME_CFG2  ---------------------------- */
#define GENERIC_PKT_FRAME_CFG2_Addr                                  0x20000504UL                                                 /*!< GENERIC_PKT FRAME_CFG2: Address                              */
#define pGENERIC_PKT_FRAME_CFG2                                      (*(volatile uint32_t *) GENERIC_PKT_FRAME_CFG2_Addr)         /*!< GENERIC_PKT FRAME_CFG2: Pointer                              */
#define GENERIC_PKT_FRAME_CFG2_Msk                                   0xFFFF39F8UL                                                 /*!< GENERIC_PKT FRAME_CFG2: Mask                                 */
#define GENERIC_PKT_FRAME_CFG2_Rst                                   0x00001800UL                                                 /*!< GENERIC_PKT FRAME_CFG2: Reset                                */

#define GENERIC_PKT_FRAME_CFG2_SYNC1_LEN_Size                        6                                                            /*!< GENERIC_PKT FRAME_CFG2: SYNC1_LEN Size                       */
#define GENERIC_PKT_FRAME_CFG2_SYNC1_LEN_Pos                         3                                                            /*!< GENERIC_PKT FRAME_CFG2: SYNC1_LEN Position                   */
#define GENERIC_PKT_FRAME_CFG2_SYNC1_LEN_Msk                         (0x003fUL << GENERIC_PKT_FRAME_CFG2_SYNC1_LEN_Pos)           /*!< GENERIC_PKT FRAME_CFG2: SYNC1_LEN Mask                       */
#define GENERIC_PKT_FRAME_CFG2_SYNC1_LEN_Rst                         0x0000UL                                                     /*!< GENERIC_PKT FRAME_CFG2: SYNC1_LEN Reset                      */
#define GENERIC_PKT_FRAME_CFG2_SYNC1_LEN_Addr                        0x20000504UL                                                 /*!< GENERIC_PKT FRAME_CFG2: SYNC1_LEN Address                    */

#define GENERIC_PKT_FRAME_CFG2_CRC_SHIFT_IN_ZEROS_Size               1                                                            /*!< GENERIC_PKT FRAME_CFG2: CRC_SHIFT_IN_ZEROS Size              */
#define GENERIC_PKT_FRAME_CFG2_CRC_SHIFT_IN_ZEROS_Pos                11                                                           /*!< GENERIC_PKT FRAME_CFG2: CRC_SHIFT_IN_ZEROS Position          */
#define GENERIC_PKT_FRAME_CFG2_CRC_SHIFT_IN_ZEROS_Msk                (0x0001UL << GENERIC_PKT_FRAME_CFG2_CRC_SHIFT_IN_ZEROS_Pos)  /*!< GENERIC_PKT FRAME_CFG2: CRC_SHIFT_IN_ZEROS Mask              */
#define GENERIC_PKT_FRAME_CFG2_CRC_SHIFT_IN_ZEROS_Rst                0x0001UL                                                     /*!< GENERIC_PKT FRAME_CFG2: CRC_SHIFT_IN_ZEROS Reset             */
#define GENERIC_PKT_FRAME_CFG2_CRC_SHIFT_IN_ZEROS_Addr               0x20000504UL                                                 /*!< GENERIC_PKT FRAME_CFG2: CRC_SHIFT_IN_ZEROS Address           */
#define GENERIC_PKT_FRAME_CFG2_CRC_SHIFT_IN_ZEROS_DISABLE_Eval       0
#define GENERIC_PKT_FRAME_CFG2_CRC_SHIFT_IN_ZEROS_ENABLE_Eval        1

#define GENERIC_PKT_FRAME_CFG2_LEN_SEL_Size                          2                                                            /*!< GENERIC_PKT FRAME_CFG2: LEN_SEL Size                         */
#define GENERIC_PKT_FRAME_CFG2_LEN_SEL_Pos                           12                                                           /*!< GENERIC_PKT FRAME_CFG2: LEN_SEL Position                     */
#define GENERIC_PKT_FRAME_CFG2_LEN_SEL_Msk                           (0x0003UL << GENERIC_PKT_FRAME_CFG2_LEN_SEL_Pos)             /*!< GENERIC_PKT FRAME_CFG2: LEN_SEL Mask                         */
#define GENERIC_PKT_FRAME_CFG2_LEN_SEL_Rst                           0x0001UL                                                     /*!< GENERIC_PKT FRAME_CFG2: LEN_SEL Reset                        */
#define GENERIC_PKT_FRAME_CFG2_LEN_SEL_Addr                          0x20000504UL                                                 /*!< GENERIC_PKT FRAME_CFG2: LEN_SEL Address                      */
#define GENERIC_PKT_FRAME_CFG2_LEN_SEL_LEN_0_Eval                    0
#define GENERIC_PKT_FRAME_CFG2_LEN_SEL_LEN_8_Eval                    1
#define GENERIC_PKT_FRAME_CFG2_LEN_SEL_LEN_16_Eval                   2

#define GENERIC_PKT_FRAME_CFG2_PREAMBLE_VAL_Size                     8                                                            /*!< GENERIC_PKT FRAME_CFG2: PREAMBLE_VAL Size                    */
#define GENERIC_PKT_FRAME_CFG2_PREAMBLE_VAL_Pos                      16                                                           /*!< GENERIC_PKT FRAME_CFG2: PREAMBLE_VAL Position                */
#define GENERIC_PKT_FRAME_CFG2_PREAMBLE_VAL_Msk                      (0x00ffUL << GENERIC_PKT_FRAME_CFG2_PREAMBLE_VAL_Pos)        /*!< GENERIC_PKT FRAME_CFG2: PREAMBLE_VAL Mask                    */
#define GENERIC_PKT_FRAME_CFG2_PREAMBLE_VAL_Rst                      0x0000UL                                                     /*!< GENERIC_PKT FRAME_CFG2: PREAMBLE_VAL Reset                   */
#define GENERIC_PKT_FRAME_CFG2_PREAMBLE_VAL_Addr                     0x20000504UL                                                 /*!< GENERIC_PKT FRAME_CFG2: PREAMBLE_VAL Address                 */

#define GENERIC_PKT_FRAME_CFG2_ENDEC_MODE_Size                       8                                                            /*!< GENERIC_PKT FRAME_CFG2: ENDEC_MODE Size                      */
#define GENERIC_PKT_FRAME_CFG2_ENDEC_MODE_Pos                        24                                                           /*!< GENERIC_PKT FRAME_CFG2: ENDEC_MODE Position                  */
#define GENERIC_PKT_FRAME_CFG2_ENDEC_MODE_Msk                        (0x00ffUL << GENERIC_PKT_FRAME_CFG2_ENDEC_MODE_Pos)          /*!< GENERIC_PKT FRAME_CFG2: ENDEC_MODE Mask                      */
#define GENERIC_PKT_FRAME_CFG2_ENDEC_MODE_Rst                        0x0000UL                                                     /*!< GENERIC_PKT FRAME_CFG2: ENDEC_MODE Reset                     */
#define GENERIC_PKT_FRAME_CFG2_ENDEC_MODE_Addr                       0x20000504UL                                                 /*!< GENERIC_PKT FRAME_CFG2: ENDEC_MODE Address                   */
#define GENERIC_PKT_FRAME_CFG2_ENDEC_MODE_CODE_NRZ_Eval              1
#define GENERIC_PKT_FRAME_CFG2_ENDEC_MODE_CODE_MANCHESTER_Eval       4
#define GENERIC_PKT_FRAME_CFG2_ENDEC_MODE_CODE_INVERT_Eval           128

/* ----------------------------  GENERIC_PKT_FRAME_CFG3  ---------------------------- */
#define GENERIC_PKT_FRAME_CFG3_Addr                                  0x20000508UL                                                 /*!< GENERIC_PKT FRAME_CFG3: Address                              */
#define pGENERIC_PKT_FRAME_CFG3                                      (*(volatile uint32_t *) GENERIC_PKT_FRAME_CFG3_Addr)         /*!< GENERIC_PKT FRAME_CFG3: Pointer                              */
#define GENERIC_PKT_FRAME_CFG3_Msk                                   0xFFFF0000UL                                                 /*!< GENERIC_PKT FRAME_CFG3: Mask                                 */
#define GENERIC_PKT_FRAME_CFG3_Rst                                   0x00000000UL                                                 /*!< GENERIC_PKT FRAME_CFG3: Reset                                */

#define GENERIC_PKT_FRAME_CFG3_RX_LENGTH_Size                        16                                                           /*!< GENERIC_PKT FRAME_CFG3: RX_LENGTH Size                       */
#define GENERIC_PKT_FRAME_CFG3_RX_LENGTH_Pos                         16                                                           /*!< GENERIC_PKT FRAME_CFG3: RX_LENGTH Position                   */
#define GENERIC_PKT_FRAME_CFG3_RX_LENGTH_Msk                         (0xffffUL << GENERIC_PKT_FRAME_CFG3_RX_LENGTH_Pos)           /*!< GENERIC_PKT FRAME_CFG3: RX_LENGTH Mask                       */
#define GENERIC_PKT_FRAME_CFG3_RX_LENGTH_Rst                         0x0000UL                                                     /*!< GENERIC_PKT FRAME_CFG3: RX_LENGTH Reset                      */
#define GENERIC_PKT_FRAME_CFG3_RX_LENGTH_Addr                        0x20000508UL                                                 /*!< GENERIC_PKT FRAME_CFG3: RX_LENGTH Address                    */

/* ----------------------------  GENERIC_PKT_FRAME_CFG5  ---------------------------- */
#define GENERIC_PKT_FRAME_CFG5_Addr                                  0x20000510UL                                                 /*!< GENERIC_PKT FRAME_CFG5: Address                              */
#define pGENERIC_PKT_FRAME_CFG5                                      (*(volatile uint32_t *) GENERIC_PKT_FRAME_CFG5_Addr)         /*!< GENERIC_PKT FRAME_CFG5: Pointer                              */
#define GENERIC_PKT_FRAME_CFG5_Msk                                   0x0000FFFFUL                                                 /*!< GENERIC_PKT FRAME_CFG5: Mask                                 */
#define GENERIC_PKT_FRAME_CFG5_Rst                                   0x00000000UL                                                 /*!< GENERIC_PKT FRAME_CFG5: Reset                                */

#define GENERIC_PKT_FRAME_CFG5_TX_PHR_Size                           16                                                           /*!< GENERIC_PKT FRAME_CFG5: TX_PHR Size                          */
#define GENERIC_PKT_FRAME_CFG5_TX_PHR_Pos                            0                                                            /*!< GENERIC_PKT FRAME_CFG5: TX_PHR Position                      */
#define GENERIC_PKT_FRAME_CFG5_TX_PHR_Msk                            (0xffffUL << GENERIC_PKT_FRAME_CFG5_TX_PHR_Pos)              /*!< GENERIC_PKT FRAME_CFG5: TX_PHR Mask                          */
#define GENERIC_PKT_FRAME_CFG5_TX_PHR_Rst                            0x0000UL                                                     /*!< GENERIC_PKT FRAME_CFG5: TX_PHR Reset                         */
#define GENERIC_PKT_FRAME_CFG5_TX_PHR_Addr                           0x20000510UL                                                 /*!< GENERIC_PKT FRAME_CFG5: TX_PHR Address                       */

/* ----------------------------  GENERIC_PKT_SYNCWORD0  ---------------------------- */
#define GENERIC_PKT_SYNCWORD0_Addr                                   0x20000514UL                                                 /*!< GENERIC_PKT SYNCWORD0: Address                               */
#define pGENERIC_PKT_SYNCWORD0                                       (*(volatile uint32_t *) GENERIC_PKT_SYNCWORD0_Addr)          /*!< GENERIC_PKT SYNCWORD0: Pointer                               */
#define GENERIC_PKT_SYNCWORD0_Msk                                    0xFFFFFFFFUL                                                 /*!< GENERIC_PKT SYNCWORD0: Mask                                  */
#define GENERIC_PKT_SYNCWORD0_Rst                                    0x00000000UL                                                 /*!< GENERIC_PKT SYNCWORD0: Reset                                 */

#define GENERIC_PKT_SYNCWORD0_VAL_Size                               32                                                           /*!< GENERIC_PKT SYNCWORD0: VAL Size                              */
#define GENERIC_PKT_SYNCWORD0_VAL_Pos                                0                                                            /*!< GENERIC_PKT SYNCWORD0: VAL Position                          */
#define GENERIC_PKT_SYNCWORD0_VAL_Msk                                (0xffffffffUL << GENERIC_PKT_SYNCWORD0_VAL_Pos)              /*!< GENERIC_PKT SYNCWORD0: VAL Mask                              */
#define GENERIC_PKT_SYNCWORD0_VAL_Rst                                0x0000UL                                                     /*!< GENERIC_PKT SYNCWORD0: VAL Reset                             */
#define GENERIC_PKT_SYNCWORD0_VAL_Addr                               0x20000514UL                                                 /*!< GENERIC_PKT SYNCWORD0: VAL Address                           */

/* ----------------------------  GENERIC_PKT_SYNCWORD1  ---------------------------- */
#define GENERIC_PKT_SYNCWORD1_Addr                                   0x20000518UL                                                 /*!< GENERIC_PKT SYNCWORD1: Address                               */
#define pGENERIC_PKT_SYNCWORD1                                       (*(volatile uint32_t *) GENERIC_PKT_SYNCWORD1_Addr)          /*!< GENERIC_PKT SYNCWORD1: Pointer                               */
#define GENERIC_PKT_SYNCWORD1_Msk                                    0xFFFFFFFFUL                                                 /*!< GENERIC_PKT SYNCWORD1: Mask                                  */
#define GENERIC_PKT_SYNCWORD1_Rst                                    0x00000000UL                                                 /*!< GENERIC_PKT SYNCWORD1: Reset                                 */

#define GENERIC_PKT_SYNCWORD1_VAL_Size                               32                                                           /*!< GENERIC_PKT SYNCWORD1: VAL Size                              */
#define GENERIC_PKT_SYNCWORD1_VAL_Pos                                0                                                            /*!< GENERIC_PKT SYNCWORD1: VAL Position                          */
#define GENERIC_PKT_SYNCWORD1_VAL_Msk                                (0xffffffffUL << GENERIC_PKT_SYNCWORD1_VAL_Pos)              /*!< GENERIC_PKT SYNCWORD1: VAL Mask                              */
#define GENERIC_PKT_SYNCWORD1_VAL_Rst                                0x0000UL                                                     /*!< GENERIC_PKT SYNCWORD1: VAL Reset                             */
#define GENERIC_PKT_SYNCWORD1_VAL_Addr                               0x20000518UL                                                 /*!< GENERIC_PKT SYNCWORD1: VAL Address                           */

/* ----------------------------  GENERIC_PKT_CRC_POLY  ---------------------------- */
#define GENERIC_PKT_CRC_POLY_Addr                                    0x2000051CUL                                                 /*!< GENERIC_PKT CRC_POLY: Address                                */
#define pGENERIC_PKT_CRC_POLY                                        (*(volatile uint32_t *) GENERIC_PKT_CRC_POLY_Addr)           /*!< GENERIC_PKT CRC_POLY: Pointer                                */
#define GENERIC_PKT_CRC_POLY_Msk                                     0xFFFFFFFFUL                                                 /*!< GENERIC_PKT CRC_POLY: Mask                                   */
#define GENERIC_PKT_CRC_POLY_Rst                                     0x00000000UL                                                 /*!< GENERIC_PKT CRC_POLY: Reset                                  */

#define GENERIC_PKT_CRC_POLY_VAL_Size                                32                                                           /*!< GENERIC_PKT CRC_POLY: VAL Size                               */
#define GENERIC_PKT_CRC_POLY_VAL_Pos                                 0                                                            /*!< GENERIC_PKT CRC_POLY: VAL Position                           */
#define GENERIC_PKT_CRC_POLY_VAL_Msk                                 (0xffffffffUL << GENERIC_PKT_CRC_POLY_VAL_Pos)               /*!< GENERIC_PKT CRC_POLY: VAL Mask                               */
#define GENERIC_PKT_CRC_POLY_VAL_Rst                                 0x0000UL                                                     /*!< GENERIC_PKT CRC_POLY: VAL Reset                              */
#define GENERIC_PKT_CRC_POLY_VAL_Addr                                0x2000051cUL                                                 /*!< GENERIC_PKT CRC_POLY: VAL Address                            */

/* ----------------------------  GENERIC_PKT_CRC_SEED  ---------------------------- */
#define GENERIC_PKT_CRC_SEED_Addr                                    0x20000520UL                                                 /*!< GENERIC_PKT CRC_SEED: Address                                */
#define pGENERIC_PKT_CRC_SEED                                        (*(volatile uint32_t *) GENERIC_PKT_CRC_SEED_Addr)           /*!< GENERIC_PKT CRC_SEED: Pointer                                */
#define GENERIC_PKT_CRC_SEED_Msk                                     0xFFFFFFFFUL                                                 /*!< GENERIC_PKT CRC_SEED: Mask                                   */
#define GENERIC_PKT_CRC_SEED_Rst                                     0x00000000UL                                                 /*!< GENERIC_PKT CRC_SEED: Reset                                  */

#define GENERIC_PKT_CRC_SEED_VAL_Size                                32                                                           /*!< GENERIC_PKT CRC_SEED: VAL Size                               */
#define GENERIC_PKT_CRC_SEED_VAL_Pos                                 0                                                            /*!< GENERIC_PKT CRC_SEED: VAL Position                           */
#define GENERIC_PKT_CRC_SEED_VAL_Msk                                 (0xffffffffUL << GENERIC_PKT_CRC_SEED_VAL_Pos)               /*!< GENERIC_PKT CRC_SEED: VAL Mask                               */
#define GENERIC_PKT_CRC_SEED_VAL_Rst                                 0x0000UL                                                     /*!< GENERIC_PKT CRC_SEED: VAL Reset                              */
#define GENERIC_PKT_CRC_SEED_VAL_Addr                                0x20000520UL                                                 /*!< GENERIC_PKT CRC_SEED: VAL Address                            */

/* ----------------------------  GENERIC_PKT_CRC_FINAL_XOR  ---------------------------- */
#define GENERIC_PKT_CRC_FINAL_XOR_Addr                               0x20000524UL                                                 /*!< GENERIC_PKT CRC_FINAL_XOR: Address                           */
#define pGENERIC_PKT_CRC_FINAL_XOR                                   (*(volatile uint32_t *) GENERIC_PKT_CRC_FINAL_XOR_Addr)      /*!< GENERIC_PKT CRC_FINAL_XOR: Pointer                           */
#define GENERIC_PKT_CRC_FINAL_XOR_Msk                                0xFFFFFFFFUL                                                 /*!< GENERIC_PKT CRC_FINAL_XOR: Mask                              */
#define GENERIC_PKT_CRC_FINAL_XOR_Rst                                0xFFFFFFFFUL                                                 /*!< GENERIC_PKT CRC_FINAL_XOR: Reset                             */

#define GENERIC_PKT_CRC_FINAL_XOR_VAL_Size                           32                                                           /*!< GENERIC_PKT CRC_FINAL_XOR: VAL Size                          */
#define GENERIC_PKT_CRC_FINAL_XOR_VAL_Pos                            0                                                            /*!< GENERIC_PKT CRC_FINAL_XOR: VAL Position                      */
#define GENERIC_PKT_CRC_FINAL_XOR_VAL_Msk                            (0xffffffffUL << GENERIC_PKT_CRC_FINAL_XOR_VAL_Pos)          /*!< GENERIC_PKT CRC_FINAL_XOR: VAL Mask                          */
#define GENERIC_PKT_CRC_FINAL_XOR_VAL_Rst                            0xffffffffUL                                                 /*!< GENERIC_PKT CRC_FINAL_XOR: VAL Reset                         */
#define GENERIC_PKT_CRC_FINAL_XOR_VAL_Addr                           0x20000524UL                                                 /*!< GENERIC_PKT CRC_FINAL_XOR: VAL Address                       */

/* ----------------------------  GENERIC_PKT_LIVE_LINK_QUAL  ---------------------------- */
#define GENERIC_PKT_LIVE_LINK_QUAL_Addr                              0x20000538UL                                                 /*!< GENERIC_PKT LIVE_LINK_QUAL: Address                          */
#define pGENERIC_PKT_LIVE_LINK_QUAL                                  (*(volatile uint32_t *) GENERIC_PKT_LIVE_LINK_QUAL_Addr)     /*!< GENERIC_PKT LIVE_LINK_QUAL: Pointer                          */
#define GENERIC_PKT_LIVE_LINK_QUAL_Msk                               0x07FF0000UL                                                 /*!< GENERIC_PKT LIVE_LINK_QUAL: Mask                             */
#define GENERIC_PKT_LIVE_LINK_QUAL_Rst                               0x076A0000UL                                                 /*!< GENERIC_PKT LIVE_LINK_QUAL: Reset                            */

#define GENERIC_PKT_LIVE_LINK_QUAL_RSSI_Size                         11                                                           /*!< GENERIC_PKT LIVE_LINK_QUAL: RSSI Size                        */
#define GENERIC_PKT_LIVE_LINK_QUAL_RSSI_Pos                          16                                                           /*!< GENERIC_PKT LIVE_LINK_QUAL: RSSI Position                    */
#define GENERIC_PKT_LIVE_LINK_QUAL_RSSI_Msk                          (0x07ffUL << GENERIC_PKT_LIVE_LINK_QUAL_RSSI_Pos)            /*!< GENERIC_PKT LIVE_LINK_QUAL: RSSI Mask                        */
#define GENERIC_PKT_LIVE_LINK_QUAL_RSSI_Rst                          0x076aUL                                                     /*!< GENERIC_PKT LIVE_LINK_QUAL: RSSI Reset                       */
#define GENERIC_PKT_LIVE_LINK_QUAL_RSSI_Addr                         0x20000538UL                                                 /*!< GENERIC_PKT LIVE_LINK_QUAL: RSSI Address                     */

/* ----------------------------  GENERIC_PKT_MISC0  ---------------------------- */
#define GENERIC_PKT_MISC0_Addr                                       0x2000053CUL                                                 /*!< GENERIC_PKT MISC0: Address                                   */
#define pGENERIC_PKT_MISC0                                           (*(volatile uint32_t *) GENERIC_PKT_MISC0_Addr)              /*!< GENERIC_PKT MISC0: Pointer                                   */
#define GENERIC_PKT_MISC0_Msk                                        0xFFFFFFFFUL                                                 /*!< GENERIC_PKT MISC0: Mask                                      */
#define GENERIC_PKT_MISC0_Rst                                        0x00000000UL                                                 /*!< GENERIC_PKT MISC0: Reset                                     */

#define GENERIC_PKT_MISC0_UNUSED_Size                                32                                                           /*!< GENERIC_PKT MISC0: UNUSED Size                               */
#define GENERIC_PKT_MISC0_UNUSED_Pos                                 0                                                            /*!< GENERIC_PKT MISC0: UNUSED Position                           */
#define GENERIC_PKT_MISC0_UNUSED_Msk                                 (0xffffffffUL << GENERIC_PKT_MISC0_UNUSED_Pos)               /*!< GENERIC_PKT MISC0: UNUSED Mask                               */
#define GENERIC_PKT_MISC0_UNUSED_Rst                                 0x0000UL                                                     /*!< GENERIC_PKT MISC0: UNUSED Reset                              */
#define GENERIC_PKT_MISC0_UNUSED_Addr                                0x2000053cUL                                                 /*!< GENERIC_PKT MISC0: UNUSED Address                            */

/* ----------------------------  GENERIC_PKT_MISC1  ---------------------------- */
#define GENERIC_PKT_MISC1_Addr                                       0x20000540UL                                                 /*!< GENERIC_PKT MISC1: Address                                   */
#define pGENERIC_PKT_MISC1                                           (*(volatile uint32_t *) GENERIC_PKT_MISC1_Addr)              /*!< GENERIC_PKT MISC1: Pointer                                   */
#define GENERIC_PKT_MISC1_Msk                                        0xFFFFFFFFUL                                                 /*!< GENERIC_PKT MISC1: Mask                                      */
#define GENERIC_PKT_MISC1_Rst                                        0x00000000UL                                                 /*!< GENERIC_PKT MISC1: Reset                                     */

#define GENERIC_PKT_MISC1_UNUSED_Size                                32                                                           /*!< GENERIC_PKT MISC1: UNUSED Size                               */
#define GENERIC_PKT_MISC1_UNUSED_Pos                                 0                                                            /*!< GENERIC_PKT MISC1: UNUSED Position                           */
#define GENERIC_PKT_MISC1_UNUSED_Msk                                 (0xffffffffUL << GENERIC_PKT_MISC1_UNUSED_Pos)               /*!< GENERIC_PKT MISC1: UNUSED Mask                               */
#define GENERIC_PKT_MISC1_UNUSED_Rst                                 0x0000UL                                                     /*!< GENERIC_PKT MISC1: UNUSED Reset                              */
#define GENERIC_PKT_MISC1_UNUSED_Addr                                0x20000540UL                                                 /*!< GENERIC_PKT MISC1: UNUSED Address                            */

/* ----------------------------  GENERIC_PKT_LPM_CFG  ---------------------------- */
#define GENERIC_PKT_LPM_CFG_Addr                                     0x20000544UL                                                 /*!< GENERIC_PKT LPM_CFG: Address                                 */
#define pGENERIC_PKT_LPM_CFG                                         (*(volatile uint32_t *) GENERIC_PKT_LPM_CFG_Addr)            /*!< GENERIC_PKT LPM_CFG: Pointer                                 */
#define GENERIC_PKT_LPM_CFG_Msk                                      0xFFFFFF00UL                                                 /*!< GENERIC_PKT LPM_CFG: Mask                                    */
#define GENERIC_PKT_LPM_CFG_Rst                                      0x06066400UL                                                 /*!< GENERIC_PKT LPM_CFG: Reset                                   */

#define GENERIC_PKT_LPM_CFG_PREAMBLE_DWELL_TIME_Size                 8                                                            /*!< GENERIC_PKT LPM_CFG: PREAMBLE_DWELL_TIME Size                */
#define GENERIC_PKT_LPM_CFG_PREAMBLE_DWELL_TIME_Pos                  8                                                            /*!< GENERIC_PKT LPM_CFG: PREAMBLE_DWELL_TIME Position            */
#define GENERIC_PKT_LPM_CFG_PREAMBLE_DWELL_TIME_Msk                  (0x00ffUL << GENERIC_PKT_LPM_CFG_PREAMBLE_DWELL_TIME_Pos)    /*!< GENERIC_PKT LPM_CFG: PREAMBLE_DWELL_TIME Mask                */
#define GENERIC_PKT_LPM_CFG_PREAMBLE_DWELL_TIME_Rst                  0x0064UL                                                     /*!< GENERIC_PKT LPM_CFG: PREAMBLE_DWELL_TIME Reset               */
#define GENERIC_PKT_LPM_CFG_PREAMBLE_DWELL_TIME_Addr                 0x20000544UL                                                 /*!< GENERIC_PKT LPM_CFG: PREAMBLE_DWELL_TIME Address             */

#define GENERIC_PKT_LPM_CFG_PREAMBLE_QUAL_DWELL_TIME_Size            8                                                            /*!< GENERIC_PKT LPM_CFG: PREAMBLE_QUAL_DWELL_TIME Size           */
#define GENERIC_PKT_LPM_CFG_PREAMBLE_QUAL_DWELL_TIME_Pos             16                                                           /*!< GENERIC_PKT LPM_CFG: PREAMBLE_QUAL_DWELL_TIME Position       */
#define GENERIC_PKT_LPM_CFG_PREAMBLE_QUAL_DWELL_TIME_Msk             (0x00ffUL << GENERIC_PKT_LPM_CFG_PREAMBLE_QUAL_DWELL_TIME_Pos) /*!< GENERIC_PKT LPM_CFG: PREAMBLE_QUAL_DWELL_TIME Mask           */
#define GENERIC_PKT_LPM_CFG_PREAMBLE_QUAL_DWELL_TIME_Rst             0x0006UL                                                     /*!< GENERIC_PKT LPM_CFG: PREAMBLE_QUAL_DWELL_TIME Reset          */
#define GENERIC_PKT_LPM_CFG_PREAMBLE_QUAL_DWELL_TIME_Addr            0x20000544UL                                                 /*!< GENERIC_PKT LPM_CFG: PREAMBLE_QUAL_DWELL_TIME Address        */

#define GENERIC_PKT_LPM_CFG_PREAMBLE_DETECT_DWELL_TIME_Size          8                                                            /*!< GENERIC_PKT LPM_CFG: PREAMBLE_DETECT_DWELL_TIME Size         */
#define GENERIC_PKT_LPM_CFG_PREAMBLE_DETECT_DWELL_TIME_Pos           24                                                           /*!< GENERIC_PKT LPM_CFG: PREAMBLE_DETECT_DWELL_TIME Position     */
#define GENERIC_PKT_LPM_CFG_PREAMBLE_DETECT_DWELL_TIME_Msk           (0x00ffUL << GENERIC_PKT_LPM_CFG_PREAMBLE_DETECT_DWELL_TIME_Pos) /*!< GENERIC_PKT LPM_CFG: PREAMBLE_DETECT_DWELL_TIME Mask         */
#define GENERIC_PKT_LPM_CFG_PREAMBLE_DETECT_DWELL_TIME_Rst           0x0006UL                                                     /*!< GENERIC_PKT LPM_CFG: PREAMBLE_DETECT_DWELL_TIME Reset        */
#define GENERIC_PKT_LPM_CFG_PREAMBLE_DETECT_DWELL_TIME_Addr          0x20000544UL                                                 /*!< GENERIC_PKT LPM_CFG: PREAMBLE_DETECT_DWELL_TIME Address      */

/* ----------------------------  GENERIC_PKT_TEST_MODES0  ---------------------------- */
#define GENERIC_PKT_TEST_MODES0_Addr                                 0x20000548UL                                                 /*!< GENERIC_PKT TEST_MODES0: Address                             */
#define pGENERIC_PKT_TEST_MODES0                                     (*(volatile uint32_t *) GENERIC_PKT_TEST_MODES0_Addr)        /*!< GENERIC_PKT TEST_MODES0: Pointer                             */
#define GENERIC_PKT_TEST_MODES0_Msk                                  0x000F0000UL                                                 /*!< GENERIC_PKT TEST_MODES0: Mask                                */
#define GENERIC_PKT_TEST_MODES0_Rst                                  0x00000000UL                                                 /*!< GENERIC_PKT TEST_MODES0: Reset                               */

#define GENERIC_PKT_TEST_MODES0_PER_EN_Size                          1
#define GENERIC_PKT_TEST_MODES0_PER_EN_Pos                           0
#define GENERIC_PKT_TEST_MODES0_PER_EN_Addr                          0x20000548UL

#define GENERIC_PKT_TEST_MODES0_PER_IRQ_SELF_CLEAR_Size              1
#define GENERIC_PKT_TEST_MODES0_PER_IRQ_SELF_CLEAR_Pos               12
#define GENERIC_PKT_TEST_MODES0_PER_IRQ_SELF_CLEAR_Addr              0x20000548UL

#define GENERIC_PKT_TEST_MODES0_TX_TEST_Size                         4                                                            /*!< GENERIC_PKT TEST_MODES0: TX_TEST Size                        */
#define GENERIC_PKT_TEST_MODES0_TX_TEST_Pos                          16                                                           /*!< GENERIC_PKT TEST_MODES0: TX_TEST Position                    */
#define GENERIC_PKT_TEST_MODES0_TX_TEST_Msk                          (0x000fUL << GENERIC_PKT_TEST_MODES0_TX_TEST_Pos)            /*!< GENERIC_PKT TEST_MODES0: TX_TEST Mask                        */
#define GENERIC_PKT_TEST_MODES0_TX_TEST_Rst                          0x0000UL                                                     /*!< GENERIC_PKT TEST_MODES0: TX_TEST Reset                       */
#define GENERIC_PKT_TEST_MODES0_TX_TEST_Addr                         0x20000548UL                                                 /*!< GENERIC_PKT TEST_MODES0: TX_TEST Address                     */
#define GENERIC_PKT_TEST_MODES0_TX_TEST_NONE_Eval                    0
#define GENERIC_PKT_TEST_MODES0_TX_TEST_CARRIER_Eval                 1
#define GENERIC_PKT_TEST_MODES0_TX_TEST_FDEV_NEG_Eval                2
#define GENERIC_PKT_TEST_MODES0_TX_TEST_FDEV_MAXNEG_Eval             3
#define GENERIC_PKT_TEST_MODES0_TX_TEST_FDEV_POS_Eval                4
#define GENERIC_PKT_TEST_MODES0_TX_TEST_FDEV_MAXPOS_Eval             5
#define GENERIC_PKT_TEST_MODES0_TX_TEST_ZERO_ONE_Eval                6
#define GENERIC_PKT_TEST_MODES0_TX_TEST_PN9_Eval                     7

#define GENERIC_PKT_TEST_MODES1_Addr                                 0x2000054cUL

#define GENERIC_PKT_TEST_MODES1_PACKET_CNT_Size                      16
#define GENERIC_PKT_TEST_MODES1_PACKET_CNT_Pos                       0
#define GENERIC_PKT_TEST_MODES1_PACKET_CNT_Addr                      0x2000054cUL

#define GENERIC_PKT_TEST_MODES1_CURRENT_CNT_Size                     16
#define GENERIC_PKT_TEST_MODES1_CURRENT_CNT_Pos                      16
#define GENERIC_PKT_TEST_MODES1_CURRENT_CNT_Addr                     0x2000054cUL

/* ----------------------------  GENERIC_PKT_LCPSM1  ---------------------------- */
#define GENERIC_PKT_LCPSM1_Addr                                      0x20000554UL                                                 /*!< GENERIC_PKT LCPSM1: Address                                  */
#define pGENERIC_PKT_LCPSM1                                          (*(volatile uint32_t *) GENERIC_PKT_LCPSM1_Addr)             /*!< GENERIC_PKT LCPSM1: Pointer                                  */
#define GENERIC_PKT_LCPSM1_Msk                                       0xFFFFFFFFUL                                                 /*!< GENERIC_PKT LCPSM1: Mask                                     */
#define GENERIC_PKT_LCPSM1_Rst                                       0x00080004UL                                                 /*!< GENERIC_PKT LCPSM1: Reset                                    */

#define GENERIC_PKT_LCPSM1_LCPSM_ENERGY_CNT_Size                     16                                                           /*!< GENERIC_PKT LCPSM1: LCPSM_ENERGY_CNT Size                    */
#define GENERIC_PKT_LCPSM1_LCPSM_ENERGY_CNT_Pos                      0                                                            /*!< GENERIC_PKT LCPSM1: LCPSM_ENERGY_CNT Position                */
#define GENERIC_PKT_LCPSM1_LCPSM_ENERGY_CNT_Msk                      (0xffffUL << GENERIC_PKT_LCPSM1_LCPSM_ENERGY_CNT_Pos)        /*!< GENERIC_PKT LCPSM1: LCPSM_ENERGY_CNT Mask                    */
#define GENERIC_PKT_LCPSM1_LCPSM_ENERGY_CNT_Rst                      0x0004UL                                                     /*!< GENERIC_PKT LCPSM1: LCPSM_ENERGY_CNT Reset                   */
#define GENERIC_PKT_LCPSM1_LCPSM_ENERGY_CNT_Addr                     0x20000554UL                                                 /*!< GENERIC_PKT LCPSM1: LCPSM_ENERGY_CNT Address                 */

#define GENERIC_PKT_LCPSM1_LCPSM_LOW_CNT_Size                        16                                                           /*!< GENERIC_PKT LCPSM1: LCPSM_LOW_CNT Size                       */
#define GENERIC_PKT_LCPSM1_LCPSM_LOW_CNT_Pos                         16                                                           /*!< GENERIC_PKT LCPSM1: LCPSM_LOW_CNT Position                   */
#define GENERIC_PKT_LCPSM1_LCPSM_LOW_CNT_Msk                         (0xffffUL << GENERIC_PKT_LCPSM1_LCPSM_LOW_CNT_Pos)           /*!< GENERIC_PKT LCPSM1: LCPSM_LOW_CNT Mask                       */
#define GENERIC_PKT_LCPSM1_LCPSM_LOW_CNT_Rst                         0x0008UL                                                     /*!< GENERIC_PKT LCPSM1: LCPSM_LOW_CNT Reset                      */
#define GENERIC_PKT_LCPSM1_LCPSM_LOW_CNT_Addr                        0x20000554UL                                                 /*!< GENERIC_PKT LCPSM1: LCPSM_LOW_CNT Address                    */

/* ----------------------------  GENERIC_PKT_LCPSM2  ---------------------------- */
#define GENERIC_PKT_LCPSM2_Addr                                      0x20000558UL                                                 /*!< GENERIC_PKT LCPSM2: Address                                  */
#define pGENERIC_PKT_LCPSM2                                          (*(volatile uint32_t *) GENERIC_PKT_LCPSM2_Addr)             /*!< GENERIC_PKT LCPSM2: Pointer                                  */
#define GENERIC_PKT_LCPSM2_Msk                                       0x8000FFFFUL                                                 /*!< GENERIC_PKT LCPSM2: Mask                                     */
#define GENERIC_PKT_LCPSM2_Rst                                       0x00000000UL                                                 /*!< GENERIC_PKT LCPSM2: Reset                                    */

#define GENERIC_PKT_LCPSM2_LCPSM_AFC_THRESHOLD_Size                  16                                                           /*!< GENERIC_PKT LCPSM2: LCPSM_AFC_THRESHOLD Size                 */
#define GENERIC_PKT_LCPSM2_LCPSM_AFC_THRESHOLD_Pos                   0                                                            /*!< GENERIC_PKT LCPSM2: LCPSM_AFC_THRESHOLD Position             */
#define GENERIC_PKT_LCPSM2_LCPSM_AFC_THRESHOLD_Msk                   (0xffffUL << GENERIC_PKT_LCPSM2_LCPSM_AFC_THRESHOLD_Pos)     /*!< GENERIC_PKT LCPSM2: LCPSM_AFC_THRESHOLD Mask                 */
#define GENERIC_PKT_LCPSM2_LCPSM_AFC_THRESHOLD_Rst                   0x0000UL                                                     /*!< GENERIC_PKT LCPSM2: LCPSM_AFC_THRESHOLD Reset                */
#define GENERIC_PKT_LCPSM2_LCPSM_AFC_THRESHOLD_Addr                  0x20000558UL                                                 /*!< GENERIC_PKT LCPSM2: LCPSM_AFC_THRESHOLD Address              */

#define GENERIC_PKT_LCPSM2_LCPSM_ENABLED_Size                        1                                                            /*!< GENERIC_PKT LCPSM2: LCPSM_ENABLED Size                       */
#define GENERIC_PKT_LCPSM2_LCPSM_ENABLED_Pos                         31                                                           /*!< GENERIC_PKT LCPSM2: LCPSM_ENABLED Position                   */
#define GENERIC_PKT_LCPSM2_LCPSM_ENABLED_Msk                         (0x0001UL << GENERIC_PKT_LCPSM2_LCPSM_ENABLED_Pos)           /*!< GENERIC_PKT LCPSM2: LCPSM_ENABLED Mask                       */
#define GENERIC_PKT_LCPSM2_LCPSM_ENABLED_Rst                         0x0000UL                                                     /*!< GENERIC_PKT LCPSM2: LCPSM_ENABLED Reset                      */
#define GENERIC_PKT_LCPSM2_LCPSM_ENABLED_Addr                        0x20000558UL                                                 /*!< GENERIC_PKT LCPSM2: LCPSM_ENABLED Address                    */
#define GENERIC_PKT_LCPSM2_LCPSM_ENABLED_DISABLED_Eval               0
#define GENERIC_PKT_LCPSM2_LCPSM_ENABLED_ENABLED_Eval                1

/* ----------------------------  GENERIC_PKT_MISC2  ---------------------------- */
#define GENERIC_PKT_MISC2_Addr                                       0x2000055CUL                                                 /*!< GENERIC_PKT MISC2: Address                                   */
#define pGENERIC_PKT_MISC2                                           (*(volatile uint32_t *) GENERIC_PKT_MISC2_Addr)              /*!< GENERIC_PKT MISC2: Pointer                                   */
#define GENERIC_PKT_MISC2_Msk                                        0xFFFFFFFFUL                                                 /*!< GENERIC_PKT MISC2: Mask                                      */
#define GENERIC_PKT_MISC2_Rst                                        0x00000000UL                                                 /*!< GENERIC_PKT MISC2: Reset                                     */

#define GENERIC_PKT_MISC2_UNUSED_Size                                32                                                           /*!< GENERIC_PKT MISC2: UNUSED Size                               */
#define GENERIC_PKT_MISC2_UNUSED_Pos                                 0                                                            /*!< GENERIC_PKT MISC2: UNUSED Position                           */
#define GENERIC_PKT_MISC2_UNUSED_Msk                                 (0xffffffffUL << GENERIC_PKT_MISC2_UNUSED_Pos)               /*!< GENERIC_PKT MISC2: UNUSED Mask                               */
#define GENERIC_PKT_MISC2_UNUSED_Rst                                 0x0000UL                                                     /*!< GENERIC_PKT MISC2: UNUSED Reset                              */
#define GENERIC_PKT_MISC2_UNUSED_Addr                                0x2000055cUL                                                 /*!< GENERIC_PKT MISC2: UNUSED Address                            */

/* ----------------------------  GENERIC_PKT_LCPSM3  ---------------------------- */
#define GENERIC_PKT_LCPSM3_Addr                                      0x20000560UL                                                 /*!< GENERIC_PKT LCPSM3: Address                                  */
#define pGENERIC_PKT_LCPSM3                                          (*(volatile uint32_t *) GENERIC_PKT_LCPSM3_Addr)             /*!< GENERIC_PKT LCPSM3: Pointer                                  */
#define GENERIC_PKT_LCPSM3_Msk                                       0xFFFFFFFFUL                                                 /*!< GENERIC_PKT LCPSM3: Mask                                     */
#define GENERIC_PKT_LCPSM3_Rst                                       0x00000000UL                                                 /*!< GENERIC_PKT LCPSM3: Reset                                    */

#define GENERIC_PKT_LCPSM3_LCPSM_COUNT_Size                          32                                                           /*!< GENERIC_PKT LCPSM3: LCPSM_COUNT Size                         */
#define GENERIC_PKT_LCPSM3_LCPSM_COUNT_Pos                           0                                                            /*!< GENERIC_PKT LCPSM3: LCPSM_COUNT Position                     */
#define GENERIC_PKT_LCPSM3_LCPSM_COUNT_Msk                           (0xffffffffUL << GENERIC_PKT_LCPSM3_LCPSM_COUNT_Pos)         /*!< GENERIC_PKT LCPSM3: LCPSM_COUNT Mask                         */
#define GENERIC_PKT_LCPSM3_LCPSM_COUNT_Rst                           0x0000UL                                                     /*!< GENERIC_PKT LCPSM3: LCPSM_COUNT Reset                        */
#define GENERIC_PKT_LCPSM3_LCPSM_COUNT_Addr                          0x20000560UL                                                 /*!< GENERIC_PKT LCPSM3: LCPSM_COUNT Address                      */

/* ======================================================================================== */
/* ================          struct 'ANAFILT_LUTS'                         ================ */
/* ======================================================================================== */


/* ----------------------------  ANAFILT_LUTS_DATA0  ---------------------------- */
#define ANAFILT_LUTS_DATA0_Addr                                      0x2000060CUL                                                 /*!< ANAFILT_LUTS DATA0: Address                                  */
#define pANAFILT_LUTS_DATA0                                          (*(volatile uint32_t *) ANAFILT_LUTS_DATA0_Addr)             /*!< ANAFILT_LUTS DATA0: Pointer                                  */
#define ANAFILT_LUTS_DATA0_Msk                                       0xFFFFFFFFUL                                                 /*!< ANAFILT_LUTS DATA0: Mask                                     */
#define ANAFILT_LUTS_DATA0_Rst                                       0x00000000UL                                                 /*!< ANAFILT_LUTS DATA0: Reset                                    */

#define ANAFILT_LUTS_DATA0_VAL_Size                                  32                                                           /*!< ANAFILT_LUTS DATA0: VAL Size                                 */
#define ANAFILT_LUTS_DATA0_VAL_Pos                                   0                                                            /*!< ANAFILT_LUTS DATA0: VAL Position                             */
#define ANAFILT_LUTS_DATA0_VAL_Msk                                   (0xffffffffUL << ANAFILT_LUTS_DATA0_VAL_Pos)                 /*!< ANAFILT_LUTS DATA0: VAL Mask                                 */
#define ANAFILT_LUTS_DATA0_VAL_Rst                                   0x0000UL                                                     /*!< ANAFILT_LUTS DATA0: VAL Reset                                */
#define ANAFILT_LUTS_DATA0_VAL_Addr                                  0x2000060cUL                                                 /*!< ANAFILT_LUTS DATA0: VAL Address                              */

/* ----------------------------  ANAFILT_LUTS_DATA1  ---------------------------- */
#define ANAFILT_LUTS_DATA1_Addr                                      0x20000610UL                                                 /*!< ANAFILT_LUTS DATA1: Address                                  */
#define pANAFILT_LUTS_DATA1                                          (*(volatile uint32_t *) ANAFILT_LUTS_DATA1_Addr)             /*!< ANAFILT_LUTS DATA1: Pointer                                  */
#define ANAFILT_LUTS_DATA1_Msk                                       0xFFFFFFFFUL                                                 /*!< ANAFILT_LUTS DATA1: Mask                                     */
#define ANAFILT_LUTS_DATA1_Rst                                       0x00000000UL                                                 /*!< ANAFILT_LUTS DATA1: Reset                                    */

#define ANAFILT_LUTS_DATA1_VAL_Size                                  32                                                           /*!< ANAFILT_LUTS DATA1: VAL Size                                 */
#define ANAFILT_LUTS_DATA1_VAL_Pos                                   0                                                            /*!< ANAFILT_LUTS DATA1: VAL Position                             */
#define ANAFILT_LUTS_DATA1_VAL_Msk                                   (0xffffffffUL << ANAFILT_LUTS_DATA1_VAL_Pos)                 /*!< ANAFILT_LUTS DATA1: VAL Mask                                 */
#define ANAFILT_LUTS_DATA1_VAL_Rst                                   0x0000UL                                                     /*!< ANAFILT_LUTS DATA1: VAL Reset                                */
#define ANAFILT_LUTS_DATA1_VAL_Addr                                  0x20000610UL                                                 /*!< ANAFILT_LUTS DATA1: VAL Address                              */

/* ----------------------------  ANAFILT_LUTS_DATA2  ---------------------------- */
#define ANAFILT_LUTS_DATA2_Addr                                      0x20000614UL                                                 /*!< ANAFILT_LUTS DATA2: Address                                  */
#define pANAFILT_LUTS_DATA2                                          (*(volatile uint32_t *) ANAFILT_LUTS_DATA2_Addr)             /*!< ANAFILT_LUTS DATA2: Pointer                                  */
#define ANAFILT_LUTS_DATA2_Msk                                       0xFFFFFFFFUL                                                 /*!< ANAFILT_LUTS DATA2: Mask                                     */
#define ANAFILT_LUTS_DATA2_Rst                                       0x00000000UL                                                 /*!< ANAFILT_LUTS DATA2: Reset                                    */

#define ANAFILT_LUTS_DATA2_VAL_Size                                  32                                                           /*!< ANAFILT_LUTS DATA2: VAL Size                                 */
#define ANAFILT_LUTS_DATA2_VAL_Pos                                   0                                                            /*!< ANAFILT_LUTS DATA2: VAL Position                             */
#define ANAFILT_LUTS_DATA2_VAL_Msk                                   (0xffffffffUL << ANAFILT_LUTS_DATA2_VAL_Pos)                 /*!< ANAFILT_LUTS DATA2: VAL Mask                                 */
#define ANAFILT_LUTS_DATA2_VAL_Rst                                   0x0000UL                                                     /*!< ANAFILT_LUTS DATA2: VAL Reset                                */
#define ANAFILT_LUTS_DATA2_VAL_Addr                                  0x20000614UL                                                 /*!< ANAFILT_LUTS DATA2: VAL Address                              */

/* ----------------------------  ANAFILT_LUTS_DATA3  ---------------------------- */
#define ANAFILT_LUTS_DATA3_Addr                                      0x20000618UL                                                 /*!< ANAFILT_LUTS DATA3: Address                                  */
#define pANAFILT_LUTS_DATA3                                          (*(volatile uint32_t *) ANAFILT_LUTS_DATA3_Addr)             /*!< ANAFILT_LUTS DATA3: Pointer                                  */
#define ANAFILT_LUTS_DATA3_Msk                                       0xFFFFFFFFUL                                                 /*!< ANAFILT_LUTS DATA3: Mask                                     */
#define ANAFILT_LUTS_DATA3_Rst                                       0x00000000UL                                                 /*!< ANAFILT_LUTS DATA3: Reset                                    */

#define ANAFILT_LUTS_DATA3_VAL_Size                                  32                                                           /*!< ANAFILT_LUTS DATA3: VAL Size                                 */
#define ANAFILT_LUTS_DATA3_VAL_Pos                                   0                                                            /*!< ANAFILT_LUTS DATA3: VAL Position                             */
#define ANAFILT_LUTS_DATA3_VAL_Msk                                   (0xffffffffUL << ANAFILT_LUTS_DATA3_VAL_Pos)                 /*!< ANAFILT_LUTS DATA3: VAL Mask                                 */
#define ANAFILT_LUTS_DATA3_VAL_Rst                                   0x0000UL                                                     /*!< ANAFILT_LUTS DATA3: VAL Reset                                */
#define ANAFILT_LUTS_DATA3_VAL_Addr                                  0x20000618UL                                                 /*!< ANAFILT_LUTS DATA3: VAL Address                              */

/* ----------------------------  ANAFILT_LUTS_DATA4  ---------------------------- */
#define ANAFILT_LUTS_DATA4_Addr                                      0x2000061CUL                                                 /*!< ANAFILT_LUTS DATA4: Address                                  */
#define pANAFILT_LUTS_DATA4                                          (*(volatile uint32_t *) ANAFILT_LUTS_DATA4_Addr)             /*!< ANAFILT_LUTS DATA4: Pointer                                  */
#define ANAFILT_LUTS_DATA4_Msk                                       0xFFFFFFFFUL                                                 /*!< ANAFILT_LUTS DATA4: Mask                                     */
#define ANAFILT_LUTS_DATA4_Rst                                       0x00000000UL                                                 /*!< ANAFILT_LUTS DATA4: Reset                                    */

#define ANAFILT_LUTS_DATA4_VAL_Size                                  32                                                           /*!< ANAFILT_LUTS DATA4: VAL Size                                 */
#define ANAFILT_LUTS_DATA4_VAL_Pos                                   0                                                            /*!< ANAFILT_LUTS DATA4: VAL Position                             */
#define ANAFILT_LUTS_DATA4_VAL_Msk                                   (0xffffffffUL << ANAFILT_LUTS_DATA4_VAL_Pos)                 /*!< ANAFILT_LUTS DATA4: VAL Mask                                 */
#define ANAFILT_LUTS_DATA4_VAL_Rst                                   0x0000UL                                                     /*!< ANAFILT_LUTS DATA4: VAL Reset                                */
#define ANAFILT_LUTS_DATA4_VAL_Addr                                  0x2000061cUL                                                 /*!< ANAFILT_LUTS DATA4: VAL Address                              */

/* ----------------------------  ANAFILT_LUTS_DATA5  ---------------------------- */
#define ANAFILT_LUTS_DATA5_Addr                                      0x20000620UL                                                 /*!< ANAFILT_LUTS DATA5: Address                                  */
#define pANAFILT_LUTS_DATA5                                          (*(volatile uint32_t *) ANAFILT_LUTS_DATA5_Addr)             /*!< ANAFILT_LUTS DATA5: Pointer                                  */
#define ANAFILT_LUTS_DATA5_Msk                                       0xFFFFFFFFUL                                                 /*!< ANAFILT_LUTS DATA5: Mask                                     */
#define ANAFILT_LUTS_DATA5_Rst                                       0x00000000UL                                                 /*!< ANAFILT_LUTS DATA5: Reset                                    */

#define ANAFILT_LUTS_DATA5_VAL_Size                                  32                                                           /*!< ANAFILT_LUTS DATA5: VAL Size                                 */
#define ANAFILT_LUTS_DATA5_VAL_Pos                                   0                                                            /*!< ANAFILT_LUTS DATA5: VAL Position                             */
#define ANAFILT_LUTS_DATA5_VAL_Msk                                   (0xffffffffUL << ANAFILT_LUTS_DATA5_VAL_Pos)                 /*!< ANAFILT_LUTS DATA5: VAL Mask                                 */
#define ANAFILT_LUTS_DATA5_VAL_Rst                                   0x0000UL                                                     /*!< ANAFILT_LUTS DATA5: VAL Reset                                */
#define ANAFILT_LUTS_DATA5_VAL_Addr                                  0x20000620UL                                                 /*!< ANAFILT_LUTS DATA5: VAL Address                              */

/* ----------------------------  ANAFILT_LUTS_DATA6  ---------------------------- */
#define ANAFILT_LUTS_DATA6_Addr                                      0x20000624UL                                                 /*!< ANAFILT_LUTS DATA6: Address                                  */
#define pANAFILT_LUTS_DATA6                                          (*(volatile uint32_t *) ANAFILT_LUTS_DATA6_Addr)             /*!< ANAFILT_LUTS DATA6: Pointer                                  */
#define ANAFILT_LUTS_DATA6_Msk                                       0xFFFFFFFFUL                                                 /*!< ANAFILT_LUTS DATA6: Mask                                     */
#define ANAFILT_LUTS_DATA6_Rst                                       0x00000000UL                                                 /*!< ANAFILT_LUTS DATA6: Reset                                    */

#define ANAFILT_LUTS_DATA6_VAL_Size                                  32                                                           /*!< ANAFILT_LUTS DATA6: VAL Size                                 */
#define ANAFILT_LUTS_DATA6_VAL_Pos                                   0                                                            /*!< ANAFILT_LUTS DATA6: VAL Position                             */
#define ANAFILT_LUTS_DATA6_VAL_Msk                                   (0xffffffffUL << ANAFILT_LUTS_DATA6_VAL_Pos)                 /*!< ANAFILT_LUTS DATA6: VAL Mask                                 */
#define ANAFILT_LUTS_DATA6_VAL_Rst                                   0x0000UL                                                     /*!< ANAFILT_LUTS DATA6: VAL Reset                                */
#define ANAFILT_LUTS_DATA6_VAL_Addr                                  0x20000624UL                                                 /*!< ANAFILT_LUTS DATA6: VAL Address                              */

/* ----------------------------  ANAFILT_LUTS_DATA7  ---------------------------- */
#define ANAFILT_LUTS_DATA7_Addr                                      0x20000628UL                                                 /*!< ANAFILT_LUTS DATA7: Address                                  */
#define pANAFILT_LUTS_DATA7                                          (*(volatile uint32_t *) ANAFILT_LUTS_DATA7_Addr)             /*!< ANAFILT_LUTS DATA7: Pointer                                  */
#define ANAFILT_LUTS_DATA7_Msk                                       0xFFFFFFFFUL                                                 /*!< ANAFILT_LUTS DATA7: Mask                                     */
#define ANAFILT_LUTS_DATA7_Rst                                       0x00000000UL                                                 /*!< ANAFILT_LUTS DATA7: Reset                                    */

#define ANAFILT_LUTS_DATA7_VAL_Size                                  32                                                           /*!< ANAFILT_LUTS DATA7: VAL Size                                 */
#define ANAFILT_LUTS_DATA7_VAL_Pos                                   0                                                            /*!< ANAFILT_LUTS DATA7: VAL Position                             */
#define ANAFILT_LUTS_DATA7_VAL_Msk                                   (0xffffffffUL << ANAFILT_LUTS_DATA7_VAL_Pos)                 /*!< ANAFILT_LUTS DATA7: VAL Mask                                 */
#define ANAFILT_LUTS_DATA7_VAL_Rst                                   0x0000UL                                                     /*!< ANAFILT_LUTS DATA7: VAL Reset                                */
#define ANAFILT_LUTS_DATA7_VAL_Addr                                  0x20000628UL                                                 /*!< ANAFILT_LUTS DATA7: VAL Address                              */

/* ----------------------------  ANAFILT_LUTS_DATA8  ---------------------------- */
#define ANAFILT_LUTS_DATA8_Addr                                      0x2000062CUL                                                 /*!< ANAFILT_LUTS DATA8: Address                                  */
#define pANAFILT_LUTS_DATA8                                          (*(volatile uint32_t *) ANAFILT_LUTS_DATA8_Addr)             /*!< ANAFILT_LUTS DATA8: Pointer                                  */
#define ANAFILT_LUTS_DATA8_Msk                                       0xFFFFFFFFUL                                                 /*!< ANAFILT_LUTS DATA8: Mask                                     */
#define ANAFILT_LUTS_DATA8_Rst                                       0x00000000UL                                                 /*!< ANAFILT_LUTS DATA8: Reset                                    */

#define ANAFILT_LUTS_DATA8_VAL_Size                                  32                                                           /*!< ANAFILT_LUTS DATA8: VAL Size                                 */
#define ANAFILT_LUTS_DATA8_VAL_Pos                                   0                                                            /*!< ANAFILT_LUTS DATA8: VAL Position                             */
#define ANAFILT_LUTS_DATA8_VAL_Msk                                   (0xffffffffUL << ANAFILT_LUTS_DATA8_VAL_Pos)                 /*!< ANAFILT_LUTS DATA8: VAL Mask                                 */
#define ANAFILT_LUTS_DATA8_VAL_Rst                                   0x0000UL                                                     /*!< ANAFILT_LUTS DATA8: VAL Reset                                */
#define ANAFILT_LUTS_DATA8_VAL_Addr                                  0x2000062cUL                                                 /*!< ANAFILT_LUTS DATA8: VAL Address                              */

/* ----------------------------  ANAFILT_LUTS_DATA9  ---------------------------- */
#define ANAFILT_LUTS_DATA9_Addr                                      0x20000630UL                                                 /*!< ANAFILT_LUTS DATA9: Address                                  */
#define pANAFILT_LUTS_DATA9                                          (*(volatile uint32_t *) ANAFILT_LUTS_DATA9_Addr)             /*!< ANAFILT_LUTS DATA9: Pointer                                  */
#define ANAFILT_LUTS_DATA9_Msk                                       0xFFFFFFFFUL                                                 /*!< ANAFILT_LUTS DATA9: Mask                                     */
#define ANAFILT_LUTS_DATA9_Rst                                       0x00000000UL                                                 /*!< ANAFILT_LUTS DATA9: Reset                                    */

#define ANAFILT_LUTS_DATA9_VAL_Size                                  32                                                           /*!< ANAFILT_LUTS DATA9: VAL Size                                 */
#define ANAFILT_LUTS_DATA9_VAL_Pos                                   0                                                            /*!< ANAFILT_LUTS DATA9: VAL Position                             */
#define ANAFILT_LUTS_DATA9_VAL_Msk                                   (0xffffffffUL << ANAFILT_LUTS_DATA9_VAL_Pos)                 /*!< ANAFILT_LUTS DATA9: VAL Mask                                 */
#define ANAFILT_LUTS_DATA9_VAL_Rst                                   0x0000UL                                                     /*!< ANAFILT_LUTS DATA9: VAL Reset                                */
#define ANAFILT_LUTS_DATA9_VAL_Addr                                  0x20000630UL                                                 /*!< ANAFILT_LUTS DATA9: VAL Address                              */

/* ----------------------------  ANAFILT_LUTS_DATA10  ---------------------------- */
#define ANAFILT_LUTS_DATA10_Addr                                     0x20000634UL                                                 /*!< ANAFILT_LUTS DATA10: Address                                 */
#define pANAFILT_LUTS_DATA10                                         (*(volatile uint32_t *) ANAFILT_LUTS_DATA10_Addr)            /*!< ANAFILT_LUTS DATA10: Pointer                                 */
#define ANAFILT_LUTS_DATA10_Msk                                      0xFFFFFFFFUL                                                 /*!< ANAFILT_LUTS DATA10: Mask                                    */
#define ANAFILT_LUTS_DATA10_Rst                                      0x00000000UL                                                 /*!< ANAFILT_LUTS DATA10: Reset                                   */

#define ANAFILT_LUTS_DATA10_VAL_Size                                 32                                                           /*!< ANAFILT_LUTS DATA10: VAL Size                                */
#define ANAFILT_LUTS_DATA10_VAL_Pos                                  0                                                            /*!< ANAFILT_LUTS DATA10: VAL Position                            */
#define ANAFILT_LUTS_DATA10_VAL_Msk                                  (0xffffffffUL << ANAFILT_LUTS_DATA10_VAL_Pos)                /*!< ANAFILT_LUTS DATA10: VAL Mask                                */
#define ANAFILT_LUTS_DATA10_VAL_Rst                                  0x0000UL                                                     /*!< ANAFILT_LUTS DATA10: VAL Reset                               */
#define ANAFILT_LUTS_DATA10_VAL_Addr                                 0x20000634UL                                                 /*!< ANAFILT_LUTS DATA10: VAL Address                             */

/* ----------------------------  ANAFILT_LUTS_DATA11  ---------------------------- */
#define ANAFILT_LUTS_DATA11_Addr                                     0x20000638UL                                                 /*!< ANAFILT_LUTS DATA11: Address                                 */
#define pANAFILT_LUTS_DATA11                                         (*(volatile uint32_t *) ANAFILT_LUTS_DATA11_Addr)            /*!< ANAFILT_LUTS DATA11: Pointer                                 */
#define ANAFILT_LUTS_DATA11_Msk                                      0xFFFFFFFFUL                                                 /*!< ANAFILT_LUTS DATA11: Mask                                    */
#define ANAFILT_LUTS_DATA11_Rst                                      0x00000000UL                                                 /*!< ANAFILT_LUTS DATA11: Reset                                   */

#define ANAFILT_LUTS_DATA11_VAL_Size                                 32                                                           /*!< ANAFILT_LUTS DATA11: VAL Size                                */
#define ANAFILT_LUTS_DATA11_VAL_Pos                                  0                                                            /*!< ANAFILT_LUTS DATA11: VAL Position                            */
#define ANAFILT_LUTS_DATA11_VAL_Msk                                  (0xffffffffUL << ANAFILT_LUTS_DATA11_VAL_Pos)                /*!< ANAFILT_LUTS DATA11: VAL Mask                                */
#define ANAFILT_LUTS_DATA11_VAL_Rst                                  0x0000UL                                                     /*!< ANAFILT_LUTS DATA11: VAL Reset                               */
#define ANAFILT_LUTS_DATA11_VAL_Addr                                 0x20000638UL                                                 /*!< ANAFILT_LUTS DATA11: VAL Address                             */

/* ----------------------------  ANAFILT_LUTS_DATA12  ---------------------------- */
#define ANAFILT_LUTS_DATA12_Addr                                     0x2000063CUL                                                 /*!< ANAFILT_LUTS DATA12: Address                                 */
#define pANAFILT_LUTS_DATA12                                         (*(volatile uint32_t *) ANAFILT_LUTS_DATA12_Addr)            /*!< ANAFILT_LUTS DATA12: Pointer                                 */
#define ANAFILT_LUTS_DATA12_Msk                                      0xFFFFFFFFUL                                                 /*!< ANAFILT_LUTS DATA12: Mask                                    */
#define ANAFILT_LUTS_DATA12_Rst                                      0x00000000UL                                                 /*!< ANAFILT_LUTS DATA12: Reset                                   */

#define ANAFILT_LUTS_DATA12_VAL_Size                                 32                                                           /*!< ANAFILT_LUTS DATA12: VAL Size                                */
#define ANAFILT_LUTS_DATA12_VAL_Pos                                  0                                                            /*!< ANAFILT_LUTS DATA12: VAL Position                            */
#define ANAFILT_LUTS_DATA12_VAL_Msk                                  (0xffffffffUL << ANAFILT_LUTS_DATA12_VAL_Pos)                /*!< ANAFILT_LUTS DATA12: VAL Mask                                */
#define ANAFILT_LUTS_DATA12_VAL_Rst                                  0x0000UL                                                     /*!< ANAFILT_LUTS DATA12: VAL Reset                               */
#define ANAFILT_LUTS_DATA12_VAL_Addr                                 0x2000063cUL                                                 /*!< ANAFILT_LUTS DATA12: VAL Address                             */

/* ======================================================================================== */
/* ================          struct 'DIGFILT_LUTS'                         ================ */
/* ======================================================================================== */


/* ----------------------------  DIGFILT_LUTS_DATA0  ---------------------------- */
#define DIGFILT_LUTS_DATA0_Addr                                      0x200006B4UL                                                 /*!< DIGFILT_LUTS DATA0: Address                                  */
#define pDIGFILT_LUTS_DATA0                                          (*(volatile uint32_t *) DIGFILT_LUTS_DATA0_Addr)             /*!< DIGFILT_LUTS DATA0: Pointer                                  */
#define DIGFILT_LUTS_DATA0_Msk                                       0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA0: Mask                                     */
#define DIGFILT_LUTS_DATA0_Rst                                       0x00000000UL                                                 /*!< DIGFILT_LUTS DATA0: Reset                                    */

#define DIGFILT_LUTS_DATA0_VAL_Size                                  32                                                           /*!< DIGFILT_LUTS DATA0: VAL Size                                 */
#define DIGFILT_LUTS_DATA0_VAL_Pos                                   0                                                            /*!< DIGFILT_LUTS DATA0: VAL Position                             */
#define DIGFILT_LUTS_DATA0_VAL_Msk                                   (0xffffffffUL << DIGFILT_LUTS_DATA0_VAL_Pos)                 /*!< DIGFILT_LUTS DATA0: VAL Mask                                 */
#define DIGFILT_LUTS_DATA0_VAL_Rst                                   0x0000UL                                                     /*!< DIGFILT_LUTS DATA0: VAL Reset                                */
#define DIGFILT_LUTS_DATA0_VAL_Addr                                  0x200006b4UL                                                 /*!< DIGFILT_LUTS DATA0: VAL Address                              */

/* ----------------------------  DIGFILT_LUTS_DATA1  ---------------------------- */
#define DIGFILT_LUTS_DATA1_Addr                                      0x200006B8UL                                                 /*!< DIGFILT_LUTS DATA1: Address                                  */
#define pDIGFILT_LUTS_DATA1                                          (*(volatile uint32_t *) DIGFILT_LUTS_DATA1_Addr)             /*!< DIGFILT_LUTS DATA1: Pointer                                  */
#define DIGFILT_LUTS_DATA1_Msk                                       0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA1: Mask                                     */
#define DIGFILT_LUTS_DATA1_Rst                                       0x00000000UL                                                 /*!< DIGFILT_LUTS DATA1: Reset                                    */

#define DIGFILT_LUTS_DATA1_VAL_Size                                  32                                                           /*!< DIGFILT_LUTS DATA1: VAL Size                                 */
#define DIGFILT_LUTS_DATA1_VAL_Pos                                   0                                                            /*!< DIGFILT_LUTS DATA1: VAL Position                             */
#define DIGFILT_LUTS_DATA1_VAL_Msk                                   (0xffffffffUL << DIGFILT_LUTS_DATA1_VAL_Pos)                 /*!< DIGFILT_LUTS DATA1: VAL Mask                                 */
#define DIGFILT_LUTS_DATA1_VAL_Rst                                   0x0000UL                                                     /*!< DIGFILT_LUTS DATA1: VAL Reset                                */
#define DIGFILT_LUTS_DATA1_VAL_Addr                                  0x200006b8UL                                                 /*!< DIGFILT_LUTS DATA1: VAL Address                              */

/* ----------------------------  DIGFILT_LUTS_DATA2  ---------------------------- */
#define DIGFILT_LUTS_DATA2_Addr                                      0x200006BCUL                                                 /*!< DIGFILT_LUTS DATA2: Address                                  */
#define pDIGFILT_LUTS_DATA2                                          (*(volatile uint32_t *) DIGFILT_LUTS_DATA2_Addr)             /*!< DIGFILT_LUTS DATA2: Pointer                                  */
#define DIGFILT_LUTS_DATA2_Msk                                       0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA2: Mask                                     */
#define DIGFILT_LUTS_DATA2_Rst                                       0x00000000UL                                                 /*!< DIGFILT_LUTS DATA2: Reset                                    */

#define DIGFILT_LUTS_DATA2_VAL_Size                                  32                                                           /*!< DIGFILT_LUTS DATA2: VAL Size                                 */
#define DIGFILT_LUTS_DATA2_VAL_Pos                                   0                                                            /*!< DIGFILT_LUTS DATA2: VAL Position                             */
#define DIGFILT_LUTS_DATA2_VAL_Msk                                   (0xffffffffUL << DIGFILT_LUTS_DATA2_VAL_Pos)                 /*!< DIGFILT_LUTS DATA2: VAL Mask                                 */
#define DIGFILT_LUTS_DATA2_VAL_Rst                                   0x0000UL                                                     /*!< DIGFILT_LUTS DATA2: VAL Reset                                */
#define DIGFILT_LUTS_DATA2_VAL_Addr                                  0x200006bcUL                                                 /*!< DIGFILT_LUTS DATA2: VAL Address                              */

/* ----------------------------  DIGFILT_LUTS_DATA3  ---------------------------- */
#define DIGFILT_LUTS_DATA3_Addr                                      0x200006C0UL                                                 /*!< DIGFILT_LUTS DATA3: Address                                  */
#define pDIGFILT_LUTS_DATA3                                          (*(volatile uint32_t *) DIGFILT_LUTS_DATA3_Addr)             /*!< DIGFILT_LUTS DATA3: Pointer                                  */
#define DIGFILT_LUTS_DATA3_Msk                                       0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA3: Mask                                     */
#define DIGFILT_LUTS_DATA3_Rst                                       0x00000000UL                                                 /*!< DIGFILT_LUTS DATA3: Reset                                    */

#define DIGFILT_LUTS_DATA3_VAL_Size                                  32                                                           /*!< DIGFILT_LUTS DATA3: VAL Size                                 */
#define DIGFILT_LUTS_DATA3_VAL_Pos                                   0                                                            /*!< DIGFILT_LUTS DATA3: VAL Position                             */
#define DIGFILT_LUTS_DATA3_VAL_Msk                                   (0xffffffffUL << DIGFILT_LUTS_DATA3_VAL_Pos)                 /*!< DIGFILT_LUTS DATA3: VAL Mask                                 */
#define DIGFILT_LUTS_DATA3_VAL_Rst                                   0x0000UL                                                     /*!< DIGFILT_LUTS DATA3: VAL Reset                                */
#define DIGFILT_LUTS_DATA3_VAL_Addr                                  0x200006c0UL                                                 /*!< DIGFILT_LUTS DATA3: VAL Address                              */

/* ----------------------------  DIGFILT_LUTS_DATA4  ---------------------------- */
#define DIGFILT_LUTS_DATA4_Addr                                      0x200006C4UL                                                 /*!< DIGFILT_LUTS DATA4: Address                                  */
#define pDIGFILT_LUTS_DATA4                                          (*(volatile uint32_t *) DIGFILT_LUTS_DATA4_Addr)             /*!< DIGFILT_LUTS DATA4: Pointer                                  */
#define DIGFILT_LUTS_DATA4_Msk                                       0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA4: Mask                                     */
#define DIGFILT_LUTS_DATA4_Rst                                       0x00000000UL                                                 /*!< DIGFILT_LUTS DATA4: Reset                                    */

#define DIGFILT_LUTS_DATA4_VAL_Size                                  32                                                           /*!< DIGFILT_LUTS DATA4: VAL Size                                 */
#define DIGFILT_LUTS_DATA4_VAL_Pos                                   0                                                            /*!< DIGFILT_LUTS DATA4: VAL Position                             */
#define DIGFILT_LUTS_DATA4_VAL_Msk                                   (0xffffffffUL << DIGFILT_LUTS_DATA4_VAL_Pos)                 /*!< DIGFILT_LUTS DATA4: VAL Mask                                 */
#define DIGFILT_LUTS_DATA4_VAL_Rst                                   0x0000UL                                                     /*!< DIGFILT_LUTS DATA4: VAL Reset                                */
#define DIGFILT_LUTS_DATA4_VAL_Addr                                  0x200006c4UL                                                 /*!< DIGFILT_LUTS DATA4: VAL Address                              */

/* ----------------------------  DIGFILT_LUTS_DATA5  ---------------------------- */
#define DIGFILT_LUTS_DATA5_Addr                                      0x200006C8UL                                                 /*!< DIGFILT_LUTS DATA5: Address                                  */
#define pDIGFILT_LUTS_DATA5                                          (*(volatile uint32_t *) DIGFILT_LUTS_DATA5_Addr)             /*!< DIGFILT_LUTS DATA5: Pointer                                  */
#define DIGFILT_LUTS_DATA5_Msk                                       0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA5: Mask                                     */
#define DIGFILT_LUTS_DATA5_Rst                                       0x00000000UL                                                 /*!< DIGFILT_LUTS DATA5: Reset                                    */

#define DIGFILT_LUTS_DATA5_VAL_Size                                  32                                                           /*!< DIGFILT_LUTS DATA5: VAL Size                                 */
#define DIGFILT_LUTS_DATA5_VAL_Pos                                   0                                                            /*!< DIGFILT_LUTS DATA5: VAL Position                             */
#define DIGFILT_LUTS_DATA5_VAL_Msk                                   (0xffffffffUL << DIGFILT_LUTS_DATA5_VAL_Pos)                 /*!< DIGFILT_LUTS DATA5: VAL Mask                                 */
#define DIGFILT_LUTS_DATA5_VAL_Rst                                   0x0000UL                                                     /*!< DIGFILT_LUTS DATA5: VAL Reset                                */
#define DIGFILT_LUTS_DATA5_VAL_Addr                                  0x200006c8UL                                                 /*!< DIGFILT_LUTS DATA5: VAL Address                              */

/* ----------------------------  DIGFILT_LUTS_DATA6  ---------------------------- */
#define DIGFILT_LUTS_DATA6_Addr                                      0x200006CCUL                                                 /*!< DIGFILT_LUTS DATA6: Address                                  */
#define pDIGFILT_LUTS_DATA6                                          (*(volatile uint32_t *) DIGFILT_LUTS_DATA6_Addr)             /*!< DIGFILT_LUTS DATA6: Pointer                                  */
#define DIGFILT_LUTS_DATA6_Msk                                       0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA6: Mask                                     */
#define DIGFILT_LUTS_DATA6_Rst                                       0x00000000UL                                                 /*!< DIGFILT_LUTS DATA6: Reset                                    */

#define DIGFILT_LUTS_DATA6_VAL_Size                                  32                                                           /*!< DIGFILT_LUTS DATA6: VAL Size                                 */
#define DIGFILT_LUTS_DATA6_VAL_Pos                                   0                                                            /*!< DIGFILT_LUTS DATA6: VAL Position                             */
#define DIGFILT_LUTS_DATA6_VAL_Msk                                   (0xffffffffUL << DIGFILT_LUTS_DATA6_VAL_Pos)                 /*!< DIGFILT_LUTS DATA6: VAL Mask                                 */
#define DIGFILT_LUTS_DATA6_VAL_Rst                                   0x0000UL                                                     /*!< DIGFILT_LUTS DATA6: VAL Reset                                */
#define DIGFILT_LUTS_DATA6_VAL_Addr                                  0x200006ccUL                                                 /*!< DIGFILT_LUTS DATA6: VAL Address                              */

/* ----------------------------  DIGFILT_LUTS_DATA7  ---------------------------- */
#define DIGFILT_LUTS_DATA7_Addr                                      0x200006D0UL                                                 /*!< DIGFILT_LUTS DATA7: Address                                  */
#define pDIGFILT_LUTS_DATA7                                          (*(volatile uint32_t *) DIGFILT_LUTS_DATA7_Addr)             /*!< DIGFILT_LUTS DATA7: Pointer                                  */
#define DIGFILT_LUTS_DATA7_Msk                                       0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA7: Mask                                     */
#define DIGFILT_LUTS_DATA7_Rst                                       0x00000000UL                                                 /*!< DIGFILT_LUTS DATA7: Reset                                    */

#define DIGFILT_LUTS_DATA7_VAL_Size                                  32                                                           /*!< DIGFILT_LUTS DATA7: VAL Size                                 */
#define DIGFILT_LUTS_DATA7_VAL_Pos                                   0                                                            /*!< DIGFILT_LUTS DATA7: VAL Position                             */
#define DIGFILT_LUTS_DATA7_VAL_Msk                                   (0xffffffffUL << DIGFILT_LUTS_DATA7_VAL_Pos)                 /*!< DIGFILT_LUTS DATA7: VAL Mask                                 */
#define DIGFILT_LUTS_DATA7_VAL_Rst                                   0x0000UL                                                     /*!< DIGFILT_LUTS DATA7: VAL Reset                                */
#define DIGFILT_LUTS_DATA7_VAL_Addr                                  0x200006d0UL                                                 /*!< DIGFILT_LUTS DATA7: VAL Address                              */

/* ----------------------------  DIGFILT_LUTS_DATA8  ---------------------------- */
#define DIGFILT_LUTS_DATA8_Addr                                      0x200006D4UL                                                 /*!< DIGFILT_LUTS DATA8: Address                                  */
#define pDIGFILT_LUTS_DATA8                                          (*(volatile uint32_t *) DIGFILT_LUTS_DATA8_Addr)             /*!< DIGFILT_LUTS DATA8: Pointer                                  */
#define DIGFILT_LUTS_DATA8_Msk                                       0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA8: Mask                                     */
#define DIGFILT_LUTS_DATA8_Rst                                       0x00000000UL                                                 /*!< DIGFILT_LUTS DATA8: Reset                                    */

#define DIGFILT_LUTS_DATA8_VAL_Size                                  32                                                           /*!< DIGFILT_LUTS DATA8: VAL Size                                 */
#define DIGFILT_LUTS_DATA8_VAL_Pos                                   0                                                            /*!< DIGFILT_LUTS DATA8: VAL Position                             */
#define DIGFILT_LUTS_DATA8_VAL_Msk                                   (0xffffffffUL << DIGFILT_LUTS_DATA8_VAL_Pos)                 /*!< DIGFILT_LUTS DATA8: VAL Mask                                 */
#define DIGFILT_LUTS_DATA8_VAL_Rst                                   0x0000UL                                                     /*!< DIGFILT_LUTS DATA8: VAL Reset                                */
#define DIGFILT_LUTS_DATA8_VAL_Addr                                  0x200006d4UL                                                 /*!< DIGFILT_LUTS DATA8: VAL Address                              */

/* ----------------------------  DIGFILT_LUTS_DATA9  ---------------------------- */
#define DIGFILT_LUTS_DATA9_Addr                                      0x200006D8UL                                                 /*!< DIGFILT_LUTS DATA9: Address                                  */
#define pDIGFILT_LUTS_DATA9                                          (*(volatile uint32_t *) DIGFILT_LUTS_DATA9_Addr)             /*!< DIGFILT_LUTS DATA9: Pointer                                  */
#define DIGFILT_LUTS_DATA9_Msk                                       0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA9: Mask                                     */
#define DIGFILT_LUTS_DATA9_Rst                                       0x00000000UL                                                 /*!< DIGFILT_LUTS DATA9: Reset                                    */

#define DIGFILT_LUTS_DATA9_VAL_Size                                  32                                                           /*!< DIGFILT_LUTS DATA9: VAL Size                                 */
#define DIGFILT_LUTS_DATA9_VAL_Pos                                   0                                                            /*!< DIGFILT_LUTS DATA9: VAL Position                             */
#define DIGFILT_LUTS_DATA9_VAL_Msk                                   (0xffffffffUL << DIGFILT_LUTS_DATA9_VAL_Pos)                 /*!< DIGFILT_LUTS DATA9: VAL Mask                                 */
#define DIGFILT_LUTS_DATA9_VAL_Rst                                   0x0000UL                                                     /*!< DIGFILT_LUTS DATA9: VAL Reset                                */
#define DIGFILT_LUTS_DATA9_VAL_Addr                                  0x200006d8UL                                                 /*!< DIGFILT_LUTS DATA9: VAL Address                              */

/* ----------------------------  DIGFILT_LUTS_DATA10  ---------------------------- */
#define DIGFILT_LUTS_DATA10_Addr                                     0x200006DCUL                                                 /*!< DIGFILT_LUTS DATA10: Address                                 */
#define pDIGFILT_LUTS_DATA10                                         (*(volatile uint32_t *) DIGFILT_LUTS_DATA10_Addr)            /*!< DIGFILT_LUTS DATA10: Pointer                                 */
#define DIGFILT_LUTS_DATA10_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA10: Mask                                    */
#define DIGFILT_LUTS_DATA10_Rst                                      0x00000000UL                                                 /*!< DIGFILT_LUTS DATA10: Reset                                   */

#define DIGFILT_LUTS_DATA10_VAL_Size                                 32                                                           /*!< DIGFILT_LUTS DATA10: VAL Size                                */
#define DIGFILT_LUTS_DATA10_VAL_Pos                                  0                                                            /*!< DIGFILT_LUTS DATA10: VAL Position                            */
#define DIGFILT_LUTS_DATA10_VAL_Msk                                  (0xffffffffUL << DIGFILT_LUTS_DATA10_VAL_Pos)                /*!< DIGFILT_LUTS DATA10: VAL Mask                                */
#define DIGFILT_LUTS_DATA10_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT_LUTS DATA10: VAL Reset                               */
#define DIGFILT_LUTS_DATA10_VAL_Addr                                 0x200006dcUL                                                 /*!< DIGFILT_LUTS DATA10: VAL Address                             */

/* ----------------------------  DIGFILT_LUTS_DATA11  ---------------------------- */
#define DIGFILT_LUTS_DATA11_Addr                                     0x200006E0UL                                                 /*!< DIGFILT_LUTS DATA11: Address                                 */
#define pDIGFILT_LUTS_DATA11                                         (*(volatile uint32_t *) DIGFILT_LUTS_DATA11_Addr)            /*!< DIGFILT_LUTS DATA11: Pointer                                 */
#define DIGFILT_LUTS_DATA11_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA11: Mask                                    */
#define DIGFILT_LUTS_DATA11_Rst                                      0x00000000UL                                                 /*!< DIGFILT_LUTS DATA11: Reset                                   */

#define DIGFILT_LUTS_DATA11_VAL_Size                                 32                                                           /*!< DIGFILT_LUTS DATA11: VAL Size                                */
#define DIGFILT_LUTS_DATA11_VAL_Pos                                  0                                                            /*!< DIGFILT_LUTS DATA11: VAL Position                            */
#define DIGFILT_LUTS_DATA11_VAL_Msk                                  (0xffffffffUL << DIGFILT_LUTS_DATA11_VAL_Pos)                /*!< DIGFILT_LUTS DATA11: VAL Mask                                */
#define DIGFILT_LUTS_DATA11_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT_LUTS DATA11: VAL Reset                               */
#define DIGFILT_LUTS_DATA11_VAL_Addr                                 0x200006e0UL                                                 /*!< DIGFILT_LUTS DATA11: VAL Address                             */

/* ----------------------------  DIGFILT_LUTS_DATA12  ---------------------------- */
#define DIGFILT_LUTS_DATA12_Addr                                     0x200006E4UL                                                 /*!< DIGFILT_LUTS DATA12: Address                                 */
#define pDIGFILT_LUTS_DATA12                                         (*(volatile uint32_t *) DIGFILT_LUTS_DATA12_Addr)            /*!< DIGFILT_LUTS DATA12: Pointer                                 */
#define DIGFILT_LUTS_DATA12_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA12: Mask                                    */
#define DIGFILT_LUTS_DATA12_Rst                                      0x00000000UL                                                 /*!< DIGFILT_LUTS DATA12: Reset                                   */

#define DIGFILT_LUTS_DATA12_VAL_Size                                 32                                                           /*!< DIGFILT_LUTS DATA12: VAL Size                                */
#define DIGFILT_LUTS_DATA12_VAL_Pos                                  0                                                            /*!< DIGFILT_LUTS DATA12: VAL Position                            */
#define DIGFILT_LUTS_DATA12_VAL_Msk                                  (0xffffffffUL << DIGFILT_LUTS_DATA12_VAL_Pos)                /*!< DIGFILT_LUTS DATA12: VAL Mask                                */
#define DIGFILT_LUTS_DATA12_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT_LUTS DATA12: VAL Reset                               */
#define DIGFILT_LUTS_DATA12_VAL_Addr                                 0x200006e4UL                                                 /*!< DIGFILT_LUTS DATA12: VAL Address                             */

/* ----------------------------  DIGFILT_LUTS_DATA13  ---------------------------- */
#define DIGFILT_LUTS_DATA13_Addr                                     0x200006E8UL                                                 /*!< DIGFILT_LUTS DATA13: Address                                 */
#define pDIGFILT_LUTS_DATA13                                         (*(volatile uint32_t *) DIGFILT_LUTS_DATA13_Addr)            /*!< DIGFILT_LUTS DATA13: Pointer                                 */
#define DIGFILT_LUTS_DATA13_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA13: Mask                                    */
#define DIGFILT_LUTS_DATA13_Rst                                      0x00000000UL                                                 /*!< DIGFILT_LUTS DATA13: Reset                                   */

#define DIGFILT_LUTS_DATA13_VAL_Size                                 32                                                           /*!< DIGFILT_LUTS DATA13: VAL Size                                */
#define DIGFILT_LUTS_DATA13_VAL_Pos                                  0                                                            /*!< DIGFILT_LUTS DATA13: VAL Position                            */
#define DIGFILT_LUTS_DATA13_VAL_Msk                                  (0xffffffffUL << DIGFILT_LUTS_DATA13_VAL_Pos)                /*!< DIGFILT_LUTS DATA13: VAL Mask                                */
#define DIGFILT_LUTS_DATA13_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT_LUTS DATA13: VAL Reset                               */
#define DIGFILT_LUTS_DATA13_VAL_Addr                                 0x200006e8UL                                                 /*!< DIGFILT_LUTS DATA13: VAL Address                             */

/* ----------------------------  DIGFILT_LUTS_DATA14  ---------------------------- */
#define DIGFILT_LUTS_DATA14_Addr                                     0x200006ECUL                                                 /*!< DIGFILT_LUTS DATA14: Address                                 */
#define pDIGFILT_LUTS_DATA14                                         (*(volatile uint32_t *) DIGFILT_LUTS_DATA14_Addr)            /*!< DIGFILT_LUTS DATA14: Pointer                                 */
#define DIGFILT_LUTS_DATA14_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA14: Mask                                    */
#define DIGFILT_LUTS_DATA14_Rst                                      0x00000000UL                                                 /*!< DIGFILT_LUTS DATA14: Reset                                   */

#define DIGFILT_LUTS_DATA14_VAL_Size                                 32                                                           /*!< DIGFILT_LUTS DATA14: VAL Size                                */
#define DIGFILT_LUTS_DATA14_VAL_Pos                                  0                                                            /*!< DIGFILT_LUTS DATA14: VAL Position                            */
#define DIGFILT_LUTS_DATA14_VAL_Msk                                  (0xffffffffUL << DIGFILT_LUTS_DATA14_VAL_Pos)                /*!< DIGFILT_LUTS DATA14: VAL Mask                                */
#define DIGFILT_LUTS_DATA14_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT_LUTS DATA14: VAL Reset                               */
#define DIGFILT_LUTS_DATA14_VAL_Addr                                 0x200006ecUL                                                 /*!< DIGFILT_LUTS DATA14: VAL Address                             */

/* ----------------------------  DIGFILT_LUTS_DATA15  ---------------------------- */
#define DIGFILT_LUTS_DATA15_Addr                                     0x200006F0UL                                                 /*!< DIGFILT_LUTS DATA15: Address                                 */
#define pDIGFILT_LUTS_DATA15                                         (*(volatile uint32_t *) DIGFILT_LUTS_DATA15_Addr)            /*!< DIGFILT_LUTS DATA15: Pointer                                 */
#define DIGFILT_LUTS_DATA15_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA15: Mask                                    */
#define DIGFILT_LUTS_DATA15_Rst                                      0x00000000UL                                                 /*!< DIGFILT_LUTS DATA15: Reset                                   */

#define DIGFILT_LUTS_DATA15_VAL_Size                                 32                                                           /*!< DIGFILT_LUTS DATA15: VAL Size                                */
#define DIGFILT_LUTS_DATA15_VAL_Pos                                  0                                                            /*!< DIGFILT_LUTS DATA15: VAL Position                            */
#define DIGFILT_LUTS_DATA15_VAL_Msk                                  (0xffffffffUL << DIGFILT_LUTS_DATA15_VAL_Pos)                /*!< DIGFILT_LUTS DATA15: VAL Mask                                */
#define DIGFILT_LUTS_DATA15_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT_LUTS DATA15: VAL Reset                               */
#define DIGFILT_LUTS_DATA15_VAL_Addr                                 0x200006f0UL                                                 /*!< DIGFILT_LUTS DATA15: VAL Address                             */

/* ----------------------------  DIGFILT_LUTS_DATA16  ---------------------------- */
#define DIGFILT_LUTS_DATA16_Addr                                     0x200006F4UL                                                 /*!< DIGFILT_LUTS DATA16: Address                                 */
#define pDIGFILT_LUTS_DATA16                                         (*(volatile uint32_t *) DIGFILT_LUTS_DATA16_Addr)            /*!< DIGFILT_LUTS DATA16: Pointer                                 */
#define DIGFILT_LUTS_DATA16_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT_LUTS DATA16: Mask                                    */
#define DIGFILT_LUTS_DATA16_Rst                                      0x00000000UL                                                 /*!< DIGFILT_LUTS DATA16: Reset                                   */

#define DIGFILT_LUTS_DATA16_VAL_Size                                 32                                                           /*!< DIGFILT_LUTS DATA16: VAL Size                                */
#define DIGFILT_LUTS_DATA16_VAL_Pos                                  0                                                            /*!< DIGFILT_LUTS DATA16: VAL Position                            */
#define DIGFILT_LUTS_DATA16_VAL_Msk                                  (0xffffffffUL << DIGFILT_LUTS_DATA16_VAL_Pos)                /*!< DIGFILT_LUTS DATA16: VAL Mask                                */
#define DIGFILT_LUTS_DATA16_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT_LUTS DATA16: VAL Reset                               */
#define DIGFILT_LUTS_DATA16_VAL_Addr                                 0x200006f4UL                                                 /*!< DIGFILT_LUTS DATA16: VAL Address                             */

/* ======================================================================================== */
/* ================          struct 'DIGFILT2_LUTS'                        ================ */
/* ======================================================================================== */


/* ----------------------------  DIGFILT2_LUTS_DATA0  ---------------------------- */
#define DIGFILT2_LUTS_DATA0_Addr                                     0x20000794UL                                                 /*!< DIGFILT2_LUTS DATA0: Address                                 */
#define pDIGFILT2_LUTS_DATA0                                         (*(volatile uint32_t *) DIGFILT2_LUTS_DATA0_Addr)            /*!< DIGFILT2_LUTS DATA0: Pointer                                 */
#define DIGFILT2_LUTS_DATA0_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT2_LUTS DATA0: Mask                                    */
#define DIGFILT2_LUTS_DATA0_Rst                                      0x00000000UL                                                 /*!< DIGFILT2_LUTS DATA0: Reset                                   */

#define DIGFILT2_LUTS_DATA0_VAL_Size                                 32                                                           /*!< DIGFILT2_LUTS DATA0: VAL Size                                */
#define DIGFILT2_LUTS_DATA0_VAL_Pos                                  0                                                            /*!< DIGFILT2_LUTS DATA0: VAL Position                            */
#define DIGFILT2_LUTS_DATA0_VAL_Msk                                  (0xffffffffUL << DIGFILT2_LUTS_DATA0_VAL_Pos)                /*!< DIGFILT2_LUTS DATA0: VAL Mask                                */
#define DIGFILT2_LUTS_DATA0_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT2_LUTS DATA0: VAL Reset                               */
#define DIGFILT2_LUTS_DATA0_VAL_Addr                                 0x20000794UL                                                 /*!< DIGFILT2_LUTS DATA0: VAL Address                             */

/* ----------------------------  DIGFILT2_LUTS_DATA1  ---------------------------- */
#define DIGFILT2_LUTS_DATA1_Addr                                     0x20000798UL                                                 /*!< DIGFILT2_LUTS DATA1: Address                                 */
#define pDIGFILT2_LUTS_DATA1                                         (*(volatile uint32_t *) DIGFILT2_LUTS_DATA1_Addr)            /*!< DIGFILT2_LUTS DATA1: Pointer                                 */
#define DIGFILT2_LUTS_DATA1_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT2_LUTS DATA1: Mask                                    */
#define DIGFILT2_LUTS_DATA1_Rst                                      0x00000000UL                                                 /*!< DIGFILT2_LUTS DATA1: Reset                                   */

#define DIGFILT2_LUTS_DATA1_VAL_Size                                 32                                                           /*!< DIGFILT2_LUTS DATA1: VAL Size                                */
#define DIGFILT2_LUTS_DATA1_VAL_Pos                                  0                                                            /*!< DIGFILT2_LUTS DATA1: VAL Position                            */
#define DIGFILT2_LUTS_DATA1_VAL_Msk                                  (0xffffffffUL << DIGFILT2_LUTS_DATA1_VAL_Pos)                /*!< DIGFILT2_LUTS DATA1: VAL Mask                                */
#define DIGFILT2_LUTS_DATA1_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT2_LUTS DATA1: VAL Reset                               */
#define DIGFILT2_LUTS_DATA1_VAL_Addr                                 0x20000798UL                                                 /*!< DIGFILT2_LUTS DATA1: VAL Address                             */

/* ----------------------------  DIGFILT2_LUTS_DATA2  ---------------------------- */
#define DIGFILT2_LUTS_DATA2_Addr                                     0x2000079CUL                                                 /*!< DIGFILT2_LUTS DATA2: Address                                 */
#define pDIGFILT2_LUTS_DATA2                                         (*(volatile uint32_t *) DIGFILT2_LUTS_DATA2_Addr)            /*!< DIGFILT2_LUTS DATA2: Pointer                                 */
#define DIGFILT2_LUTS_DATA2_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT2_LUTS DATA2: Mask                                    */
#define DIGFILT2_LUTS_DATA2_Rst                                      0x00000000UL                                                 /*!< DIGFILT2_LUTS DATA2: Reset                                   */

#define DIGFILT2_LUTS_DATA2_VAL_Size                                 32                                                           /*!< DIGFILT2_LUTS DATA2: VAL Size                                */
#define DIGFILT2_LUTS_DATA2_VAL_Pos                                  0                                                            /*!< DIGFILT2_LUTS DATA2: VAL Position                            */
#define DIGFILT2_LUTS_DATA2_VAL_Msk                                  (0xffffffffUL << DIGFILT2_LUTS_DATA2_VAL_Pos)                /*!< DIGFILT2_LUTS DATA2: VAL Mask                                */
#define DIGFILT2_LUTS_DATA2_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT2_LUTS DATA2: VAL Reset                               */
#define DIGFILT2_LUTS_DATA2_VAL_Addr                                 0x2000079cUL                                                 /*!< DIGFILT2_LUTS DATA2: VAL Address                             */

/* ----------------------------  DIGFILT2_LUTS_DATA3  ---------------------------- */
#define DIGFILT2_LUTS_DATA3_Addr                                     0x200007A0UL                                                 /*!< DIGFILT2_LUTS DATA3: Address                                 */
#define pDIGFILT2_LUTS_DATA3                                         (*(volatile uint32_t *) DIGFILT2_LUTS_DATA3_Addr)            /*!< DIGFILT2_LUTS DATA3: Pointer                                 */
#define DIGFILT2_LUTS_DATA3_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT2_LUTS DATA3: Mask                                    */
#define DIGFILT2_LUTS_DATA3_Rst                                      0x00000000UL                                                 /*!< DIGFILT2_LUTS DATA3: Reset                                   */

#define DIGFILT2_LUTS_DATA3_VAL_Size                                 32                                                           /*!< DIGFILT2_LUTS DATA3: VAL Size                                */
#define DIGFILT2_LUTS_DATA3_VAL_Pos                                  0                                                            /*!< DIGFILT2_LUTS DATA3: VAL Position                            */
#define DIGFILT2_LUTS_DATA3_VAL_Msk                                  (0xffffffffUL << DIGFILT2_LUTS_DATA3_VAL_Pos)                /*!< DIGFILT2_LUTS DATA3: VAL Mask                                */
#define DIGFILT2_LUTS_DATA3_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT2_LUTS DATA3: VAL Reset                               */
#define DIGFILT2_LUTS_DATA3_VAL_Addr                                 0x200007a0UL                                                 /*!< DIGFILT2_LUTS DATA3: VAL Address                             */

/* ----------------------------  DIGFILT2_LUTS_DATA4  ---------------------------- */
#define DIGFILT2_LUTS_DATA4_Addr                                     0x200007A4UL                                                 /*!< DIGFILT2_LUTS DATA4: Address                                 */
#define pDIGFILT2_LUTS_DATA4                                         (*(volatile uint32_t *) DIGFILT2_LUTS_DATA4_Addr)            /*!< DIGFILT2_LUTS DATA4: Pointer                                 */
#define DIGFILT2_LUTS_DATA4_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT2_LUTS DATA4: Mask                                    */
#define DIGFILT2_LUTS_DATA4_Rst                                      0x00000000UL                                                 /*!< DIGFILT2_LUTS DATA4: Reset                                   */

#define DIGFILT2_LUTS_DATA4_VAL_Size                                 32                                                           /*!< DIGFILT2_LUTS DATA4: VAL Size                                */
#define DIGFILT2_LUTS_DATA4_VAL_Pos                                  0                                                            /*!< DIGFILT2_LUTS DATA4: VAL Position                            */
#define DIGFILT2_LUTS_DATA4_VAL_Msk                                  (0xffffffffUL << DIGFILT2_LUTS_DATA4_VAL_Pos)                /*!< DIGFILT2_LUTS DATA4: VAL Mask                                */
#define DIGFILT2_LUTS_DATA4_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT2_LUTS DATA4: VAL Reset                               */
#define DIGFILT2_LUTS_DATA4_VAL_Addr                                 0x200007a4UL                                                 /*!< DIGFILT2_LUTS DATA4: VAL Address                             */

/* ----------------------------  DIGFILT2_LUTS_DATA5  ---------------------------- */
#define DIGFILT2_LUTS_DATA5_Addr                                     0x200007A8UL                                                 /*!< DIGFILT2_LUTS DATA5: Address                                 */
#define pDIGFILT2_LUTS_DATA5                                         (*(volatile uint32_t *) DIGFILT2_LUTS_DATA5_Addr)            /*!< DIGFILT2_LUTS DATA5: Pointer                                 */
#define DIGFILT2_LUTS_DATA5_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT2_LUTS DATA5: Mask                                    */
#define DIGFILT2_LUTS_DATA5_Rst                                      0x00000000UL                                                 /*!< DIGFILT2_LUTS DATA5: Reset                                   */

#define DIGFILT2_LUTS_DATA5_VAL_Size                                 32                                                           /*!< DIGFILT2_LUTS DATA5: VAL Size                                */
#define DIGFILT2_LUTS_DATA5_VAL_Pos                                  0                                                            /*!< DIGFILT2_LUTS DATA5: VAL Position                            */
#define DIGFILT2_LUTS_DATA5_VAL_Msk                                  (0xffffffffUL << DIGFILT2_LUTS_DATA5_VAL_Pos)                /*!< DIGFILT2_LUTS DATA5: VAL Mask                                */
#define DIGFILT2_LUTS_DATA5_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT2_LUTS DATA5: VAL Reset                               */
#define DIGFILT2_LUTS_DATA5_VAL_Addr                                 0x200007a8UL                                                 /*!< DIGFILT2_LUTS DATA5: VAL Address                             */

/* ----------------------------  DIGFILT2_LUTS_DATA6  ---------------------------- */
#define DIGFILT2_LUTS_DATA6_Addr                                     0x200007ACUL                                                 /*!< DIGFILT2_LUTS DATA6: Address                                 */
#define pDIGFILT2_LUTS_DATA6                                         (*(volatile uint32_t *) DIGFILT2_LUTS_DATA6_Addr)            /*!< DIGFILT2_LUTS DATA6: Pointer                                 */
#define DIGFILT2_LUTS_DATA6_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT2_LUTS DATA6: Mask                                    */
#define DIGFILT2_LUTS_DATA6_Rst                                      0x00000000UL                                                 /*!< DIGFILT2_LUTS DATA6: Reset                                   */

#define DIGFILT2_LUTS_DATA6_VAL_Size                                 32                                                           /*!< DIGFILT2_LUTS DATA6: VAL Size                                */
#define DIGFILT2_LUTS_DATA6_VAL_Pos                                  0                                                            /*!< DIGFILT2_LUTS DATA6: VAL Position                            */
#define DIGFILT2_LUTS_DATA6_VAL_Msk                                  (0xffffffffUL << DIGFILT2_LUTS_DATA6_VAL_Pos)                /*!< DIGFILT2_LUTS DATA6: VAL Mask                                */
#define DIGFILT2_LUTS_DATA6_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT2_LUTS DATA6: VAL Reset                               */
#define DIGFILT2_LUTS_DATA6_VAL_Addr                                 0x200007acUL                                                 /*!< DIGFILT2_LUTS DATA6: VAL Address                             */

/* ----------------------------  DIGFILT2_LUTS_DATA7  ---------------------------- */
#define DIGFILT2_LUTS_DATA7_Addr                                     0x200007B0UL                                                 /*!< DIGFILT2_LUTS DATA7: Address                                 */
#define pDIGFILT2_LUTS_DATA7                                         (*(volatile uint32_t *) DIGFILT2_LUTS_DATA7_Addr)            /*!< DIGFILT2_LUTS DATA7: Pointer                                 */
#define DIGFILT2_LUTS_DATA7_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT2_LUTS DATA7: Mask                                    */
#define DIGFILT2_LUTS_DATA7_Rst                                      0x00000000UL                                                 /*!< DIGFILT2_LUTS DATA7: Reset                                   */

#define DIGFILT2_LUTS_DATA7_VAL_Size                                 32                                                           /*!< DIGFILT2_LUTS DATA7: VAL Size                                */
#define DIGFILT2_LUTS_DATA7_VAL_Pos                                  0                                                            /*!< DIGFILT2_LUTS DATA7: VAL Position                            */
#define DIGFILT2_LUTS_DATA7_VAL_Msk                                  (0xffffffffUL << DIGFILT2_LUTS_DATA7_VAL_Pos)                /*!< DIGFILT2_LUTS DATA7: VAL Mask                                */
#define DIGFILT2_LUTS_DATA7_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT2_LUTS DATA7: VAL Reset                               */
#define DIGFILT2_LUTS_DATA7_VAL_Addr                                 0x200007b0UL                                                 /*!< DIGFILT2_LUTS DATA7: VAL Address                             */

/* ----------------------------  DIGFILT2_LUTS_DATA8  ---------------------------- */
#define DIGFILT2_LUTS_DATA8_Addr                                     0x200007B4UL                                                 /*!< DIGFILT2_LUTS DATA8: Address                                 */
#define pDIGFILT2_LUTS_DATA8                                         (*(volatile uint32_t *) DIGFILT2_LUTS_DATA8_Addr)            /*!< DIGFILT2_LUTS DATA8: Pointer                                 */
#define DIGFILT2_LUTS_DATA8_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT2_LUTS DATA8: Mask                                    */
#define DIGFILT2_LUTS_DATA8_Rst                                      0x00000000UL                                                 /*!< DIGFILT2_LUTS DATA8: Reset                                   */

#define DIGFILT2_LUTS_DATA8_VAL_Size                                 32                                                           /*!< DIGFILT2_LUTS DATA8: VAL Size                                */
#define DIGFILT2_LUTS_DATA8_VAL_Pos                                  0                                                            /*!< DIGFILT2_LUTS DATA8: VAL Position                            */
#define DIGFILT2_LUTS_DATA8_VAL_Msk                                  (0xffffffffUL << DIGFILT2_LUTS_DATA8_VAL_Pos)                /*!< DIGFILT2_LUTS DATA8: VAL Mask                                */
#define DIGFILT2_LUTS_DATA8_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT2_LUTS DATA8: VAL Reset                               */
#define DIGFILT2_LUTS_DATA8_VAL_Addr                                 0x200007b4UL                                                 /*!< DIGFILT2_LUTS DATA8: VAL Address                             */

/* ----------------------------  DIGFILT2_LUTS_DATA9  ---------------------------- */
#define DIGFILT2_LUTS_DATA9_Addr                                     0x200007B8UL                                                 /*!< DIGFILT2_LUTS DATA9: Address                                 */
#define pDIGFILT2_LUTS_DATA9                                         (*(volatile uint32_t *) DIGFILT2_LUTS_DATA9_Addr)            /*!< DIGFILT2_LUTS DATA9: Pointer                                 */
#define DIGFILT2_LUTS_DATA9_Msk                                      0xFFFFFFFFUL                                                 /*!< DIGFILT2_LUTS DATA9: Mask                                    */
#define DIGFILT2_LUTS_DATA9_Rst                                      0x00000000UL                                                 /*!< DIGFILT2_LUTS DATA9: Reset                                   */

#define DIGFILT2_LUTS_DATA9_VAL_Size                                 32                                                           /*!< DIGFILT2_LUTS DATA9: VAL Size                                */
#define DIGFILT2_LUTS_DATA9_VAL_Pos                                  0                                                            /*!< DIGFILT2_LUTS DATA9: VAL Position                            */
#define DIGFILT2_LUTS_DATA9_VAL_Msk                                  (0xffffffffUL << DIGFILT2_LUTS_DATA9_VAL_Pos)                /*!< DIGFILT2_LUTS DATA9: VAL Mask                                */
#define DIGFILT2_LUTS_DATA9_VAL_Rst                                  0x0000UL                                                     /*!< DIGFILT2_LUTS DATA9: VAL Reset                               */
#define DIGFILT2_LUTS_DATA9_VAL_Addr                                 0x200007b8UL                                                 /*!< DIGFILT2_LUTS DATA9: VAL Address                             */

/* ----------------------------  DIGFILT2_LUTS_DATA10  ---------------------------- */
#define DIGFILT2_LUTS_DATA10_Addr                                    0x200007BCUL                                                 /*!< DIGFILT2_LUTS DATA10: Address                                */
#define pDIGFILT2_LUTS_DATA10                                        (*(volatile uint32_t *) DIGFILT2_LUTS_DATA10_Addr)           /*!< DIGFILT2_LUTS DATA10: Pointer                                */
#define DIGFILT2_LUTS_DATA10_Msk                                     0xFFFFFFFFUL                                                 /*!< DIGFILT2_LUTS DATA10: Mask                                   */
#define DIGFILT2_LUTS_DATA10_Rst                                     0x00000000UL                                                 /*!< DIGFILT2_LUTS DATA10: Reset                                  */

#define DIGFILT2_LUTS_DATA10_VAL_Size                                32                                                           /*!< DIGFILT2_LUTS DATA10: VAL Size                               */
#define DIGFILT2_LUTS_DATA10_VAL_Pos                                 0                                                            /*!< DIGFILT2_LUTS DATA10: VAL Position                           */
#define DIGFILT2_LUTS_DATA10_VAL_Msk                                 (0xffffffffUL << DIGFILT2_LUTS_DATA10_VAL_Pos)               /*!< DIGFILT2_LUTS DATA10: VAL Mask                               */
#define DIGFILT2_LUTS_DATA10_VAL_Rst                                 0x0000UL                                                     /*!< DIGFILT2_LUTS DATA10: VAL Reset                              */
#define DIGFILT2_LUTS_DATA10_VAL_Addr                                0x200007bcUL                                                 /*!< DIGFILT2_LUTS DATA10: VAL Address                            */

/* ======================================================================================== */
/* ================          struct 'PLLBW_LUTS'                           ================ */
/* ======================================================================================== */


/* ----------------------------  PLLBW_LUTS_DATA0  ---------------------------- */
#define PLLBW_LUTS_DATA0_Addr                                        0x20000820UL                                                 /*!< PLLBW_LUTS DATA0: Address                                    */
#define pPLLBW_LUTS_DATA0                                            (*(volatile uint32_t *) PLLBW_LUTS_DATA0_Addr)               /*!< PLLBW_LUTS DATA0: Pointer                                    */
#define PLLBW_LUTS_DATA0_Msk                                         0xFFFFFFFFUL                                                 /*!< PLLBW_LUTS DATA0: Mask                                       */
#define PLLBW_LUTS_DATA0_Rst                                         0x00000000UL                                                 /*!< PLLBW_LUTS DATA0: Reset                                      */

#define PLLBW_LUTS_DATA0_VAL_Size                                    32                                                           /*!< PLLBW_LUTS DATA0: VAL Size                                   */
#define PLLBW_LUTS_DATA0_VAL_Pos                                     0                                                            /*!< PLLBW_LUTS DATA0: VAL Position                               */
#define PLLBW_LUTS_DATA0_VAL_Msk                                     (0xffffffffUL << PLLBW_LUTS_DATA0_VAL_Pos)                   /*!< PLLBW_LUTS DATA0: VAL Mask                                   */
#define PLLBW_LUTS_DATA0_VAL_Rst                                     0x0000UL                                                     /*!< PLLBW_LUTS DATA0: VAL Reset                                  */
#define PLLBW_LUTS_DATA0_VAL_Addr                                    0x20000820UL                                                 /*!< PLLBW_LUTS DATA0: VAL Address                                */

/* ----------------------------  PLLBW_LUTS_DATA1  ---------------------------- */
#define PLLBW_LUTS_DATA1_Addr                                        0x20000824UL                                                 /*!< PLLBW_LUTS DATA1: Address                                    */
#define pPLLBW_LUTS_DATA1                                            (*(volatile uint32_t *) PLLBW_LUTS_DATA1_Addr)               /*!< PLLBW_LUTS DATA1: Pointer                                    */
#define PLLBW_LUTS_DATA1_Msk                                         0xFFFFFFFFUL                                                 /*!< PLLBW_LUTS DATA1: Mask                                       */
#define PLLBW_LUTS_DATA1_Rst                                         0x00000000UL                                                 /*!< PLLBW_LUTS DATA1: Reset                                      */

#define PLLBW_LUTS_DATA1_VAL_Size                                    32                                                           /*!< PLLBW_LUTS DATA1: VAL Size                                   */
#define PLLBW_LUTS_DATA1_VAL_Pos                                     0                                                            /*!< PLLBW_LUTS DATA1: VAL Position                               */
#define PLLBW_LUTS_DATA1_VAL_Msk                                     (0xffffffffUL << PLLBW_LUTS_DATA1_VAL_Pos)                   /*!< PLLBW_LUTS DATA1: VAL Mask                                   */
#define PLLBW_LUTS_DATA1_VAL_Rst                                     0x0000UL                                                     /*!< PLLBW_LUTS DATA1: VAL Reset                                  */
#define PLLBW_LUTS_DATA1_VAL_Addr                                    0x20000824UL                                                 /*!< PLLBW_LUTS DATA1: VAL Address                                */

/* ======================================================================================== */
/* ================          struct 'VCO_CAL_RESULTS'                      ================ */
/* ======================================================================================== */


/* ----------------------------  VCO_CAL_RESULTS_DATA0  ---------------------------- */
#define VCO_CAL_RESULTS_DATA0_Addr                                   0x20000844UL                                                 /*!< VCO_CAL_RESULTS DATA0: Address                               */
#define pVCO_CAL_RESULTS_DATA0                                       (*(volatile uint32_t *) VCO_CAL_RESULTS_DATA0_Addr)          /*!< VCO_CAL_RESULTS DATA0: Pointer                               */
#define VCO_CAL_RESULTS_DATA0_Msk                                    0xFFFFFFFFUL                                                 /*!< VCO_CAL_RESULTS DATA0: Mask                                  */
#define VCO_CAL_RESULTS_DATA0_Rst                                    0x00000000UL                                                 /*!< VCO_CAL_RESULTS DATA0: Reset                                 */

#define VCO_CAL_RESULTS_DATA0_VAL_Size                               32                                                           /*!< VCO_CAL_RESULTS DATA0: VAL Size                              */
#define VCO_CAL_RESULTS_DATA0_VAL_Pos                                0                                                            /*!< VCO_CAL_RESULTS DATA0: VAL Position                          */
#define VCO_CAL_RESULTS_DATA0_VAL_Msk                                (0xffffffffUL << VCO_CAL_RESULTS_DATA0_VAL_Pos)              /*!< VCO_CAL_RESULTS DATA0: VAL Mask                              */
#define VCO_CAL_RESULTS_DATA0_VAL_Rst                                0x0000UL                                                     /*!< VCO_CAL_RESULTS DATA0: VAL Reset                             */
#define VCO_CAL_RESULTS_DATA0_VAL_Addr                               0x20000844UL                                                 /*!< VCO_CAL_RESULTS DATA0: VAL Address                           */

/* ----------------------------  VCO_CAL_RESULTS_DATA1  ---------------------------- */
#define VCO_CAL_RESULTS_DATA1_Addr                                   0x20000848UL                                                 /*!< VCO_CAL_RESULTS DATA1: Address                               */
#define pVCO_CAL_RESULTS_DATA1                                       (*(volatile uint32_t *) VCO_CAL_RESULTS_DATA1_Addr)          /*!< VCO_CAL_RESULTS DATA1: Pointer                               */
#define VCO_CAL_RESULTS_DATA1_Msk                                    0xFFFFFFFFUL                                                 /*!< VCO_CAL_RESULTS DATA1: Mask                                  */
#define VCO_CAL_RESULTS_DATA1_Rst                                    0x00000000UL                                                 /*!< VCO_CAL_RESULTS DATA1: Reset                                 */

#define VCO_CAL_RESULTS_DATA1_VAL_Size                               32                                                           /*!< VCO_CAL_RESULTS DATA1: VAL Size                              */
#define VCO_CAL_RESULTS_DATA1_VAL_Pos                                0                                                            /*!< VCO_CAL_RESULTS DATA1: VAL Position                          */
#define VCO_CAL_RESULTS_DATA1_VAL_Msk                                (0xffffffffUL << VCO_CAL_RESULTS_DATA1_VAL_Pos)              /*!< VCO_CAL_RESULTS DATA1: VAL Mask                              */
#define VCO_CAL_RESULTS_DATA1_VAL_Rst                                0x0000UL                                                     /*!< VCO_CAL_RESULTS DATA1: VAL Reset                             */
#define VCO_CAL_RESULTS_DATA1_VAL_Addr                               0x20000848UL                                                 /*!< VCO_CAL_RESULTS DATA1: VAL Address                           */

/* ----------------------------  VCO_CAL_RESULTS_DATA2  ---------------------------- */
#define VCO_CAL_RESULTS_DATA2_Addr                                   0x2000084CUL                                                 /*!< VCO_CAL_RESULTS DATA2: Address                               */
#define pVCO_CAL_RESULTS_DATA2                                       (*(volatile uint32_t *) VCO_CAL_RESULTS_DATA2_Addr)          /*!< VCO_CAL_RESULTS DATA2: Pointer                               */
#define VCO_CAL_RESULTS_DATA2_Msk                                    0xFFFFFFFFUL                                                 /*!< VCO_CAL_RESULTS DATA2: Mask                                  */
#define VCO_CAL_RESULTS_DATA2_Rst                                    0x00000000UL                                                 /*!< VCO_CAL_RESULTS DATA2: Reset                                 */

#define VCO_CAL_RESULTS_DATA2_VAL_Size                               32                                                           /*!< VCO_CAL_RESULTS DATA2: VAL Size                              */
#define VCO_CAL_RESULTS_DATA2_VAL_Pos                                0                                                            /*!< VCO_CAL_RESULTS DATA2: VAL Position                          */
#define VCO_CAL_RESULTS_DATA2_VAL_Msk                                (0xffffffffUL << VCO_CAL_RESULTS_DATA2_VAL_Pos)              /*!< VCO_CAL_RESULTS DATA2: VAL Mask                              */
#define VCO_CAL_RESULTS_DATA2_VAL_Rst                                0x0000UL                                                     /*!< VCO_CAL_RESULTS DATA2: VAL Reset                             */
#define VCO_CAL_RESULTS_DATA2_VAL_Addr                               0x2000084cUL                                                 /*!< VCO_CAL_RESULTS DATA2: VAL Address                           */

/* ----------------------------  VCO_CAL_RESULTS_DATA3  ---------------------------- */
#define VCO_CAL_RESULTS_DATA3_Addr                                   0x20000850UL                                                 /*!< VCO_CAL_RESULTS DATA3: Address                               */
#define pVCO_CAL_RESULTS_DATA3                                       (*(volatile uint32_t *) VCO_CAL_RESULTS_DATA3_Addr)          /*!< VCO_CAL_RESULTS DATA3: Pointer                               */
#define VCO_CAL_RESULTS_DATA3_Msk                                    0xFFFFFFFFUL                                                 /*!< VCO_CAL_RESULTS DATA3: Mask                                  */
#define VCO_CAL_RESULTS_DATA3_Rst                                    0x00000000UL                                                 /*!< VCO_CAL_RESULTS DATA3: Reset                                 */

#define VCO_CAL_RESULTS_DATA3_VAL_Size                               32                                                           /*!< VCO_CAL_RESULTS DATA3: VAL Size                              */
#define VCO_CAL_RESULTS_DATA3_VAL_Pos                                0                                                            /*!< VCO_CAL_RESULTS DATA3: VAL Position                          */
#define VCO_CAL_RESULTS_DATA3_VAL_Msk                                (0xffffffffUL << VCO_CAL_RESULTS_DATA3_VAL_Pos)              /*!< VCO_CAL_RESULTS DATA3: VAL Mask                              */
#define VCO_CAL_RESULTS_DATA3_VAL_Rst                                0x0000UL                                                     /*!< VCO_CAL_RESULTS DATA3: VAL Reset                             */
#define VCO_CAL_RESULTS_DATA3_VAL_Addr                               0x20000850UL                                                 /*!< VCO_CAL_RESULTS DATA3: VAL Address                           */

/* ----------------------------  VCO_CAL_RESULTS_DATA4  ---------------------------- */
#define VCO_CAL_RESULTS_DATA4_Addr                                   0x20000854UL                                                 /*!< VCO_CAL_RESULTS DATA4: Address                               */
#define pVCO_CAL_RESULTS_DATA4                                       (*(volatile uint32_t *) VCO_CAL_RESULTS_DATA4_Addr)          /*!< VCO_CAL_RESULTS DATA4: Pointer                               */
#define VCO_CAL_RESULTS_DATA4_Msk                                    0xFFFFFFFFUL                                                 /*!< VCO_CAL_RESULTS DATA4: Mask                                  */
#define VCO_CAL_RESULTS_DATA4_Rst                                    0x00000000UL                                                 /*!< VCO_CAL_RESULTS DATA4: Reset                                 */

#define VCO_CAL_RESULTS_DATA4_VAL_Size                               32                                                           /*!< VCO_CAL_RESULTS DATA4: VAL Size                              */
#define VCO_CAL_RESULTS_DATA4_VAL_Pos                                0                                                            /*!< VCO_CAL_RESULTS DATA4: VAL Position                          */
#define VCO_CAL_RESULTS_DATA4_VAL_Msk                                (0xffffffffUL << VCO_CAL_RESULTS_DATA4_VAL_Pos)              /*!< VCO_CAL_RESULTS DATA4: VAL Mask                              */
#define VCO_CAL_RESULTS_DATA4_VAL_Rst                                0x0000UL                                                     /*!< VCO_CAL_RESULTS DATA4: VAL Reset                             */
#define VCO_CAL_RESULTS_DATA4_VAL_Addr                               0x20000854UL                                                 /*!< VCO_CAL_RESULTS DATA4: VAL Address                           */

/* ----------------------------  VCO_CAL_RESULTS_DATA5  ---------------------------- */
#define VCO_CAL_RESULTS_DATA5_Addr                                   0x20000858UL                                                 /*!< VCO_CAL_RESULTS DATA5: Address                               */
#define pVCO_CAL_RESULTS_DATA5                                       (*(volatile uint32_t *) VCO_CAL_RESULTS_DATA5_Addr)          /*!< VCO_CAL_RESULTS DATA5: Pointer                               */
#define VCO_CAL_RESULTS_DATA5_Msk                                    0xFFFFFFFFUL                                                 /*!< VCO_CAL_RESULTS DATA5: Mask                                  */
#define VCO_CAL_RESULTS_DATA5_Rst                                    0x00000000UL                                                 /*!< VCO_CAL_RESULTS DATA5: Reset                                 */

#define VCO_CAL_RESULTS_DATA5_VAL_Size                               32                                                           /*!< VCO_CAL_RESULTS DATA5: VAL Size                              */
#define VCO_CAL_RESULTS_DATA5_VAL_Pos                                0                                                            /*!< VCO_CAL_RESULTS DATA5: VAL Position                          */
#define VCO_CAL_RESULTS_DATA5_VAL_Msk                                (0xffffffffUL << VCO_CAL_RESULTS_DATA5_VAL_Pos)              /*!< VCO_CAL_RESULTS DATA5: VAL Mask                              */
#define VCO_CAL_RESULTS_DATA5_VAL_Rst                                0x0000UL                                                     /*!< VCO_CAL_RESULTS DATA5: VAL Reset                             */
#define VCO_CAL_RESULTS_DATA5_VAL_Addr                               0x20000858UL                                                 /*!< VCO_CAL_RESULTS DATA5: VAL Address                           */

/* ----------------------------  VCO_CAL_RESULTS_DATA6  ---------------------------- */
#define VCO_CAL_RESULTS_DATA6_Addr                                   0x2000085CUL                                                 /*!< VCO_CAL_RESULTS DATA6: Address                               */
#define pVCO_CAL_RESULTS_DATA6                                       (*(volatile uint32_t *) VCO_CAL_RESULTS_DATA6_Addr)          /*!< VCO_CAL_RESULTS DATA6: Pointer                               */
#define VCO_CAL_RESULTS_DATA6_Msk                                    0xFFFFFFFFUL                                                 /*!< VCO_CAL_RESULTS DATA6: Mask                                  */
#define VCO_CAL_RESULTS_DATA6_Rst                                    0x00000000UL                                                 /*!< VCO_CAL_RESULTS DATA6: Reset                                 */

#define VCO_CAL_RESULTS_DATA6_VAL_Size                               32                                                           /*!< VCO_CAL_RESULTS DATA6: VAL Size                              */
#define VCO_CAL_RESULTS_DATA6_VAL_Pos                                0                                                            /*!< VCO_CAL_RESULTS DATA6: VAL Position                          */
#define VCO_CAL_RESULTS_DATA6_VAL_Msk                                (0xffffffffUL << VCO_CAL_RESULTS_DATA6_VAL_Pos)              /*!< VCO_CAL_RESULTS DATA6: VAL Mask                              */
#define VCO_CAL_RESULTS_DATA6_VAL_Rst                                0x0000UL                                                     /*!< VCO_CAL_RESULTS DATA6: VAL Reset                             */
#define VCO_CAL_RESULTS_DATA6_VAL_Addr                               0x2000085cUL                                                 /*!< VCO_CAL_RESULTS DATA6: VAL Address                           */

/* ----------------------------  VCO_CAL_RESULTS_DATA7  ---------------------------- */
#define VCO_CAL_RESULTS_DATA7_Addr                                   0x20000860UL                                                 /*!< VCO_CAL_RESULTS DATA7: Address                               */
#define pVCO_CAL_RESULTS_DATA7                                       (*(volatile uint32_t *) VCO_CAL_RESULTS_DATA7_Addr)          /*!< VCO_CAL_RESULTS DATA7: Pointer                               */
#define VCO_CAL_RESULTS_DATA7_Msk                                    0xFFFFFFFFUL                                                 /*!< VCO_CAL_RESULTS DATA7: Mask                                  */
#define VCO_CAL_RESULTS_DATA7_Rst                                    0x00000000UL                                                 /*!< VCO_CAL_RESULTS DATA7: Reset                                 */

#define VCO_CAL_RESULTS_DATA7_VAL_Size                               32                                                           /*!< VCO_CAL_RESULTS DATA7: VAL Size                              */
#define VCO_CAL_RESULTS_DATA7_VAL_Pos                                0                                                            /*!< VCO_CAL_RESULTS DATA7: VAL Position                          */
#define VCO_CAL_RESULTS_DATA7_VAL_Msk                                (0xffffffffUL << VCO_CAL_RESULTS_DATA7_VAL_Pos)              /*!< VCO_CAL_RESULTS DATA7: VAL Mask                              */
#define VCO_CAL_RESULTS_DATA7_VAL_Rst                                0x0000UL                                                     /*!< VCO_CAL_RESULTS DATA7: VAL Reset                             */
#define VCO_CAL_RESULTS_DATA7_VAL_Addr                               0x20000860UL                                                 /*!< VCO_CAL_RESULTS DATA7: VAL Address                           */

/* ======================================================================================== */
/* ================          struct 'RSSICFG_LUTS'                         ================ */
/* ======================================================================================== */


/* ----------------------------  RSSICFG_LUTS_DATA0  ---------------------------- */
#define RSSICFG_LUTS_DATA0_Addr                                      0x20000864UL                                                 /*!< RSSICFG_LUTS DATA0: Address                                  */
#define pRSSICFG_LUTS_DATA0                                          (*(volatile uint32_t *) RSSICFG_LUTS_DATA0_Addr)             /*!< RSSICFG_LUTS DATA0: Pointer                                  */
#define RSSICFG_LUTS_DATA0_Msk                                       0xFFFFFFFFUL                                                 /*!< RSSICFG_LUTS DATA0: Mask                                     */
#define RSSICFG_LUTS_DATA0_Rst                                       0x00000000UL                                                 /*!< RSSICFG_LUTS DATA0: Reset                                    */

#define RSSICFG_LUTS_DATA0_VAL_Size                                  32                                                           /*!< RSSICFG_LUTS DATA0: VAL Size                                 */
#define RSSICFG_LUTS_DATA0_VAL_Pos                                   0                                                            /*!< RSSICFG_LUTS DATA0: VAL Position                             */
#define RSSICFG_LUTS_DATA0_VAL_Msk                                   (0xffffffffUL << RSSICFG_LUTS_DATA0_VAL_Pos)                 /*!< RSSICFG_LUTS DATA0: VAL Mask                                 */
#define RSSICFG_LUTS_DATA0_VAL_Rst                                   0x0000UL                                                     /*!< RSSICFG_LUTS DATA0: VAL Reset                                */
#define RSSICFG_LUTS_DATA0_VAL_Addr                                  0x20000864UL                                                 /*!< RSSICFG_LUTS DATA0: VAL Address                              */

/* ----------------------------  RSSICFG_LUTS_DATA1  ---------------------------- */
#define RSSICFG_LUTS_DATA1_Addr                                      0x20000868UL                                                 /*!< RSSICFG_LUTS DATA1: Address                                  */
#define pRSSICFG_LUTS_DATA1                                          (*(volatile uint32_t *) RSSICFG_LUTS_DATA1_Addr)             /*!< RSSICFG_LUTS DATA1: Pointer                                  */
#define RSSICFG_LUTS_DATA1_Msk                                       0xFFFFFFFFUL                                                 /*!< RSSICFG_LUTS DATA1: Mask                                     */
#define RSSICFG_LUTS_DATA1_Rst                                       0x00000000UL                                                 /*!< RSSICFG_LUTS DATA1: Reset                                    */

#define RSSICFG_LUTS_DATA1_VAL_Size                                  32                                                           /*!< RSSICFG_LUTS DATA1: VAL Size                                 */
#define RSSICFG_LUTS_DATA1_VAL_Pos                                   0                                                            /*!< RSSICFG_LUTS DATA1: VAL Position                             */
#define RSSICFG_LUTS_DATA1_VAL_Msk                                   (0xffffffffUL << RSSICFG_LUTS_DATA1_VAL_Pos)                 /*!< RSSICFG_LUTS DATA1: VAL Mask                                 */
#define RSSICFG_LUTS_DATA1_VAL_Rst                                   0x0000UL                                                     /*!< RSSICFG_LUTS DATA1: VAL Reset                                */
#define RSSICFG_LUTS_DATA1_VAL_Addr                                  0x20000868UL                                                 /*!< RSSICFG_LUTS DATA1: VAL Address                              */

/* ----------------------------  RSSICFG_LUTS_DATA2  ---------------------------- */
#define RSSICFG_LUTS_DATA2_Addr                                      0x2000086CUL                                                 /*!< RSSICFG_LUTS DATA2: Address                                  */
#define pRSSICFG_LUTS_DATA2                                          (*(volatile uint32_t *) RSSICFG_LUTS_DATA2_Addr)             /*!< RSSICFG_LUTS DATA2: Pointer                                  */
#define RSSICFG_LUTS_DATA2_Msk                                       0xFFFFFFFFUL                                                 /*!< RSSICFG_LUTS DATA2: Mask                                     */
#define RSSICFG_LUTS_DATA2_Rst                                       0x00000000UL                                                 /*!< RSSICFG_LUTS DATA2: Reset                                    */

#define RSSICFG_LUTS_DATA2_VAL_Size                                  32                                                           /*!< RSSICFG_LUTS DATA2: VAL Size                                 */
#define RSSICFG_LUTS_DATA2_VAL_Pos                                   0                                                            /*!< RSSICFG_LUTS DATA2: VAL Position                             */
#define RSSICFG_LUTS_DATA2_VAL_Msk                                   (0xffffffffUL << RSSICFG_LUTS_DATA2_VAL_Pos)                 /*!< RSSICFG_LUTS DATA2: VAL Mask                                 */
#define RSSICFG_LUTS_DATA2_VAL_Rst                                   0x0000UL                                                     /*!< RSSICFG_LUTS DATA2: VAL Reset                                */
#define RSSICFG_LUTS_DATA2_VAL_Addr                                  0x2000086cUL                                                 /*!< RSSICFG_LUTS DATA2: VAL Address                              */

/* ----------------------------  RSSICFG_LUTS_DATA3  ---------------------------- */
#define RSSICFG_LUTS_DATA3_Addr                                      0x20000870UL                                                 /*!< RSSICFG_LUTS DATA3: Address                                  */
#define pRSSICFG_LUTS_DATA3                                          (*(volatile uint32_t *) RSSICFG_LUTS_DATA3_Addr)             /*!< RSSICFG_LUTS DATA3: Pointer                                  */
#define RSSICFG_LUTS_DATA3_Msk                                       0xFFFFFFFFUL                                                 /*!< RSSICFG_LUTS DATA3: Mask                                     */
#define RSSICFG_LUTS_DATA3_Rst                                       0x00000000UL                                                 /*!< RSSICFG_LUTS DATA3: Reset                                    */

#define RSSICFG_LUTS_DATA3_VAL_Size                                  32                                                           /*!< RSSICFG_LUTS DATA3: VAL Size                                 */
#define RSSICFG_LUTS_DATA3_VAL_Pos                                   0                                                            /*!< RSSICFG_LUTS DATA3: VAL Position                             */
#define RSSICFG_LUTS_DATA3_VAL_Msk                                   (0xffffffffUL << RSSICFG_LUTS_DATA3_VAL_Pos)                 /*!< RSSICFG_LUTS DATA3: VAL Mask                                 */
#define RSSICFG_LUTS_DATA3_VAL_Rst                                   0x0000UL                                                     /*!< RSSICFG_LUTS DATA3: VAL Reset                                */
#define RSSICFG_LUTS_DATA3_VAL_Addr                                  0x20000870UL                                                 /*!< RSSICFG_LUTS DATA3: VAL Address                              */

/* ----------------------------  RSSICFG_LUTS_DATA4  ---------------------------- */
#define RSSICFG_LUTS_DATA4_Addr                                      0x20000874UL                                                 /*!< RSSICFG_LUTS DATA4: Address                                  */
#define pRSSICFG_LUTS_DATA4                                          (*(volatile uint32_t *) RSSICFG_LUTS_DATA4_Addr)             /*!< RSSICFG_LUTS DATA4: Pointer                                  */
#define RSSICFG_LUTS_DATA4_Msk                                       0xFFFFFFFFUL                                                 /*!< RSSICFG_LUTS DATA4: Mask                                     */
#define RSSICFG_LUTS_DATA4_Rst                                       0x00000000UL                                                 /*!< RSSICFG_LUTS DATA4: Reset                                    */

#define RSSICFG_LUTS_DATA4_VAL_Size                                  32                                                           /*!< RSSICFG_LUTS DATA4: VAL Size                                 */
#define RSSICFG_LUTS_DATA4_VAL_Pos                                   0                                                            /*!< RSSICFG_LUTS DATA4: VAL Position                             */
#define RSSICFG_LUTS_DATA4_VAL_Msk                                   (0xffffffffUL << RSSICFG_LUTS_DATA4_VAL_Pos)                 /*!< RSSICFG_LUTS DATA4: VAL Mask                                 */
#define RSSICFG_LUTS_DATA4_VAL_Rst                                   0x0000UL                                                     /*!< RSSICFG_LUTS DATA4: VAL Reset                                */
#define RSSICFG_LUTS_DATA4_VAL_Addr                                  0x20000874UL                                                 /*!< RSSICFG_LUTS DATA4: VAL Address                              */

/* ----------------------------  RSSICFG_LUTS_DATA5  ---------------------------- */
#define RSSICFG_LUTS_DATA5_Addr                                      0x20000878UL                                                 /*!< RSSICFG_LUTS DATA5: Address                                  */
#define pRSSICFG_LUTS_DATA5                                          (*(volatile uint32_t *) RSSICFG_LUTS_DATA5_Addr)             /*!< RSSICFG_LUTS DATA5: Pointer                                  */
#define RSSICFG_LUTS_DATA5_Msk                                       0xFFFFFFFFUL                                                 /*!< RSSICFG_LUTS DATA5: Mask                                     */
#define RSSICFG_LUTS_DATA5_Rst                                       0x00000000UL                                                 /*!< RSSICFG_LUTS DATA5: Reset                                    */

#define RSSICFG_LUTS_DATA5_VAL_Size                                  32                                                           /*!< RSSICFG_LUTS DATA5: VAL Size                                 */
#define RSSICFG_LUTS_DATA5_VAL_Pos                                   0                                                            /*!< RSSICFG_LUTS DATA5: VAL Position                             */
#define RSSICFG_LUTS_DATA5_VAL_Msk                                   (0xffffffffUL << RSSICFG_LUTS_DATA5_VAL_Pos)                 /*!< RSSICFG_LUTS DATA5: VAL Mask                                 */
#define RSSICFG_LUTS_DATA5_VAL_Rst                                   0x0000UL                                                     /*!< RSSICFG_LUTS DATA5: VAL Reset                                */
#define RSSICFG_LUTS_DATA5_VAL_Addr                                  0x20000878UL                                                 /*!< RSSICFG_LUTS DATA5: VAL Address                              */

/* ----------------------------  RSSICFG_LUTS_DATA6  ---------------------------- */
#define RSSICFG_LUTS_DATA6_Addr                                      0x2000087CUL                                                 /*!< RSSICFG_LUTS DATA6: Address                                  */
#define pRSSICFG_LUTS_DATA6                                          (*(volatile uint32_t *) RSSICFG_LUTS_DATA6_Addr)             /*!< RSSICFG_LUTS DATA6: Pointer                                  */
#define RSSICFG_LUTS_DATA6_Msk                                       0xFFFFFFFFUL                                                 /*!< RSSICFG_LUTS DATA6: Mask                                     */
#define RSSICFG_LUTS_DATA6_Rst                                       0x00000000UL                                                 /*!< RSSICFG_LUTS DATA6: Reset                                    */

#define RSSICFG_LUTS_DATA6_VAL_Size                                  32                                                           /*!< RSSICFG_LUTS DATA6: VAL Size                                 */
#define RSSICFG_LUTS_DATA6_VAL_Pos                                   0                                                            /*!< RSSICFG_LUTS DATA6: VAL Position                             */
#define RSSICFG_LUTS_DATA6_VAL_Msk                                   (0xffffffffUL << RSSICFG_LUTS_DATA6_VAL_Pos)                 /*!< RSSICFG_LUTS DATA6: VAL Mask                                 */
#define RSSICFG_LUTS_DATA6_VAL_Rst                                   0x0000UL                                                     /*!< RSSICFG_LUTS DATA6: VAL Reset                                */
#define RSSICFG_LUTS_DATA6_VAL_Addr                                  0x2000087cUL                                                 /*!< RSSICFG_LUTS DATA6: VAL Address                              */

/* ----------------------------  RSSICFG_LUTS_DATA7  ---------------------------- */
#define RSSICFG_LUTS_DATA7_Addr                                      0x20000880UL                                                 /*!< RSSICFG_LUTS DATA7: Address                                  */
#define pRSSICFG_LUTS_DATA7                                          (*(volatile uint32_t *) RSSICFG_LUTS_DATA7_Addr)             /*!< RSSICFG_LUTS DATA7: Pointer                                  */
#define RSSICFG_LUTS_DATA7_Msk                                       0xFFFFFFFFUL                                                 /*!< RSSICFG_LUTS DATA7: Mask                                     */
#define RSSICFG_LUTS_DATA7_Rst                                       0x00000000UL                                                 /*!< RSSICFG_LUTS DATA7: Reset                                    */

#define RSSICFG_LUTS_DATA7_VAL_Size                                  32                                                           /*!< RSSICFG_LUTS DATA7: VAL Size                                 */
#define RSSICFG_LUTS_DATA7_VAL_Pos                                   0                                                            /*!< RSSICFG_LUTS DATA7: VAL Position                             */
#define RSSICFG_LUTS_DATA7_VAL_Msk                                   (0xffffffffUL << RSSICFG_LUTS_DATA7_VAL_Pos)                 /*!< RSSICFG_LUTS DATA7: VAL Mask                                 */
#define RSSICFG_LUTS_DATA7_VAL_Rst                                   0x0000UL                                                     /*!< RSSICFG_LUTS DATA7: VAL Reset                                */
#define RSSICFG_LUTS_DATA7_VAL_Addr                                  0x20000880UL                                                 /*!< RSSICFG_LUTS DATA7: VAL Address                              */

/* ----------------------------  RSSICFG_LUTS_DATA8  ---------------------------- */
#define RSSICFG_LUTS_DATA8_Addr                                      0x20000884UL                                                 /*!< RSSICFG_LUTS DATA8: Address                                  */
#define pRSSICFG_LUTS_DATA8                                          (*(volatile uint32_t *) RSSICFG_LUTS_DATA8_Addr)             /*!< RSSICFG_LUTS DATA8: Pointer                                  */
#define RSSICFG_LUTS_DATA8_Msk                                       0xFFFFFFFFUL                                                 /*!< RSSICFG_LUTS DATA8: Mask                                     */
#define RSSICFG_LUTS_DATA8_Rst                                       0x00000000UL                                                 /*!< RSSICFG_LUTS DATA8: Reset                                    */

#define RSSICFG_LUTS_DATA8_VAL_Size                                  32                                                           /*!< RSSICFG_LUTS DATA8: VAL Size                                 */
#define RSSICFG_LUTS_DATA8_VAL_Pos                                   0                                                            /*!< RSSICFG_LUTS DATA8: VAL Position                             */
#define RSSICFG_LUTS_DATA8_VAL_Msk                                   (0xffffffffUL << RSSICFG_LUTS_DATA8_VAL_Pos)                 /*!< RSSICFG_LUTS DATA8: VAL Mask                                 */
#define RSSICFG_LUTS_DATA8_VAL_Rst                                   0x0000UL                                                     /*!< RSSICFG_LUTS DATA8: VAL Reset                                */
#define RSSICFG_LUTS_DATA8_VAL_Addr                                  0x20000884UL                                                 /*!< RSSICFG_LUTS DATA8: VAL Address                              */

/* ----------------------------  RSSICFG_LUTS_DATA9  ---------------------------- */
#define RSSICFG_LUTS_DATA9_Addr                                      0x20000888UL                                                 /*!< RSSICFG_LUTS DATA9: Address                                  */
#define pRSSICFG_LUTS_DATA9                                          (*(volatile uint32_t *) RSSICFG_LUTS_DATA9_Addr)             /*!< RSSICFG_LUTS DATA9: Pointer                                  */
#define RSSICFG_LUTS_DATA9_Msk                                       0xFFFFFFFFUL                                                 /*!< RSSICFG_LUTS DATA9: Mask                                     */
#define RSSICFG_LUTS_DATA9_Rst                                       0x00000000UL                                                 /*!< RSSICFG_LUTS DATA9: Reset                                    */

#define RSSICFG_LUTS_DATA9_VAL_Size                                  32                                                           /*!< RSSICFG_LUTS DATA9: VAL Size                                 */
#define RSSICFG_LUTS_DATA9_VAL_Pos                                   0                                                            /*!< RSSICFG_LUTS DATA9: VAL Position                             */
#define RSSICFG_LUTS_DATA9_VAL_Msk                                   (0xffffffffUL << RSSICFG_LUTS_DATA9_VAL_Pos)                 /*!< RSSICFG_LUTS DATA9: VAL Mask                                 */
#define RSSICFG_LUTS_DATA9_VAL_Rst                                   0x0000UL                                                     /*!< RSSICFG_LUTS DATA9: VAL Reset                                */
#define RSSICFG_LUTS_DATA9_VAL_Addr                                  0x20000888UL                                                 /*!< RSSICFG_LUTS DATA9: VAL Address                              */

/* ----------------------------  RSSICFG_LUTS_DATA10  ---------------------------- */
#define RSSICFG_LUTS_DATA10_Addr                                     0x2000088CUL                                                 /*!< RSSICFG_LUTS DATA10: Address                                 */
#define pRSSICFG_LUTS_DATA10                                         (*(volatile uint32_t *) RSSICFG_LUTS_DATA10_Addr)            /*!< RSSICFG_LUTS DATA10: Pointer                                 */
#define RSSICFG_LUTS_DATA10_Msk                                      0xFFFFFFFFUL                                                 /*!< RSSICFG_LUTS DATA10: Mask                                    */
#define RSSICFG_LUTS_DATA10_Rst                                      0x00000000UL                                                 /*!< RSSICFG_LUTS DATA10: Reset                                   */

#define RSSICFG_LUTS_DATA10_VAL_Size                                 32                                                           /*!< RSSICFG_LUTS DATA10: VAL Size                                */
#define RSSICFG_LUTS_DATA10_VAL_Pos                                  0                                                            /*!< RSSICFG_LUTS DATA10: VAL Position                            */
#define RSSICFG_LUTS_DATA10_VAL_Msk                                  (0xffffffffUL << RSSICFG_LUTS_DATA10_VAL_Pos)                /*!< RSSICFG_LUTS DATA10: VAL Mask                                */
#define RSSICFG_LUTS_DATA10_VAL_Rst                                  0x0000UL                                                     /*!< RSSICFG_LUTS DATA10: VAL Reset                               */
#define RSSICFG_LUTS_DATA10_VAL_Addr                                 0x2000088cUL                                                 /*!< RSSICFG_LUTS DATA10: VAL Address                             */

/* ----------------------------  RSSICFG_LUTS_DATA11  ---------------------------- */
#define RSSICFG_LUTS_DATA11_Addr                                     0x20000890UL                                                 /*!< RSSICFG_LUTS DATA11: Address                                 */
#define pRSSICFG_LUTS_DATA11                                         (*(volatile uint32_t *) RSSICFG_LUTS_DATA11_Addr)            /*!< RSSICFG_LUTS DATA11: Pointer                                 */
#define RSSICFG_LUTS_DATA11_Msk                                      0xFFFFFFFFUL                                                 /*!< RSSICFG_LUTS DATA11: Mask                                    */
#define RSSICFG_LUTS_DATA11_Rst                                      0x00000000UL                                                 /*!< RSSICFG_LUTS DATA11: Reset                                   */

#define RSSICFG_LUTS_DATA11_VAL_Size                                 32                                                           /*!< RSSICFG_LUTS DATA11: VAL Size                                */
#define RSSICFG_LUTS_DATA11_VAL_Pos                                  0                                                            /*!< RSSICFG_LUTS DATA11: VAL Position                            */
#define RSSICFG_LUTS_DATA11_VAL_Msk                                  (0xffffffffUL << RSSICFG_LUTS_DATA11_VAL_Pos)                /*!< RSSICFG_LUTS DATA11: VAL Mask                                */
#define RSSICFG_LUTS_DATA11_VAL_Rst                                  0x0000UL                                                     /*!< RSSICFG_LUTS DATA11: VAL Reset                               */
#define RSSICFG_LUTS_DATA11_VAL_Addr                                 0x20000890UL                                                 /*!< RSSICFG_LUTS DATA11: VAL Address                             */

/* ----------------------------  RSSICFG_LUTS_DATA12  ---------------------------- */
#define RSSICFG_LUTS_DATA12_Addr                                     0x20000894UL                                                 /*!< RSSICFG_LUTS DATA12: Address                                 */
#define pRSSICFG_LUTS_DATA12                                         (*(volatile uint32_t *) RSSICFG_LUTS_DATA12_Addr)            /*!< RSSICFG_LUTS DATA12: Pointer                                 */
#define RSSICFG_LUTS_DATA12_Msk                                      0xFFFFFFFFUL                                                 /*!< RSSICFG_LUTS DATA12: Mask                                    */
#define RSSICFG_LUTS_DATA12_Rst                                      0x00000000UL                                                 /*!< RSSICFG_LUTS DATA12: Reset                                   */

#define RSSICFG_LUTS_DATA12_VAL_Size                                 32                                                           /*!< RSSICFG_LUTS DATA12: VAL Size                                */
#define RSSICFG_LUTS_DATA12_VAL_Pos                                  0                                                            /*!< RSSICFG_LUTS DATA12: VAL Position                            */
#define RSSICFG_LUTS_DATA12_VAL_Msk                                  (0xffffffffUL << RSSICFG_LUTS_DATA12_VAL_Pos)                /*!< RSSICFG_LUTS DATA12: VAL Mask                                */
#define RSSICFG_LUTS_DATA12_VAL_Rst                                  0x0000UL                                                     /*!< RSSICFG_LUTS DATA12: VAL Reset                               */
#define RSSICFG_LUTS_DATA12_VAL_Addr                                 0x20000894UL                                                 /*!< RSSICFG_LUTS DATA12: VAL Address                             */

/* ----------------------------  RSSICFG_LUTS_DATA13  ---------------------------- */
#define RSSICFG_LUTS_DATA13_Addr                                     0x20000898UL                                                 /*!< RSSICFG_LUTS DATA13: Address                                 */
#define pRSSICFG_LUTS_DATA13                                         (*(volatile uint32_t *) RSSICFG_LUTS_DATA13_Addr)            /*!< RSSICFG_LUTS DATA13: Pointer                                 */
#define RSSICFG_LUTS_DATA13_Msk                                      0xFFFFFFFFUL                                                 /*!< RSSICFG_LUTS DATA13: Mask                                    */
#define RSSICFG_LUTS_DATA13_Rst                                      0x00000000UL                                                 /*!< RSSICFG_LUTS DATA13: Reset                                   */

#define RSSICFG_LUTS_DATA13_VAL_Size                                 32                                                           /*!< RSSICFG_LUTS DATA13: VAL Size                                */
#define RSSICFG_LUTS_DATA13_VAL_Pos                                  0                                                            /*!< RSSICFG_LUTS DATA13: VAL Position                            */
#define RSSICFG_LUTS_DATA13_VAL_Msk                                  (0xffffffffUL << RSSICFG_LUTS_DATA13_VAL_Pos)                /*!< RSSICFG_LUTS DATA13: VAL Mask                                */
#define RSSICFG_LUTS_DATA13_VAL_Rst                                  0x0000UL                                                     /*!< RSSICFG_LUTS DATA13: VAL Reset                               */
#define RSSICFG_LUTS_DATA13_VAL_Addr                                 0x20000898UL                                                 /*!< RSSICFG_LUTS DATA13: VAL Address                             */

/* ----------------------------  RSSICFG_LUTS_DATA14  ---------------------------- */
#define RSSICFG_LUTS_DATA14_Addr                                     0x2000089CUL                                                 /*!< RSSICFG_LUTS DATA14: Address                                 */
#define pRSSICFG_LUTS_DATA14                                         (*(volatile uint32_t *) RSSICFG_LUTS_DATA14_Addr)            /*!< RSSICFG_LUTS DATA14: Pointer                                 */
#define RSSICFG_LUTS_DATA14_Msk                                      0xFFFFFFFFUL                                                 /*!< RSSICFG_LUTS DATA14: Mask                                    */
#define RSSICFG_LUTS_DATA14_Rst                                      0x00000000UL                                                 /*!< RSSICFG_LUTS DATA14: Reset                                   */

#define RSSICFG_LUTS_DATA14_VAL_Size                                 32                                                           /*!< RSSICFG_LUTS DATA14: VAL Size                                */
#define RSSICFG_LUTS_DATA14_VAL_Pos                                  0                                                            /*!< RSSICFG_LUTS DATA14: VAL Position                            */
#define RSSICFG_LUTS_DATA14_VAL_Msk                                  (0xffffffffUL << RSSICFG_LUTS_DATA14_VAL_Pos)                /*!< RSSICFG_LUTS DATA14: VAL Mask                                */
#define RSSICFG_LUTS_DATA14_VAL_Rst                                  0x0000UL                                                     /*!< RSSICFG_LUTS DATA14: VAL Reset                               */
#define RSSICFG_LUTS_DATA14_VAL_Addr                                 0x2000089cUL                                                 /*!< RSSICFG_LUTS DATA14: VAL Address                             */

/* ====================================================================================== */

#define __FIRMWARE_MODULE_NAME_BASE              (0x20002fa0UL)
#define __FIRMWARE_MODULE_NAME_SIZE              20

#define __FIRMWARE_MODULE_VERSION_BASE           (0x20002fb4UL)
#define __FIRMWARE_MODULE_VERSION_SIZE           8

#define __ADI_REFERENCE_BASE                     (0x20002ff2UL)
#define __ADI_REFERENCE_SIZE                     2


/* ====================================================================================== */
/* ================          Misc 'ADF7030_1'Parameters                  ================ */
/* ====================================================================================== */

#define PARAM_ADF7030_1_ADC_BASE                 0x40004078
#define PARAM_ADF7030_1_ADC_nb                   0x3
#define PARAM_ADF7030_1_AES_BASE                 0x40004400
#define PARAM_ADF7030_1_AES_nb                   0x9
#define PARAM_ADF7030_1_AFC_BASE                 0x400041f8
#define PARAM_ADF7030_1_AFC_nb                   0x5
#define PARAM_ADF7030_1_AFERX_BASE               0x40004050
#define PARAM_ADF7030_1_AFERX_nb                 0xa
#define PARAM_ADF7030_1_AGC_BASE                 0x400041bc
#define PARAM_ADF7030_1_AGC_nb                   0xf
#define PARAM_ADF7030_1_ANAFILT_LUTS_OFFSET      0x60c
#define PARAM_ADF7030_1_ANAFILT_LUT_nb           0x6
#define PARAM_ADF7030_1_ANAFILT_LUT_number       0x7
#define PARAM_ADF7030_1_ANAFILT_ROM_LUTS_OFFSET  0xc0
#define PARAM_ADF7030_1_ANCPLL_BASE              0x40004040
#define PARAM_ADF7030_1_ANCPLL_nb                0x4
#define PARAM_ADF7030_1_APB_MISC_BASE            0x40002040
#define PARAM_ADF7030_1_BUS_MATRIX_BASE          0x40002100
#define PARAM_ADF7030_1_CALIBRATION_BASE         0x40004248
#define PARAM_ADF7030_1_CALIBRATION_nb           0x2
#define PARAM_ADF7030_1_CDR_BASE                 0x400040bc
#define PARAM_ADF7030_1_CDR_nb                   0x8
#define PARAM_ADF7030_1_CRC_BASE                 0x40001c00
#define PARAM_ADF7030_1_CRMGT_BASE               0x40004270
#define PARAM_ADF7030_1_CRMGT_nb                 0x8
#define PARAM_ADF7030_1_DIGFILT2_LUTS_OFFSET     0x794
#define PARAM_ADF7030_1_DIGFILT2_LUT_nb          0x5
#define PARAM_ADF7030_1_DIGFILT2_LUT_number      0x7
#define PARAM_ADF7030_1_DIGFILT2_ROM_LUTS_OFFSET 0x248
#define PARAM_ADF7030_1_DIGFILT_LUTS_OFFSET      0x6b4
#define PARAM_ADF7030_1_DIGFILT_LUT_nb           0x8
#define PARAM_ADF7030_1_DIGFILT_LUT_number       0x7
#define PARAM_ADF7030_1_DIGFILT_ROM_LUTS_OFFSET  0x168
#define PARAM_ADF7030_1_DIG_RECEIVER_BASE        0x400040a4
#define PARAM_ADF7030_1_DIG_RECEIVER_nb          0x6
#define PARAM_ADF7030_1_DIG_TRANSMITTER_BASE     0x400041ac
#define PARAM_ADF7030_1_DIG_TRANSMITTER_nb       0x4
#define PARAM_ADF7030_1_DIVIDER_BASE             0x400042cc
#define PARAM_ADF7030_1_EVAL_GUI                 0x0
#define PARAM_ADF7030_1_FEC_BASE                 0x400042cc
#define PARAM_ADF7030_1_FEC_nb                   0x4
#define PARAM_ADF7030_1_GENERATOR_TYPE           0x0
#define PARAM_ADF7030_1_GENERIC_PKT_OFFSET       0x4f4
#define PARAM_ADF7030_1_GENERIC_PKT_nb           0x23
#define PARAM_ADF7030_1_GPIO_BASE                0x40000800
#define PARAM_ADF7030_1_IRQ_CTRL_BASE            0x40003800
#define PARAM_ADF7030_1_IR_CAL_BASE              0x4000421c
#define PARAM_ADF7030_1_IR_CAL_nb                0x4
#define PARAM_ADF7030_1_LPM_CRC_OFFSET           0xffc
#define PARAM_ADF7030_1_LPM_CRC_nb               0x1
#define PARAM_ADF7030_1_MAKE_CALIBRATION_SUBFIELD_VISIBLE 0x1
#define PARAM_ADF7030_1_MAKE_DOC_LUTS_VISIBLE    0x1
#define PARAM_ADF7030_1_MAKE_DUMMY_SUBFIELD_VISIBLE 0x1
#define PARAM_ADF7030_1_MAKE_PRIVATE_IN_PROFILE_OR_PACKECT_VISIBLE 0x0
#define PARAM_ADF7030_1_MAKE_PRIVATE_SUBFIELD_VISIBLE 0x0
#define PARAM_ADF7030_1_MAKE_RAM_LUTS_VISIBLE    0x0
#define PARAM_ADF7030_1_MCR_BASE                 0x40004000
#define PARAM_ADF7030_1_MEM_PATCH_BASE           0x40003e00
#define PARAM_ADF7030_1_MISC_BASE                0x400042b4
#define PARAM_ADF7030_1_MISC_nb                  0x6
#define PARAM_ADF7030_1_OCL_BASE                 0x4000420c
#define PARAM_ADF7030_1_OCL_nb                   0x4
#define PARAM_ADF7030_1_PACKET_MEM_OFFSET        0xaf0
#define PARAM_ADF7030_1_PACKET_MEM_size          0x100
#define PARAM_ADF7030_1_PART_ID                  0x0
#define PARAM_ADF7030_1_PERIPH_BASE              0x40000000
#define PARAM_ADF7030_1_PLLBW_LUTS_OFFSET        0x820
#define PARAM_ADF7030_1_PLLBW_LUT_nb             0x1
#define PARAM_ADF7030_1_PLLBW_LUT_number         0x9
#define PARAM_ADF7030_1_PLLBW_ROM_LUTS_OFFSET    0x2d4
#define PARAM_ADF7030_1_PLL_BASE                 0x4000400c
#define PARAM_ADF7030_1_PLL_nb                   0xd
#define PARAM_ADF7030_1_PMGT_BASE                0x40004250
#define PARAM_ADF7030_1_PMGT_nb                  0x8
#define PARAM_ADF7030_1_PMU_BASE                 0x40000c00
#define PARAM_ADF7030_1_PROFILE_OFFSET           0x2e4
#define PARAM_ADF7030_1_PROFILE_nb               0x42
#define PARAM_ADF7030_1_REMAP_TABLE_OFFSET       0xc0
#define PARAM_ADF7030_1_REMAP_TABLE_nb           0x4
#define PARAM_ADF7030_1_RESET_CTL_BASE           0x40002000
#define PARAM_ADF7030_1_ROM_BASE                 0x0
#define PARAM_ADF7030_1_RSSICFG_LUTS_OFFSET      0x864
#define PARAM_ADF7030_1_RSSICFG_LUT_nb           0x7
#define PARAM_ADF7030_1_RSSICFG_LUT_number       0x2
#define PARAM_ADF7030_1_RSSICFG_ROM_LUTS_OFFSET  0x318
#define PARAM_ADF7030_1_RTC_BASE                 0x40003400
#define PARAM_ADF7030_1_SCRATCH_MEM_OFFSET       0x1000
#define PARAM_ADF7030_1_SCRATCH_MEM_nb           0x44
#define PARAM_ADF7030_1_SCRIPT_MEM_OFFSET        0x89c
#define PARAM_ADF7030_1_SCRIPT_MEM_size          0x254
#define PARAM_ADF7030_1_SENSORS_BASE             0x40004290
#define PARAM_ADF7030_1_SENSORS_nb               0x1
#define PARAM_ADF7030_1_SERDES_BASE              0x400040dc
#define PARAM_ADF7030_1_SERDES_nb                0x34
#define PARAM_ADF7030_1_SHOW_LCPSM_BITFIELDS     0x0
#define PARAM_ADF7030_1_SHOW__TBR_FIELD          0x0
#define PARAM_ADF7030_1_SM_CONFIG_OFFSET         0xd0
#define PARAM_ADF7030_1_SM_CONFIG_nb             0x14
#define PARAM_ADF7030_1_SM_DATA_OFFSET           0x120
#define PARAM_ADF7030_1_SM_DATA_nb               0x10
#define PARAM_ADF7030_1_SM_STRUCT_OFFSET         0x160
#define PARAM_ADF7030_1_SM_STRUCT_nb             0x61
#define PARAM_ADF7030_1_SPI_BASE                 0x40001000
#define PARAM_ADF7030_1_SPI_HOST_BASE            0x40001800
#define PARAM_ADF7030_1_SRAM_BASE                0x20000000
#define PARAM_ADF7030_1_SW_STACK_BASE            0x20002000
#define PARAM_ADF7030_1_SW_STACK_END             0x20002400
#define PARAM_ADF7030_1_SW_STACK_size            0x400
#define PARAM_ADF7030_1_SYS_OSC_BASE             0x40004000
#define PARAM_ADF7030_1_SYS_OSC_nb               0x3
#define PARAM_ADF7030_1_TEST_MODES_BASE          0x40004294
#define PARAM_ADF7030_1_TEST_MODES_nb            0x8
#define PARAM_ADF7030_1_TIMER0_BASE              0x40002400
#define PARAM_ADF7030_1_TIMER1_BASE              0x40002800
#define PARAM_ADF7030_1_TIMER2_BASE              0x40002c00
#define PARAM_ADF7030_1_TIMER3_BASE              0x40003000
#define PARAM_ADF7030_1_TRX_CAPTURE_OFFSET       0x1110
#define PARAM_ADF7030_1_TRX_FILTER_BASE          0x40004084
#define PARAM_ADF7030_1_TRX_FILTER_nb            0x8
#define PARAM_ADF7030_1_UART_BASE                0x40001400
#define PARAM_ADF7030_1_VCOCAL_LUTS_OFFSET       0x844
#define PARAM_ADF7030_1_VCOCAL_LUT_nb            0x1
#define PARAM_ADF7030_1_VCOCAL_LUT_number        0x8
#define PARAM_ADF7030_1_VCOCAL_ROM_LUTS_OFFSET   0x2f8
#define PARAM_ADF7030_1_VCO_CAL_BASE             0x4000422c
#define PARAM_ADF7030_1_VCO_CAL_RESULTS0_OFFSET  0x844
#define PARAM_ADF7030_1_VCO_CAL_nb               0x7
#define PARAM_ADF7030_1_VECTOR_TABLE_ROM_BASE    0x0
#define PARAM_ADF7030_1_VECTOR_TABLE_nb          0x30
#define PARAM_ADF7030_1_WDT_BASE                 0x40000000
#define PARAM_ADF7030_1_eFUSE_BASE               0x40003c00


/** @} */ /* End of group FW_Macro Firmware Macro Definition */
/** @} */ /* End of group PHY_headers PHY */
/** @} */ /* End of group adf7030-1_headers Headers */
/** @} */ /* End of group adf7030-1 ADF7030-1 Driver */

#ifdef __cplusplus
}
#endif


#endif  /* ADF7030_FW_MACRO_H */

