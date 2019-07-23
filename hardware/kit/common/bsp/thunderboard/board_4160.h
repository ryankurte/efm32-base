/***************************************************************************//**
 * @file
 * @brief BOARD module header file
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

#ifndef BOARD_4160_H
#define BOARD_4160_H

#include <stdint.h>
#include <stdbool.h>

typedef void (*BOARD_IrqCallback)(void);  /**< Interrupt callback function type definition */

/**************************************************************************//**
* @addtogroup TBSense_BSP
* @{
******************************************************************************/

/***************************************************************************//**
 * @addtogroup BOARD_4160
 * @{
 ******************************************************************************/

uint32_t BOARD_picIntGet           (uint8_t *flags);
uint32_t BOARD_picIntClear         (uint8_t flags);
uint32_t BOARD_picWriteReg         (uint8_t addr, uint8_t value);
uint32_t BOARD_picReadReg          (uint8_t addr, uint8_t *result);
uint32_t BOARD_picRegBitsSet       (uint8_t addr, bool set, uint8_t bitMask);
uint32_t BOARD_picGetDeviceId      (void);
uint32_t BOARD_picGetFwRevision    (uint8_t *major, uint8_t *minor, uint8_t *patch);
uint8_t  BOARD_picGetHwRevision    (void);
bool     BOARD_picIsLegacyIntCtrl  (void);

void     BOARD_gasSensorSetIRQCallback   (BOARD_IrqCallback cb);
void     BOARD_alsSetIRQCallback         (BOARD_IrqCallback cb);
void     BOARD_imuSetIRQCallback         (BOARD_IrqCallback cb);
void     BOARD_pushButton0SetIRQCallback (BOARD_IrqCallback cb);
void     BOARD_pushButton0ClearIRQ       (void);
void     BOARD_pushButton1SetIRQCallback (BOARD_IrqCallback cb);
void     BOARD_pushButton1ClearIRQ       (void);

/** @} */
/** @} */

#endif // BOARD_4160_H
