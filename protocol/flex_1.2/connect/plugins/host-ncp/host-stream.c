// File: host-stream.c
//
// Description: host message streams
//
// Copyright 2013 by Silicon Laboratories. All rights reserved.             *80*

#include <stdlib.h>
#include <unistd.h>

#include PLATFORM_HEADER
#include "stack/include/ember.h"
#include "hal/hal.h"
#include "uart-link-protocol.h"
#include "host-stream.h"

void emRemoveStreamBytes(Stream *stream, int count)
{
  assert(count <= stream->index);
  memmove(stream->buffer, stream->buffer + count, stream->index - count);
  stream->index -= count;

  // help the debugger
  MEMSET(stream->buffer + stream->index,
         0,
         sizeof(stream->buffer) - stream->index);
}

ncpReadStatus processNcpInput(Stream *stream,
                              ncpMsgHandler *handler)
{
  ncpReadStatus result = NCP_READ_PENDING;
  uint16_t length = emberFetchHighLowInt16u(stream->buffer + 2);
  uint8_t messageType = stream->buffer[1];

  if (stream->buffer[0] != '['
      || MAX_UART_LINK_TYPE < messageType
      || length == 0) {
    // We get here if there is a framing error
    return NCP_READ_FORMAT_ERROR;
  }

  if (UART_LINK_HEADER_SIZE + length <= stream->index) {
    handler(messageType, stream->buffer + UART_LINK_HEADER_SIZE, length);
    emRemoveStreamBytes(stream, length + UART_LINK_HEADER_SIZE);
    result = NCP_READ_PROGRESS;
  }

  return (stream->index == 0
          ? NCP_READ_DONE
          : result);
}
