/***************************************************************************//**
 * @file
 * @brief SPI interface API for KSZ8851SNL Ethernet controller
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

#ifndef KSZ8851SNL_SPI_H__
#define KSZ8851SNL_SPI_H__
#include <stdint.h>

/**************************************************************************//**
* @addtogroup kitdrv
* @{
******************************************************************************/

/**************************************************************************//**
* @addtogroup ksz8851snl
* @{
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void KSZ8851SNL_SPI_Init(void);
uint16_t KSZ8851SNL_SPI_ReadRegister(uint8_t reg);
void KSZ8851SNL_SPI_WriteRegister(uint8_t reg, uint16_t value);
void KSZ8851SNL_SPI_ReadFifo(int numBytes, uint8_t *data);
void KSZ8851SNL_SPI_WriteFifoBegin(void);
void KSZ8851SNL_SPI_WriteFifo(int numBytes, const uint8_t *data);
void KSZ8851SNL_SPI_WriteFifoEnd(void);

#ifdef __cplusplus
}
#endif

/** @} (end group EthSpi) */
/** @} (end group kitdrv) */

#endif
