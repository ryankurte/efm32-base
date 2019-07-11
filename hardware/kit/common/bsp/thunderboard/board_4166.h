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

#ifndef BOARD_4166_H
#define BOARD_4166_H

#include <stdint.h>

typedef void (*BOARD_IrqCallback)(void);/**< Interrupt callback function type definition */

/**************************************************************************//**
* @addtogroup TBSense_BSP
* @{
******************************************************************************/

/***************************************************************************//**
 * @addtogroup BOARD_4166
 * @{
 ******************************************************************************/

void     BOARD_gasSensorSetIRQCallback(BOARD_IrqCallback cb);

uint32_t BOARD_hallSensorEnable    (bool enable);
uint32_t BOARD_hallSensorEnableIRQ (bool enable);
void     BOARD_hallSensorSetIRQCallback(BOARD_IrqCallback cb);
void     BOARD_hallSensorClearIRQ(void);

void     BOARD_alsSetIRQCallback(BOARD_IrqCallback cb);

void     BOARD_imuSetIRQCallback(BOARD_IrqCallback cb);

void     BOARD_pushButton0SetIRQCallback(BOARD_IrqCallback cb);
void     BOARD_pushButton0ClearIRQ(void);
void     BOARD_pushButton1SetIRQCallback(BOARD_IrqCallback cb);
void     BOARD_pushButton1ClearIRQ(void);

/** @} */
/** @} */

#endif // BOARD_4166_H
