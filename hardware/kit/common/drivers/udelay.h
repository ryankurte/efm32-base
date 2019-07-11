/***************************************************************************//**
 * @file
 * @brief Microsecond delay routine.
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

#ifndef __UDELAY_H
#define __UDELAY_H

#include <stdint.h>

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Udelay
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void UDELAY_Calibrate(void);
void UDELAY_Delay(uint32_t usecs);

#ifdef __cplusplus
}
#endif

/** @} (end group Udelay) */
/** @} (end group kitdrv) */

#endif
