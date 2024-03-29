/**
  * @file bsp_uart.c
  * @brief This file expose public functions of uart devices.
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
  * @par 1.0.0 : 2019/12/20 [BPI]
  * Initial version
  *
  *
  */

/*!
 * @addtogroup uart
 * @ingroup bsp
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_uart.h"
#include "platform.h"
#include <stm32l4xx_hal.h>

extern uart_dev_t aDevUart[UART_ID_MAX];

extern void HAL_UART_MspInit(UART_HandleTypeDef* huart);
extern void HAL_UART_MspDeInit(UART_HandleTypeDef* huart);

/*******************************************************************************/
static void _bsp_com_TxISR_8BIT(UART_HandleTypeDef *huart);
static void _bsp_com_RxISR_8BIT(UART_HandleTypeDef *huart);

/*******************************************************************************/
uint8_t BSP_Console_Init(void)
{
	dev_res_e eRet = DEV_INVALID_PARAM;


	return eRet;
}

uint8_t BSP_Console_Send(uint8_t *pData, uint16_t u16Length)
{
	dev_res_e eRet = DEV_INVALID_PARAM;

	eRet = HAL_UART_Transmit(aDevUart[UART_ID_COM].hHandle, pData, u16Length, aDevUart[UART_ID_COM].u32TxTmo);
	//eRet = HAL_UART_Transmit(aDevUart[UART_ID_COM].hHandle, pData, u16Length, CONSOLE_TX_TIMEOUT);
	//eRet = HAL_UART_Transmit_DMA(paUART_BusHandle[UART_ID_CONSOLE], pData, u16Length);
	//eRet = HAL_UART_Transmit_IT(paUART_BusHandle[UART_ID_CONSOLE], pData, u16Length);
	return eRet;
}

uint8_t BSP_Console_Received(uint8_t *pData, uint16_t u16Length)
{
	dev_res_e eRet = DEV_INVALID_PARAM;

	eRet = HAL_UART_Receive(aDevUart[UART_ID_COM].hHandle, pData, u16Length, aDevUart[UART_ID_COM].u32RxTmo);
	//eRet = HAL_UART_Receive(aDevUart[UART_ID_COM].hHandle, pData, u16Length, CONSOLE_RX_TIMEOUT);

	return eRet;
}

inline uint8_t BSP_Console_SetRXTmo(uint32_t u32Tmo)
{
	aDevUart[UART_ID_COM].u32RxTmo = u32Tmo;
	return DEV_SUCCESS;
}

inline uint8_t BSP_Console_SetTXTmo(uint32_t u32Tmo)
{
	aDevUart[UART_ID_COM].u32TxTmo = u32Tmo;
	return DEV_SUCCESS;
}

inline void BSP_Console_FluxRx(void)
{
	uint8_t tmp;
	while( HAL_UART_Receive(aDevUart[UART_ID_COM].hHandle, &tmp, 1, 0) == 0);
}
/******************************************************************************/
/*!
  * @brief Enable the given uart and its related GPIO
  *
  * @param [in] u8DevId     Uart device id (see @link uart_id_e @endlink)
  *
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  * @retval DEV_INVALID_PARAM if the given parameter is invalid (see @link dev_res_e::DEV_INVALID_PARAM @endlink)
  *
  */
uint8_t BSP_Uart_Open(uint8_t u8DevId)
{
	UART_HandleTypeDef *huart = aDevUart[u8DevId].hHandle;
	if (u8DevId >= UART_ID_MAX)
	{
		return DEV_INVALID_PARAM;
	}
	__HAL_LOCK(huart);
	HAL_UART_MspInit(huart);
	huart->RxState = HAL_UART_STATE_READY;
	huart->gState = HAL_UART_STATE_READY;
	__HAL_UNLOCK(huart);
	__HAL_UART_ENABLE(huart);
	return DEV_SUCCESS;
}

/*!
  * @brief Disable the given uart and its related GPIO
  *
  * @param [in] u8DevId     Uart device id (see @link uart_id_e @endlink)
  *
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  * @retval DEV_INVALID_PARAM if the given parameter is invalid (see @link dev_res_e::DEV_INVALID_PARAM @endlink)
  *
  */
uint8_t BSP_Uart_Close(uint8_t u8DevId)
{
	UART_HandleTypeDef *huart = aDevUart[u8DevId].hHandle;
	if (u8DevId >= UART_ID_MAX)
	{
		return DEV_INVALID_PARAM;
	}
	HAL_NVIC_DisableIRQ(aDevUart[u8DevId].i8ItLine);
	__HAL_UART_DISABLE(huart);
	HAL_UART_MspDeInit(huart);
	return DEV_SUCCESS;
}

uint8_t BSP_Uart_SetDefault(uint8_t u8DevId)
{
	if (u8DevId >= UART_ID_MAX)
	{
		return DEV_INVALID_PARAM;
	}
	UART_HandleTypeDef *huart = aDevUart[u8DevId].hHandle;
	huart->Init.BaudRate = 115200;
	huart->Init.WordLength = UART_WORDLENGTH_8B;
	huart->Init.StopBits = UART_STOPBITS_1;
	huart->Init.Parity = UART_PARITY_NONE;
	huart->Init.Mode = UART_MODE_TX_RX;
	huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart->Init.OverSampling = UART_OVERSAMPLING_16;
	huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
	huart->AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;

	huart->AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;

	return DEV_SUCCESS;
}

/*!
  * @brief Configure the given uart (interrupt mode)
  *
  * @param [in] u8DevId     Uart device id (see @link uart_id_e @endlink)
  * @param [in] u8CharMatch Character to match
  * @param [in] u8Mode      Uart detection mode (see @link uart_mode_e @endlink)
  * @param [in] u32Tmo      Timeout (0 : timeout is not used)
  * 
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  * @retval DEV_INVALID_PARAM if the given parameter is invalid (see @link dev_res_e::DEV_INVALID_PARAM @endlink)
  * 
  */
uint8_t BSP_Uart_Init(uint8_t u8DevId, uint8_t u8CharMatch, uint8_t u8Mode)
{
	if (u8DevId >= UART_ID_MAX)
	{
		return DEV_INVALID_PARAM;
	}
	uint8_t u8_Status;
	UART_HandleTypeDef *huart = aDevUart[u8DevId].hHandle;
	/*
#ifdef COM_SWAP_PINS
	SET_BIT(huart->Instance->CR2, USART_CR2_SWAP);
#endif
	SET_BIT(huart->Instance->CR3, USART_CR3_OVRDIS);
*/
	BSP_Uart_SetDefault(u8DevId);
	u8_Status = HAL_UART_Init(huart);
	//----------------





	//----------------
	if ( u8_Status != HAL_OK)
	{
    	DBG_BSP("UART 0x%8X Init: status %d\r\n", huart->Instance, u8_Status);
		return DEV_FAILURE;
	}
	__HAL_UART_DISABLE(huart);
	// Disable all interrupt
	CLEAR_BIT(huart->Instance->CR1, (
			USART_CR1_RTOIE |
			USART_CR1_CMIE | USART_CR1_MME | USART_CR1_PCE |
			USART_CR1_PEIE | USART_CR1_TXEIE | USART_CR1_TCIE |
			USART_CR1_RXNEIE | USART_CR1_IDLEIE) );

	aDevUart[u8DevId].u8CharMatch = u8CharMatch;
	aDevUart[u8DevId].u8Mode = u8Mode;

	if (u8Mode == UART_MODE_EOB)
	{
		/* Set Address value*/
		MODIFY_REG(huart->Instance->CR2, USART_CR2_ADD, (u8CharMatch << USART_CR2_ADD_Pos) );
		/* Set 7 bits Address */
		SET_BIT(huart->Instance->CR2, USART_CR2_ADDM7);//UART_ADDRESS_DETECT_7B
	}
	/* Clear all flag : already done with __HAL_UART_DISABLE */
	// WRITE_REG(huart->Instance->ICR, 0xFFFFFFFF );

	huart->RxISR = _bsp_com_RxISR_8BIT;
	huart->TxISR = _bsp_com_TxISR_8BIT;

	 __HAL_UART_ENABLE(huart);
	return DEV_SUCCESS;
}

/*!
  * @brief Set the Uart interrupt callback
  *
  * @param [in] u8DevId  Uart device id (see @link uart_id_e @endlink)
  * @param [in] pfEvtCb  Pointer on the callback function
  * @param [in] pCbParam Pointer on the callback parameter
  *
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  * @retval DEV_INVALID_PARAM if the given parameter is invalid (see @link dev_res_e::DEV_INVALID_PARAM @endlink)
  *
  */
uint8_t BSP_Uart_SetCallback (uint8_t u8DevId, pfEvtCb_t const pfEvtCb, void *pCbParam)
{
	if (u8DevId >= UART_ID_MAX)
	{
		return DEV_INVALID_PARAM;
	}
	aDevUart[u8DevId].pfEvent = pfEvtCb;
	aDevUart[u8DevId].pCbParam = pCbParam;

	return DEV_SUCCESS;
}

/*!
  * @brief Start to transmit on the given uart (interrupt mode)
  *
  * @param [in] u8DevId   Uart device id (see @link uart_id_e @endlink)
  * @param [in] pData     Pointer on the buffer to send
  * @param [in] u16Length Size of the message
  * 
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  * @retval DEV_INVALID_PARAM if the given parameter is invalid (see @link dev_res_e::DEV_INVALID_PARAM @endlink)
  * @retval DEV_BUSY if the given device is busy (see @link dev_res_e::DEV_BUSY @endlink)
  * 
  */
uint8_t BSP_Uart_Transmit(uint8_t u8DevId, uint8_t *pData, uint16_t u16Length)
{
	if (u8DevId >= UART_ID_MAX)
	{
		return DEV_INVALID_PARAM;
	}
	UART_HandleTypeDef *huart = aDevUart[u8DevId].hHandle;

	/* Check that a Tx process is not already ongoing */
	if (huart->gState == HAL_UART_STATE_READY)
	{
		if ((pData == NULL) || (u16Length == 0U))
		{
			return DEV_INVALID_PARAM;
		}

		__HAL_LOCK(huart);
		huart->gState = HAL_UART_STATE_BUSY_TX;
		huart->ErrorCode = HAL_UART_ERROR_NONE;
		huart->pTxBuffPtr  = pData;
		huart->TxXferSize  = u16Length;
		huart->TxXferCount = u16Length;
		huart->TxISR = _bsp_com_TxISR_8BIT;
		__HAL_UNLOCK(huart);

		/* Enable the Transmit Data Register Empty interrupt */
		SET_BIT(huart->Instance->CR1, USART_CR1_TXEIE);

		return DEV_SUCCESS;
	}
	else
	{
		return DEV_BUSY;
	}
}

/*!
  * @brief Start to receive on the given uart (interrupt mode)
  *
  * @param [in] u8DevId   Uart device id (see @link uart_id_e @endlink)
  * @param [in] pData     Pointer on receiving buffer
  * @param [in] u16Length Size of expected message
  * 
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  * @retval DEV_INVALID_PARAM if the given parameter is invalid (see @link dev_res_e::DEV_INVALID_PARAM @endlink)
  * @retval DEV_BUSY if the given device is busy (see @link dev_res_e::DEV_BUSY @endlink)
  * 
  */
uint8_t BSP_Uart_Receive(uint8_t u8DevId, uint8_t *pData, uint16_t u16Length)
{
	if (u8DevId >= UART_ID_MAX)
	{
		return DEV_INVALID_PARAM;
	}
	UART_HandleTypeDef *huart = aDevUart[u8DevId].hHandle;

	register uint32_t itflags = READ_REG(huart->Instance->CR1);
	/* Check that a Rx process is not already ongoing */
	if (huart->RxState == HAL_UART_STATE_READY)
	{
		if ((pData == NULL) || (u16Length == 0U))
		{
			return DEV_INVALID_PARAM;
		}

		__HAL_LOCK(huart);
		huart->RxState = HAL_UART_STATE_BUSY_RX;
		huart->ErrorCode = HAL_UART_ERROR_NONE;
		huart->pRxBuffPtr  = pData;
		huart->RxXferSize  = u16Length;
		huart->RxXferCount = u16Length;
		//huart->RxISR       = NULL;
		huart->RxISR = _bsp_com_RxISR_8BIT;

		/* Computation of UART mask to apply to RDR register */
		UART_MASK_COMPUTATION(huart);

		/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
		SET_BIT(huart->Instance->CR3, USART_CR3_EIE);

		if(aDevUart[u8DevId].u8Mode != UART_MODE_EOB)
		{
			/* Disable Character Match interrupt*/
			itflags &= ~(USART_CR1_CMIE);
		}
		else
		{
			/* Enable Character Match interrupt*/
			itflags |= USART_CR1_CMIE;
		}

		/* Enable the UART Parity Error interrupt and Data Register Not Empty interrupt */
		itflags |= USART_CR1_PEIE | USART_CR1_RXNEIE;

		//__HAL_UNLOCK(huart);
		WRITE_REG(huart->Instance->CR1, itflags);
		__HAL_UNLOCK(huart);

		//__HAL_UART_SEND_REQ(huart, UART_MUTE_MODE_REQUEST);

		HAL_NVIC_EnableIRQ(aDevUart[u8DevId].i8ItLine);
		return DEV_SUCCESS;
	}
	else
	{
		return DEV_BUSY;
	}
}

/*!
  * @brief Abort the UART receive (interrupt mode)
  *
  * @param [in] u8DevId Uart device id (see @link uart_id_e @endlink)
  * 
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  * @retval DEV_INVALID_PARAM if the given parameter is invalid (see @link dev_res_e::DEV_INVALID_PARAM @endlink)
  * 
  */
uint8_t BSP_Uart_AbortReceive(uint8_t u8DevId)
{
	if (u8DevId >= UART_ID_MAX)
	{
		return DEV_INVALID_PARAM;
	}
	UART_HandleTypeDef *huart = aDevUart[u8DevId].hHandle;

	register uint32_t itflags = READ_REG(huart->Instance->CR1);
	if(itflags & USART_CR1_RXNEIE)
	{
		CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
		CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);

		/* Reset Rx transfer counter */
	    huart->RxXferCount = 0U;

	    /* Clear RxISR function pointer */
	    huart->pRxBuffPtr = NULL;

	    /* Clear the Error flags in the ICR register */
	    __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_PEF | UART_CLEAR_FEF);

		/* Discard the received data */
		__HAL_UART_SEND_REQ(huart, UART_RXDATA_FLUSH_REQUEST);

		/* Restore huart->RxState to Ready */
	    huart->RxState = HAL_UART_STATE_READY;
	}
	HAL_NVIC_DisableIRQ(aDevUart[u8DevId].i8ItLine);
	return DEV_SUCCESS;
}

/*!
  * @brief Get the number of byte received
  *
  * @param [in] u8DevId  Uart device id (see @link uart_id_e @endlink)
  *
  * @retval the number of bytes received
  * @retval 0 if the given u8DevId is invalid
  *
  */
uint16_t BSP_Uart_GetNbReceive(uint8_t u8DevId)
{
	if (u8DevId >= UART_ID_MAX)
	{
		return 0;
	}
	UART_HandleTypeDef *huart = aDevUart[u8DevId].hHandle;
	return (huart->RxXferSize - huart->RxXferCount);
}

/*!
  * @brief Get the number of byte transmitted
  *
  * @param [in] u8DevId  Uart device id (see @link uart_id_e @endlink)
  *
  * @retval the number of bytes transmitted
  * @retval 0 if the given u8DevId is invalid
  *
  */
uint16_t BSP_Uart_GetNbTransmit(uint8_t u8DevId)
{
	if (u8DevId >= UART_ID_MAX)
	{
		return 0;
	}
	UART_HandleTypeDef *huart = aDevUart[u8DevId].hHandle;
	return (huart->TxXferSize - huart->TxXferCount);
}

/*******************************************************************************/
/*!
  * @static
  * @brief TX interrupt handler
  *
  * @param [in] huart Pointer on the uart handle
  *
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  * @retval DEV_INVALID_PARAM if the given paramater is in valid (see @link dev_res_e::DEV_INVALID_PARAM @endlink)
  *
  */
static void _bsp_com_TxISR_8BIT(UART_HandleTypeDef *huart)
{
	uint16_t tmp;

	// Check that a Tx process is ongoing
	if (huart->gState == HAL_UART_STATE_BUSY_TX)
	{
		if (huart->TxXferCount == 0U)
		{
			// Disable the UART Transmit Data Register Empty Interrupt
			CLEAR_BIT(huart->Instance->CR1, USART_CR1_TXEIE);

			// Enable the UART Transmit Complete Interrupt
			SET_BIT(huart->Instance->CR1, USART_CR1_TCIE);
		}
		else
		{
			tmp = (uint16_t)(*huart->pTxBuffPtr & 0xFF);
			if ( (huart->Instance->CR1 & UART_WORDLENGTH_9B ) && (huart->TxXferCount == huart->TxXferSize) )
			{
				// Set the 9th bit as address marker
				tmp |= 0x100;
			}
			huart->Instance->TDR = tmp;
			huart->pTxBuffPtr++;
			huart->TxXferCount--;
		}
	}
	else
	{
		// TX flush request
		__HAL_UART_SEND_REQ(huart, UART_TXDATA_FLUSH_REQUEST);
	}
}

/*!
  * @static
  * @brief RX interrupt handler
  *
  * @param [in] huart Pointer on the uart handle
  *
  * @retval DEV_SUCCESS if everything is fine (see @link dev_res_e::DEV_SUCCESS @endlink)
  * @retval DEV_INVALID_PARAM if the given paramater is in valid (see @link dev_res_e::DEV_INVALID_PARAM @endlink)
  *
  */
static void _bsp_com_RxISR_8BIT(UART_HandleTypeDef *huart)
{
	uint8_t evt = UART_EVT_NONE;

	/* Check that a Rx process is ongoing */
	if (huart->RxState == HAL_UART_STATE_BUSY_RX)
	{
		uint32_t isrflags = READ_REG(huart->Instance->ISR);
		uint32_t cr1its   = READ_REG(huart->Instance->CR1);
		uint8_t  uhdata   = READ_REG(huart->Instance->RDR);
		// Character match detected ?
		if ( (isrflags & USART_ISR_CMF) )
		{
			if ( (cr1its & USART_CR1_CMIE)  )
			{
				/* Disable Character Match interrupt */
				CLEAR_BIT(huart->Instance->CR1, USART_CR1_CMIE);
				// EOB
				evt = UART_EVT_RX_CPLT;
			}
			/* Clear Character Match Flag */
			WRITE_REG(huart->Instance->ICR, USART_ICR_CMCF);
		}

		*huart->pRxBuffPtr = uhdata;
		huart->pRxBuffPtr++;
		huart->RxXferCount--;

		// Check buffer overflow
		if ( (huart->RxXferSize - huart->RxXferCount) == 0)
		{
			// Cancel : buffer will overflow
			evt = UART_EVT_RX_ABT;
		}

		if ( (huart->RxXferCount == 0U) && (evt != UART_EVT_RX_ABT) )
		{
			evt = UART_EVT_RX_CPLT;
		}

		if ( evt == UART_EVT_RX_CPLT )
		{
			/* Disable the UART Parity Error Interrupt and RXNE interrupts */
			CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR1_CMIE));
			/* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
			CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);
			/* Rx process is completed, restore huart->RxState to Ready */
			huart->RxState = HAL_UART_STATE_READY;
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
			/*Call registered Rx complete callback*/
			huart->RxCpltCallback(huart);
#else
			/*Call legacy weak Rx complete callback*/
			HAL_UART_RxCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
		}
		else if ( evt == UART_EVT_RX_HCPLT )
		{
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
			/*Call registered Rx complete callback*/
			huart->RxHalfCpltCallback(huart);
#else
			/*Call legacy weak Rx half complete callback*/
			HAL_UART_RxHalfCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
		}
	}
	else
	{
		/* RX flush request */
		__HAL_UART_SEND_REQ(huart, UART_RXDATA_FLUSH_REQUEST);
	}

	if (evt == UART_EVT_RX_ABT)
	{
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
		/* Call registered Abort Receive Complete Callback */
		huart->AbortReceiveCpltCallback(huart);
#else
		/* Call legacy weak Abort Receive Complete Callback */
		HAL_UART_AbortReceiveCpltCallback(huart);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
		huart->RxState = HAL_UART_STATE_READY;
	}
}

/*******************************************************************************/

#ifdef __cplusplus
}
#endif

/*! @} */
