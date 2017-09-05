/**
 * File: command-interpreter2-error.c
 * Description: processes commands incoming over the serial port.
 *
 * Culprit(s): Richard Kelsey, Matteo Paris
 *
 * Copyright 2008 by Ember Corporation.  All rights reserved.               *80*
 */

#include PLATFORM_HEADER

#ifdef EZSP_HOST
// Includes needed for ember related functions for the EZSP host
  #include "stack/include/error.h"
  #include "stack/include/ember-types.h"
  #include "ezsp/ezsp-protocol.h"
  #include "ezsp/ezsp.h"
  #include "ezsp/serial-interface.h"
extern uint8_t emberEndpointCount;
#else
  #include "stack/include/ember.h"
#endif

#include "hal/hal.h"
#include "serial/serial.h"
#include "command-interpreter/command-interpreter2.h"
#include "command-interpreter/command-interpreter2-util.h"

#if !defined APP_SERIAL
extern uint8_t serialPort;
  #define APP_SERIAL serialPort
#endif

const char * const emberCommandErrorNames[] =
{
  "",
  "Serial port error",
  "No such command",
  "Wrong number of args",
  "Arg out of range",
  "Arg syntax error",
  "Too long",
  "Bad arg type"
};

static void printCommandUsage(EmberCommandEntry *entry)
{
  const char * arg = entry->argumentTypes;
  emberSerialPrintf(APP_SERIAL, "%p", entry->name);

  if (entry->action == NULL) {
    emberSerialPrintf(APP_SERIAL, "...");
  } else {
    while (*arg) {
      uint8_t c = *arg;
      emberSerialPrintf(APP_SERIAL,
                        (c == 'u' ? " <uint8_t>"
                         : c == 'v' ? " <uint16_t>"
                         : c == 'w' ? " <uint32_t>"
                         : c == 's' ? " <int8_t>"
                         : c == 'b' ? " <string>"
                         : c == 'n' ? " ..."
                         : c == '*' ? " *"
                         : " ?"));
      arg += 1;
    }
  }

  if (entry->description) {
    emberSerialPrintf(APP_SERIAL, " - %p", entry->description);
  }

  emberSerialPrintf(APP_SERIAL, "\r\n");
}

void emberPrintCommandUsage(EmberCommandEntry *entry)
{
  EmberCommandEntry *commandFinger;
  printCommandUsage(entry);

  if (emberGetNestedCommand(entry, &commandFinger)) {
    for (; commandFinger->name != NULL; commandFinger++) {
      emberSerialPrintf(APP_SERIAL, "  ");
      printCommandUsage(commandFinger);
    }
  }
}

void emberPrintCommandUsageNotes(void)
{
  emberSerialPrintf(APP_SERIAL,
                    "Usage notes:\r\n\r\n"
#ifndef C8051_COBRA
                    "type      description\r\n"
                    "<uint8_t>   8-bit unsigned int, eg: 255, 0xAB\r\n"
                    "<int8_t>   8-bit signed int, eg: -128, 0xA9\r\n"
                    "<uint16_t>  16-bit unsigned int, eg: 3000 0xFFAA\r\n"
                    "<string>  A string, eg: \"foo\" or {0A 1B 2C}\r\n"
                    "*         Zero or more of the previous type\r\n\r\n"
#endif // ! C8051_COBRA
                    );
  emberSerialWaitSend(APP_SERIAL);
}

void emberPrintCommandTable(void)
{
  EmberCommandEntry *commandFinger = COMMAND_TABLE;
  emberPrintCommandUsageNotes();
  for (; commandFinger->name != NULL; commandFinger++) {
    printCommandUsage(commandFinger);
  }
}

void emberCommandErrorHandler(EmberCommandStatus status,
                              EmberCommandEntry *command)
{
  emberSerialPrintf(APP_SERIAL, "Error: %p\r\n", emberCommandErrorNames[status]);

  if (command == NULL) {
    emberPrintCommandTable();
  } else {
    emberSerialPrintf(APP_SERIAL, "Usage: ");
    emberPrintCommandUsage(command);
  }
}
