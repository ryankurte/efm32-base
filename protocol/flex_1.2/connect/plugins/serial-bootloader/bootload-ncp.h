// ****************************************************************************
// * bootload-ncp.h
// *
// * SPI/UART Interface to bootloading the NCP from a connected host.
// *
// * Copyright 2015 Silicon Laboratories, Inc.
// *****************************************************************************

typedef enum {
  SHUTDOWN_NOT_REQUESTED, // this is the normal state when the app is running
  SHUTDOWN_REQUESTED,     // after Bootloader has been launched
  SHUTDOWN_DONE,          // after NCP connection has been closed
} ShutDownState;

bool emberBootloadImage(const char* filepath, uint32_t offset, uint32_t length);
bool emStartNcpBootloaderCommunications(void);
void emPostNcpBootload(bool success);
bool emRebootNcpAfterBootload(void);

// These primitives are called by the Xmodem code to pass data via
// the specific mechanism (UART or SPI).
bool emBootloadSendData(const uint8_t *data, uint16_t length);
bool emBootloadSendByte(uint8_t byte);
bool emBootloadWaitChar(uint8_t *data, bool expect, uint8_t expected);
