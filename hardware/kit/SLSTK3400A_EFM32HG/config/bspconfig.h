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

#ifndef __SILICON_LABS_BSPCONFIG_H
#define __SILICON_LABS_BSPCONFIG_H

#define BSP_STK
#define BSP_STK_BRD2012

#define BSP_BCC_USART         USART1
#define BSP_BCC_CLK           cmuClock_USART1
#define BSP_BCC_LOCATION      USART_ROUTE_LOCATION_LOC4
#define BSP_BCC_TXPORT        gpioPortF
#define BSP_BCC_TXPIN         2
#define BSP_BCC_RXPORT        gpioPortA
#define BSP_BCC_RXPIN         0
#define BSP_BCC_ENABLE_PORT   gpioPortA
#define BSP_BCC_ENABLE_PIN    9

#define BSP_GPIO_LEDS
#define BSP_NO_OF_LEDS          2
#define BSP_GPIO_LED0_PORT      gpioPortF
#define BSP_GPIO_LED0_PIN       4
#define BSP_GPIO_LED1_PORT      gpioPortF
#define BSP_GPIO_LED1_PIN       5

#define BSP_GPIO_LEDARRAY_INIT  { { BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN }, { BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN } }

#define BSP_GPIO_BUTTONS
#define BSP_NO_OF_BUTTONS       2
#define BSP_GPIO_PB0_PORT       gpioPortC
#define BSP_GPIO_PB0_PIN        9
#define BSP_GPIO_PB1_PORT       gpioPortC
#define BSP_GPIO_PB1_PIN        10

#define BSP_GPIO_BUTTONARRAY_INIT { { BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN }, { BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN } }

#define BSP_INIT_DEFAULT  0

#define BSP_BCP_VERSION 2
#include "bsp_bcp.h"

#endif /* __SILICON_LABS_BSPCONFIG_H */
