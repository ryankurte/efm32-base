/***************************************************************************//**
 * @file
 * @brief Utility Functions for the Thunderboard Sense
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

#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdbool.h>

/**************************************************************************//**
* @addtogroup TBSense_BSP
* @{
******************************************************************************/

/***************************************************************************//**
 * @addtogroup Util
 * @{
 ******************************************************************************/

#define UTIL_SUPPLY_TYPE_UNKNOWN    0 /**< Unknown power supply type                */
#define UTIL_SUPPLY_TYPE_USB        1 /**< The board powered from the USB connector */
#define UTIL_SUPPLY_TYPE_AA         2 /**< The board powered from AA batteries      */
#define UTIL_SUPPLY_TYPE_AAA        3 /**< The board powered from AAA batteries     */
#define UTIL_SUPPLY_TYPE_CR2032     4 /**< The board powered from a CR2032 battery  */

uint32_t UTIL_init        (void);
void     UTIL_delay       (uint32_t ms);
void     UTIL_shutdown    (void);

uint32_t UTIL_sleepInit   (void);
void     UTIL_sleep       (uint32_t ms);
uint32_t UTIL_waitForEvent(uint32_t timeout);

void     UTIL_supplyProbe                (void);
void     UTIL_supplyGetCharacteristics   (uint8_t *type, float *voltage, float *ir);
uint8_t  UTIL_supplyGetType              (void);
bool     UTIL_isLowPower                 (void);

/** @} */
/** @} */

#endif // UTIL_H
