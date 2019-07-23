/***************************************************************************//**
 * @file
 * @brief Driver for DS75 temperature sensor
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

#ifndef __TEMPSENS_H
#define __TEMPSENS_H

#include "em_device.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup TempSensor
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

/** I2C device address for temperature sensor on EFM32GG/LG/WG DK */
#define TEMPSENS_DK_ADDR    0x90

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/** Available registers in DS75 sensor device */
typedef enum {
  tempsensRegTemp       = 0,   /**< Temperature register (read-only) */
  tempsensRegConfig     = 1,   /**< Configuration register */
  tempsensRegHysteresis = 2,   /**< Hysteresis register */
  tempsensRegShutdown   = 3    /**< Overtemperature shutdown register */
} TEMPSENS_Register_TypeDef;

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/** Structure used to fetch temperature using integer numbers. */
typedef struct {
  /** Integer part of temperature, including sign */
  int16_t i;

  /**
   * Fractional part of temperature in 1/10000 parts, including sign. Ie 0.5
   * will be stored as 5000 and -0.5 as -5000.
   */
  int16_t f;
} TEMPSENS_Temp_TypeDef;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

void TEMPSENS_Celsius2Fahrenheit(TEMPSENS_Temp_TypeDef *temp);
int TEMPSENS_RegisterGet(I2C_TypeDef *i2c,
                         uint8_t addr,
                         TEMPSENS_Register_TypeDef reg,
                         uint16_t *val);
int TEMPSENS_RegisterSet(I2C_TypeDef *i2c,
                         uint8_t addr,
                         TEMPSENS_Register_TypeDef reg,
                         uint16_t val);
int TEMPSENS_TemperatureGet(I2C_TypeDef *i2c,
                            uint8_t addr,
                            TEMPSENS_Temp_TypeDef *temp);

#ifdef __cplusplus
}
#endif

/** @} (end group TempSensor) */
/** @} (end group kitdrv) */

#endif /* __TEMPSENS_H */
