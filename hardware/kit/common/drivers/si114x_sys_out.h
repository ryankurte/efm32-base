/***************************************************************************//**
 * @file
 * @brief Implementation specific functions for HRM code
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
#ifndef SI114X_SYS_OUT
#define SI114X_SYS_OUT

#include "em_i2c.h"

typedef struct Si114xPortConfig{
  I2C_TypeDef       *i2cPort;   /**< I2C port Si114x is connected to */
  uint8_t           i2cAddress; /**< I2C address of Si114x */
  GPIO_Port_TypeDef irqPort;    /**< Port for Si114x INT pin */
  int               irqPin;     /**< Pin for Si114x INT pin */
} Si114xPortConfig_t;

int16_t Si114xProcessIrq(void* si114x_handle, uint16_t timestamp);

#endif
