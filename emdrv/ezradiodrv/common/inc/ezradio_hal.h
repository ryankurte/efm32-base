/***************************************************************************//**
 * @file
 * @brief This file contains EZRadio HAL.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc.  Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.  This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef _EZRADIO_HAL_H_
#define _EZRADIO_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ezradiodrv_config.h"

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup EZRADIODRV
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup HAL_Layer
 * @brief EzRadio HAL Layer
 * @{
 ******************************************************************************/

void    ezradio_hal_GpioInit(GPIOINT_IrqCallbackPtr_t ezradioIrqCallback, bool enablePTI);
void    ezradio_hal_SpiInit(void);

void    ezradio_hal_AssertShutdown  (void);
void    ezradio_hal_DeassertShutdown(void);
void    ezradio_hal_ClearNsel       (void);
void    ezradio_hal_SetNsel         (void);
uint8_t ezradio_hal_NirqLevel       (void);

#if defined(EZRADIODRV_DISABLE_PTI) && defined(EZRADIODRV_COMM_USE_GPIO1_FOR_CTS)
uint8_t ezradio_hal_Gpio1Level(void);
#endif

void    ezradio_hal_SpiWriteByte    (uint8_t byteToWrite);
void    ezradio_hal_SpiReadByte     (uint8_t* readByte);

void    ezradio_hal_SpiWriteData    (uint8_t byteCount, uint8_t* pData);
void    ezradio_hal_SpiReadData     (uint8_t byteCount, uint8_t* pData);

void ezradio_hal_SpiWriteReadData(uint8_t byteCount, uint8_t* txData, uint8_t* rxData);

/** @} (end addtogroup HAL_Layer) */
/** @} (end addtogroup EZRADIODRV) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif

#endif //_EZRADIO_HAL_H_
