/***************************************************************************//**
 * @file
 * @brief Capacitive sense driver
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

#ifndef __CAPSENSE_H_
#define __CAPSENSE_H_

#include <stdint.h>
#include <stdbool.h>
#include "capsenseconfig.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup CapSense
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

uint32_t CAPSENSE_getVal(uint8_t channel);
uint32_t CAPSENSE_getNormalizedVal(uint8_t channel);
bool CAPSENSE_getPressed(uint8_t channel);
int32_t CAPSENSE_getSliderPosition(void);
void CAPSENSE_Sense(void);
void CAPSENSE_Init(void);

#ifdef __cplusplus
}
#endif

/** @} (end group CapSense) */
/** @} (end group kitdrv) */

#endif /* __CAPSENSE_H_ */
