/**
  * @file: adf7030-1_phy_packet.h
  * @brief: // TODO This file ...
  * 
  *****************************************************************************
  * @Copyright 2019, GRDF, Inc.  All rights reserved.
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
  *****************************************************************************
  *
  * Revision history
  * ----------------
  * 1.0.0 : 2020/04/25[TODO: your name]
  * Initial version
  *
  *
  */

/*!
 * @ingroup OpenWize'Up
 * @{
 *
 */
#ifndef _ADF7030_PHY_PACKET_H_
#define _ADF7030_PHY_PACKET_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/* ================                GENERIC_PKT               ================ */
/* ========================================================================== */

typedef union {
    uint32_t BUFF_CFG0; /*!< Transmit/Receive Buffer Configuration 0*/
    struct {
        uint32_t PTR_RX_BASE       : 11; /*!< Rx base buffer offset pointer. The
                                              base address of the Rx payload is
                                              0x20000000 + (PTR_RX_BASE x 4) */
        uint32_t PTR_TX_BASE       : 11; /*!< Tx base buffer offset pointer. The
                                              base address of the Tx payload is
                                              0x20000000 + (PTR_TX_BASE x 4) */
        uint32_t BIT2AIR           : 1;  /*!< For generic packet format Tx:
                                              specifies which bit of payload
                                              bytes is Tx first (0 = most
                                              significant bit (MSB) first). For
                                              generic packet format Rx: specifies
                                              into which bit the first bit
                                              received of payload is written
                                              (0 = MSB) */
        uint32_t                   : 1;
        //uint32_t CHECK_BUFF_OVERLAP :;
        uint32_t ROLLING_BUFF_EN   : 1;  /*!< Enable the rolling buffer mode.
                                              When the number of bytes received
                                              equals RX_SIZE/2 or TX_SIZE/2, the
                                              half full IRQ is asserted. When
                                              number of bytes received equals
                                              RX_SIZE or TX_SIZE, the full IRQ
                                              is asserted. */
        // EXTENDED_BUFF_EN
        // EXTENDED_BUFF_BASE_SEL
         // pGENERIC_PKT->BUFF_CFG1_BIS_b.TRX_SIZE
    } BUFF_CFG0_b; /*!< BitSize */
}buff_cfg0_t;

typedef union {
    uint32_t BUFF_CFG1; /*!< Transmit/Receive Buffer Configuration 1*/
    struct {
        uint32_t RX_SIZE         : 9; /*!< Maximum size of the Rx buffer.*/
        uint32_t TX_SIZE         : 9; /*!< Maximum size of the Tx buffer in
                                           octets. In nonrolling buffer mode,
                                           the ADF7030-1 does not transmit data
                                           written beyond the buffer delimited
                                           by this size. In rolling buffer mode,
                                           this is the size of the Tx buffer. */
        uint32_t TRX_BLOCK_SIZE  : 8; /*!< Set the multiple of bytes for which
                                           the PAYLOAD_BLOC_IRQ interrupt is
                                           asserted during packet reception or
                                           transmission (not used for IEEE802.15
                                           .4g). e.g. Set to 4 to cause an IRQ
                                           every 4 bytes.*/
        uint32_t                 : 1;
        // uint32_t RX_BUFF_RAWDATA :;
        uint32_t TX_BUFF_RAWDATA : 1; /*!< Transmit only the payload */
        uint32_t                 : 1;
        //uint32_t CONTINUOUS_RX :;
        uint32_t TURNAROUND_RX   : 1; /*!< Enable automatic PHY_RX to PHY_TX
                                           transition on completion of packet
                                           reception if packet with correct CRC
                                           (if CRC enabled) */
        uint32_t                 : 1;
        //uint32_t CONTINUOUS_TX  :;
        uint32_t TURNAROUND_TX   : 1; /*!< Enable automatic PHY_TX to PHY_RX
                                            transition on completion of packet
                                            transmission */
    } BUFF_CFG1_b;                     /*!< BitSize */
}buff_cfg1_t;

typedef union {
    uint32_t  FRAME_CFG0; /*!< Generic Packet Frame Configuration 0*/
    struct {
        uint32_t PREAMBLE_LEN    : 8; /*!< Number of units of preamble at start
                                           of Tx packet (TX only); see also
                                           PREAMBLE_UNIT */
        uint32_t                 : 8;
        //uint32_t PREAMBLE_LEVEL :;
        //uint32_t SYNC0_WAIT:;
        uint32_t SYNC0_LEN       : 6; /*!< Length of the Syncword 0 in bits (Rx
                                           and Tx) */
        uint32_t                 : 2;
        //uint32_t SYNC0_ERR:;
        uint32_t CRC_LEN         : 6; /*!< Generic packet: CRC length used in Rx
                                           and Tx. IEEE802.15.4g: FCS length used
                                           in Tx only; Rx FCS length inferred
                                           from received PHR. */
        //uint32_t CRC_TYPE:;
    } FRAME_CFG0_b;                   /*!< BitSize */
}frame_cfg0_t;

typedef union {
    uint32_t FRAME_CFG1;/*!< Generic Packet Frame Configuration 1*/
    struct {
         uint32_t  PAYLOAD_SIZE      : 12; /*!< Generic packet only: sets number
                                                of payload bytes in the Tx packet
                                                (Raw mode only). Sets the number
                                                of payload bytes that are received
                                                in the incoming packet payload
                                                (Rx raw mode only).*/
         uint32_t  PREAMBLE_UNIT     : 1;  /*!< Unit of preamble length for Tx
                                                0: bits; 1: bytes */
         uint32_t                    : 3;
         //uint32_t  NRZI_RUN_LENGTH :;
         //uint32_t  NRZI_SEED:;
         uint32_t  TRX_IRQ0_TYPE     : 8;  /*!< Select sources of interrupt on
                                                IRQ_OUT0 during Rx and Tx */
         uint32_t  TRX_IRQ1_TYPE     : 8;  /*!< Select sources of interrupt on
                                                IRQ_OUT1 during Rx & Tx */
    } FRAME_CFG1_b;                        /*!< BitSize */
}frame_cfg1_t;

typedef union {
    uint32_t FRAME_CFG2; /*!< Generic Packet Frame Configuration 2*/
    struct {
        uint32_t                    : 3;
        //uint32_t SYNC1_WAIT_VAL :;
        //uint32_t SYNC1_ERR_VAL :;
        uint32_t SYNC1_LEN          : 6; /*!< Length of the Syncword 1 in bits.
                                               Only used in IEEE802.15.4g Rx */
        uint32_t                    : 2;
        uint32_t CRC_SHIFT_IN_ZEROS : 1; /*!< Shift in CRC length of zeros after
                                              all bytes have passed through CRC
                                              calculation. Determines whether the
                                              final register value is reversed*/
        uint32_t LEN_SEL            : 2; /*!< Selects the size of the length
                                              field in the received or
                                              transmitted message (Rx and Tx
                                              generic packet only) */
        uint32_t                    : 2;
        uint32_t PREAMBLE_VAL       : 8; /*!< For Tx this is the preamble
                                              pattern used in the outgoing
                                              packet. For Rx, this must be set
                                              to the expected preamble, e.g.
                                              0x55 or 0xAA. If this field is
                                              set to 0 a default value of 0x55
                                              is used. */
        uint32_t ENDEC_MODE         : 8; /*!< Line coding scheme (generic
                                              packet Tx only)
                                              00000001: NRZ encoding.
                                              00000100: Manchester encoding.
                                              00001000: FEC encoding.
                                              10000000: Polarity bit. Set to 1
                                              to invert polarity of the data
                                              over the air. */
        // uint32_t CRC_REFLECT_IN : ?;
        // uint32_t CRC_REFLECT_OUTPUT_CRC : ?;
    } FRAME_CFG2_b;                      /*!< BitSize */
}frame_cfg2_t;

typedef union {
    uint32_t  FRAME_CFG3; /*!< Generic Packet Frame Configuration 3*/
    struct {
        uint32_t                  : 16;
        //uint32_t  PAYLOAD_EXTRA :;
        //uint32_t  NRZI_MODE     :;
        uint32_t  RX_LENGTH       : 16; /*!< Generic packet Rx: The contents of
                                             the length field in the received
                                             packet. IEEE802.15.4g: the received
                                             PHR.*/
    } FRAME_CFG3_b;                     /*!< BitSize */
}frame_cfg3_t;

typedef union {
    uint32_t FRAME_CFG4;
    struct {
        uint32_t : 32;
        /*
        uint32_t  RX_DELAY:;
        uint32_t  TX_DELAY:;
        uint32_t  DELAY_UNIT:;
        uint32_t  SYNC0_OFFSET:;
        uint32_t  SYNC1_OFFSET:;
        */
    } FRAME_CFG4_b;
}frame_cfg4_t;

typedef union {
    uint32_t FRAME_CFG5; /*!< Generic Packet Frame Configuration 5*/
    struct {
        uint32_t TX_PHR        : 16; /*!< PHY Header (PHR) used as first two
                                          octets of IEEE802.15.4g Tx packet */
        /*
        uint32_t  USE_DEBUG_PHR :;
        uint32_t  DEBUG_PHR:;
        uint32_t  PN_SCHEME:;
        uint32_t  INTERLEAVE_EN:;
        uint32_t  FEC_SCHEME:;
        */
    } FRAME_CFG5_b;                    /*!< BitSize */
}frame_cfg5_t;

typedef union {
    uint32_t LOCK_CFG; /*!< RX lock configuration*/
    struct {
        uint32_t : 32;
        /*
        uint32_t LOCK_CFG_b_UNUSED :;
        */
    } LOCK_CFG_b;
}lock_cfg_t;

typedef union {
    uint32_t TICK_CFG; /*!< RSSI configuration*/
    struct {
        uint32_t  SEARCH_RATE       :4; /* Adjusts the CCA tick time. */
        uint32_t  SEARCH_POSTSCALAR :4; /* Set the CCA tick time
                                           0x1 : Tx user clock (phaes adjusted
                                           from CDR clock.
                                           0x2 : 2x data rate.
                                           0x3 : 3x data rate.
                                           0x4 : 4x data rate.
                                           0x5 : 5x data rate.
                                           0x6 : 6x data rate.
                                           0x7 : 7x data rate.
                                           0x8 : 8x data rate.*/
        /*
        uint32_t  PAYLOAD_RATE :;
        uint32_t  PAYLOAD_POSTSCALAR :;
        */
    } TICK_CFG_b;
}tick_cfg_t;

typedef union {
    uint32_t SEARCH_DETECT; /*!< Packet detection configuration*/
    struct {
        uint32_t AFC_THRESHOLD  : 16;
        uint32_t RSSI_THRESHOLD : 12;
        uint32_t PREAMBLE_LEN   : 4;
    }SEARCH_DETECT_b;
}search_detect_t;

typedef union {
    uint32_t SEARCH_QUAL; /*!< Packet qualification configuration*/
    struct {
        uint32_t AFC_THRESHOLD  : 16;
        uint32_t RSSI_THRESHOLD : 12;
        uint32_t PREAMBLE_LEN   : 4;
    }SEARCH_QUAL_b;
}search_qual_t;

typedef union {
    uint32_t LIVE_LINK_QUAL; /*!< RX link quality readback */
    struct {
        uint32_t             : 16;
        // uint32_t AFC_ERROR :;
        uint32_t  RSSI       : 11; /*!< RSSI as a signed 11-bit value in units
                                        of 0.25 dBm measured during packet
                                        reception */
    } LIVE_LINK_QUAL_b;      /*!< BitSize */
}live_link_qual_t;

typedef union {
    uint32_t LPM_CFG; /*!< Low Power Mode Configuration */
    struct {
        uint32_t                            : 8;
        uint32_t PREAMBLE_DWELL_TIME        : 8; /*!< Number of symbols allowed
                                                       for start of syncword */
        uint32_t PREAMBLE_QUAL_DWELL_TIME   : 8; /*!< Number of symbols allowed
                                                      for AFC qualification */
        uint32_t PREAMBLE_DETECT_DWELL_TIME : 8; /*!< Number of symbols allowed
                                                      for RSSI qualification */
    } LPM_CFG_b; /*!< BitSize */
}lpm_cfg_t;

typedef union {
    uint32_t TEST_MODES0; /*!< Test Mode Configuration 0 */
    struct {
        uint32_t PER_EN             : 1;
        uint32_t                    : 10;
        uint32_t PER_IRQ_SELF_CLEAR : 1;
        uint32_t TX_TEST            : 4;  /*!< Continuous Tx test modes */
/*
#define TEST_MODES0_b_PER_EN
#define TEST_MODES0_b_PER_RANDOM_PAYLOAD_DATA
#define TEST_MODES0_b_PER_RANDOM_PAYLOAD_SIZE
#define TEST_MODES0_b_PER_RANDOM_DELAY
#define TEST_MODES0_b_PER_DELAY
#define TEST_MODES0_b_PER_IRQ_SELF_CLEAR
#define TEST_MODES0_b_BER_EN
#define TEST_MODES0_b_BER_SYNC_FOUND
#define TEST_MODES0_b_TX_TEST
#define TEST_MODES0_b_RX_TEST
#define TEST_MODES0_b_DELAY_UNIT
 */
    } TEST_MODES0_b;                         /*!< BitSize */
}test_modes0_t;

typedef union {
    uint32_t TEST_MODES1; /*!< Test Mode Configuration 1*/
    struct {
        uint32_t PACKET_CNT  : 16;
        uint32_t CURRENT_CNT : 16;
    } TEST_MODES1_b; /*!< BitSize */
}test_modes1_t;

typedef union {
    uint32_t RESAMPLE_CFG; /*!< Resample configuration*/
    struct {
        uint32_t : 32;
        /*
#define RESAMPLE_CFG_b_USER_CLK_CDRSYNC_EN
#define RESAMPLE_CFG_b_USER_CLK_PHASE
#define RESAMPLE_CFG_b_SAMPLE_START_POSITION
#define RESAMPLE_CFG_b_SAMPLE_NUMBER
#define RESAMPLE_CFG_b_DEMOD_SAMPLING_MODE
#define RESAMPLE_CFG_b_CLK_OUTPUT_SELECT
#define RESAMPLE_CFG_b_HARD_OUPUT_SELECT
#define RESAMPLE_CFG_b_SOFT_CH1_OUTPUT_SELECT
#define RESAMPLE_CFG_b_SOFT_CH0_OUTPUT_SELECT
#define RESAMPLE_CFG_b_LOCK_CDR
#define RESAMPLE_CFG_b_LOCK_AGC
#define RESAMPLE_CFG_b_UNLOCK_AFC
        */
    } RESAMPLE_CFG_b; /*!< BitSize */
}resample_cfg_t;

typedef union {
    uint32_t LCPSM1; /*!< Low Current Packet Search Mode Configuration 1 */
    struct {
        uint32_t LCPSM_ENERGY_CNT: 16; /*!< Number of samples of energy to take
                                            during LCPSM energy detection phase*/
        uint32_t LCPSM_LOW_CNT   : 16; /*!< Number of SERDES interrupts to to
                                            remain in LCPSM idle state for */
    } LCPSM1_b;                        /*!< BitSize       */
}lcpsm1_t;

typedef union {
    uint32_t LCPSM2; /*!< Low Current Packet Search Mode Configuration 2 */
    struct {
         uint32_t LCPSM_AFC_THRESHOLD: 16; /*!< AFC threshold to use while in
                                                LCPSM energy detection phase */
         uint32_t                    : 15;
         uint32_t LCPSM_ENABLED      : 1;  /*!< Indicate whether LCPSM is
                                                enabled or not. Note that this
                                                bit only gets set on entry into
                                                PHY_RX when LCPSM is enabled.
                                                When disabling LCPSM, this bit
                                                should be updated immediately.*/
    } LCPSM2_b;                            /*!< BitSize */
}lcpsm2_t;

/**
  * @brief Generic Packet Configuration (GENERIC_PKT)
  */
typedef struct { /*!< GENERIC_PKT Structure */
    buff_cfg0_t      BUFF_CFG0;      /*!< Transmit/Receive Buffer Configuration 0*/
    buff_cfg1_t      BUFF_CFG1;      /*!< Transmit/Receive Buffer Configuration 1*/
    frame_cfg0_t     FRAME_CFG0;     /*!< Generic Packet Frame Configuration 0*/
    frame_cfg1_t     FRAME_CFG1;     /*!< Generic Packet Frame Configuration 1*/
    frame_cfg2_t     FRAME_CFG2;     /*!< Generic Packet Frame Configuration 2*/
    frame_cfg3_t     FRAME_CFG3;     /*!< Generic Packet Frame Configuration 3*/
    frame_cfg4_t     FRAME_CFG4;     /*!< Generic Packet Frame Configuration 4*/
    frame_cfg5_t     FRAME_CFG5;     /*!< Generic Packet Frame Configuration 5*/
    uint32_t         SYNCWORD0;      /*!< Sync Word 0 */
    uint32_t         SYNCWORD1;      /*!< Sync Word 1 */
    uint32_t         CRC_POLY;       /*!< CRC polynomial */
    uint32_t         CRC_SEED;       /*!< CRC initial seed */
    uint32_t         CRC_FINAL_XOR;  /*!< CRC XOR value */
    lock_cfg_t       LOCK_CFG;       /*!< RX lock configuration*/
    tick_cfg_t       TICK_CFG;       /*!< RSSI configuration*/
    search_detect_t  SEARCH_DETECT;  /*!< Packet detection configuration*/
    search_qual_t    SEARCH_QUAL;    /*!< Packet qualification configuration*/
    live_link_qual_t LIVE_LINK_QUAL; /*!< RX link quality readback */
    uint32_t         MISC0;          /*!< Miscellaneous register 0 */
    uint32_t         MISC1;          /*!< Miscellaneous register 1 */
    lpm_cfg_t        LPM_CFG;        /*!< Low Power Mode Configuration */
    test_modes0_t    TEST_MODES0;    /*!< Test Mode Configuration 0 */
    test_modes1_t    TEST_MODES1;    /*!< Test Mode Configuration 1*/
    resample_cfg_t   RESAMPLE_CFG;   /*!< Resample configuration*/
    lcpsm1_t         LCPSM1;         /*!< Low Current Packet Search Mode
                                          Configuration 1 */
    lcpsm2_t         LCPSM2;         /*!< Low Current Packet Search Mode
                                          Configuration 2 */
    uint32_t         MISC2;          /*!< MISC2 Register */
    uint32_t         LCPSM3;         /*!< Low Current Packet Search Mode
                                          Configuration 3 */
} generic_pkt_t;

#ifdef __cplusplus
}
#endif
#endif /* _ADF7030_PHY_PACKET_H_ */

/*! @} */
