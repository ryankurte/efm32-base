/***************************************************************************//**
 * @file
 * @brief Cambridge CMOS Sensors CCS811 gas sensor configuration file
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

#ifndef CCS811_CONFIG_H
#define CCS811_CONFIG_H

#define CCS811_I2C_BUS_TIMEOUT          1000  /**< I2C bus timeout                            */
#define CCS811_I2C_DEVICE               I2C0  /**< I2C device used to control the CCS811      */
#define CCS811_BUS_ADDRESS              0xB4  /**< I2C bus address of the CCS881              */
#define CCS811_HW_ID                    0x81  /**< The hardware ID of the CCS881              */
#define CCS811_FIRMWARE_UPDATE          0     /**< Enable or disable CCS811 firmware upgrade  */

#endif // CCS811_CONFIG_H
