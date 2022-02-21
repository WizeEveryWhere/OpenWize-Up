/**
  * @file: adf7030-1_phy_luts.h
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
#ifndef _ADF7030_PHY_LUTS_H_
#define _ADF7030_PHY_LUTS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/* ================               ANAFILT_LUTS               ================ */
/* ========================================================================== */

typedef union {
    uint32_t FILT_CONFIG;
    struct {
    /*
           ANAFILT_RAM_LUT1_FILT_CONFIG_b_FAST_CHARGE_VAL
           ANAFILT_RAM_LUT1_FILT_CONFIG_b_SPARE1_VAL
           ANAFILT_RAM_LUT1_FILT_CONFIG_b_SPARE2_VAL
           ANAFILT_RAM_LUT1_FILT_CONFIG_b_CROSS_I_VAL
           ANAFILT_RAM_LUT1_FILT_CONFIG_b_CROSS_Q_VAL
           ANAFILT_RAM_LUT1_FILT_CONFIG_b_AMP_BIAS_SPARE_VAL
    */
        uint32_t PD_N_VAL                 : 5;
        uint32_t                          : 3;
        uint32_t AMP_BIAS_OUTPUT_CTRL_VAL : 2;
        uint32_t                          : 8;
        uint32_t AMP_BIAS_8U_EN_VAL       : 5;
        uint32_t                          : 1;
        uint32_t AMP_BIAS_TYPE_VAL        : 3;
        uint32_t                          : 1;
        uint32_t MODE_VAL                 : 2;
        uint32_t                          : 2;
    } FILT_CONFIG_b;
} FILT_CONFIG_t;

typedef union {
    uint32_t FILT_STG;
    struct {
        uint32_t CAP_VAL   : 8;
        uint32_t R1_VAL    : 5;
        uint32_t R2_VAL    : 5;
        uint32_t R3_VAL    : 5;
        uint32_t SPARE_VAL : 1;
        uint32_t RC_VAL    : 8;
    } FILT_STG_b;
} FILT_STG_t;

typedef struct {/*!< ANAFILT_LUTS Structure */
    FILT_CONFIG_t FILT_CONFIG;
    FILT_STG_t FILT_STG1;
    FILT_STG_t FILT_STG2;
    FILT_STG_t FILT_STG3;
    FILT_STG_t FILT_STG4;
    FILT_STG_t FILT_STG5;
} ANAFILT_LUT_t;

/**
  * @brief Analog filter LUTs (ANAFILT_LUTS)
  */

typedef struct {/*!< ANAFILT_LUTS Structure */
    ANAFILT_LUT_t ANAFILT_LUT0;
    ANAFILT_LUT_t ANAFILT_LUT1;
    ANAFILT_LUT_t ANAFILT_LUT2;
} ANAFILT_LUTS_t;

/* ========================================================================== */
/* ================               DIGFILT_LUTS               ================ */
/* ========================================================================== */

typedef union {
    uint32_t COEF_SEG;
    struct {
       uint32_t A1 : 16;
       uint32_t A2 : 16;
    } COEF_SEG_b;
} COEF_SEG_t;

typedef struct { /*!< DIGFILT_LUTS Structure  */
    COEF_SEG_t COEF_SEG0;
    COEF_SEG_t COEF_SEG1;
    COEF_SEG_t COEF_SEG2;
    COEF_SEG_t COEF_SEG3;
    COEF_SEG_t COEF_SEG4;
    COEF_SEG_t COEF_SEG5;
    COEF_SEG_t COEF_SEG6;
    COEF_SEG_t COEF_SEG7;
} DIGFILT_LUT_t;

/**
  * @brief Digital filter LUTs 1 (DIGFILT_LUTS)
  */

typedef struct { /*!< DIGFILT_LUTS Structure  */
    DIGFILT_LUT_t DIGFILT_LUT0;
    DIGFILT_LUT_t DIGFILT_LUT1;
} DIGFILT_LUTS_t;

/* ========================================================================== */
/* ================               DIGFILT2_LUTS              ================ */
/* ========================================================================== */

typedef union {
    uint32_t CHFILT_CFG;
    struct {
        uint32_t  :1;
        uint32_t SECTION_IN_FRAC :3;
        uint32_t  :1;
        uint32_t SCALE_FRAC :1;
        uint32_t SOS_CNT :3;
        //uint32_t TYPE :;
        //uint32_t BYPASS :;
    } CHFILT_CFG_b;
} CHFILT_CFG_t;

typedef union {
    uint32_t CHFILT_SCALE_01;
    struct {
        uint32_t S0 :16;
        uint32_t S1 :16;
    } CHFILT_SCALE_01_b;
} CHFILT_SCALE_01_t;

typedef union {
    uint32_t CHFILT_SCALE_23;
    struct {
        uint32_t S2 :16;
        uint32_t S3 :16;
    } CHFILT_SCALE_23_b;
} CHFILT_SCALE_23_t;

typedef union {
    uint32_t CHFILT_SCALE_45;
    struct {
        uint32_t S4 :16;
        uint32_t S5 :16;
    } CHFILT_SCALE_45_b;
} CHFILT_SCALE_45_t;

typedef union {
    uint32_t CHFILT_SCALE_67;
    struct {
        uint32_t S6 :16;
        uint32_t S7 :16;
    } CHFILT_SCALE_67_b;
} CHFILT_SCALE_67_t;

typedef struct { /*!< DIGFILT2_LUTS Structure */
    CHFILT_CFG_t CHFILT_CFG;
    CHFILT_SCALE_01_t CHFILT_SCALE_01;
    CHFILT_SCALE_23_t CHFILT_SCALE_23;
    CHFILT_SCALE_45_t CHFILT_SCALE_45;
    CHFILT_SCALE_67_t CHFILT_SCALE_67;
} DIGFILT2_LUT_t;

/**
  * @brief Digital filter LUTs (DIGFILT2_LUTS)
  */

typedef struct { /*!< DIGFILT2_LUTS Structure */
    DIGFILT2_LUT_t DIGFILT2_LUT0;
    DIGFILT2_LUT_t DIGFILT2_LUT1;
} DIGFILT2_LUTS_t;


/* ========================================================================== */
/* ================               RSSICFG_LUTS               ================ */
/* ========================================================================== */

typedef union {
    uint32_t GAIN_OFFSET_LNM_ATT;
    struct {
        uint32_t LNM_ATT_0 : 8;
        uint32_t LNM_ATT_1 : 8;
        uint32_t LNM_ATT_2 : 8;
        uint32_t LNM_ATT_3 : 8;
    } GAIN_OFFSET_LNM_ATT_b;
} GAIN_OFFSET_LNM_ATT_t;

typedef union {
    uint32_t GAIN_OFFSET_LNM_GAIN_0_1_2_3;
    struct {
        uint32_t LNM_GAIN_0 : 8;
        uint32_t LNM_GAIN_1 : 8;
        uint32_t LNM_GAIN_2 : 8;
        uint32_t LNM_GAIN_3 : 8;
    } GAIN_OFFSET_LNM_GAIN_0_1_2_3_b;
} GAIN_OFFSET_LNM_GAIN_0_1_2_3_t;

typedef union {
    uint32_t GAIN_OFFSET_LNM_GAIN_7_15_MAX;
    struct {
        uint32_t LNM_GAIN_7   : 8;
        uint32_t LNM_GAIN_15  : 8;
        uint32_t LNM_GAIN_MAX : 8;
        uint32_t : 8;
    } GAIN_OFFSET_LNM_GAIN_7_15_MAX_b;
} GAIN_OFFSET_LNM_GAIN_7_15_MAX_t;

typedef union {
    uint32_t GAIN_OFFSET_FILTER_GAIN_0_3;
    struct {
        uint32_t FILT_GAIN_0 : 8;
        uint32_t FILT_GAIN_1 : 8;
        uint32_t FILT_GAIN_2 : 8;
        uint32_t FILT_GAIN_3 : 8;
    } GAIN_OFFSET_FILTER_GAIN_0_3_b;
} GAIN_OFFSET_FILTER_GAIN_0_3_t;

typedef union {
    uint32_t GAIN_OFFSET_FILTER_GAIN_4_7;
    struct {
        uint32_t FILT_GAIN_4 : 8;
        uint32_t FILT_GAIN_5 : 8;
        uint32_t FILT_GAIN_6 : 8;
        uint32_t FILT_GAIN_7 : 8;
    }GAIN_OFFSET_FILTER_GAIN_4_7_b;
} GAIN_OFFSET_FILTER_GAIN_4_7_t;

typedef union {
    uint32_t GAIN_OFFSET_FILTER_GAIN_8_11;
    struct {
        uint32_t FILT_GAIN_8  : 8;
        uint32_t FILT_GAIN_9  : 8;
        uint32_t FILT_GAIN_10 : 8;
        uint32_t FILT_GAIN_11 : 8;
    } GAIN_OFFSET_FILTER_GAIN_8_11_b;
} GAIN_OFFSET_FILTER_GAIN_8_11_t;

typedef union {
    uint32_t GAIN_OFFSET_FILTER_GAIN_12_15;
    struct {
        uint32_t FILT_GAIN_12 : 8;
        uint32_t FILT_GAIN_13 : 8;
        uint32_t FILT_GAIN_14 : 8;
        uint32_t FILT_GAIN_15 : 8;
    } GAIN_OFFSET_FILTER_GAIN_12_15_b;
} GAIN_OFFSET_FILTER_GAIN_12_15_t;

/**
  * @brief RSSI Lookup Tables (RSSICFG_LUTS)
  */

typedef struct { /*!< RSSICFG_LUTS Structure */
    union {
        uint32_t DATA0; /*!< Not Available */
        GAIN_OFFSET_LNM_ATT_t  GAIN_OFFSET_LNM_ATT;
    };
    union {
        uint32_t DATA1; /*!< Not Available */
        GAIN_OFFSET_LNM_GAIN_0_1_2_3_t GAIN_OFFSET_LNM_GAIN_0_1_2_3;
    };
    union {
        uint32_t DATA2; /*!< Not Available */
        GAIN_OFFSET_LNM_GAIN_7_15_MAX_t GAIN_OFFSET_LNM_GAIN_7_15_MAX;
    };
    union {
        uint32_t DATA3; /*!< Not Available */
        GAIN_OFFSET_FILTER_GAIN_0_3_t GAIN_OFFSET_FILTER_GAIN_0_3;
    };
    union {
        uint32_t DATA4; /*!< Not Available */
        GAIN_OFFSET_FILTER_GAIN_4_7_t GAIN_OFFSET_FILTER_GAIN_4_7;
    };
    union {
        uint32_t DATA5; /*!< Not Available */
        GAIN_OFFSET_FILTER_GAIN_8_11_t GAIN_OFFSET_FILTER_GAIN_8_11;
    };
    union {
        uint32_t DATA6; /*!< Not Available */
        GAIN_OFFSET_FILTER_GAIN_12_15_t GAIN_OFFSET_FILTER_GAIN_12_15;
    };
    uint32_t DATA7;     /*!< Not Available */
    uint32_t DATA8;     /*!< Not Available */
    uint32_t DATA9;     /*!< Not Available */
    uint32_t DATA10;    /*!< Not Available */
    uint32_t DATA11;    /*!< Not Available */
    uint32_t DATA12;    /*!< Not Available */
    uint32_t DATA13;    /*!< Not Available */
    uint32_t DATA14;    /*!< Not Available */
} RSSICFG_LUTS_t;

/* ================================================================================ */
/* ================                   PLLBW_LUTS                   ================ */
/* ================================================================================ */

typedef union {
    uint32_t PLL_CP;
    struct {
        uint32_t          :18;
        uint32_t ICP_CODE :5; // PLL Charge Pump Code
        uint32_t          :9;
    }PLL_CP_b;
}PLL_CP_t;

typedef union {
    uint32_t PLL_LF;
    struct {
        uint32_t R1 :7; // PLL filter resistor code 1
        uint32_t R2 :2; // PLL filter resistor code 2
        uint32_t R3 :1; // PLL filter resistor code 3
        uint32_t C2 :6; // PLL filter capacitor code
        uint32_t    :16;
    }PLL_LF_b;
}PLL_LF_t;

/**
  * @brief PLL Bandwidth Lookup Tables (PLLBW_LUTS)
  */

typedef struct { /*!< PLLBW_LUTS Structure */
	PLL_CP_t  DATA0; /*!< Not Available */
	PLL_LF_t  DATA1; /*!< Not Available */
} PLLBW_LUTS_t;

#ifdef __cplusplus
}
#endif
#endif /* _ADF7030_PHY_LUTS_H_ */

/*! @} */
