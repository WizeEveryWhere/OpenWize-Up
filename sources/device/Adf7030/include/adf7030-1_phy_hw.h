/**
  * @file adf7030-1_phy_hw.h
  * @brief This file define some HW types
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
  * @par 1.0.0 : 2020/04/26 [GBI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup adf7030-1_phy
 * @ingroup device
 * @{
 *
 */
#ifndef _ADF7030_PHY_HW_H_
#define _ADF7030_PHY_HW_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*!
 * @cond INTERNAL
 * @{
 */

/* ========================================================================== */
/* =================             VCO_CAL_RESULTS              =============== */
/* ========================================================================== */

/**
  * @brief VCO Calibration Results (VCO_CAL_RESULTS)
  */

typedef struct { /*!< VCO_CAL_RESULTS Structure */
    uint32_t  DATA0; /*!< Not Available */
    uint32_t  DATA1; /*!< Not Available */
    uint32_t  DATA2; /*!< Not Available */
    uint32_t  DATA3; /*!< Not Available */
    uint32_t  DATA4; /*!< Not Available */
    uint32_t  DATA5; /*!< Not Available */
    uint32_t  DATA6; /*!< Not Available */
    uint32_t  DATA7; /*!< Not Available */
} vco_cal_results_t;

/* ========================================================================== */
/* ================                  PMU                     ================ */
/* ========================================================================== */

typedef union {
    uint32_t KEY; /*!< Gateway for Software-Keyed Instructions */
    struct {
        uint32_t SW_KEY : 6; /*!< Software-keyed instruction to the PMU.  */
    } KEY_b; /*!< BitSize */
} pmu_key_t;

/**
  * @brief Power Management (PMU)
  */

typedef struct { /*!< PMU Structure */
    uint32_t RESERVED0[2];
    pmu_key_t key;
} pmu_t;

/* ========================================================================== */
/* ================                    SPI_HOST              ================ */
/* ========================================================================== */

typedef union {
    uint32_t PNTR; /*!< SPI Slave Pointer */
    struct {
        uint32_t SPIS_PNTR : 32; /*!< SPI Pointer */
    } PNTR0_b; /*!< BitSize */
} pntr_t;

/**
  * @brief Some description. (SPI_HOST)
  */

typedef struct { /*!< SPI_HOST Structure */
	pntr_t PNTR0; /*!< SPI Slave Pointer 0 */
	pntr_t PNTR1; /*!< SPI Slave Pointer 1 */
	pntr_t PNTR2; /*!< SPI Slave Pointer 2 */
} spi_host_t;

/* ========================================================================== */
/* ================                 GPIO_CTRL                 ================ */
/* ========================================================================== */

typedef union {
    uint32_t GPIO_CTRL4_7; /*!< GPIO4 to GPIO7 pin functionality selection*/
    struct {
        uint32_t G_PIN4_CFG : 6; /*!< GPIO4 Configuration */
        uint32_t            : 2;
        uint32_t G_PIN5_CFG : 6; /*!< GPIO5 Configuration */
        uint32_t            : 2;
        uint32_t G_PIN6_CFG : 6; /*!< GPIO6 Configuration */
        uint32_t            : 2;
        uint32_t G_PIN7_CFG : 6; /*!< GPIO7 Configuration */
    } GPIO_CTRL4_7_b; /*!< BitSize */

    uint32_t GPIO_CTRL0_3; /*!< GPIO0 to GPIO3 pin functionality selection */
    struct {
        uint32_t G_PIN0_CFG : 6; /*!< GPIO0 Configuration */
        uint32_t            : 2;
        uint32_t G_PIN1_CFG : 6; /*!< GPIO1 Configuration */
        uint32_t            : 2;
        uint32_t G_PIN2_CFG : 6; /*!< GPIO2 Configuration */
        uint32_t            : 2;
        uint32_t G_PIN3_CFG : 6; /*!< GPIO3 Configuration */
    } GPIO_CTRL0_3_b; /*!< BitSize */
} gpio_ctrl_t;

/* ========================================================================== */
/* ================                 IRQ_CTRL                 ================ */
/* ========================================================================== */

typedef union {
    uint32_t MASK; /*!< Masks for IRQ_OUTx         */
    struct {
        uint32_t PREAMBLE_IRQN     : 1;
        uint32_t PREAMBLE_GONE_IRQN: 1;
        uint32_t SYNCWORD_IRQN     : 1;
        uint32_t LENGTH_IRQN       : 1;
        uint32_t PAYLOAD_IRQN      : 1;
        uint32_t PAYLOAD_BLOC_IRQN : 1;
        uint32_t CRC_CHK_IRQN      : 1;
        uint32_t EOF_IRQN          : 1;

        uint32_t BUFF_HALF_IRQN    : 1; /*!< Rx: the lower half of Rx
                                              rolling buffer is full. Tx: the
                                              lower half of Tx rolling buffer is
                                              empty (write 1 to clear event).*/
        uint32_t BUFF_FULL_IRQN    : 1; /*!< Rx: the upper half of Rx
                                              rolling buffer is full. Tx: the
                                              upper half of Tx rolling buffer is
                                              empty (write 1 to clear event).*/
        uint32_t SM_READY_IRQN     : 1; /*!< SM_RDY event has occurred.
                                              The last state transition command
                                              has been received and the transition
                                              from the origin state to destination
                                              state is underway. A new command
                                              can be issued at this point to
                                              interrupt the current transition.
                                              It indicates that CMD_READY is 1
                                              (write 1 to clear). */
        uint32_t SM_IDLE_IRQN      : 1; /*!< SM_IDLE event has occurred.
                                              The destination state has been
                                              reached and all actions associated
                                              with the destination state have
                                              been completed. The complete state
                                              transition is complete (write 1 to
                                              clear). */
        uint32_t SM_BODY_IRQN         : 1;
        uint32_t SM_ERROR_IRQN        : 1; //uint32_t SM_SCRIPT_IRQN:  1;
        uint32_t SNOOP_IRQN           : 1;
        uint32_t HARDFAULT_IRQN       : 1;

        uint32_t SPI_HOST_EXT_IRQN    : 1;
        uint32_t TMR0_EXT_IRQN        : 1;
        uint32_t TMR1_EXT_IRQN        : 1;
        uint32_t TMR2_EXT_IRQN        : 1;
        uint32_t TMR3_EXT_IRQN        : 1;
        uint32_t UNUSED0_IRQN         : 1;
        uint32_t RTC_EXT_IRQN         : 1;
        uint32_t WDT_EXT_IRQN         : 1;

        uint32_t SERDES_EXT_IRQN      : 1;
        uint32_t SERDES_MONITOR_EXT_IRQN: 1;
        uint32_t MCR_SNOOP_EXT_IRQN   : 1;
        uint32_t CAL_READY_EXT_IRQN   : 1;
        uint32_t AGC_EXT_IRQN         : 1;
        uint32_t UNUSED1_IRQN         : 1;
        uint32_t UNUSED2_IRQN         : 1;
        uint32_t UNUSED3_IRQN         : 1;
    } MASK_b; /*!< BitSize */
} irq_mask_t;

typedef union {
    uint32_t STATUS;                 /*!< Status of IRQ_OUTx         */
    struct {
        uint32_t PREAMBLE_IRQN:  1;      /*!< Generic packet and IEEE802.15.4g:
                                                preamble has been received (Rx);
                                                the first preamble bit is about to
                                                be transmitted (Tx).            */
        uint32_t PREAMBLE_GONE_IRQN:  1; /*!< Generic packet and IEEE802.15.4g:
                                                preamble pattern no longer being
                                                received in received bit stream
                                                (Rx); last preamble bit transmitted
                                                (Tx).                           */
        uint32_t SYNCWORD_IRQN:  1;      /*!< Generic packet: the programmed
                                                number of bits of Syncword 0 have
                                                been received and matched (Rx).
                                                IEEE802.15.4g: the programmed number
                                                of Syncword 0 or Syncword 1 (if
                                                enabled) bits have been received
                                                and matched (Rx). Generic packet
                                                and IEEE802.15.4g: the programmed
                                                number of bits of Syncword 0 have
                                                been transmitted (Tx).          */
        uint32_t LENGTH_IRQN:  1;        /*!< Generic packet: a length field
                                                has been received (Rx); the length
                                                field has been transmitted (Tx).
                                                IEEE802.15.4g: PHR has been received
                                                (Rx); the PHR has been transmitted
                                                (Tx)  .                         */
        uint32_t PAYLOAD_IRQN:  1;       /*!< Generic packet: full payload
                                                received (Rx)/full payload
                                                transmitted (Tx). IEEE802.15.4g:
                                                full Payload (including FCS)
                                                received (Rx); full payload (inc-
                                                -luding FCS) transmitted (Tx).  */
        uint32_t PAYLOAD_BLOC_IRQN:  1;  /*!< Generic packet only: A multiple
                                                of TRX_BLOCK_SIZE payload bytes
                                                have been received (Rx); a multiple
                                                of TRX_BLOCK_SIZE payload bytes
                                                have been transmitted (Tx).     */
        uint32_t CRC_CHK_IRQN:  1;       /*!< Generic packet and IEEE802.15.4g:
                                                The programmed number of CRC/FCS
                                                bits has been received and are
                                                correct (Rx); the programmed number
                                                of CRC/FCS bits have been trans-
                                                -mitted (Tx).                   */
        uint32_t EOF_IRQN   :  1;        /*!< Generic packet and IEEE802.15.4g:
                                                the full packet has been received
                                                (RX)/The full packet has been
                                                transmitted (Tx)                */
        uint32_t BUFF_HALF_IRQN:  1;     /*!< RX: the lower half of Rx rol-
                                                -ling buffer is full. Tx: the lower
                                                half of Tx rolling buffer is empty
                                                write 1 to clear).              */
        uint32_t BUFF_FULL_IRQN:  1;     /*!< RX: the upper half of Rx rol-
                                                -ling buffer is full. Tx: the up-
                                                -per half of Tx rolling buffer is
                                                empty (write 1 to clear event). */
        uint32_t SM_READY_IRQN:  1;      /*!< SM_RDY Event has occurred. The
                                                last state transition command has
                                                been received and the transition
                                                from the origin state to destination
                                                state is underway. A new command
                                                may be issued at this point to
                                                interrupt the current transition.
                                                It indicates that CMD_READY is 1,
                                                write 1 to clear.               */
        uint32_t SM_IDLE_IRQN:  1;       /*!< SM_IDLE Event has occurred.
                                                The destination state has been
                                                reached and all actions associated
                                                with the destination state have
                                                been completed. The complete state
                                                transition is complete, write 1 to
                                                clear.                          */

        uint32_t SM_BODY_IRQN:  1;

        uint32_t SM_ERROR_IRQN:  1; //uint32_t SM_SCRIPT_IRQN:  1;
        uint32_t SNOOP_IRQN:  1;
        uint32_t HARDFAULT_IRQN:  1;
        uint32_t SPI_HOST_EXT_IRQN:  1;
        uint32_t TMR0_EXT_IRQN:  1;
        uint32_t TMR1_EXT_IRQN:  1;
        uint32_t TMR2_EXT_IRQN:  1;
        uint32_t TMR3_EXT_IRQN:  1;
        uint32_t UNUSED0_IRQN:  1;
        uint32_t RTC_EXT_IRQN:  1;
        uint32_t WDT_EXT_IRQN:  1;
        uint32_t SERDES_EXT_IRQN:  1;
        uint32_t SERDES_MONITOR_EXT_IRQN:  1;
        uint32_t MCR_SNOOP_EXT_IRQN:  1;
        uint32_t CAL_READY_EXT_IRQN:  1;
        uint32_t AGC_EXT_IRQN:  1;
        uint32_t UNUSED1_IRQN:  1;
        uint32_t UNUSED2_IRQN:  1;
        uint32_t UNUSED3_IRQN:  1;
    } STATUS_b; /*!< BitSize */
} irq_status_t;

/**
  * @brief IRQ Control Register (IRQ_CTRL)
  */

typedef struct { /*!< IRQ_CTRL Structure */
	irq_mask_t   irq_mask[2];   /*!< Masks for IRQ_OUTx */
    irq_status_t irq_status[2]; /*!< Status of IRQ_OUTx */
} irq_ctrl_t;

/* ========================================================================== */
/* ================                       AFC                ================ */
/* ========================================================================== */

typedef union {
    uint32_t  CONFIG; /*!< Not Available */
    struct {
        uint32_t  MODE : 3; /*!< Set AFC Mode */
    } CONFIG_b; /*!< BitSize */
} config_t;

typedef union {
    uint32_t  FREQUENCY_ERROR; /*!< Not Available */
    struct {
        uint32_t  READBACK   : 16; /*!< Frequency Error correction signed
                                        16bit readback value. Frequency Error in
                                        Hz = readback *26,000,000 / 2^22 */
    } FREQUENCY_ERROR_b; /*!< BitSize */
} freq_err_t;

/**
  * @brief AFC (AFC)
  */

typedef struct { /*!< AFC Structure */
    config_t   config;
    uint32_t   RESERVED0[3];
    freq_err_t freq_err;
} afc_t;

/* ========================================================================== */
/* ================                  CRMGT                   ================ */
/* ========================================================================== */

typedef union {
    uint32_t  PROC_CLK_EN; /*!< Processor Clock Enable */
    struct {
        uint32_t CONFIGURATION : 32; /*!< Processor clock configuration    */
    } PROC_CLK_EN_b; /*!< BitSize */
} proc_clk_en_t;


/**
  * @brief Clock and Reset Control (CRMGT)
  */

typedef struct {                        /*!< CRMGT Structure                  */
    uint32_t      RESERVED0[2];
    proc_clk_en_t proc_clk_en;
} crmgt_t;


/* ========================================================================== */
/* ================                      MISC                ================ */
/* ========================================================================== */

/**
  * @brief MISC Firmware Register (MISC)
  */

typedef union { /*!< MISC Structure */
    uint32_t FW; /*!< Firmware status and debug register*/
    struct {
         uint32_t STATUS     :  2; /*!< Firmware status ID */
         uint32_t            :  6;
         uint32_t CURR_STATE :  6; /*!< Current firmware state readback */
         uint32_t            :  10;
         uint32_t ERR_CODE   :  8;
    } FW_b; /*!< BitSize */
} misc_fw_t;

/* ========================================================================== */

typedef struct {
    union {
        uint32_t AFERX_FILT_STG1;
        struct {
            uint32_t CAP :10; // Filter Capacitor code
            uint32_t     :6; //
            uint32_t R2  :5; // Filter resistor code
            uint32_t     :11;
        }AFERX_FILT_STG1_b;
    };
}aferx_filt_stg1_t;

/*!
 * @}
 * @endcond
 */

#ifdef __cplusplus
}
#endif
#endif /* _ADF7030_PHY_HW_H_ */

/*! @} */
