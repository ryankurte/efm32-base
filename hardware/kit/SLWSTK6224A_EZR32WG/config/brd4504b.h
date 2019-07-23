/***************************************************************************//**
 * @file
 * @brief Provide HAL configuration parameters.
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
#ifndef BRD4504B_H
#define BRD4504B_H

#include "hal-config-types.h"
/* Compatibility mapping to similar radio board. Frequency, matching is the same.
 *    Micro can be different though. */
#define RADIO_BOARD BRD4504A

// -----------------------------------------------------------------------------
/* BUTTON */
// Enable two buttons, 0 and 1
#define HAL_BUTTON_COUNT     2
#define HAL_BUTTON_ENABLE    { 0, 1 }
// Board has two buttons
#define BSP_BUTTON_COUNT     2
#define BSP_BUTTON_INIT                    \
  {                                        \
    { BSP_BUTTON0_PORT, BSP_BUTTON0_PIN }, \
    { BSP_BUTTON1_PORT, BSP_BUTTON1_PIN }  \
  }
// Initialize button GPIO DOUT to 0
#define BSP_BUTTON_GPIO_DOUT HAL_GPIO_DOUT_LOW
// Initialize button GPIO mode as input
#define BSP_BUTTON_GPIO_MODE HAL_GPIO_MODE_INPUT
// Define individual button GPIO port/pin
#define BSP_BUTTON0_PORT     gpioPortE
#define BSP_BUTTON0_PIN      3
#define BSP_BUTTON1_PORT     gpioPortE
#define BSP_BUTTON1_PIN      2

// -----------------------------------------------------------------------------
/* CLK */
// Set up HFCLK source as HFXO
#define HAL_CLK_HFCLK_SOURCE HAL_CLK_HFCLK_SOURCE_HFXO
// Setup LFCLK source as LFRCO
#define HAL_CLK_LFCLK_SOURCE HAL_CLK_LFCLK_SOURCE_LFRCO
// Set HFXO frequency as 48MHz
#define BSP_CLK_HFXO_FREQ 48000000UL
// HFXO initialization settings
#define BSP_CLK_HFXO_INIT CMU_HFXOINIT_DEFAULT
// Board has HFXO
#define BSP_CLK_HFXO_PRESENT 1
// Set LFXO frequency as 32.768kHz
#define BSP_CLK_LFXO_FREQ 32768UL
// Board has LFXO
#define BSP_CLK_LFXO_PRESENT 1

// -----------------------------------------------------------------------------
/* EXTDEV */
#define HAL_EXTDEV_ENABLE       1
// BSP_EXTDEV_SDN, BSP_EXTDEV_INT, BSP_EXTDEV_CS defined by device header

// -----------------------------------------------------------------------------
/* EZRADIOPRO */
#define HAL_EZRADIOPRO_ENABLE         1
#define BSP_EZRADIOPRO_USART          USARTRF0
#define BSP_EZRADIOPRO_FREQ           10
#define HAL_EZRADIOPRO_SHUTDOWN_SLEEP 1

// -----------------------------------------------------------------------------
/* LED */
// Enable two LEDs, 0 and 1
#define HAL_LED_COUNT  2
#define HAL_LED_ENABLE { 0, 1 }
// Board has two LEDs
#define BSP_LED_COUNT  2
#define BSP_LED_INIT                 \
  {                                  \
    { BSP_LED0_PORT, BSP_LED0_PIN }, \
    { BSP_LED1_PORT, BSP_LED1_PIN }  \
  }
// Define individual LED GPIO port/pin
#define BSP_LED0_PIN  6
#define BSP_LED0_PORT gpioPortF
#define BSP_LED1_PIN  7
#define BSP_LED1_PORT gpioPortF

// -----------------------------------------------------------------------------
/* USART1 */
#define BSP_USART1_CLK_PIN               2
#define BSP_USART1_CLK_PORT              gpioPortD
#define BSP_USART1_CS_PIN                3
#define BSP_USART1_CS_PORT               gpioPortD
#define BSP_USART1_MISO_PIN              1
#define BSP_USART1_MISO_PORT             gpioPortD
#define BSP_USART1_MOSI_PIN              0
#define BSP_USART1_MOSI_PORT             gpioPortD
#define BSP_USART1_ROUTE_LOC             _USART_ROUTE_LOCATION_LOC1

// -----------------------------------------------------------------------------
/* USART2 */
#define BSP_USART2_CTS_PIN               5
#define BSP_USART2_CTS_PORT              gpioPortB
#define BSP_USART2_ROUTE_LOC             _USART_ROUTE_LOCATION_LOC1
#define BSP_USART2_RTS_PIN               6
#define BSP_USART2_RTS_PORT              gpioPortB
#define BSP_USART2_RX_PIN                4
#define BSP_USART2_RX_PORT               gpioPortB
#define BSP_USART2_TX_PIN                3
#define BSP_USART2_TX_PORT               gpioPortB

// -----------------------------------------------------------------------------
/* VCOM */
#define BSP_VCOM_PRESENT                 1
#define BSP_VCOM_ENABLE_PIN              12
#define BSP_VCOM_ENABLE_PORT             gpioPortA
#define BSP_VCOM_CTS_PIN                 5
#define BSP_VCOM_CTS_PORT                gpioPortB
#define BSP_VCOM_ROUTE_LOC               _USART_ROUTE_LOCATION_LOC1
#define BSP_VCOM_RTS_PIN                 6
#define BSP_VCOM_RTS_PORT                gpioPortB
#define BSP_VCOM_RX_PIN                  4
#define BSP_VCOM_RX_PORT                 gpioPortB
#define BSP_VCOM_TX_PIN                  3
#define BSP_VCOM_TX_PORT                 gpioPortB
#define BSP_VCOM_USART                   HAL_SERIAL_PORT_USART2

#endif //BRD4504B_H
