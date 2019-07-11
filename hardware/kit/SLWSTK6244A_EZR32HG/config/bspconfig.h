/***************************************************************************//**
 * @file
 * @brief Provide BSP (board support package) configuration parameters.
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

#ifndef __SILICON_LABS_BSPCONFIG_H__
#define __SILICON_LABS_BSPCONFIG_H__

#define BSP_STK
#define BSP_NET
#define BSP_WSTK
#define BSP_WSTK_BRD4001    // WSTK mainboard
#define BSP_WSTK_BRD4544A   // MCU/Radio plug-in board

#define BSP_BCC_LEUART      LEUART0
#define BSP_BCC_CLK         cmuClock_LEUART0
#define BSP_BCC_LOCATION    LEUART_ROUTE_LOCATION_LOC0
#define BSP_BCC_TXPORT      gpioPortD
#define BSP_BCC_TXPIN       4
#define BSP_BCC_RXPORT      gpioPortD
#define BSP_BCC_RXPIN       5
#define BSP_BCC_ENABLE_PORT gpioPortC
#define BSP_BCC_ENABLE_PIN  8

#define BSP_DISP_ENABLE_PORT  gpioPortA
#define BSP_DISP_ENABLE_PIN   1                   /* MemLCD display enable */

#define BSP_GPIO_LEDS
#define BSP_NO_OF_LEDS          2
#define BSP_GPIO_LED0_PORT      gpioPortF
#define BSP_GPIO_LED0_PIN       4
#define BSP_GPIO_LED1_PORT      gpioPortF
#define BSP_GPIO_LED1_PIN       2
#define BSP_GPIO_LEDARRAY_INIT  { { BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN }, { BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN } }

#define BSP_GPIO_BUTTONARRAY
#define BSP_NO_OF_BUTTONS       2
#define BSP_GPIO_PB0_PORT       gpioPortC
#define BSP_GPIO_PB0_PIN        9
#define BSP_GPIO_PB1_PORT       gpioPortC
#define BSP_GPIO_PB1_PIN        10

#define BSP_GPIO_BUTTONARRAY_INIT { { BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN }, \
                                    { BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN } }

#define BSP_INIT_DEFAULT  0

#define BSP_BCP_VERSION 2
#include "bsp_bcp.h"

#endif
