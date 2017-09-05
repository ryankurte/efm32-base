/**
 * File: command-interpreter2b.c
 * Description: Binary command interpreter2
 *
 * Copyright 2013 by Silicon Corporation.  All rights reserved.             *80*
 */

#include PLATFORM_HEADER

#include "stack/include/ember.h"
#include "hal/hal.h"
#include "serial/serial.h"
#include "host-ncp/binary-management.h"

#define EMBER_APPLICATION_HAS_BINARY_COMMAND_INTERPRETER

#include "command-interpreter/command-interpreter2.h"
#include "command-interpreter/command-interpreter2-util.h"

static const EmberCommandEntry *getCommand(uint16_t identifier)
{
  const EmberCommandEntry *commandFinger = COMMAND_TABLE;

  while (commandFinger != NULL && commandFinger->name != NULL) {
    if (commandFinger->identifier == identifier) {
      break;
    }

    commandFinger++;
  }

  return (commandFinger->name == NULL
          ? NULL
          : commandFinger);
}

bool emberProcessBinaryCommand(const uint8_t *input, uint16_t inputSize)
{
  emCommandReaderReinit();
  // A base of 0xFF is used internally to signify that we're processing a binary
  // command.
  emCommandState.defaultBase = 0xFF;
  assert(inputSize <= EMBER_COMMAND_BUFFER_LENGTH);
  MEMCOPY(emCommandState.buffer, input, inputSize);

  emCommandState.error = EMBER_CMD_SUCCESS;
  const uint8_t *finger = input;
  uint16_t identifier = emberFetchHighLowInt16u(finger);
  finger += 2;

  // help the debugger
  const EmberCommandEntry *commandEntry = getCommand(identifier);

#ifdef EMBER_HOST
  emLogLine(IP_MODEM,
            "Management Command NCP->Host [%s]",
            emGetBinaryCommandIdentifierString(identifier));
#endif

  if (commandEntry == NULL) {
    // couldn't find a command, error
    emCommandState.error = EMBER_CMD_ERR_NO_SUCH_COMMAND;
  } else {
    // we found a command, try to parse the arguments
    const uint8_t *argumentFinger = (PGM_PU) commandEntry->argumentTypes;
    uint8_t tokenCount;

    for (argumentFinger = (PGM_PU) commandEntry->argumentTypes, tokenCount = 0;
         *argumentFinger != 0;
         argumentFinger++, tokenCount++) {
      assert(finger - input < inputSize);

      if (tokenCount == MAX_TOKEN_COUNT) {
        emCommandState.error = EMBER_CMD_ERR_WRONG_NUMBER_OF_ARGUMENTS;
        break;
      }

      emCommandState.tokenIndices[emCommandState.tokenCount] =
        (finger - input);
      emCommandState.tokenCount++;

      switch (*argumentFinger) {
        case 'u':
          finger += sizeof(uint8_t);
          break;
        case 's':
          finger += sizeof(int8_t);
          break;
        case 'v':
          finger += sizeof(uint16_t);
          break;
        case 'w':
          finger += sizeof(uint32_t);
          break;
        case 'b':
          // increment by the size at finger
          finger += *finger + 1;
          break;
        default:
          emCommandState.error = EMBER_CMD_ERR_ARGUMENT_SYNTAX_ERROR;
          break;
          // error
      }

      if ((finger - input) > inputSize) {
        // error
        emCommandState.error = EMBER_CMD_ERR_ARGUMENT_SYNTAX_ERROR;
        break;
      }
    }

    if (emCommandState.error == EMBER_CMD_SUCCESS) {
      emCommandState.tokenIndices[emCommandState.tokenCount] =
        finger - input;
      emCommandState.tokenCount++;
      commandEntry->action();
    } else {
      ERROR_HANDLER(emCommandState.error, NULL);
    }
  }

  return (emCommandState.error == EMBER_CMD_SUCCESS);
}
