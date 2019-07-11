/***************************************************************************//**
 * @file
 * @brief Driver for the Si1147 Proximity sensor
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

#ifndef __SI1147_H
#define __SI1147_H

#include "em_device.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Si114x
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/
/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

typedef struct {
  I2C_TypeDef *i2c;
  uint8_t addr;
} si114x_i2c_t;

/** @endcond */

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/
uint32_t Si1147_Write_Register (I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, uint8_t data);
uint32_t Si1147_Write_Block_Register (I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, uint8_t length, uint8_t const *data);
uint32_t Si1147_Read_Block_Register (I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, uint8_t length, uint8_t  *data);
uint32_t Si1147_Read_Register (I2C_TypeDef *i2c, uint8_t addr, uint8_t reg, uint8_t *data);

#ifdef __cplusplus
}
#endif

/** @} (end group Si114x) */
/** @} (end group kitdrv) */

#endif /* __TEMPSENS_H */
