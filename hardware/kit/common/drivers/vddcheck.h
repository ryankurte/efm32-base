/***************************************************************************//**
 * @file
 * @brief VDD Voltage Check using EMLIB Voltage Comparator API
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

#ifndef __VDDCHECK_H
#define __VDDCHECK_H

#include <stdint.h>
#include <stdbool.h>

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup VddCheck
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void VDDCHECK_Init(void);
void VDDCHECK_Disable(void);
bool VDDCHECK_LowVoltage(float vdd);

#ifdef __cplusplus
}
#endif

/** @} (end group VddCheck) */
/** @} (end group kitdrv) */

#endif
