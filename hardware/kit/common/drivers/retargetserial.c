/***************************************************************************//**
 * @file
 * @brief Provide stdio retargeting to USART/UART or LEUART.
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

#include <stdio.h>
#include "em_device.h"
#include "em_cmu.h"
#include "em_core.h"
#include "em_gpio.h"
#include "retargetserial.h"

#if defined(HAL_CONFIG)
#include "retargetserialhalconfig.h"
#else
#include "retargetserialconfig.h"
#endif

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup RetargetIo
 * @{
 ******************************************************************************/

#if defined(RETARGET_USART)
#include "em_usart.h"
#endif

#if defined(RETARGET_LEUART)
#include "em_leuart.h"
#endif

/* Receive buffer */
#ifndef RXBUFSIZE
#define RXBUFSIZE    8                          /**< Buffer size for RX */
#endif
static volatile int     rxReadIndex  = 0;       /**< Index in buffer to be read */
static volatile int     rxWriteIndex = 0;       /**< Index in buffer to be written to */
static volatile int     rxCount      = 0;       /**< Keeps track of how much data which are stored in the buffer */
static volatile uint8_t rxBuffer[RXBUFSIZE];    /**< Buffer to store data */
static uint8_t          LFtoCRLF    = 0;        /**< LF to CRLF conversion disabled */
static bool             initialized = false;    /**< Initialize UART/LEUART */

/**************************************************************************//**
 * @brief Disable RX interrupt
 *****************************************************************************/
static void disableRxInterrupt()
{
#if defined(RETARGET_USART)
  USART_IntDisable(RETARGET_UART, USART_IF_RXDATAV);
#else
  LEUART_IntDisable(RETARGET_UART, LEUART_IF_RXDATAV);
#endif
}

/**************************************************************************//**
 * @brief Enable RX interrupt
 *****************************************************************************/
static void enableRxInterrupt()
{
#if defined(RETARGET_USART)
  USART_IntEnable(RETARGET_UART, USART_IF_RXDATAV);
#else
  LEUART_IntEnable(RETARGET_UART, LEUART_IF_RXDATAV);
#endif
}

/**************************************************************************//**
 * @brief UART/LEUART IRQ Handler
 *****************************************************************************/
void RETARGET_IRQ_NAME(void)
{
#if defined(RETARGET_USART)
  if (RETARGET_UART->STATUS & USART_STATUS_RXDATAV) {
#else
  if (RETARGET_UART->IF & LEUART_IF_RXDATAV) {
#endif

    if (rxCount < RXBUFSIZE) {
      /* There is room for data in the RX buffer so we store the data. */
      rxBuffer[rxWriteIndex] = RETARGET_RX(RETARGET_UART);
      rxWriteIndex++;
      rxCount++;
      if (rxWriteIndex == RXBUFSIZE) {
        rxWriteIndex = 0;
      }
    } else {
      /* The RX buffer is full so we must wait for the RETARGET_ReadChar()
       * function to make some more room in the buffer. RX interrupts are
       * disabled to let the ISR exit. The RX interrupt will be enabled in
       * RETARGET_ReadChar(). */
      disableRxInterrupt();
    }
  }
}

/**************************************************************************//**
 * @brief UART/LEUART toggle LF to CRLF conversion
 * @param on If non-zero, automatic LF to CRLF conversion will be enabled
 *****************************************************************************/
void RETARGET_SerialCrLf(int on)
{
  if (on) {
    LFtoCRLF = 1;
  } else {
    LFtoCRLF = 0;
  }
}

/**************************************************************************//**
 * @brief Intializes UART/LEUART
 *****************************************************************************/
void RETARGET_SerialInit(void)
{
  /* Enable peripheral clocks */
#if defined(_CMU_HFPERCLKEN0_MASK)
  CMU_ClockEnable(cmuClock_HFPER, true);
#endif
  /* Configure GPIO pins */
  CMU_ClockEnable(cmuClock_GPIO, true);
  /* To avoid false start, configure output as high */
  GPIO_PinModeSet(RETARGET_TXPORT, RETARGET_TXPIN, gpioModePushPull, 1);
  GPIO_PinModeSet(RETARGET_RXPORT, RETARGET_RXPIN, gpioModeInputPull, 1);

#if defined(RETARGET_USART)
  USART_TypeDef           *usart = RETARGET_UART;
  USART_InitAsync_TypeDef init   = USART_INITASYNC_DEFAULT;

  /* Enable DK RS232/UART switch */
  RETARGET_PERIPHERAL_ENABLE();

  CMU_ClockEnable(RETARGET_CLK, true);

  /* Configure USART for basic async operation */
  init.enable = usartDisable;
  USART_InitAsync(usart, &init);

#if defined(GPIO_USART_ROUTEEN_TXPEN)
  /* Enable pins at correct UART/USART location. */
  GPIO->USARTROUTE[RETARGET_UART_INDEX].ROUTEEN =
    GPIO_USART_ROUTEEN_TXPEN | GPIO_USART_ROUTEEN_RXPEN;
  GPIO->USARTROUTE[RETARGET_UART_INDEX].TXROUTE =
    (RETARGET_TXPORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
    | (RETARGET_TXPIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
  GPIO->USARTROUTE[RETARGET_UART_INDEX].RXROUTE =
    (RETARGET_RXPORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
    | (RETARGET_RXPIN << _GPIO_USART_RXROUTE_PIN_SHIFT);

#else
  /* Enable pins at correct UART/USART location. */
  #if defined(USART_ROUTEPEN_RXPEN)
  usart->ROUTEPEN = USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
  usart->ROUTELOC0 = (usart->ROUTELOC0
                      & ~(_USART_ROUTELOC0_TXLOC_MASK
                          | _USART_ROUTELOC0_RXLOC_MASK) )
                     | (RETARGET_TX_LOCATION << _USART_ROUTELOC0_TXLOC_SHIFT)
                     | (RETARGET_RX_LOCATION << _USART_ROUTELOC0_RXLOC_SHIFT);
  #else
  usart->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | RETARGET_LOCATION;
  #endif
#endif

  /* Clear previous RX interrupts */
  USART_IntClear(RETARGET_UART, USART_IF_RXDATAV);
  NVIC_ClearPendingIRQ(RETARGET_IRQn);

  /* Enable RX interrupts */
  USART_IntEnable(RETARGET_UART, USART_IF_RXDATAV);
  NVIC_EnableIRQ(RETARGET_IRQn);

  /* Finally enable it */
  USART_Enable(usart, usartEnable);

#else
  LEUART_TypeDef      *leuart = RETARGET_UART;
  LEUART_Init_TypeDef init    = LEUART_INIT_DEFAULT;

  /* Enable DK LEUART/RS232 switch */
  RETARGET_PERIPHERAL_ENABLE();

  /* Enable CORE LE clock in order to access LE modules */
  CMU_ClockEnable(cmuClock_CORELE, true);

#if defined(RETARGET_VCOM)
  /* Select HFXO/2 for LEUARTs (and wait for it to stabilize) */
#if defined(_CMU_LFCLKSEL_LFB_HFCORECLKLEDIV2)
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_CORELEDIV2);
#else
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_HFCLKLE);
#endif
#else
  /* Select LFXO for LEUARTs (and wait for it to stabilize) */
  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
#endif

  CMU_ClockEnable(RETARGET_CLK, true);

  /* Do not prescale clock */
  CMU_ClockDivSet(RETARGET_CLK, cmuClkDiv_1);

  /* Configure LEUART */
  init.enable = leuartDisable;
#if defined(RETARGET_VCOM)
  init.baudrate = 115200;
#endif
  LEUART_Init(leuart, &init);
  /* Enable pins at default location */
  #if defined(LEUART_ROUTEPEN_RXPEN)
  leuart->ROUTEPEN = USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
  leuart->ROUTELOC0 = (leuart->ROUTELOC0
                       & ~(_LEUART_ROUTELOC0_TXLOC_MASK
                           | _LEUART_ROUTELOC0_RXLOC_MASK) )
                      | (RETARGET_TX_LOCATION << _LEUART_ROUTELOC0_TXLOC_SHIFT)
                      | (RETARGET_RX_LOCATION << _LEUART_ROUTELOC0_RXLOC_SHIFT);
  #else
  leuart->ROUTE = LEUART_ROUTE_RXPEN | LEUART_ROUTE_TXPEN | RETARGET_LOCATION;
  #endif

  /* Clear previous RX interrupts */
  LEUART_IntClear(RETARGET_UART, LEUART_IF_RXDATAV);
  NVIC_ClearPendingIRQ(RETARGET_IRQn);

  /* Enable RX interrupts */
  LEUART_IntEnable(RETARGET_UART, LEUART_IF_RXDATAV);
  NVIC_EnableIRQ(RETARGET_IRQn);

  /* Finally enable it */
  LEUART_Enable(leuart, leuartEnable);
#endif

#if !defined(__CROSSWORKS_ARM) && defined(__GNUC__)
  setvbuf(stdout, NULL, _IONBF, 0);   /*Set unbuffered mode for stdout (newlib)*/
#endif

  initialized = true;
}

/**************************************************************************//**
 * @brief Receive a byte from USART/LEUART and put into global buffer
 * @return -1 on failure, or positive character integer on sucesss
 *****************************************************************************/
int RETARGET_ReadChar(void)
{
  int c = -1;
  CORE_DECLARE_IRQ_STATE;

  if (initialized == false) {
    RETARGET_SerialInit();
  }

  CORE_ENTER_ATOMIC();
  if (rxCount > 0) {
    c = rxBuffer[rxReadIndex];
    rxReadIndex++;
    if (rxReadIndex == RXBUFSIZE) {
      rxReadIndex = 0;
    }
    rxCount--;
    /* Unconditionally enable the RX interrupt. RX interrupts are disabled when
     * a buffer full condition is entered. This way flow control can be handled
     * automatically by the hardware. */
    enableRxInterrupt();
  }

  CORE_EXIT_ATOMIC();

  return c;
}

/**************************************************************************//**
 * @brief Transmit single byte to USART/LEUART
 * @param c Character to transmit
 * @return Transmitted character
 *****************************************************************************/
int RETARGET_WriteChar(char c)
{
  if (initialized == false) {
    RETARGET_SerialInit();
  }

  /* Add CR or LF to CRLF if enabled */
  if (LFtoCRLF && (c == '\n')) {
    RETARGET_TX(RETARGET_UART, '\r');
  }
  RETARGET_TX(RETARGET_UART, c);

  return c;
}

/**************************************************************************//**
 * @brief Enable hardware flow control. (RTS + CTS)
 * @return true if hardware flow control was enabled and false otherwise.
 *****************************************************************************/
bool RETARGET_SerialEnableFlowControl(void)
{
#if defined(RETARGET_USART)               \
  && defined(_USART_ROUTEPEN_CTSPEN_MASK) \
  && defined(RETARGET_CTSPORT)            \
  && defined(RETARGET_RTSPORT)
  GPIO_PinModeSet(RETARGET_CTSPORT, RETARGET_CTSPIN, gpioModeInputPull, 0);
  GPIO_PinModeSet(RETARGET_RTSPORT, RETARGET_RTSPIN, gpioModePushPull, 0);
  RETARGET_UART->ROUTELOC1 = (RETARGET_CTS_LOCATION << _USART_ROUTELOC1_CTSLOC_SHIFT)
                             | (RETARGET_RTS_LOCATION << _USART_ROUTELOC1_RTSLOC_SHIFT);
  RETARGET_UART->ROUTEPEN |= (USART_ROUTEPEN_CTSPEN | USART_ROUTEPEN_RTSPEN);
  RETARGET_UART->CTRLX    |= USART_CTRLX_CTSEN;
  return true;
#else
  return false;
#endif
}

/**************************************************************************//**
 * @brief Flush UART/LEUART
 *****************************************************************************/
void RETARGET_SerialFlush(void)
{
#if defined(RETARGET_USART)

#if defined(USART_STATUS_TXIDLE)
#define _GENERIC_UART_STATUS_IDLE     USART_STATUS_TXIDLE
#else
#define _GENERIC_UART_STATUS_IDLE     USART_STATUS_TXC
#endif

#else

#if defined(LEUART_STATUS_TXIDLE)
#define _GENERIC_UART_STATUS_IDLE     LEUART_STATUS_TXIDLE
#else
#define _GENERIC_UART_STATUS_IDLE     LEUART_STATUS_TXC
#endif

#endif

  while (!(RETARGET_UART->STATUS & _GENERIC_UART_STATUS_IDLE)) ;
}

/** @} (end group RetargetIo) */
/** @} (end group kitdrv) */
