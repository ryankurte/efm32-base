// File: host-stream.c
//
// Description: host message streams
//
// Copyright 2013 by Silicon Laboratories. All rights reserved.             *80*

// Struct for buffering partial messages.
typedef struct {
  uint8_t buffer[4096];
  uint16_t index;
} Stream;

typedef enum {
  NCP_READ_DONE,          // There is no input pending.
  NCP_READ_PROGRESS,      // A command executed but there is input pending.
  NCP_READ_PENDING,       // There is input pending.
  NCP_READ_FORMAT_ERROR,
  NCP_READ_IO_ERROR,
} ncpReadStatus;

//----------------------------------------------------------------
// Reading NCP messages.

// 'Type' is one of the UART_LINK_TYPE_... values.  'message' points
// to the first byte of message data, and 'length' is the number of
// message bytes.

typedef void (ncpMsgHandler)(SerialLinkMessageType type,
                             const uint8_t *message,
                             uint16_t length);

// Process NCP messages from 'stream', passing any complete messages
// to 'handler'.

ncpReadStatus processNcpInput(Stream *stream,
                              ncpMsgHandler *handler);

void emRemoveStreamBytes(Stream *stream, int count);
