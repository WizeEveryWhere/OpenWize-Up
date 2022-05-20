/**
  * @file adf7030-1_phy_conv.c
  * @brief This file expose some convenient function to convert RSSI, AFC error...
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
  * @par 1.0.0 : 2020/04/29[GBI]
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
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "adf7030-1_phy_conv.h"

/*!
 * @brief  This function convert "IHM" (human readable) RSSI to WIZE compliant RSSI.
 *
 * @param [in]  u8_IntPart Integer part of RSSI value.
 * @param [in]  u8_DecPart Decimal part of RSSI value.
 * @param [out] u8_Rssi    Reference variable to hold the WIZE RSSI compliant.
 *
 * @return None
 */
inline void PHY_CONV_IhmToRssi( uint8_t u8_IntPart,
                           uint8_t u8_DecPart,
                           uint8_t *u8_Rssi )
{
	uint8_t u8_Tmp;
	u8_Tmp = ( u8_IntPart << 1 ) + ( u8_DecPart & 0x1 );
	*u8_Rssi = 255 - u8_Tmp + 40;
}

/*!
 * @brief  This function convert a WIZE compliant RSSI to "IHM" (human readable) RSSI.
 *
 * @param [in]  u8_Rssi     The WIZE RSSI compliant value.
 * @param [out] i16_IntPart Reference variable to hold the Integer part of IHM RSSI.
 * @param [out] u8_DecPart  Reference variable to hold the Decimal part of IHM RSSI.
 *
 * @return None
 */
inline
void PHY_CONV_RssiToIhm( uint8_t u8_Rssi,
                           int16_t *i16_IntPart,
                           uint8_t *u8_DecPart )
{
	uint8_t u8_Tmp;
	u8_Tmp =  255 - u8_Rssi + 40;
	*i16_IntPart = (int16_t)( u8_Tmp >> 1 );
	*u8_DecPart = ( u8_Tmp & 0x1 ) * 50;
}

/*!
 * @brief  This function convert a ADF7030 (signed 11 bits) RSSI to "IHM" (human readable) RSSI.
 *
 * @param [in]  u16_Signed11 The ADF7030 (signed 11 bits) RSSI value.
 * @param [out] i16_IntPart   Reference variable to hold the Integer part of IHM RSSI.
 * @param [out] u8_DecPart   Reference variable to hold the Decimal part of IHM RSSI.
 *
 * @return None
 */
inline
void PHY_CONV_Signed11ToIhm( uint16_t u16_Signed11,
                               int16_t *i16_IntPart,
                               uint8_t *u8_DecPart )
{
	int16_t i16_Tmp;
	i16_Tmp = (int16_t)(u16_Signed11 << 5);

	*i16_IntPart = i16_Tmp/128;
	*u8_DecPart = ( ( ( u16_Signed11^0x7FF ) +1 )& 0x3) * 25;
}


/*!
 * @brief  This function convert a ADF7030 "IHM" (human readable) to signed 11 bits RSSI.
 *
 * @param [in]  i16_IntPart   Reference variable to hold the Integer part of IHM RSSI.
 * @param [in]  u8_DecPart   Reference variable to hold the Decimal part of IHM RSSI.
 * @param [out] u16_Signed11 The ADF7030 (signed 11 bits) RSSI value.
 *
 * @return None
 */
inline
void PHY_CONV_IhmToSigned11( int16_t i16_IntPart,
                             uint8_t u8_DecPart,
							 uint16_t *u16_Signed11)
{
	int16_t i16_Tmp;
	i16_Tmp = i16_IntPart >> 5;
	i16_Tmp += (u8_DecPart / 25) & 0x3;
	*u16_Signed11 = (i16_Tmp -1)^0x7FF;
}

/*!
 * @brief  This function convert a ADF7030 (signed 11 bits) RSSI to WIZE compliant RSSI.
 *
 * @param [in]  u16_Signed11 The ADF7030 (signed 11 bits) RSSI value.
 *
 * @return The WIZE compliant RSSI value
 */
inline uint8_t PHY_CONV_Signed11ToRssi(uint16_t u16_Signed11)
{
	uint8_t u8_Rssi;
	u8_Rssi = 255 - ( ( ( u16_Signed11^0x7FF ) + 1 ) >> 1 ) + 40;
	return u8_Rssi;
}

/*!
 * @brief  This function convert a ADF7030 (signed 11 bits) RSSI to float.
 *
 * @param [in] u16_Signed11 The ADF7030 (signed 11 bits) RSSI value.
 *
 * @return The float representation of RSSI
 */
inline float PHY_CONV_Signed11ToFloat(uint16_t u16_Signed11)
{
    float Rssi;

	int16_t i16_Tmp = ( (int16_t)(u16_Signed11 << 5) ) /128;
	uint8_t u8DecPart = ( ( ( u16_Signed11^0x7FF ) +1 )& 0x3) * 25;

	Rssi = (float)(i16_Tmp) ;
	if (i16_Tmp < 0)
	{
		Rssi -= (float)(u8DecPart)/100 ;
	}
	else
	{
		Rssi += (float)(u8DecPart)/100 ;
	}
	return Rssi;
}

/*!
 * @brief  This function convert a ADF7030 AFC frequency error to float.
 *
 * @param [in]  i16AfcFreqErr The ADF7030 (signed 11 bits) AFC frequency error value.
 *
 * @return The float representation of AFC frequency error
 */
inline float PHY_CONV_AfcFreqErrToFloat(int16_t i16AfcFreqErr)
{
    float FreqErr;
	FreqErr = ((float)i16AfcFreqErr)* 26000000.00 / 4194304.00;
	return FreqErr;
}

#ifdef __cplusplus
}
#endif

/*! @} */
