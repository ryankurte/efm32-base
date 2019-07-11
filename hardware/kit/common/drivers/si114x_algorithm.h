/***************************************************************************//**
 * @file
 * @brief Swipe algorithm for Si114x
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

#ifndef __SI114X_ALGORITHM_H
#define __SI114X_ALGORITHM_H

#include "em_device.h"
#include "si114x_functions.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Si114x
 * @{
 ******************************************************************************/

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/
/** I2C device address for Si1147 on weather station board. */
#define SI1147_ADDR    0xc0

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/** Interrupt Sample */
typedef struct {
  u32 timestamp;         /* Timestamp to record */
  u16 vis;               /* VIS */
  u16 ir;                /* IR */
  u16 ps1;               /* PS1 */
  u16 ps2;               /* PS2 */
  u16 ps3;               /* PS3 */
  u16 aux;               /* AUX */
} Si114x_Sample_TypeDef;

/** @endcond */

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/
/** Si114x gestures */
typedef enum {
  NONE,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  TAP
} gesture_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/
gesture_t Si1147_NewSample(I2C_TypeDef *i2c, uint8_t addr, uint32_t timestamp);
int Si1147_ConfigureDetection(I2C_TypeDef *i2c, uint8_t addr, int slow);
int Si1147_SetInterruptOutputEnable(I2C_TypeDef *i2c, uint8_t addr, int enable);
int Si1147_GetInterruptOutputEnable(I2C_TypeDef *i2c, uint8_t addr, int *enable);
int Si1147_Detect_Device(I2C_TypeDef *i2c, uint8_t addr);
int Si1147_MeasureUVAndObjectPresent(I2C_TypeDef *i2c, uint8_t addr, uint16_t *uvIndex, int* objectDetect);

/** @} (end group Si114x) */
/** @} (end group kitdrv) */

#ifdef __cplusplus
}
#endif

#endif /* #define SI114X_ALGORITHM_H */
