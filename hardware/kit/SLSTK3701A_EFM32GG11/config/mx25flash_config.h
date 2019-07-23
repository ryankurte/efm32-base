/***************************************************************************//**
 * @file
 * @brief SLSTK3701A specific configuration for on-board serial flash.
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

#ifndef MX25CONFIG_H
#define MX25CONFIG_H

#include "em_device.h"
#include "em_gpio.h"

#define MX25_PORT_PWR_EN       gpioPortG
#define MX25_PIN_PWR_EN        13

#define MX25_PORT_DQ0          gpioPortG
#define MX25_PIN_DQ0           1
#define MX25_PORT_DQ1          gpioPortG
#define MX25_PIN_DQ1           2
#define MX25_PORT_DQ2          gpioPortG
#define MX25_PIN_DQ2           3
#define MX25_PORT_DQ3          gpioPortG
#define MX25_PIN_DQ3           4
#define MX25_PORT_MOSI         MX25_PORT_DQ0
#define MX25_PIN_MOSI          MX25_PIN_DQ0
#define MX25_PORT_MISO         MX25_PORT_DQ1
#define MX25_PIN_MISO          MX25_PIN_DQ1
#define MX25_PORT_SCLK         gpioPortG
#define MX25_PIN_SCLK          0
#define MX25_PORT_CS           gpioPortG
#define MX25_PIN_CS            9

#define MX25_USART             USART1
#define MX25_USART_CLK         cmuClock_USART1

#define MX25_QSPI              QSPI0
#define MX25_QSPI_CLK          cmuClock_QSPI0
#define MX25_QSPI_LOC          QSPI_ROUTELOC0_QSPILOC_LOC2
#define MX25_QSPI_CSPEN        QSPI_ROUTEPEN_CS0PEN
#define MX25_QSPI_CSNUM        0

#endif // MX25CONFIG_H
