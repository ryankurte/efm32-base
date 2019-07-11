/***************************************************************************//**
 * @file
 * @brief MX25R8035F NOR flash configuration file
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

#ifndef MX25_CONFIG_H
#define MX25_CONFIG_H

#include "em_device.h"
#include "em_gpio.h"

#define MX25_PORT_MOSI         gpioPortK
#define MX25_PIN_MOSI          0
#define MX25_PORT_MISO         gpioPortK
#define MX25_PIN_MISO          2
#define MX25_PORT_SCLK         gpioPortF
#define MX25_PIN_SCLK          7
#define MX25_PORT_CS           gpioPortK
#define MX25_PIN_CS            1

#define MX25_USART             USART2
#define MX25_USART_CLK         cmuClock_USART2

#define MX25_USART_LOC_MOSI    USART_ROUTELOC0_TXLOC_LOC29
#define MX25_USART_LOC_MISO    USART_ROUTELOC0_RXLOC_LOC30
#define MX25_USART_LOC_SCLK    USART_ROUTELOC0_CLKLOC_LOC18

#endif // MX25_CONFIG_H
