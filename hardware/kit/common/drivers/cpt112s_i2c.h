/***************************************************************************//**
 * @file
 * @brief Driver for retrieving capsense button and slider inputs from CPT112S
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

#ifndef CPT112S_I2C_H
#define CPT112S_I2C_H

#include <stdint.h>

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup CPT112S_I2C
 * @brief Silicon Labs 12-pad Capacitive Touch Evaluation Board I2C driver.
 *
 * @details
 * <B>I2C data structure</B>
 *
 * For CPT112S there are 12 capsense outputs, each pin
 * corresponds to a capsenseCurrent bit showing whether a
 * cap-sense button pressed or not, and capsensePrevious
 * bit showing the status of previous cycle.
 *
 * If capsenseCurrent==1 && capsensePrevious==0, detects a "press"
 * If capsenseCurrent==0 && capsensePrevious==1, detects a "release"
 *
 * capsenseCurrent bit-map byte:
 * | N/A | N/A | N/A | N/A| cC11 | cC10| cC9 | cC8 |
 * | cC7 | cC6 | cC5 | cC4 | cC3 | cC2 | cC1 | cC0 |
 *
 * capsensePrevious bit-map byte:
 * | N/A | N/A | N/A | N/A| cP11 | cP10| cP9 | cP8 |
 * | cP7 | cP6 | cP5 | cP4 | cP3 | cP2 | cP1 | cP0 |
 *
 * *cC,cP are abbreviation for capsenseCurrent and capsensePrevious
 *
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

#define CPT112S_I2C_ADDRESS         0xE0
#define CPT112S_I2C_RXBUFFER_SIZE   3

#define CPT112S_I2C_TOUCH_EVENT     0
#define CPT112S_I2C_RELEASE_EVENT   1
#define CPT112S_I2C_SLIDER_ACTIVITY 2

/*******************************************************************************
 *******************************  FUNCTIONS  ***********************************
 ******************************************************************************/

uint16_t CPT112S_getCapsenseCurrent(void);
uint16_t CPT112S_getCapsensePrevious(void);
uint16_t CPT112S_getSliderCurrent(void);
uint16_t CPT112S_getSliderPrevious(void);
void CPT112S_init(void);
void CPT112S_update(void);

#ifdef __cplusplus
}
#endif

/** @} (end group CPT112S_I2C) */
/** @} (end group kitdrv) */

#endif /* CPT112S_I2C_H */
