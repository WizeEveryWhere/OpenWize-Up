/**
  * @file adf7030-1_phy_fw.h
  * @brief This file define some FW types
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
  * @par 1.0.0 : 2020/04/25 [GBI]
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
#ifndef _ADF7030_PHY_FW_H_
#define _ADF7030_PHY_FW_H_
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Note :
 * ARM CPU is Little Endian. The bit filed in the following structure are
 * in the LSb (Least Significant Bit) first ordering.
 */

/*!
 * @cond INTERNAL
 * @{
 */

typedef enum {
    MOD_2FSK = 0b000,
    MOD_4FSK = 0b010,
    OOK      = 0b101
}mod_type_e;

typedef enum {
    FSK  = 0b0,
    GFSK = 0b1,
}tx_filter_e;

typedef enum {
    BT_0_3  = 0b00,
    BT_0_35 = 0b01,
    BT_0_4  = 0b10,
    BT_0_5  = 0b11,
}tx_gaussion_bt_e;

typedef enum {
    RAMP_OFF            = 0b000,
    RAMP_4_bit_per_db   = 0b001,
    RAMP_8_bit_per_db   = 0b010,
    RAMP_16_bit_per_db  = 0b011,
    RAMP_32_bit_per_db  = 0b100,
    RAMP_64_bit_per_db  = 0b101,
    RAMP_128_bit_per_db = 0b110,
    RAMP_256_bit_per_db = 0b111,
}pa_ramp_rate_e;

typedef enum {
    WIDEBAND_MODE   = 0b0, /*!< Wideband phy mode */
    NARROWBAND_MODE = 0b1  /*!< Narrowband phy mode */
}trx_phy_mode_e;

typedef enum {
    NRZ        = 0b00000001, // NRZ encoding.
    MANCHESTER = 0b00000100, // Manchester encoding.
    FEC        = 0b00001000, // FEC encoding.
    POLARITY   = 0b10000000  // Polarity bit.
}encode_mode_e;

typedef enum {
    PA1 = 0b0, /*!< Select PA1 */
    PA2 = 0b1  /*!< Select PA2 */
}PA_sel_e;

typedef enum {
	PA_C4 = 0x4, /*!< Select curve 4 */
	PA_C6 = 0x6  /*!< Select curve 6 */
}PA_pwr_coarse;

typedef enum {
                        // micro (7bits) fine (7bits) coarse (4bits)
    PA_p13dBm   = 0x64, //   0            100(0x64)      6
    PA_p11_5dBm = 0x50, //   0             80(0x50)      6
    PA_p9_5dBm  = 0x3C, //   0             60(0x3C)      6
    PA_p6_5dBm  = 0x28, //   0             40(0x28)      6
    PA_p4dBm    = 0x1A, //   0             26(0x1A)      6
    PA_p2_5dBm  = 0x16, //   0             22(0x16)      6
    PA_0dBm     = 0x12, //   0             18(0x12)      6
    PA_m2_5dBm  = 0x0E, //   0             14(0x0E)      6
    PA_m4dBm    = 0x0C, //   0             12(0x0C)      6
    PA_m5dBm    = 0x0A, //   0             10(0x0A)      6
    PA_m7dBm    = 0x08, //   0              8(0x08)      6
    PA_m9_5dBm  = 0x06, //   0              6(0x06)      6
    PA_m13dBm   = 0x04, //   0              4(0x04)      6
}PA_pwr_c6_e;

typedef enum {
                        // micro (7bits) fine (7bits) coarse (4bits)
    PA_p10_5dBm = 0x64, //   0            100(0x64)      4
    PA_p8_5dBm  = 0x50, //   0             80(0x50)      4
    PA_p3dBm    = 0x28, //   0             40(0x28)      4
    PA_m1dBm    = 0x16, //   0             22(0x16)      4
    PA_m3dBm    = 0x14, //   0             20(0x14)      4
    PA_m6dBm    = 0x0E, //   0             14(0x0E)      4
    PA_m17dBm   = 0x04, //   0              4(0x04)      4
}PA_pwr_c4_e;


typedef enum {
	WAKE_UP_NONE = 0b0000, /*!< None wake-up source */
	WAKE_UP_IRQ0 = 0b0001, /*!< Select IRQ0 as wake-up source */
	WAKE_UP_IRQ1 = 0b0010, /*!< Select IRQ1 as wake-up source */
	WAKE_UP_RTC  = 0b0100, /*!< Select RTC as wake-up source */
	WAKE_UP_CS   = 0b1000, /*!< Select CS as wake-up source */
}wake_src_e;

/* The following come from teh "ADF7030-1 Software Reference Manual : UG-1002.
 * Unfortunately, the tests had shown that is wrong.
	typedef enum {
		GPIO_CLK_FREQ_D1   = 0b000, // base frequency     : 6.5 MHz.
		GPIO_CLK_FREQ_D2   = 0b001, // base frequency/2   : 3.25 MHz.
		GPIO_CLK_FREQ_D4   = 0b010, // base frequency/4   : 1.625 MHz.
		GPIO_CLK_FREQ_D8   = 0b011, // base frequency/8   : 0.8125 MHz.
		GPIO_CLK_FREQ_D16  = 0b100, // base frequency/16  : 0.40625 MHz.
		GPIO_CLK_FREQ_D32  = 0b101, // base frequency/32  : 0.203125 MHz.
		GPIO_CLK_FREQ_D64  = 0b110, // base frequency/64  : 0.1015625 MHz.
		GPIO_CLK_FREQ_D128 = 0b111, // base frequency/128 : 0.05078125 MHz.
	}gpio_clk_freq_e;
 *
 * Correct values follows...
 * Note : it seems we could consider the base frequency = 26Mhz / 16
 */
typedef enum {
	GPIO_CLK_FREQ_D1   = 0b000, // base frequency     : 1.625 MHz.
	GPIO_CLK_FREQ_D2   = 0b001, // base frequency/2   : 0.8125 MHz.
	GPIO_CLK_FREQ_D4   = 0b010, // base frequency/4   : 0.40625 MHz.
	GPIO_CLK_FREQ_D8   = 0b011, // base frequency/8   : 0.203125 MHz.
	GPIO_CLK_FREQ_D16  = 0b100, // base frequency/16  : 0.1015625 MHz.
	GPIO_CLK_FREQ_D32  = 0b101, // base frequency/32  : 0.05078125 MHz.
	GPIO_CLK_FREQ_D64  = 0b110, // base frequency/64  : 0.025390625 MHz.
	GPIO_CLK_FREQ_D128 = 0b111, // base frequency/128 : 0.0126953125 MHz.
}gpio_clk_freq_e;

typedef enum {
	TICK_RATEx1   = 0x0, //!< 1× data rate.
	TICK_RATEx2   = 0x2, //!< 2× data rate.
	TICK_RATEx4   = 0x3, //!< 4× data rate.
	TICK_RATEx8   = 0x4, //!< 8× data rate.
	TICK_RATEx16  = 0x5, //!< 16× data rate.
	TICK_RATEx32  = 0x6, //!< 32× data rate.
	TICK_RATEx64  = 0x7, //!< 64× data rate.
	TICK_RATEx128 = 0x8, //!< 128× data rate.
}cca_tick_rate_e;

typedef enum {
	TMODE_TX_NONE        = 0, //!< Disable.
	TMODE_TX_CARRIER     = 1, //!< Transmit a carrier.
	TMODE_TX_FDEV_NEG    = 2, //!< Transmit frequency deviation tone, −fDEV, in 2FSK or off in OOK.
	TMODE_TX_FDEV_MAXNEG = 3, //!< Transmit −fDEV_MAX in 4FSK only.
	TMODE_TX_FDEV_POS    = 4, //!< Transmit +fDEV in 2FSK or on in OOK.
	TMODE_TX_FDEV_MAXPOS = 5, //!< Transmit +fDEV_MAX in 4FSK only.
	TMODE_TX_ZERO_ONE    = 6, //!< Transmit preamble pattern.
	TMODE_TX_PN9         = 7, //!< Transmit pseudorandom (PN9) sequence.
	//TMODE_TX_PN          = 8,//!< Transmit a custom pseudorandom sequence.
	TMODE_TX_NB
}test_modes_tx_e;

/* ========================================================================== */
/* ================                 SM_CONFIG                ================ */
/* ========================================================================== */

typedef union {
    uint32_t WAKE_SOURCE;
    struct {
        uint32_t :16;
        uint32_t IRQ:8; // 100 : CSN low
        uint32_t EXT:8; /*
                            1   : IRQ0
                            10  : IRQ1
                            100 : RTC
                             */
    } WAKE_SOURCE_b;
} wake_source_t;

typedef union {
    uint32_t TRANSISION_TIME;
    struct {
        uint32_t TRANSISION_TIME_VAL:16;
    } TRANSISION_TIME_b;
} transision_time_t;

/**
  * @brief Internal State Machine Configuration (SM_CONFIG)
  */

typedef struct { /*!< SM_CONFIG Structure */
    uint32_t            RESERVED0[5];
    wake_source_t       wake_source;     /*!< Select the wake */
    uint32_t            RESERVED0a[4];
    transision_time_t   transision_time; /*!< Undocumented */
    uint8_t             GPIO_CMD_0;      /*!< State machine command triggered by IRQ_IN0 */
    uint8_t             RESERVED1;
    uint8_t             GPIO_CMD_1;      /*!< State machine command triggered by IRQ_IN1 */
} sm_config_t;

/*!
 * @}
 * @endcond
 */
/* ========================================================================== */

#include "adf7030-1_phy_profile.h"
#include "adf7030-1_phy_packet.h"
#include "adf7030-1_phy_luts.h"

/* ========================================================================== */
/* ================                 UNDOCUMENTED             ================ */
/* ========================================================================== */
// SERDES : Serializer/Deserializer
// MCR : Modem Configuration RAM
// CDR : Clock and Data Recovery
// AGC : Automatic Gain Control
// AFC : Automatic Frequency Control
// PGA : Programmable Gain Array

#ifdef __cplusplus
}
#endif
#endif /* _ADF7030_PHY_FW_H_ */

/*! @} */
