// File: host-driver.c
//
// Description: host-side driver for the IP NCP.
//
// Copyright 2013 by Silicon Laboratories. All rights reserved.             *80*

// Handle messages received from the NCP.

void ncpIncomingMessageHandler(SerialLinkMessageType type,
                               const uint8_t *message,
                               uint16_t length);

// Send messages to the NCP or management client.  This callback is
// implemented in host-driver-app.c.

extern bool logEnabled;
extern int ncpFileDescriptor;

uint16_t ncpWrite(int fd,
                  SerialLinkMessageType type,
                  const uint8_t *buffer,
                  uint16_t length);

extern Stream ncpStream;

void managementHandler(SerialLinkMessageType type,
                       const uint8_t *message,
                       uint16_t length);

void ncpMessageHandler(SerialLinkMessageType type,
                       const uint8_t *message,
                       uint16_t length);

ncpReadStatus readNcpAshInput
  (int fd, ncpMsgHandler messageHandler);

void txBufferFullHandler(const uint8_t *packet,
                         uint16_t packetLength,
                         uint16_t written);

void txFailedHandler(uint8_t fd,
                     const uint8_t *packet,
                     uint16_t packetLength,
                     uint16_t written);

void halHostSerialTick(void);
#ifdef EMBER_TEST
void halHostSerialInit(void);
#else
void halHostSerialInit(int argc, char **argv);
#endif
