/***************************************************************************//**
 * @file
 * @brief Provide stdio retargeting configuration parameters
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

#ifndef RETARGETSERIALCONFIG_H
#define RETARGETSERIALCONFIG_H

#include "bsp.h"

/* Override if needed with commandline parameter -DRETARGET_xxx */

#if !defined(RETARGET_USART0) && !defined(RETARGET_VCOM)
#define RETARGET_USART0    /* Use USART0 by default. */
#endif

#if defined(RETARGET_USART0) || defined(RETARGET_VCOM)
  #define RETARGET_IRQ_NAME    USART0_RX_IRQHandler         /* UART IRQ Handler */
  #define RETARGET_CLK         cmuClock_USART0              /* HFPER Clock */
  #define RETARGET_IRQn        USART0_RX_IRQn               /* IRQ number */
  #define RETARGET_UART        USART0                       /* UART instance */
  #define RETARGET_TX          USART_Tx                     /* Set TX to USART_Tx */
  #define RETARGET_RX          USART_Rx                     /* Set RX to USART_Rx */

  #define RETARGET_TX_LOCATION _USART_ROUTELOC0_TXLOC_LOC1  /* Location of of USART TX pin */
  #define RETARGET_RX_LOCATION _USART_ROUTELOC0_RXLOC_LOC1  /* Location of of USART RX pin */

  #define RETARGET_TXPORT      gpioPortE                    /* UART transmission port */
  #define RETARGET_TXPIN       7                            /* UART transmission pin */
  #define RETARGET_RXPORT      gpioPortE                    /* UART reception port */
  #define RETARGET_RXPIN       6                            /* UART reception pin */
  #define RETARGET_USART       1                            /* Includes em_usart.h */

  #define RETARGET_CTS_LOCATION _USART_ROUTELOC1_CTSLOC_LOC4
  #define RETARGET_RTS_LOCATION _USART_ROUTELOC1_CTSLOC_LOC4
  #define RETARGET_CTSPORT      gpioPortB
  #define RETARGET_CTSPIN       6
  #define RETARGET_RTSPORT      gpioPortB
  #define RETARGET_RTSPIN       5

#if defined(RETARGET_VCOM)
  #define RETARGET_PERIPHERAL_ENABLE()  \
  GPIO_PinModeSet(BSP_VCOM_ENABLE_PORT, \
                  BSP_VCOM_ENABLE_PIN,  \
                  gpioModePushPull,     \
                  1);
#else
  #define RETARGET_PERIPHERAL_ENABLE()
#endif

#else
#error "Illegal USART selection."
#endif

#endif // RETARGETSERIALCONFIG_H
