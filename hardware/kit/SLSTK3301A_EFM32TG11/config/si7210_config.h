/***************************************************************************//**
 * @file
 * @brief Si7210 Hall Effect Sensor configuration file
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

#ifndef SI7210_CONFIG_H
#define SI7210_CONFIG_H

#define SI7210_CONFIG_I2C_BUS_TIMEOUT   1000
#define SI7210_CONFIG_I2C_BUS_ADDRESS   0x30
#define SI7210_CONFIG_DEVICE_ID         0x13

/***************************************************************************//**
 * @cond DOXYGEN_INCLUDE_HALL_EFFECT_SENSOR
 * @defgroup Si7210_Error_Messages Si7210 Error Messages
 * @{
 * @brief Si7210 error message macro definitions
 ******************************************************************************/

#define SI7210_OK                            0        /**< No errors                               */
#define SI7210_ERROR_I2C_TRANSFER_FAILED     0x0001   /**< I2C transaction failed                  */
#define SI7210_ERROR_INVALID_DEVICE_ID       0x0002   /**< The device ID is invalid                */
#define SI7210_ERROR_CONFIG_INVALID_MODE     0x0003   /**< The chosen configuration is invalid     */
#define SI7210_ERROR_OTP_BUSY                0x0004   /**< The OTP memory is busy                  */
#define SI7210_ERROR_READ_TIMEOUT            0x0005   /**< Timeout occurred during read operation  */

/** @} {end defgroup Si7210_Error_Messages} */
/** @endcond {DOXYGEN_INCLUDE_HALL_EFFECT_SENSOR} */

/***************************************************************************//**
 * @cond DOXYGEN_INCLUDE_HALL_EFFECT_SENSOR
 * @defgroup Si7210_Config_Defines Si7210 Configuration Definitions
 * @{
 * @brief Si7210 configuration macro definitions
 ******************************************************************************/

#define SI7210_CONFIG_MODE_THRESHOLD         0x01     /**< Configure the device in Threshold mode  */
#define SI7210_CONFIG_MODE_MEASURE           0x02     /**< Configure the device in Measure mode    */

#define SI7210_CONFIG_SCALE_20mT             0x00     /**< Set full scale to 20mT                  */
#define SI7210_CONFIG_SCALE_200mT            0x01     /**< Set full scale to 200mT                 */

#define SI7210_CONFIG_POLARITY_OMNI          0x00     /**< Omnipolar field polarity                */
#define SI7210_CONFIG_POLARITY_NEG           0x01     /**< Negative field polarity                 */
#define SI7210_CONFIG_POLARITY_POS           0x02     /**< Positive field polarity                 */

#define SI7210_CONFIG_THRESHOLD_MIN          0.08f    /**< Minimum threshold value                 */
#define SI7210_CONFIG_THRESHOLD_MAX          19.2f    /**< Maximum threshold value                 */

#define SI7210_CONFIG_I2C_DEVICE      (I2C0)
#define SI7210_CONFIG_SDA_LOCATION    (I2C_ROUTELOC0_SDALOC_LOC1)
#define SI7210_CONFIG_SCL_LOCATION    (I2C_ROUTELOC0_SCLLOC_LOC1)
#define SI7210_CONFIG_SDA_LOC         1
#define SI7210_CONFIG_SCL_LOC         1
#define SI7210_CONFIG_SDA_PORT        gpioPortD
#define SI7210_CONFIG_SDA_PIN         7
#define SI7210_CONFIG_SCL_PORT        gpioPortD
#define SI7210_CONFIG_SCL_PIN         6

#define I2CSPM_INIT_SI7210                                                    \
  { SI7210_CONFIG_I2C_DEVICE,  /* I2C instance */                             \
    SI7210_CONFIG_SCL_PORT,    /* SCL port */                                 \
    SI7210_CONFIG_SCL_PIN,     /* SCL pin */                                  \
    SI7210_CONFIG_SDA_PORT,    /* SDA port */                                 \
    SI7210_CONFIG_SDA_PIN,     /* SDA pin */                                  \
    SI7210_CONFIG_SCL_LOC,     /* Port location of SCL signal */              \
    SI7210_CONFIG_SDA_LOC,     /* Port location of SDA signal */              \
    0,                         /* Use currently configured reference clock */ \
    I2C_FREQ_STANDARD_MAX,     /* Set to standard rate  */                    \
    i2cClockHLRStandard,       /* Set to use 4:4 low/high duty cycle */       \
  }

#define SI7210_CONFIG_PORT_OUT1         gpioPortC
#define SI7210_CONFIG_PIN_OUT1          12

#endif /* SI7210_CONFIG_H */
