/**
  * @file adf7030-1_phy_conv.h
  * @brief This file define some convenient function to convert RSSI, AFC error...
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
  * @par 1.0.0 : 2020/04/29 [GBI]
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
#ifndef _ADF7030_PHY_CONV_H_
#define _ADF7030_PHY_CONV_H_
#ifdef __cplusplus
extern "C" {
#endif

extern inline
void PHY_CONV_IhmToRssi( uint8_t u8_IntPart,
                           uint8_t u8_DecPart,
                           uint8_t *u8_Rssi );

extern inline
void PHY_CONV_RssiToIhm( uint8_t u8_Rssi,
                           int16_t *i16_IntPart,
                           uint8_t *u8_DecPart );

extern inline
void PHY_CONV_Signed11ToIhm( uint16_t u16_Signed11,
                               int16_t *i16_IntPart,
                               uint8_t *u8_DecPart ) ;

extern inline
uint8_t PHY_CONV_Signed11ToRssi(uint16_t u16_Signed11);

extern inline
float PHY_CONV_Signed11ToFloat(uint16_t u16_Signed11);

extern inline
float PHY_CONV_AfcFreqErrToFloat(int16_t i16AfcFreqErr);

#ifdef __cplusplus
}
#endif
#endif /* _ADF7030_PHY_CONV_H_ */

/*! @} */
