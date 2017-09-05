// File: simple-linux-serial.c
//
// Description: Simple implementation of the Ember serial functions for PCs.
//
// Copyright 2010 by Ember Corporation. All rights reserved.                *80*

#include PLATFORM_HEADER
#include "stack/include/ember-types.h"
#include "hal/hal.h"
#include "stack/include/error.h"
#include "serial/serial.h"
#include <unistd.h>
#include <poll.h>

EmberStatus emberSerialInit(uint8_t port,
                            SerialBaudRate rate,
                            SerialParity parity,
                            uint8_t stopBits)
{
  return EMBER_SUCCESS;
}

EmberStatus emberSerialReadByte(uint8_t port, uint8_t *dataByte)
{
  struct pollfd fds = { 0, POLLIN, 0 };
  if (poll(&fds, 1, 0) == 1) {
    assert(read(0, dataByte, 1) == 1);
    return EMBER_SUCCESS;
  } else {
    return EMBER_SERIAL_RX_EMPTY;
  }
}

EmberStatus emberSerialWaitSend(uint8_t port)
{
  return EMBER_SUCCESS;
}

EmberStatus emberSerialPrintfLine(uint8_t port, PGM_P formatString, ...)
{
  EmberStatus stat;
  va_list ap;
  va_start(ap, formatString);
  stat = emberSerialPrintfVarArg(port, formatString, ap);
  va_end(ap);
  emberSerialPrintf(port, "\r\n");
  return stat;
}

EmberStatus emberSerialPrintf(uint8_t port, PGM_P formatString, ...)
{
  EmberStatus stat;
  va_list ap;
  va_start(ap, formatString);
  stat = emberSerialPrintfVarArg(port, formatString, ap);
  va_end(ap);
  return stat;
}

EmberStatus emberSerialGuaranteedPrintf(uint8_t port, PGM_P formatString, ...)
{
  EmberStatus stat;
  va_list ap;
  va_start(ap, formatString);
  stat = emberSerialPrintfVarArg(port, formatString, ap);
  va_end(ap);
  return stat;
}

void emberSerialBufferTick(void)
{
}

EmberStatus emberSerialWriteByte(uint8_t port, uint8_t byte)
{
  return emberSerialPrintf(port, "%c", byte);
}

EmberStatus emberSerialPrintCarriageReturn(uint8_t port)
{
  return emberSerialPrintf(port, "\r\n");
}

EmberStatus emberSerialPrintfVarArg(uint8_t port, PGM_P string, va_list args)
{
  for (; *string != 0; string++) {
    uint8_t next = *string;
    if (next != '%') {
      putchar(next);
    } else {
      string += 1;
      switch (*string) {
        case '%':
          putchar('%');
          break;
        case 'c':
          putchar(va_arg(args, unsigned int) & 0xFF);
          break;
        case 'p':
        case 's':
          fputs(va_arg(args, char *), stdout);
          break;
        case 'l':
          if (string[1] == 'u') {
            printf("%lu", va_arg(args, unsigned long int));
            string++;
          } else {
            printf("%ld", va_arg(args, long int));
            if (string[1] == 'd') {
              string++;
            }
          }
          break;
        case 'u':
          printf("%u", va_arg(args, int));
          break;
        case 'd':
          printf("%d", va_arg(args, int));
          break;
        case 'x':
        case 'X':
          printf("%02X", va_arg(args, int));
          break;
        case '2':
        case '4':
          if (string[1] == 'x' || string[1] == 'X') {
            if (*string == '2') {
              printf("%04X", va_arg(args, int));
            } else {
              printf("%08X", va_arg(args, uint32_t));
            }
            string++;
          }
          break;
      }
    }
  }
  fflush(stdout);
  return 0;
}

EmberStatus emberSerialWriteData(uint8_t port, uint8_t *data, uint8_t length)
{
  while (length--) {
    putchar(*data);
    data++;
  }
  return EMBER_SUCCESS;
}

EmberStatus emberSerialWriteString(uint8_t port, PGM_P string)
{
  while (*string != '\0') {
    putchar(*string);
    string++;
  }
  return EMBER_SUCCESS;
}

uint8_t *emWriteHexInternal(uint8_t *charBuffer, uint16_t value, uint8_t charCount)
{
  uint8_t c = charCount;
  charBuffer += charCount;
  for (; c; c--) {
    uint8_t n = value & 0x0F;
    value = value >> 4;
    *(--charBuffer) = n + (n < 10
                           ? '0'
                           : 'A' - 10);
  }
  return charBuffer + charCount;
}
