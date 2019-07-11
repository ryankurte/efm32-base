/***************************************************************************//**
 * @file
 * @brief Si1133 Ambient Light and UV sensor configuration file
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

#ifndef SI1133_CONFIG_H
#define SI1133_CONFIG_H

#define SI1133_I2C_BUS_TIMEOUT          (1000)
#define SI1133_I2C_DEVICE_BUS_ADDRESS   (0xAA)

#define SI1133_I2C_DEVICE      (I2C1)
#define SI1133_SDA_LOCATION    (I2C_ROUTELOC0_SDALOC_LOC17)
#define SI1133_SCL_LOCATION    (I2C_ROUTELOC0_SCLLOC_LOC17)
#define SI1133_SDA_LOC         17
#define SI1133_SCL_LOC         17
#define SI1133_SDA_PORT        gpioPortC
#define SI1133_SDA_PIN         4
#define SI1133_SCL_PORT        gpioPortC
#define SI1133_SCL_PIN         5

#define I2CSPM_INIT_SI1133                                                \
  { SI1133_I2C_DEVICE,     /* I2C instance                             */ \
    SI1133_SCL_PORT,       /* SCL port                                 */ \
    SI1133_SCL_PIN,        /* SCL pin                                  */ \
    SI1133_SDA_PORT,       /* SDA port                                 */ \
    SI1133_SDA_PIN,        /* SDA pin                                  */ \
    SI1133_SCL_LOC,        /* Port location of SCL signal              */ \
    SI1133_SDA_LOC,        /* Port location of SDA signal              */ \
    0,                     /* Use currently configured reference clock */ \
    I2C_FREQ_STANDARD_MAX, /* Set to standard rate                     */ \
    i2cClockHLRStandard,   /* Set to use 4:4 low/high duty cycle       */ \
  }

#endif // SI1133_CONFIG_H
