/***************************************************************************//**
 * @file
 * @brief SPI controller interface for SSD2119 display
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef __SPI_TFT_H
#define __SPI_TFT_H

#include <stdint.h>

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Tft
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void SPI_TFT_Init(void);
void SPI_TFT_WriteRegister(uint8_t reg, uint16_t data);

#ifdef __cplusplus
}
#endif

/** @} (end group Tft) */
/** @} (end group kitdrv) */

#endif
