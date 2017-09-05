// File: binary-management.c
//
// Description: Functions for sending binary management commands between the
// host and NCP.
//
// Copyright 2015 by Silicon Laboratories. All rights reserved.             *80*

#include PLATFORM_HEADER
#include "stack/include/ember.h"
#include "hal/hal.h"
#include "uart-link-protocol.h"
#include "host-ncp/ncp-link.h"
#include "host-ncp/binary-management.h"
#include "command-interpreter/command-interpreter2.h"
#include "command-interpreter/command-interpreter2-util.h"
#include "host-ncp/serial-link.h"
#include "csp/csp-enum.h"
#include "stack/framework/event-queue.h"
#include "serial/serial.h"

#ifdef EMBER_TEST
#define CSP_RESPONSE_TIMEOUT 20000
#else
#define CSP_RESPONSE_TIMEOUT 2000
#endif

extern EventQueue emApiAppEventQueue;
bool waitingForResponse;

static uint16_t formatBinaryManagementCommand(uint8_t *buffer,
                                              uint16_t bufferSize,
                                              uint16_t identifier,
                                              const char *format,
                                              va_list argumentList)
{
  // Use memset instead of "uint8_t buffer[256] = {0};" to avoid the spurious
  // IAR C8051 compiler Warning[ta010]: Write to a __constptr pointer
  MEMSET(buffer, 0, bufferSize);

  uint8_t *finger = buffer;

  // store the identifier
  emberStoreHighLowInt16u(finger, identifier);
  finger += sizeof(uint16_t);

  const char *formatFinger;

  // We use primitive types to retrieve va_args because gcc on some
  // platforms (eg., unix hosts) complains about automatic variable
  // promotion in variadic functions.
  // [eg: warning: 'uint8_t' is promoted to 'int' when passed through '...']
  //
  // An exception is made for the uint32_t case:  Here we need to check the size
  // of unsigned int, which can be smaller on platforms such as the c8051.

  for (formatFinger = (char *) format; *formatFinger != 0; formatFinger++) {
    switch (*formatFinger) {
      case 'u':
        *finger++ = va_arg(argumentList, unsigned int);
        break;
      case 's':
        *finger++ = va_arg(argumentList, int);
        break;
      case 'v':
        emberStoreHighLowInt16u(finger, va_arg(argumentList, unsigned int));
        finger += sizeof(uint16_t);
        break;
      case 'w':
        if (sizeof(unsigned int) < sizeof(uint32_t)) {
          emberStoreHighLowInt32u(finger, va_arg(argumentList, uint32_t));
        } else {
          emberStoreHighLowInt32u(finger, va_arg(argumentList, unsigned int));
        }
        finger += sizeof(uint32_t);
        break;
      case 'b': {
        const uint8_t *data = va_arg(argumentList, const uint8_t*);
        // store the size, must fit into a byte
        uint8_t dataSize = va_arg(argumentList, int);
        *finger++ = dataSize;

        if (dataSize > 0) {
          // Checking for NULL here save's every caller from checking.  We assume
          // the if dataSize is not zero then we should send all zeroes.
          if (data != NULL) {
            MEMCOPY(finger, data, dataSize);
          } else {
            MEMSET(finger, 0, dataSize);
          }
        }

        finger += dataSize;
        break;
      }
      default:
        // confused!
        assert(false);
        break;
    }
  }

  uint16_t length = finger - buffer;
  // sanity check
  assert(length <= bufferSize);

  return length;
}
uint16_t emFormatBinaryManagementCommand(uint8_t *buffer,
                                         uint16_t bufferSize,
                                         uint16_t identifier,
                                         const char *format,
                                         ...)
{
  va_list argumentList;
  va_start(argumentList, format);
  uint16_t length = formatBinaryManagementCommand(buffer,
                                                  bufferSize,
                                                  identifier,
                                                  format,
                                                  argumentList);
  va_end(argumentList);
  return length;
}

void emSendBinaryManagementCommand(uint16_t identifier,
                                   const char *format,
                                   ...)
{
  va_list argumentList;
  va_start(argumentList, format);
  // 5 is the size of the management header
  uint8_t buffer[EMBER_COMMAND_BUFFER_LENGTH + 5];

  // keep the header at four bytes until the header size is configurable
  buffer[0] = '[';
  buffer[1] = UART_LINK_TYPE_MANAGEMENT;
  // bytes 2 and 3, the management packet length, are set below
  buffer[4] = MANAGEMENT_COMMAND;

  uint16_t length = formatBinaryManagementCommand(buffer + 5,
                                                  sizeof(buffer) - 5,
                                                  identifier,
                                                  format,
                                                  argumentList);
  va_end(argumentList);

  // Length starts after the fourth byte
  emberStoreHighLowInt16u(buffer + 2, length + 1);
  emReallySendManagementCommand(buffer, length + 5);
}

#if defined(UNIX_HOST) && !defined(EMBER_ASH_V3_TEST_APP)
void halHostSerialTick(void);
void initializeCspBuffers(void);

void emSendBinaryManagementBlockingCommand(uint16_t identifier,
                                           const char *format,
                                           ...)
{
  waitingForResponse = true;
  initializeCspBuffers();

  va_list argumentList;
  va_start(argumentList, format);
  // 5 is the size of the management header
  uint8_t buffer[EMBER_COMMAND_BUFFER_LENGTH + 5];

  // keep the header at four bytes until the header size is configurable
  buffer[0] = '[';
  buffer[1] = UART_LINK_TYPE_MANAGEMENT;
  // bytes 2 and 3, the management packet length, are set below
  buffer[4] = MANAGEMENT_COMMAND;

  uint16_t length = formatBinaryManagementCommand(buffer + 5,
                                                  sizeof(buffer) - 5,
                                                  identifier,
                                                  format,
                                                  argumentList);
  va_end(argumentList);

  // Length starts after the fourth byte
  emberStoreHighLowInt16u(buffer + 2, length + 1);
  emReallySendManagementCommand(buffer, length + 5);

  int16u start = halCommonGetInt16uMillisecondTick();
  int16u now;
  while (true) {
    now = halCommonGetInt16uMillisecondTick();
    if (elapsedTimeInt16u(start, now) > CSP_RESPONSE_TIMEOUT) {
      emberSerialPrintfLine(APP_SERIAL,
                            "CSP command timed out: %s\n",
                            emGetBinaryCommandIdentifierString(identifier));
      waitingForResponse = false;
      break;
    } else if (waitingForResponse) {
      halHostSerialTick();
      emberRunEventQueue(&emApiAppEventQueue);
    } else {
      break;
    }
    #ifdef EMBER_TEST
    simulatedTimePasses();
    #endif
  }
}

#include "csp/csp-enum-decode.c"

const char *emGetBinaryCommandIdentifierString(uint16_t identifier)
{
  return decodeFrameId(identifier);
}

#endif
