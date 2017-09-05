// File: ncp-uart-interface.h
//
// Description: NCP UART interface
//
// Copyright 2013 by Silicon Laboratories. All rights reserved.             *80*

extern bool ncpUartUseAsh;

void emOpenNcpUart(const char *uartDevice);
void emCheckNcpUartInput(fd_set *input);
void emTestNcpReadStatusResult(ncpReadStatus result, Stream *stream);
void emProcessNcpManagementCommand(SerialLinkMessageType type,
                                   const uint8_t *message,
                                   uint16_t length);

void openNcpReset(void);
void resetNcp(void);
void enableResetNcp(uint8_t enable);

// Define the raspberry pi gpio used to reset the NCP
// pin 23, asserted low, deasserted tri-stated
#define EMBER_GPIO_PATH "/sys/class/gpio/"
#define EMBER_NCP_RESET_GPIO "23"
#define EMBER_NCP_RESET_ASSERT_LOW
// Define EMBER_NCP_RESET_DEASSERT_TRISTATE if ncp reset signal is wire-ORed
#define EMBER_NCP_RESET_DEASSERT_TRISTATED
// Define EMBER_NCP_RESET_REQUIRE_ENABLE for debugging to inhibit ncp resets
// until enabled by a "enable_reset_ncp_gpio 1" command
#define EMBER_NCP_RESET_REQUIRE_ENABLE
