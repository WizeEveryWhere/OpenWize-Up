/*!
  * @file: bsp_spi.h
  * @brief: This file defines functions to deal with SPI.
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
  * 1.0.0 : 2019/12/14[GBI]
  * Initial version
  *
  *
  */
#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "bsp_gpio.h"

typedef struct
{
    uint8_t*    pTransmitter;/*!< Pointer to transmit data.        */
    uint8_t*    pReceiver; /*!< Pointer to receive data.         */
    uint32_t    TransmitterBytes; /*!< Data size  for TX(bytes).          */
    uint32_t    ReceiverBytes; /*!< Data size  for RX(bytes).          */
    uint32_t    nTxIncrement;/*!<  Transmit pointer increment flag. */
    uint32_t    nRxIncrement; /*!< Receive pointer increment flag.  */
} spi_transceiver_s;


typedef struct
{
    uint8_t bus_id;
    uint32_t ss_port;
    uint16_t ss_pin;
} spi_dev_t;

typedef spi_dev_t* p_spi_dev_t;

uint8_t BSP_Spi_Init(const p_spi_dev_t p_Device);
uint8_t BSP_Spi_SetDefault(const p_spi_dev_t p_Device);

uint8_t BSP_Spi_Open(const p_spi_dev_t p_Device);
uint8_t BSP_Spi_Close (const p_spi_dev_t p_Device);

uint8_t BSP_Spi_SetBitrate (const p_spi_dev_t p_Device, const uint32_t u32_Hertz);
uint8_t BSP_Spi_SetClockPhase (const p_spi_dev_t p_Device, const bool b_Flag);
uint8_t BSP_Spi_SetClockPol (const p_spi_dev_t p_Device, const bool b_Flag);
uint8_t BSP_Spi_ReadWrite (const p_spi_dev_t p_Device, spi_transceiver_s* const p_Xfr);

#ifdef __cplusplus
}
#endif
#endif /* _BSP_SPI_H_ */
