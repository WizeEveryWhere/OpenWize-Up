/**
  * @file adf7030-1_phy_profile.h
  * @brief This file define adf7030 internal registers
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
#ifndef _ADF7030_PHY_PROFILE_H_
#define _ADF7030_PHY_PROFILE_H_
#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @cond INTERNAL
 * @{
 */

/* ========================================================================== */
/* ================                PROFILE_CFG               ================ */
/* ========================================================================== */

typedef union {
    uint32_t XTAL_CFG;
    struct {
        uint32_t HFXTAL_LNB_TUNE : 7;
        uint32_t                 : 25 ;
        /*
        uint32_t HFXTAL_XTAL_FREQ_CON_1_0 :;
        uint32_t POST_HFXTAL_BIAS_SEL_DELAY :2;
        uint32_t HFXTAL_AMPCTRL :3;
        uint32_t HFXTAL_SPARE_WRITE_1_0 :2;
        uint32_t HFXTAL_ACB_GAIN :2;
        */
    } XTAL_CFG_b;
} xtal_cfg_t;

typedef union {
    uint32_t REF_CLK_CFG; /*!< Reference clock configuration */
    struct {
        uint32_t CLK_FREQ   : 25; // 25 or 26 bits (26 000 000 Hz)
        uint32_t            : 1;
        uint32_t CLK_TYPE   : 1;  /*!< External Reference Clock Source Type */
        uint32_t            : 1;
        //uint32_t DYNAMIC_SYSCLK_EN :;
        uint32_t SYSCLK_DIV : 1; // < 4 bits
        uint32_t            : 3;
    } REF_CLK_CFG_b; /*!< BitSize */
} ref_clk_cfg_t;

typedef union {
    uint32_t PACKET_CFG; /*!< Packet handler configuration */
    struct {
        uint32_t            : 14;
        //uint32_t PTR_FRAME0 :;
        uint32_t TYPE_FRAME0:  2; /*!< Select the packet format
                                     0 : Generic Packet Format
                                     1 : IEEE 502.15.4g-2012 packet format
                                    */
        /*
        uint32_t PTR_FRAME1 :;
        uint32_t TYPE_FRAME1 :;
        uint32_t RX_FRAME_SEL :;
        uint32_t TX_FRAME_SEL :;
        */

    } PACKET_CFG_b; /*!< BitSize */
} packet_cfg_t;

typedef union {
    uint32_t RADIO_MODES; /*!< Radio mode configuration */
    struct {
        uint32_t TRX_PHY_MODE       : 1; // See trx_phy_mode_e
        uint32_t FREQ_BAND          : 3;
        uint32_t                    : 1;
        uint32_t COMBINED_TRX_MATCH : 2; /*!< Combined match configuration
                                            0 : Disable combined match
                                            1 : Enable combined match
                                           */
        uint32_t                    : 1;
        uint32_t TX_MOD_TYPE        : 3; // see mod_type_e
        uint32_t RX_MOD_TYPE        : 3; // see mod_type_e
        uint32_t                    : 2;
        uint32_t GPIO_CLK_FREQ_SEL  : 3; /*!< Selection of Clock Frequency on
                                               selected GPIO with command
                                               GEN_GPIO_CLK
                                               (see gpio_clk_freq_e)*/
        uint32_t                    : 5;
        //uint32_t AUTO_PLL_CFG
        //uint32_t AUTO_CFG_DIG_RX
        //uint32_t AUTO_CFG_DIG_TX

        uint32_t AGC_OPER           : 2;
        uint32_t                    : 1;
        uint32_t AFC_OPER           : 2;
        uint32_t                    : 3;
    } RADIO_MODES_b; /*!< BitSize */
} radio_modes_t;

typedef union {
    uint32_t RADIO_DATA_RATE; /*!< */
    struct {
        uint32_t DATA_RATE                  : 16;
        uint32_t NUMERATOR_FRACN_DATARATE   : 8;
        uint32_t DENOMINATOR_FRACN_DATARATE : 8;
    } RADIO_DATA_RATE_b;
} radio_data_rate_t;

typedef union {
    uint32_t RADIO_DIG_RX_CFG; /*!< Rx Configuration */
    struct {
        uint32_t DEMOD_DISC_BW              : 8; /*!< Receiver discriminator bandwidth. */
        uint32_t DEMOD_POST_DEMOD_FILTER_BW : 4;
        uint32_t DEMOD_PRODUCT_SEL          : 1; /*!< Dot/cross product select.*/
        uint32_t DEMOD_CORE_CLK_DIVIDE      : 4; /*!< Divide down ratio of the
        											  demodulation core clock
        											  relative to the master clock.*/
        uint32_t LOW_SIDE                   : 1; /*!< Select high sided or low sided injection.
													  0: high sided injection.
													  1: low sided injection.
         	 	 	 	 	 	 	 	 	 	 	 */
        uint32_t DECIMATE_8XIF_CLK_DIVIDE   : 4; /*!< Divide down ratio of
        											  DECIMATE_8XIF_CLK relative
        											   to the master clock.*/
        uint32_t ADC_ANALOG_CLK_DIVIDE      : 4; /*!< Divide down ratio of SigmaDelta
        										 	  analog-to-digital converter
        										 	  (ADC) clock relative to the
        										 	  master clock rate.*/
        uint32_t INVERT                     : 1; /*!< 0: configure demodulation with dot product.
        											  1: configure demodulation with cross product. */
        uint32_t MAPPING_4FSK_SEL           : 3;
        uint32_t DEMOD_SCALING              : 2;
    } RADIO_DIG_RX_CFG_b;
} radio_dig_rx_cfg_t;

typedef union {
    uint32_t RADIO_DIG_TX_CFG0; /*!< Tx Configuration 0 */
    struct {
        uint32_t TX_FILTER_TYPE         : 1;
        uint32_t TX_FILTER_ENABLE       : 1; /*!< TX Filter state  (see
                                                   tx_filter_e)*/
        uint32_t TX_GAUSSIAN_BT         : 2; /*!< Time Constant (Bt Value) of
                                                   Gaussian Filter (see
                                                   tx_gaussion_bt_e) */
        uint32_t TX_INTERPOLATOR_EN     : 1;
        uint32_t TX_INTERPOLATOR_FACTOR : 2;
        uint32_t TX_SW_FILTER_LENGTH    : 2;
        uint32_t TX_OVERSAMPLING_FACTOR : 2;
        uint32_t                        : 1;
        uint32_t PA_COARSE              : 4; /*!< PA Output Power Coarse Value
                                                   (for PA1 : 1 to 6; for PA2 :
                                                   1 to 10)*/
        uint32_t PA_FINE                : 7; /*!< PA Output Power Fine Value.
                                                   Value may be 0 or 3 to 127
                                                   inclusive.*/
        uint32_t PA_MICRO               : 7; /*!< PA Output Power Micro Value*/
        uint32_t PA_SEL                 : 1; /*!< PA selected for Tx
                                                   0 : PA1
                                                   1 : PA2 */
        uint32_t PAO_LDO_SEL            : 1;
    } RADIO_DIG_TX_CFG0_b; /*!< BitSize */
} radio_dig_tx_cfg0_t;

typedef union {
    uint32_t RADIO_DIG_TX_CFG1; /*!< Tx Configuration 1 */
    struct {
        uint32_t FDEV                      : 12; // Frequency deviation in Hz/100)
        uint32_t PA_RAMP_RATE              : 3; /*!< Internal PA Ramp Rate (see
                                                      pa_ramp_rate_e)*/
        uint32_t EXT_PA_OOK_BIT_FRAMING_EN : 1; /*!< External PA OOK Bit Framing
                                                     Configuration
                                                 0 : Disable external PA OOK bit
                                                     framing.
                                                 1 : Enable external PA OOK bit
                                                     framing.
                                                */
        uint32_t EXT_PA_FRAMING_EN         : 1; /*!< External PA Framing
                                                      Configuration
                                                0 : Disable external PA framing.
                                                1 : Enable external PA framing.
                                                */
        uint32_t EXT_PA_PIN_SEL            : 3; /*!< GPIO selection for External
                                                      PA control*/
        uint32_t EXT_PA_GUARD_TIME         : 4;
        uint32_t EXT_LNA_FRAMING_EN        : 1; /*!< External LNA Framing
                                                     Configuration
                                                 0 : Disable external LNA framing.
                                                 1 : Enable external LNA framing.
                                                 */
        uint32_t EXT_LNA_PIN_SEL           : 3; /*!< GPIO selection for External
                                                      LNA*/
        uint32_t EXT_LNA_GUARD_TIME        : 4;
    } RADIO_DIG_TX_CFG1_b; /*!< BitSize*/
} radio_dig_tx_cfg1_t;

typedef union {
    uint32_t RADIO_DIG_TX_CFG2; /*!< Tx Configuration 2 */
    struct {
        uint32_t PAOLDO_VOUT_CON  : 4; /*!< LDO output Voltage Regulator Trim voltage */
        //uint32_t MAPPING_4FSK_SEL : ;
    } RADIO_DIG_TX_CFG2_b; /*!< BitSize */
} radio_dig_tx_cfg2_t;

typedef union {
    uint32_t RADIO_CDR_CFG;
    struct {
        uint32_t    :  32 ;
        /*
        uint32_t  CDR_EDGE_SELECT :;
        uint32_t  CDR_KNCO  :;
        uint32_t  CDR_KI  :;
        uint32_t  CDR_KT  :;
        uint32_t  MODE  :;
        uint32_t  LOCKUP_QUAL_VALUE :;
        uint32_t  SAMPLING_INST_DELAY_EN :;
        uint32_t  SAMPLING_INST_AT_NCO_VALUE :;
        */
    } RADIO_CDR_CFG_b;
} radio_cdr_cfg_t;

typedef union {
    uint32_t RADIO_PLL_CFG;
    struct {
        uint32_t    :  32 ;
        /*
        uint32_t  DELAYED_BLEED_EN :;
        uint32_t  BLEED_DEL :;
        uint32_t  BLEED_MODE :;
        uint32_t  SIGMA_DELTA_CLK_INVERT:;
        uint32_t  CBLD_CODE :;
        uint32_t  CBLD_MODE :;
        uint32_t  INTEGER_MODE:;
        uint32_t  POST_LOCK_DELAY:;
        uint32_t  DISABLE_HK :;
        uint32_t  PFDPHASE :;
        uint32_t  SDM_CLK_PHASE:;
        uint32_t  SDMDUTY :;
        uint32_t  SD_ORDER_TX:;
        uint32_t  SD_ORDER_RX :;
        uint32_t  SIGMA_DELTA_RESYNC_CLK_INVERT:;
        uint32_t  POST_BLEED_EN_DELAY  :;
        */
    } RADIO_PLL_CFG_b;
} radio_pll_cfg_t;

typedef union {
    uint32_t RADIO_AFC_CFG0;
    struct {
        uint32_t    :  32 ;
        /*
        uint32_t  AFC_ACQUIRE_KI  :;
        uint32_t  AFC_ACQUIRE_KP  :;
        uint32_t  AFC_TRACK_KI  :;
        uint32_t  AFC_TRACK_KP  :;
        */
    } RADIO_AFC_CFG0_b;
} radio_afc_cfg0_t;

typedef union {
    uint32_t RADIO_AFC_CFG1;
    struct {
        uint32_t AFC_MAX_RANGE     : 8;
        uint32_t KI_SCALING_FACTOR : 8;
        uint32_t AVG_FILT_LENGTH   : 8;
        uint32_t                   : 8;
    } RADIO_AFC_CFG1_b;
} radio_afc_cfg1_t;

typedef union {
    uint32_t RADIO_AFC_CFG2; /*!< AFC Configuration Register 2*/
    struct {
        uint32_t AFC_MODE              : 3; /*!< AFC mode */
        uint32_t AFC_INITIAL_CONDITION : 16;
        uint32_t AFC_SAMPLE_RATE       : 3;
        uint32_t AFC_BW                : 6; /*!< AFC measurement bandwidth (BW).*/
        uint32_t AFC_INVERT            : 1; /*!< AFC invert.*/
        uint32_t AFC_PRODUCT_SEL       : 1;
        uint32_t                       : 2;
    } RADIO_AFC_CFG2_b; /*!< BitSize*/
} radio_afc_cfg2_t;

typedef union {
    uint32_t RADIO_AGC_CFG0;
    struct {
        uint32_t MODE                     : 4;
        uint32_t LOW_THRESHOLD            : 8;
        uint32_t HIGH_THRESHOLD           : 8;
        uint32_t ALLOW_EXTRA_LO_LNA_GAIN  : 1;
        uint32_t NUM_ALLOWED_GAIN_CHANGES : 3;
        uint32_t SATURATION_LEVEL         : 8;
    } RADIO_AGC_CFG0_b;
} radio_agc_cfg0_t;

typedef union {
    uint32_t RADIO_AGC_CFG1;
    struct {
        uint32_t DISALLOW_MAX_GAIN                        : 1;
        uint32_t RSSI_OUT_SAT_GC                          : 4;
        uint32_t GC_HYST_LEVEL                            : 4;
        uint32_t PROG_SYM_COUNT_GC_DELAY                  : 8;
        uint32_t WIDEBAND_NOT_NARROWBAND_SELECT           : 4;
        uint32_t FILTER_GAIN_CTL_WB                       : 1;
        uint32_t DISALLOW_GAIN_CHANGE_BASED_ON_FILT_PKDET : 1;
        uint32_t DISALLOW_GAIN_CHANGE_BASED_ON_ADC_OF_OLF : 1;
        uint32_t DISALLOW_GAIN_CHANGE_BASED_ON_ADC_EST    : 1;
        uint32_t DISALLOW_GAIN_CHANGE_BASED_ON_CHFILT_EST : 1;
        uint32_t FILTERED_EST_BITS_SELECT                 : 1;
        uint32_t DISABLE_LAST_AGC_GAIN_CHANGE             : 1;
        uint32_t DISABLE_INTERFERER_3DB_TRIMING           : 1;
        uint32_t LIMIT_LNM_MAX_ATT                        : 1;
        uint32_t FUNC_RESET                               : 1;
        uint32_t                                          : 1;
    } RADIO_AGC_CFG1_b ;
} radio_agc_cfg1_t;

typedef union {
    uint32_t RADIO_AGC_CFG2;
    struct {
        uint32_t INT_HIGH_THRESHOLD   : 8;
        uint32_t INT_LOW_THRESHOLD    : 8;
        uint32_t INT_IGNORE_THRESHOLD : 8;
        uint32_t GEAR_SHIFT_THRESHOLD : 8;
    } RADIO_AGC_CFG2_b;
} radio_agc_cfg2_t;

typedef union {
    uint32_t RADIO_AGC_CFG3;
    struct {
        uint32_t FILTER_HI_GAIN_REG                  : 4;
        uint32_t FILTER_LO_GAIN_REG                  : 4;
        uint32_t FILT_PEAK_DETECT_QUAL_EN            : 1;
        uint32_t FILT_PEAK_DETECT_MAX_COUNT          : 6;
        uint32_t NB_INT_HI_TRIM_THRESHOLD            : 6;
        uint32_t FILT_PEAK_DETECT_SLIDING_OBS_WINDOW : 8;
        uint32_t                                     : 3;
    } RADIO_AGC_CFG3_b ;
} radio_agc_cfg3_t;

typedef union {
    uint32_t RADIO_AGC_CFG4;
    struct {
        uint32_t SKIP_NEXT_GAIN_STEP   : 1;
        uint32_t GAIN_CHANGE_ORDER     : 4;
        uint32_t LIMIT_NB_MAX_PGA_GAIN : 3;
        uint32_t LNM_GAIN              : 5;
        uint32_t LNM_ATT               : 3;
        uint32_t PGA_GAIN              : 5;
        uint32_t FILTER_GAIN           : 3;
        uint32_t GC_PHASE_EN           : 1;
        uint32_t GC_PHASE              : 4;
        uint32_t FILTER_AVG_WINDOW     : 3;
    } RADIO_AGC_CFG4_b ;
} radio_agc_cfg4_t;

typedef union {
    uint32_t RADIO_AGC_CFG5;
    struct {
    uint32_t    :  32 ;
        /*
        uint32_t ADC_ESTIMATOR_BANDWIDTH   :   ;
        uint32_t AGC_RSSI_NB_INT_LO_TRIM_THRESHOLD   :   ;
        uint32_t AGC_RSSI_HYST_CTL_SELECT   :   ;
        uint32_t PEAK_DETECT_BIAS_SEL   :   ;
        uint32_t PEAK_DETECT_HYST_EN   :  1 ;
        uint32_t FILT_OVERLOAD_6DB_GAIN_RED_EN   : 1  ;
        uint32_t ADC_OLF_6DB_GAIN_RED_EN   : 1  ;
        uint32_t ADC_OVERFLOW_QUAL_EN   :  1 ;
        uint32_t ADC_OVERFLOW_OBS_WINDOW   :   ;
        uint32_t ADC_OVERFLOW_QUAL_WINDOW   :   ;
        uint32_t ADC_OVERFLOW_QUAL_MAX_THRESHOLD   :   ;
        */
    } RADIO_AGC_CFG5_b ;
} radio_agc_cfg5_t;

typedef union {
    uint32_t RADIO_AGC_CFG6;
    struct {
        uint32_t INTERFERERS_HYST_HI_THR        : 5;
        uint32_t INTERFERERS_HYST_LO_THR        : 5;
        uint32_t INTERFERERS_HYST_QUAL_MAX_THR  : 6;
        uint32_t INTERFERERS_HYST_HI_THR_OFFSET : 6;
        uint32_t INTERFERERS_HYST_LO_THR_OFFSET : 6;
        uint32_t INTERFERERS_HYST_THR_QUAL_EN   : 1;
        uint32_t                                : 3;
    } RADIO_AGC_CFG6_b ;
} radio_agc_cfg6_t;

typedef union {
    uint32_t RADIO_AGC_CFG7;
    struct {
        uint32_t AGC_NB_CLK_DIVIDE                 : 8 ;
        uint32_t AGC_FILT_GAIN_OVRWT_VALUE         : 8;
        uint32_t AGC_NB_FILT_PKDET_QUAL_CLK_DIVIDE : 8;
        uint32_t AGC_WB_CLK_DIVIDE                 : 8;
    } RADIO_AGC_CFG7_b ;
} radio_agc_cfg7_t;

typedef union {
    uint32_t RADIO_AGC_CFG8;
    struct {
        uint32_t                        : 2;
        uint32_t WB_COMP_VREF_HI_CTRL   : 5;
        uint32_t WB_COMP_VREF_LO_CTRL   : 5;
        uint32_t WB_COMP_VREF_HYST_CTRL : 4;
        uint32_t WB_COMP_BW_CTRL        : 8;
        uint32_t AGC_DELAY_2_IF         : 8;
    } RADIO_AGC_CFG8_b ;
} radio_agc_cfg8_t;

typedef union {
    uint32_t OCL_CFG0;
    struct {
        uint32_t    :  32 ;
        /*
        uint32_t  DAC_RANGE :;
        uint32_t  LNA_OFF_DURING_OCL:;
        uint32_t  PGA_MEAS_CONFIG_DELAY:;
        uint32_t  LNM_GAIN_DURING_OCL:;
        uint32_t  LNM_ATT_DURING_OCL:;
        uint32_t  CAL_DELAY:;
        */
    } OCL_CFG0_b;
} ocl_cfg0_t;

typedef union {
    uint32_t OCL_CFG1;
    struct {
        uint32_t    :  32 ;
        /*
        uint32_t  INNER_PGA_MEAS_CONFIG_DELAY:;
        uint32_t  OUTER_PGA_MEAS_CONFIG_DELAY:;
        uint32_t  ALGO_SEL:;
        uint32_t  INNER_LOOP_DELAY_AFTER_GAIN_CHANGE:;
        uint32_t  OUTER_LOOP_DELAY_AFTER_GAIN_CHANGE:;
        */
    } OCL_CFG1_b;
} ocl_cfg1_t;

typedef union {
    uint32_t RADIO_VCO_CFG0;
    struct {
        uint32_t    :  32 ;
        /*
        uint32_t  AMPDET_REF_HP:;
        uint32_t  AMPDET_REF_LC:;
        uint32_t  KV_CAL_LUT_POPULATE_MODE:;
        uint32_t  AMPDET_SEL_RX:;
        uint32_t  AMPDET_SEL_TX:;
        uint32_t  KV_TARGET_KCONST:;
        uint32_t  EXEC_CONFIG:;
        uint32_t  AMP_CAL_SETTLE:;

         */
    } RADIO_VCO_CFG0_b;
} radio_vco_cfg0_t;

typedef union {
    uint32_t RADIO_VCO_CFG1;
    struct {
        uint32_t    :  32 ;
        /*

        uint32_t  VFORCE_VTUNE_DAC:;
        uint32_t  FREQ_MEASURE1_SETTLE:;
        uint32_t  FREQ_MEASURE1_WINDOW_NCYCLE:;
        uint32_t  FREQ_CAL_FINE_AMP_CAL_AFTER_BTEST:;
        uint32_t  FREQ_CAL_COARSE_AMP_CAL_AFTER_BTEST:;

         */
    } RADIO_VCO_CFG1_b;
} radio_vco_cfg1_t;

typedef union {
    uint32_t RADIO_ANC_CFG0;
    struct {
        uint32_t :   32;
        /*
        uint32_t  OPDIV_CTRL:;
        uint32_t  VTUNE_TRIM_LOWER:;
        uint32_t  KV_TARGET:;
        uint32_t  COARSE_CAL_FREQ_TARGET:;
         */
    } RADIO_ANC_CFG0_b;
} radio_anc_cfg0_t;

typedef union {
    uint32_t RADIO_ANC_CFG1;
    struct {
        uint32_t :   32;
        /*
        uint32_t  POST_TRIM_DELAY:;
        uint32_t  VTUNE_TRIM_MID:;
        uint32_t  VTUNE_TRIM_UPPER:;
        uint32_t  FREQ_MEASURE1_WINDOW_NCYCLE:;
        uint32_t  OPDIV_RATIO:;
         */
    } RADIO_ANC_CFG1_b;
} radio_anc_cfg1_t;

typedef union {
    uint32_t RADIO_LUT_CFG;
    struct {
        uint32_t VCO_SEL      : 1;
        uint32_t VCO_LOC      : 3;
        uint32_t PLLBW_SEL    : 1;
        uint32_t PLLBW_LOC    : 3;
        uint32_t PLLTXBW_SEL  : 1;
        uint32_t PLLTXBW_LOC  : 3;
        uint32_t ANAFILT_SEL  : 3;
        uint32_t ANAFILT_LOC  : 1;
        uint32_t DIGFILT_SEL  : 3;
        uint32_t DIGFILT_LOC  : 1;
        uint32_t DIGFILT2_SEL : 3;
        uint32_t DIGFILT2_LOC : 1;
        uint32_t              : 4;
        uint32_t RSSI_SEL     : 3;
        uint32_t RSSI_LOC     : 1;
    } RADIO_LUT_CFG_b;
} radio_lut_cfg_t;

typedef union {
    uint32_t RADIO_CAL_CFG0; /*!< Calibration Configuration 0 */
    struct {
        uint32_t  ANAFILT_RC_CAL_ENABLE :1;
        uint32_t                        :3;
        uint32_t  ADC_NOTCH_CAL_ENABLE  :1;
        uint32_t                        :1;
        uint32_t  ANA_QEC_CAL_ENABLE    :1;
        uint32_t                        :1;
        uint32_t  DIG_QEC_CAL_ENABLE    :1;
        uint32_t                        :1;
        uint32_t  ANCPLL_CAL_ENABLE     :1;
        uint32_t                        :1;
        uint32_t  LF_RC_CAL_ENABLE      :1;
        uint32_t                        :1;
        uint32_t  HF_RC_CAL_ENABLE      :1;
        uint32_t                        :1;

        uint32_t  VCO_CAL_ENABLE        :1;
        uint32_t                        :1;
        uint32_t  VCO_KV_CAL_ENABLE     :1;
        uint32_t                        :1;
        uint32_t  TEMP_XTAL_CAL_ENABLE  :1;
        uint32_t                        :1;
        uint32_t  HF_XTAL               :1;
        uint32_t                        :7;
        //uint32_t  OCL_CAL_ENABLE        :1;
        uint32_t  INLINE_OCL_CAL_ENABLE :1;
        uint32_t                        :1;
    } RADIO_CAL_CFG0_b ;
} radio_cal_cfg0_t;

typedef union {
    uint32_t RADIO_CAL_CFG1; /*!< Calibration Configuration 1 */
    struct {
        uint32_t             : 29;
        /*
        uint32_t  CAL_RCM_CTRL:;
        uint32_t  CAL_RLOAD_CTRL:;
        uint32_t  ADC_NOTCH_SAMPLE_CNT:;
        uint32_t  CALGAIN:;
        uint32_t  RASTER_STEP:;
        uint32_t  IR_CAL_DEBUG:;
        uint32_t  QEC_UPDATE_MODE:;
        uint32_t  QEC_EN:;
        uint32_t  BOUNDARY_SEARCH:;
        uint32_t  PUTTER:;
        uint32_t  RECENTER_PUTTER:;
        uint32_t  QEC_CAL_START:;
        uint32_t  IR_CAL_SOURCE:;
        uint32_t  ANC_PLL_LEVEL:;
        uint32_t  ALGORITHM:;
        */
        uint32_t  CAL_SUCCESS : 1; /*!< Cal Success*/
        /*
        uint32_t  EXT_OFFSET:;
        uint32_t  RSSI_METHOD:;
        0x2814350b
        1 01000000101000011010100001011
        */
    } RADIO_CAL_CFG1_b; /*!< BitSize */
} radio_cal_cfg1_t;

typedef union {
    uint32_t RADIO_CAL_CFG2; /*!< Calibration Configuration 2 */
    struct {
        uint32_t :   32;
        /*
        uint32_t  RSSI_READ_CNT:;
        uint32_t  DAC0_INT_EXT_OFF:;
        uint32_t  RSSI_DELAY_BETWEEN_READS:;
        uint32_t  DAC1_INT_EXT_OFF:;
         */
    } RADIO_CAL_CFG2_b ;
} radio_cal_cfg2_t;

typedef union {
    uint32_t RSSI_CFG; /*!< RSSI configuration */
    struct {
        uint32_t WB_OFFSET  : 10; /*!< Wideband RSSI offset in units of 0.36dBm.
                                       The offset value is an unsigned 10-bit
                                       number.*/
        uint32_t            :  6;
        uint32_t NB_OFFSET  : 10; /*!< Narrowband RSSI offset in units of 0.25dBm.
                                       The offset value is an unsigned 10-bit
                                       number. */
    } RSSI_CFG_b; /*!< BitSize */
} rssi_cfg_t;

typedef union {
    uint32_t CCA_CFG; /*!< CCA configuration */
    struct {
        uint32_t TICK_RATE       : 4;  /*!< This field sets the number of CCA
                                            ticks per RX data bit period (see
                                            cca_tick_rate_e) */
        uint32_t TICK_POSTSCALAR : 4;  /*!< Sets the number of CCA ticks between
                                            RSSI samples*/
        uint32_t DETECTION_TIME  : 8;  /*!< The number of RSSI samples taken
                                            before CCA detection period ends.
                                            A value 0 implies 'infinity mode'
                                            whereby CCA live status is
                                            continually updated and the
                                            ADF7030-1 stays in the CCA state */
        uint32_t THRESHOLD       : 11; /*!< Signed 11-bit number representing
                                            the CCA RSSI threshold, in units of
                                            0.25 dBm. */
    } CCA_CFG_b; /*!< BitSize */
} cca_cfg_t;

typedef union {
    uint32_t CCA_READBACK; /*!< CCA readback */
    struct {
        uint32_t VALUE         : 11; /*!< Signed 11-bit number representing the
                                          CCA RSSI value read in units of 0.25
                                          dBm */
        uint32_t               : 3;
        uint32_t LIVE_STATUS   : 1;  /*!< Live indication of CCA status, updated
                                          every RSSI sample.
                                          0 : RSSI >= THRESHOLD
                                          1 : RSSI < THRESHOLD
                                          */
        uint32_t STATUS        : 1;  /*!< Indicates CCA status at end of
                                          DETECTION_TIME
                                          0 : Channel is clear.
                                          1 : Channel is busy.
                                          */
        uint32_t BACKOFF_TICKS : 8;
        //uint32_t AVG_CNT : ;
        //uint32_t AVG : ;
    } CCA_READBACK_b; /*!< BitSize */
} cca_read_back_t;

typedef union {
    uint32_t LPM_CFG0; /*!< Low power mode configuration */
    struct {
        uint32_t                 : 10;
        /*
        uint32_t RTC_TRMVAL: ;
        uint32_t RTC_TRMADD: ;
        uint32_t RTC_TRMLVL: ;
        uint32_t RTC_TRMIVL2EXPMIN: ;
         */
        uint32_t RTC_EN          : 1;  /*!< Configure RTC alarm to wake device
                                            on expire
                                            0 : The RTC is disabled.
                                            1 : The RTC is enabled. */
        uint32_t                 : 1;
        // uint32_t RTC_TRMEN: ;
        uint32_t RTC_RESYNC      : 1;  /*!< Realign the RTC clock on entering
                                            PHY_SLEEP */
        uint32_t RTC_RECONFIG_EN : 1; /*!< Autoclearing RTC configuration
                                            enable flag. Set to 1 to trigger an
                                            RTC reconfiguration on CMD_CFG_DEV
                                            0 : The RTC is not reconfigured
                                            when CMD_CFG_DEV is issued.
                                            1 : The RTC is automatically
                                            reconfigured when CMD_CFG_DEV is
                                            issued. */
        uint32_t                 : 1;
        // uint32_t RTC_ONESHOT: ;

        uint32_t RTC_LF_SRC_SEL  : 1;  /*!< Use LFRC or LFXTAL as RTC source
                                            0 : LFRC selected as RTC source.
                                            1 : LFXTAL selected as RTC source.
                                        */
        uint32_t RETAIN_SRAM     : 1;  /*!< Enable retention of the BBRAM
                                            during PHY_SLEEP
                                            0 : SRAM is not retained in sleep.
                                            1 : SRAM is retained in sleep
                                            (requires more current). */
        uint32_t                 : 14;
        /*
        uint32_t RETAIN_GPIO: ;
        uint32_t SEQUENCER_EN: ;
        uint32_t SCRIPT_SEL: ;
        uint32_t BYPASS_CRC: ;
        */
        uint32_t ENABLE          : 1;  /*!< Global enable/disable for RTC/
                                            sequencer/BBRAM retention/GPIO
                                            behavior on startup */
    } LPM_CFG0_b; /*!< BitSize */
} lpm_cfg0_t;

typedef union {
    uint32_t LPM_CFG1; /*!< RTC configuration */
    struct {
        uint32_t :   32;
        /*
        uint32_t LPM_CFG1_b_RTC_PERIOD :;
         */
    } LPM_CFG1_b ;
} lpm_cfg1_t;

typedef union {
    uint32_t MONITOR0;
    struct {
        uint32_t :   32;
        /*
        uint32_t  TEMP_SCALING_CORR :;
        uint32_t  TEMP_OFFSET_CORR :;
        uint32_t  TEMP_DO_AS_PART_OF_CAL :;
         */
    } MONITOR0_b;
} monitor0_t;

typedef union {
    uint32_t MONITOR1; /*!< Monitor readback */
    struct {
        uint32_t TEMP_OUTPUT: 12; /*!< Temperature as a signed 12bit number in
                                        units of 0.0625 degrees Celsius.*/
        //uint32_t TEMP_SAMPLE_CNT :;
    } MONITOR1_b; /*!< BitSize */
} monitor1_t;

typedef union {
    uint32_t MISC0;
    struct {
        uint32_t :   32;
        /*
        uint32_t  SM_IRQ0_TYPE:;
        uint32_t  SM_IRQ1_TYPE:;
        uint32_t  PTRSTATIC_REG_FIX:;
        uint32_t  EN_BBREG_FIX:;
        uint32_t  EN_FUZREG_FIX:;
         */
    } MISC0_b;
} misc0_t;

typedef union {
    uint32_t GPCON0_3; /*!< GPIO0 to GPIO3 pin functionality selection */
    struct {
        uint32_t PIN0_CFG : 6; /*!< GPIO0 Configuration */
        uint32_t          : 2;
        uint32_t PIN1_CFG : 6; /*!< GPIO1 Configuration */
        uint32_t          : 2;
        uint32_t PIN2_CFG : 6; /*!< GPIO2 Configuration */
        uint32_t          : 2;
        uint32_t PIN3_CFG : 6; /*!< GPIO3 Configuration */
    } GPCON0_3_b; /*!< BitSize */
} gpcon0_3_t;

typedef union {
    uint32_t GPCON4_7; /*!< GPIO4 to GPIO7 pin functionality selection*/
    struct {
        uint32_t PIN4_CFG : 6; /*!< GPIO4 Configuration */
        uint32_t          : 2;
        uint32_t PIN5_CFG : 6; /*!< GPIO5 Configuration */
        uint32_t          : 2;
        uint32_t PIN6_CFG : 6; /*!< GPIO6 Configuration */
        uint32_t          : 2;
        uint32_t PIN7_CFG : 6; /*!< GPIO7 Configuration */
    } GPCON4_7_b; /*!< BitSize */
} gpcon4_7_t;

typedef union {
    uint32_t GPIO_CFG;
    struct {
        uint32_t GP_LOOP_PADS : 8;
        uint32_t GP_LOOP      : 8;
        uint32_t CUSTOM_SEL   : 8;
        uint32_t              : 8;
    } GPIO_CFG_b;
} gpio_cfg_t;

typedef union {
    uint32_t RADIO_CAL_RESULTS0; /*!< Radio Calibration Results 0 */
    struct {
        uint32_t :   32;
        /*
        uint32_t  FILT_M6DB_PGA_CTRL_OUT_MAX_1_I:;
        uint32_t  FILT_M6DB_PGA_CTRL_OUT_MAX_1_Q:;
        uint32_t  PGA_CTRL_OUT_MAX_1_I:;
        uint32_t  PGA_CTRL_OUT_MAX_1_Q:;

         */
    } RADIO_CAL_RESULTS0_b;
} radio_cal_results0_t;

typedef union {
    uint32_t RADIO_CAL_RESULTS1; /*!< Radio Calibration Results 1 */
    struct {
        uint32_t :   32;
        /*
        uint32_t  FILT_M6DB_PGA_CTRL_OUT_MAX_I:;
        uint32_t  FILT_M6DB_PGA_CTRL_OUT_MAX_Q:;
        uint32_t  PGA_CTRL_OUT_MAX_I:;
        uint32_t  PGA_CTRL_OUT_MAX_Q:;

         */
    } RADIO_CAL_RESULTS1_b;
} radio_cal_results1_t;

typedef union {
    uint32_t RADIO_CAL_RESULTS2; /*!< Radio Calibration Results 2 */
    struct {
        uint32_t :   32;
        /*
        uint32_t  RC_SCALE_FACTOR:;
        uint32_t  ADC_NOTCH_TUNE_I:;
        uint32_t  ADC_NOTCH_TUNE_Q:;
        uint32_t  RSSI_CALIBRATED_OFFSET:;

         */
    } RADIO_CAL_RESULTS2_b;
} radio_cal_results2_t;

typedef union {
    uint32_t RADIO_CAL_RESULTS3; /*!< Radio Calibration Results 3 */
    struct {
        uint32_t :   32;
        /*
        uint32_t  RSSI_CALIBRATED_OFFSET_ATTON:;
        uint32_t  NB_ALPHA:;
        uint32_t  NB_BETA:;

         */
    } RADIO_CAL_RESULTS3_b;
} radio_cal_results3_t;

typedef union {
    uint32_t RADIO_CAL_RESULTS4; /*!< Radio Calibration Results 4 */
    struct {
        uint32_t :   32;
        /*
        uint32_t  NB_K1:;
        uint32_t  NB_K2:;

         */
    } RADIO_CAL_RESULTS4_b;
} radio_cal_results4_t;

typedef union {
    uint32_t RADIO_CAL_RESULTS5; /*!< Radio Calibration Results 5 */
    struct {
        uint32_t :   32;
        /*
        uint32_t  WB_DAC0:;
        uint32_t  WB_DAC1:;

         */
    } RADIO_CAL_RESULTS5_b;
} radio_cal_results5_t;

typedef union {
    uint32_t RADIO_CAL_RESULTS6; /*!< Radio Calibration Results 6 */
    struct {
        uint32_t :   32;
        /*
        uint32_t  XTAL_COMP_VAL0:;
        uint32_t  XTAL_COMP_VAL1:;

         */
    } RADIO_CAL_RESULTS6_b;
} radio_cal_results6_t;

typedef union {
    uint32_t RADIO_CAL_RESULTS7; /*!< Radio Calibration Results 7 */
    struct {
        uint32_t :   32;
        /*
        uint32_t  XTAL_COMP_VAL2:;
        uint32_t  XTAL_COMP_VAL3:;
         */
    }RADIO_CAL_RESULTS7_b;
}radio_cal_results7_t;

typedef union {
    uint32_t RADIO_CAL_RESULTS8; /*!< Radio Calibration Results 8 */
    struct {
        uint32_t :   32;
        /*
        uint32_t  ANC_PLL_KV_CAL:;
        uint32_t  ANC_PLL_COARSE_CAL:;
        uint32_t  NOTCH_CTRL:;
        uint32_t  LFRC_FINE_CAL:;
        uint32_t  LFRC_COARSE_CAL:;
         */
    } RADIO_CAL_RESULTS8_b;
} radio_cal_results8_t;


typedef struct {
	radio_cal_results0_t RADIO_CAL_RESULTS0;
	radio_cal_results1_t RADIO_CAL_RESULTS1;
	radio_cal_results2_t RADIO_CAL_RESULTS2;
	radio_cal_results3_t RADIO_CAL_RESULTS3;
	radio_cal_results4_t RADIO_CAL_RESULTS4;
	radio_cal_results5_t RADIO_CAL_RESULTS5;
	radio_cal_results6_t RADIO_CAL_RESULTS6;
	radio_cal_results7_t RADIO_CAL_RESULTS7;
	radio_cal_results8_t RADIO_CAL_RESULTS8;
} radio_cal_results_t;


/**
  * @brief Radio Profile Configuration (PROFILE)
  */

typedef struct { /*!< PROFILE Structure */
    xtal_cfg_t            XTAL_CFG;
    ref_clk_cfg_t         REF_CLK_CFG;        /*!< Reference clock configuration*/
    uint32_t              CH_FREQ;            /*!< RF channel frequency */
    uint32_t              IF;
    packet_cfg_t          PACKET_CFG;         /*!< Packet handler configuration */
    radio_modes_t         RADIO_MODES;        /*!< Radio mode configuration */
    radio_data_rate_t     RADIO_DATA_RATE;    /*!<                          */
    radio_dig_rx_cfg_t    RADIO_DIG_RX_CFG;   /*!< Rx Configuration  */
    radio_dig_tx_cfg0_t   RADIO_DIG_TX_CFG0;  /*!< Tx Configuration 0 */
    radio_dig_tx_cfg1_t   RADIO_DIG_TX_CFG1;  /*!< Tx Configuration 1 */
    radio_dig_tx_cfg2_t   RADIO_DIG_TX_CFG2;  /*!< Tx Configuration 2 */
    radio_cdr_cfg_t       RADIO_CDR_CFG;
    radio_pll_cfg_t       RADIO_PLL_CFG;
    radio_afc_cfg0_t      RADIO_AFC_CFG0;
    radio_afc_cfg1_t      RADIO_AFC_CFG1;
    radio_afc_cfg2_t      RADIO_AFC_CFG2;     /*!< AFC Configuration Register 2*/
    radio_agc_cfg0_t      RADIO_AGC_CFG0;
    radio_agc_cfg1_t      RADIO_AGC_CFG1;
    radio_agc_cfg2_t      RADIO_AGC_CFG2;
    radio_agc_cfg3_t      RADIO_AGC_CFG3;
    radio_agc_cfg4_t      RADIO_AGC_CFG4;
    radio_agc_cfg5_t      RADIO_AGC_CFG5;
    radio_agc_cfg6_t      RADIO_AGC_CFG6;
    radio_agc_cfg7_t      RADIO_AGC_CFG7;
    radio_agc_cfg8_t      RADIO_AGC_CFG8;
    ocl_cfg0_t            OCL_CFG0;
    ocl_cfg1_t            OCL_CFG1;
    radio_vco_cfg0_t      RADIO_VCO_CFG0;
    radio_vco_cfg1_t      RADIO_VCO_CFG1;
    radio_anc_cfg0_t      RADIO_ANC_CFG0;
    radio_anc_cfg1_t      RADIO_ANC_CFG1;
    radio_lut_cfg_t       RADIO_LUT_CFG;
    uint32_t              RADIO_LUT_PTR;
    radio_cal_cfg0_t      RADIO_CAL_CFG0;     /*!< Calibration Configuration 0 */
    radio_cal_cfg1_t      RADIO_CAL_CFG1;     /*!< Calibration Configuration 1 */
    radio_cal_cfg2_t      RADIO_CAL_CFG2;     /*!< Calibration Configuration 2 */
    rssi_cfg_t            RSSI_CFG;           /*!< RSSI configuration */
    cca_cfg_t             CCA_CFG;            /*!< CCA configuration */
    cca_read_back_t       CCA_READBACK;       /*!< CCA readback */
    lpm_cfg0_t            LPM_CFG0;           /*!< Low power mode configuration */
    lpm_cfg1_t            LPM_CFG1;           /*!< RTC configuration */
    monitor0_t            MONITOR0;
    monitor1_t            MONITOR1;           /*!< Monitor readback */
    misc0_t               MISC0;
    gpcon0_3_t            GPCON0_3;           /*!< GPIO0 to GPIO3 pin
                                                   functionality selection */
    gpcon4_7_t            GPCON4_7;           /*!< GPIO4 to GPIO7 pin
                                                   functionality selection */
    gpio_cfg_t            GPIO_CFG;
    uint32_t              SPARE0;             /*!< Spare Register 0 */
    uint32_t              SPARE1;             /*!< Spare Register 1 */
    uint32_t              SPARE2;             /*!< Spare Register 2 */
    uint32_t              SPARE3;             /*!< Spare Register 3 */
    uint32_t              SPARE4;             /*!< Spare Register 4 */
    uint32_t              SPARE5;             /*!< Spare Register 5 */
    uint32_t              SPARE6;             /*!< Spare Register 6 */
    uint32_t              SPARE7;             /*!< Spare Register 7 */
    uint32_t              SPARE8;             /*!< Spare Register 8 */
    uint32_t              SPARE9;             /*!< Spare Register 9 */
    radio_cal_results0_t  RADIO_CAL_RESULTS0; /*!< Radio Calibration Results 0 */
    radio_cal_results1_t  RADIO_CAL_RESULTS1; /*!< Radio Calibration Results 1 */
    radio_cal_results2_t  RADIO_CAL_RESULTS2; /*!< Radio Calibration Results 2 */
    radio_cal_results3_t  RADIO_CAL_RESULTS3; /*!< Radio Calibration Results 3 */
    radio_cal_results4_t  RADIO_CAL_RESULTS4; /*!< Radio Calibration Results 4 */
    radio_cal_results5_t  RADIO_CAL_RESULTS5; /*!< Radio Calibration Results 5 */
    radio_cal_results6_t  RADIO_CAL_RESULTS6; /*!< Radio Calibration Results 6 */
    radio_cal_results7_t  RADIO_CAL_RESULTS7; /*!< Radio Calibration Results 7 */
    radio_cal_results8_t  RADIO_CAL_RESULTS8; /*!< Radio Calibration Results 8 */
} profile_t;

/*!
 * @}
 * @endcond
 */

#ifdef __cplusplus
}
#endif
#endif /* _ADF7030_PHY_PROFILE_H_ */

/*! @} */
