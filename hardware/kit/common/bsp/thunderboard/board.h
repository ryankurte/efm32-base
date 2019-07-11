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

#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include "gpiointerrupt.h"

#include "bspconfig.h"

/***************************************************************************//**
 * @defgroup TBSense_BSP Thunderboard Sense BSP
 * @{
 * @brief BSP for Thunderboard Sense and Thunderboard Sense 2
 ******************************************************************************/

/**************************************************************************//**
* @name BOARD Error Codes
* @{
******************************************************************************/
#define BOARD_OK                              0     /**< OK                                        */
#define BOARD_ERROR_I2C_TRANSFER_TIMEOUT      0x01  /**< I2C timeout occurred                      */
#define BOARD_ERROR_I2C_TRANSFER_NACK         0x02  /**< No acknowledgement received               */
#define BOARD_ERROR_I2C_TRANSFER_FAILED       0x03  /**< I2C transaction failed                    */
#define BOARD_ERROR_PIC_ID_MISMATCH           0x04  /**< The ID of the PIC is invalid              */
#define BOARD_ERROR_PIC_FW_INVALID            0x05  /**< Invalid PIC firmware                      */
#define BOARD_ERROR_PIC_FW_UPDATE_FAILED      0x06  /**< PIC firmware update failed                */

#define BOARD_ERROR_NO_POWER_INT_CTRL         0x10  /**< Power and Interrupt Controller not found  */
#define BOARD_ERROR_I2C_BUS_SELECT_INVALID    0x11  /**< Invalid I2C bus selection                 */
#define BOARD_ERROR_I2C_BUS_SELECT_FAILED     0x12  /**< I2C bus selection failed                  */
/**@}*/
/**@}*/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#define BOARD_I2C_BUS_SELECT_NONE         0           /**< No I2C bus selected                               */
#define BOARD_I2C_BUS_SELECT_ENV_SENSOR   (1 << 0)    /**< The I2C bus of the environmental sensors selected */
#define BOARD_I2C_BUS_SELECT_GAS          (1 << 1)    /**< The I2C bus of the gas sensors selected           */
#define BOARD_I2C_BUS_SELECT_HALL         (1 << 2)    /**< The I2C bus of the Hall sensor selected           */
/** @endcond */

uint32_t BOARD_init                (void);

uint32_t BOARD_imuEnable           (bool enable);
uint32_t BOARD_imuEnableIRQ        (bool enable);
void     BOARD_imuClearIRQ         (void);

uint32_t BOARD_envSensEnable       (bool enable);
uint32_t BOARD_envSensEnableIRQ    (bool enable);

void     BOARD_flashDeepPowerDown  (void);

uint32_t BOARD_gasSensorEnable     (bool enable);
uint32_t BOARD_gasSensorEnableIRQ  (bool enable);
void     BOARD_gasSensorClearIRQ   (void);
uint32_t BOARD_gasSensorWake       (bool wake);

uint32_t BOARD_bapEnable           (bool enable);

uint32_t BOARD_rhtempEnable        (bool enable);

uint32_t BOARD_alsEnable           (bool enable);
uint32_t BOARD_alsEnableIRQ        (bool enable);
void     BOARD_alsClearIRQ         (void);

void     BOARD_ledSet              (uint8_t leds);

uint32_t BOARD_micEnable           (bool enable);

uint32_t BOARD_i2cBusSelect        (uint8_t select);

uint8_t  BOARD_pushButtonGetState  (void);
void     BOARD_pushButtonEnableIRQ (bool enable);

void     BOARD_rgbledEnable        (bool enable, uint8_t mask);
void     BOARD_rgbledSetColor      (uint8_t red, uint8_t green, uint8_t blue);
void     BOARD_rgbledSetRawColor   (uint16_t red, uint16_t green, uint16_t blue);
void     BOARD_rgbledPowerEnable   (bool enable);

#ifdef BSP_TBSENSE
  #include "board_4160.h"
#endif

#ifdef BSP_TBSENSE2
  #include "board_4166.h"
#endif

#endif // BOARD_H
