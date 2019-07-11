/***************************************************************************//**
 * @file
 * @brief Retarget stdout to a serial port.
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

#ifndef __RETARGETSERIAL_H
#define __RETARGETSERIAL_H

#if defined(HAL_CONFIG)
#include "retargetserialhalconfig.h"
#else
#include "retargetserialconfig.h"
#endif
#include <stdbool.h>

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup RetargetIo
 * @brief Standard I/O retargeting driver
 * @details
 *    This module provide low-level retargeting of stdio for all
 *    supported toolchains.
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__CROSSWORKS_ARM)
int __putchar(int ch);
int __getchar(void);
#endif

int  RETARGET_ReadChar(void);
int  RETARGET_WriteChar(char c);

void RETARGET_SerialCrLf(int on);
void RETARGET_SerialInit(void);
bool RETARGET_SerialEnableFlowControl(void);
void RETARGET_SerialFlush(void);

#ifdef __cplusplus
}
#endif

/** @} (end group RetargetIo) */
/** @} (end group kitdrv) */

#endif
