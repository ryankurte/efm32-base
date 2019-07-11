/***************************************************************************//**
 * @file
 * @brief Driver for the Silicon Labs Si7210 Hall Effect Sensor
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

#ifndef HALL_SI7210_H
#define HALL_SI7210_H

#include <stdint.h>
#include <stdbool.h>

/**************************************************************************//**
* @addtogroup TBSense_BSP
* @{
******************************************************************************/

/**************************************************************************//**
* @addtogroup HALL
* @{
******************************************************************************/

/***************************************************************************//**
 * @addtogroup Si7210
 * @{
 ******************************************************************************/

/**************************************************************************//**
* @name Error Codes
* @{
******************************************************************************/
#define SI7210_OK                            0        /**< No errors                               */
#define SI7210_ERROR_I2C_TRANSFER_FAILED     0x0001   /**< I2C transaction failed                  */
#define SI7210_ERROR_INVALID_DEVICE_ID       0x0002   /**< The device ID is invalid                */
#define SI7210_ERROR_CONFIG_INVALID_MODE     0x0003   /**< The chosen configuration is invalid     */
#define SI7210_ERROR_OTP_BUSY                0x0004   /**< The OTP memory is busy                  */
#define SI7210_ERROR_READ_TIMEOUT            0x0005   /**< Timeout occurred during read operation  */
#define SI7210_ERROR_NODATA                  0x0006   /**< No measurement data available           */
/**@}*/

uint32_t SI7210_sleep(void);
uint32_t SI7210_wakeUp(void);
uint32_t SI7210_dataRead(int16_t *data);
uint32_t SI7210_sleepSltimeena(void);
uint32_t SI7210_identify(uint8_t *id, uint8_t *rev);
uint32_t SI7210_regReadOTP(uint8_t otpAddr, uint8_t *otpData);
uint32_t SI7210_set200mTRange(void);
uint32_t SI7210_readmTDataSltimeena(bool range200mT, int32_t *mTdata);
uint32_t SI7210_readmTDataSleep(bool range200mT, int32_t *mTdata);
uint8_t  SI7210_calculateSWOP(float threshold);
uint8_t  SI7210_calculateSWHYST(float hysteresis, bool scale200mT);
uint8_t  SI7210_calculateSWTAMPER(float tamper, bool scale200mT);
uint8_t  SI7210_calculateSlTime(uint32_t samplePeriod, uint8_t *slFast);
uint32_t SI7210_registerRead(uint8_t addr, uint8_t *data);
uint32_t SI7210_registerWrite(uint8_t addr, uint8_t data);
uint32_t SI7210_regSetBits(uint8_t addr, uint8_t mask);
uint32_t SI7210_regClearBits(uint8_t addr, uint8_t mask);

/** @} */
/** @} */
/** @} */

#endif // HALL_SI7210_H
