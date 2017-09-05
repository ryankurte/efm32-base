// File: uart-link-protocol.h
//
// Description: serial link protocol values, used over both SPI and UART links
//
// Copyright 2013 by Silicon Laboratories. All rights reserved.             *80*

//  Each message has a four-byte header:
//   '[' type high-length-byte low-length-byte
//
// (With only three types and a two-byte length we could encode the type in
// the high byte of the length.)

#define UART_LINK_HEADER_SIZE 4

// Non Management is not used for now.
// This is so we can easily add more functionality in the future.
typedef enum {
  UART_LINK_TYPE_MANAGEMENT        = 1,
  UART_LINK_TYPE_NON_MANAGEMENT    = 2,
} SerialLinkMessageType; // use 'serial' in new names, for both UART and SPI

#define MAX_UART_LINK_TYPE UART_LINK_TYPE_NON_MANAGEMENT
#define EMBER_APPLICATION_HAS_NOTIFY_SERIAL_RX
#define EMBER_APPLICATION_HAS_BINARY_COMMAND_INTERPRETER
