/***************************************************************************//**
 * @file
 * @brief BRD4101B specific configuration for on-board serial flash.
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

#define MX25_PORT_MOSI         gpioPortA
#define MX25_PIN_MOSI          0
#define MX25_PORT_MISO         gpioPortA
#define MX25_PIN_MISO          1
#define MX25_PORT_SCLK         gpioPortB
#define MX25_PIN_SCLK          11
#define MX25_PORT_CS           gpioPortC
#define MX25_PIN_CS            7

#define MX25_USART             USART1
#define MX25_USART_CLK         cmuClock_USART1

#endif // MX25CONFIG_H
