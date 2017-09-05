#ifndef UART_CONFIG_H
#define UART_CONFIG_H
#include <stddef.h>
#include "em_usart.h"

#define WSTK_VCOM 0

static const char *tx_pin = "PA0";
static const char *rx_pin = "PA1";
static const char *cts_pin = NULL;
static const char *rts_pin = NULL;

static const USART_InitAsync_TypeDef uartInit =
{
  .enable       = usartDisable,     // Wait to enable the transmitter and receiver
  .refFreq      = 38000000,         // 38MHz HFRCO
  .baudrate     = 115200,           // Desired baud rate
  .databits     = usartDatabits8,   // 8 data bits
  .mvdis        = false,            // Use majority voting
  .prsRxEnable  = false,            // Not using PRS input
  .prsRxCh      = usartPrsRxCh0,    // Doesn't matter which channel we select
  .oversampling = usartOVS16,       // Set oversampling value to x16
  .parity       = usartNoParity,    // No parity
  .stopbits     = usartStopbits1,   // 1 stop bit
};

#endif
