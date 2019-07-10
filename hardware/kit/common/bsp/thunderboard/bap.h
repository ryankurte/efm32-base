/***************************************************************************//**
 * @file
 * @brief Driver for the Bosch Sensortec BMP280 barometric pressure sensor
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

#ifndef BAP_H
#define BAP_H

/***************************************************************************//**
 * @addtogroup BAP
 * @{
 ******************************************************************************/

#include "bap_config.h"

/**************************************************************************//**
* @name Error Codes
* @{
******************************************************************************/
#define BAP_OK                             0x0000  /**< No errors                                        */
#define BAP_ERROR_DRIVER_NOT_INITIALIZED   0x0001  /**< The driver is not initialized                    */
#define BAP_ERROR_I2C_TRANSACTION_FAILED   0x0002  /**< I2C transaction failed                           */
#define BAP_ERROR_DEVICE_ID_MISMATCH       0x0003  /**< The device ID does not match the expected value  */
/**@}*/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#define BAP_DEVICE_ID_BMP280               0x58   /**< Device ID of the BMP280 chip          */
/** @endcond */

/***************************************************************************//**
 * @brief
 *    Structure to configure the BMP280 device
 ******************************************************************************/
typedef struct __BAP_Config {
  uint8_t oversampling;       /**< Oversampling value                         */
  uint8_t powerMode;          /**< SLEEP, FORCED or NORMAL power mode setting */
  uint8_t standbyTime;        /**< Standby time setting                       */
} BAP_Config;

uint32_t BAP_init             (uint8_t *deviceId);
void     BAP_deInit           (void);
uint32_t BAP_config           (BAP_Config *cfg);
uint32_t BAP_getTemperature   (float *temperature);
uint32_t BAP_getPressure      (float *pressure);

/** @} */
/** @} */

#endif // BMP_H
