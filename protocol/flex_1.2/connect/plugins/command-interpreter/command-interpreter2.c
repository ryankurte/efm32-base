/**
 * File: command-interpreter2.c
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

#if defined(EMBER_REQUIRE_FULL_COMMAND_NAME) \
  || defined(EMBER_REQUIRE_EXACT_COMMAND_NAME)
  #undef EMBER_REQUIRE_EXACT_COMMAND_NAME
  #define EMBER_REQUIRE_EXACT_COMMAND_NAME true
#else
  #define EMBER_REQUIRE_EXACT_COMMAND_NAME false
#endif

// forward declarations
static void callCommandAction(void);
static uint8_t charDowncase(uint8_t c);

// This byte is used to toggle certain internal features on or off.
// By default all are off.
uint8_t emberCommandInterpreter2Configuration = 0x00;

// Tick callback
void emberAfPluginCommandInterpreterTickCallback(void)
{
  if (emberProcessCommandInput(APP_SERIAL)) {
    emberSerialPrintf(APP_SERIAL, "%p>", EMBER_AF_DEVICE_NAME);
  }

#ifdef EMBER_AF_PLUGIN_FREE_RTOS
#if 0
  // Put in a pause here since we don't need to poll the serial port all
  // the time. This will allow us to idle and save power.
  hostAppIdle(100);
#endif
#endif
}

//----------------------------------------------------------------

// Some users of command-interpreter2 need the command buffer to be set to 0
// so the command arg is NULL terminated when a pointer is returned.
// It might be better to always zero out the buffer when we reset
// commandState.state to CMD_AWAITING_ARGUMENT, but I don't want to break any
// other existing apps, so I'm letting the app decide if it wants to zero out
// the buffer.
void emberCommandClearBuffer(void)
{
  MEMSET(emCommandState.buffer, 0, EMBER_COMMAND_BUFFER_LENGTH);
}

//----------------------------------------------------------------
// This is a state machine for parsing commands.  If 'input' is NULL
// 'sizeOrPort' is treated as a port and characters are read from there.
//
// Goto's are used where one parse state naturally falls into another,
// and to save flash.

bool emberProcessCommandString(const uint8_t *input, uint16_t sizeOrPort)
{
  bool isEol = false;
  bool isSpace, isQuote;

  while (true) {
    uint8_t next;

    if (input == NULL) {
      switch (emberSerialReadByte(sizeOrPort, &next)) {
        case EMBER_SUCCESS:
          break;
        case EMBER_SERIAL_RX_EMPTY:
          return isEol;
        default:
          emCommandState.error = EMBER_CMD_ERR_PORT_PROBLEM;
          goto READING_TO_EOL;
      }
    } else if (sizeOrPort == 0) {
      return isEol;
    } else {
      next = *input;
      input += 1;
      sizeOrPort -= 1;
    }

    if (emCommandState.previousCharacter == '\r' && next == '\n') {
      emCommandState.previousCharacter = next;
      continue;
    }
    emCommandState.previousCharacter = next;
    isEol = ((next == '\r') || (next == '\n'));
    isSpace = (next == ' ');
    isQuote = (next == '"');

    // fprintf(stderr, "[processing '%c' (%s)]\n", next, stateNames[emCommandState.state]);

    switch (emCommandState.state) {
      case CMD_AWAITING_ARGUMENT:
        if (isEol) {
          goto CALL_COMMAND_ACTION;
        } else if (!isSpace) {
          if (isQuote) {
            emCommandState.state = CMD_READING_STRING;
          } else if (next == '{') {
            emCommandState.state = CMD_READING_HEX_STRING;
          } else {
            emCommandState.state = CMD_READING_ARGUMENT;
          }
          goto WRITE_TO_BUFFER;
        }
        break;

      case CMD_READING_ARGUMENT:
        if (isEol || isSpace) {
          goto END_ARGUMENT;
        } else {
          goto WRITE_TO_BUFFER;
        }

      case CMD_READING_STRING:
        if (isQuote) {
          next = 0;
        } else if (isEol) {
          emCommandState.error = EMBER_CMD_ERR_ARGUMENT_SYNTAX_ERROR;
          goto READING_TO_EOL;
        }
        goto WRITE_TO_BUFFER;

      case CMD_READING_HEX_STRING: {
        bool waitingForLowNibble = (emCommandState.hexHighNibble != 0xFF);
        if (next == '}') {
          if (waitingForLowNibble) {
            emCommandState.error = EMBER_CMD_ERR_ARGUMENT_SYNTAX_ERROR;
            goto READING_TO_EOL;
          }
          goto END_ARGUMENT;
        } else {
          uint8_t value = emberHexToInt(next);
          if (value < 16) {
            if (waitingForLowNibble) {
              next = (emCommandState.hexHighNibble << 4) + value;
              emCommandState.hexHighNibble = 0xFF;
              goto WRITE_TO_BUFFER;
            } else {
              emCommandState.hexHighNibble = value;
            }
          } else if (!isSpace) {
            emCommandState.error = EMBER_CMD_ERR_ARGUMENT_SYNTAX_ERROR;
            goto READING_TO_EOL;
          }
        }
        break;
      }

        CALL_COMMAND_ACTION:
        callCommandAction();
        // fall through to READING_TO_EOL for error chack and reinitialization

        READING_TO_EOL:
        emCommandState.state = CMD_READING_TO_EOL;

      case CMD_READING_TO_EOL:
        if (isEol) {
          if (emCommandState.error != EMBER_CMD_SUCCESS) {
            ERROR_HANDLER(emCommandState.error, NULL);
          }
          emCommandReaderReinit();
          emCommandState.previousCharacter = next;
        }
        break;

        END_ARGUMENT:
        if (emCommandState.tokenCount == MAX_TOKEN_COUNT) {
          emCommandState.error = EMBER_CMD_ERR_WRONG_NUMBER_OF_ARGUMENTS;

          goto READING_TO_EOL;
        }
        emCommandState.tokenCount += 1;
        emCommandState.tokenIndices[emCommandState.tokenCount] = emCommandState.index;
        emCommandState.state = CMD_AWAITING_ARGUMENT;
        if (isEol) {
          goto CALL_COMMAND_ACTION;
        }
        break;

        WRITE_TO_BUFFER:
        if (emCommandState.index == EMBER_COMMAND_BUFFER_LENGTH) {
          emCommandState.error = EMBER_CMD_ERR_STRING_TOO_LONG;
          goto READING_TO_EOL;
        }
        if (emCommandState.state == CMD_READING_ARGUMENT) {
          next = charDowncase(next);
        }
        emCommandState.buffer[emCommandState.index] = next;
        emCommandState.index += 1;
        if (emCommandState.state == CMD_READING_STRING && next == 0) {
          goto END_ARGUMENT;
        }
        break;

      default: {
      }
    } //close switch.
  }
}

static uint8_t charDowncase(uint8_t c)
{
  if ('A' <= c && c <= 'Z') {
    return c + 'a' - 'A';
  } else {
    return c;
  }
}

// To support existing lazy-typer functionality in the app framework,
// we allow the user to shorten the entered command so long as the
// substring matches no more than one command in the table.
//
// To allow CONST savings by storing abbreviated command names, we also
// allow matching if the input command is longer than the stored command.
// To reduce complexity, we do not handle multiple inexact matches.
// For example, if there are commands 'A' and 'AB', and the user enters
// 'ABC', nothing will match.

static EmberCommandEntry *commandLookup(EmberCommandEntry *commandFinger,
                                        uint8_t tokenNum)
{
  EmberCommandEntry *inexactMatch = NULL;
  uint8_t *inputCommand = emTokenPointer(tokenNum);
  uint8_t inputLength = emTokenLength(tokenNum);
  bool multipleMatches = false;

  for (; commandFinger->name != NULL; commandFinger++) {
    PGM_P entryFinger = commandFinger->name;
    uint8_t *inputFinger = inputCommand;
    for (;; entryFinger++, inputFinger++) {
      bool endInput = (inputFinger - inputCommand == inputLength);
      bool endEntry = (*entryFinger == 0);
      if (endInput && endEntry) {
        return commandFinger;  // Exact match.
      } else if (endInput || endEntry) {
        if (inexactMatch != NULL) {
          multipleMatches = true;  // Multiple matches.
          break;
        } else {
          inexactMatch = commandFinger;
          break;
        }
      } else if (charDowncase(*inputFinger) != charDowncase(*entryFinger)) {
        break;
      }
    }
  }
  return ((multipleMatches || EMBER_REQUIRE_EXACT_COMMAND_NAME)
          ? NULL
          : inexactMatch);
}

static void callCommandAction(void)
{
  EmberCommandEntry *commandFinger = COMMAND_TABLE;
  uint8_t tokenNum = 0;
  // We need a separate argTypeNum index because of the '*' arg type.
  uint8_t argTypeNum, argNum;

  if (emCommandState.tokenCount == 0) {
    return;
  }

  // Lookup the command.
  while (true) {
    commandFinger = commandLookup(commandFinger, tokenNum);
    if (commandFinger == NULL) {
      emCommandState.error = EMBER_CMD_ERR_NO_SUCH_COMMAND;
      goto kickout;
    } else {
      emCommandState.currentCommand = commandFinger;
      tokenNum += 1;
      emCommandState.argOffset += 1;

      if (emberGetNestedCommand(commandFinger, &commandFinger)) {
        if (tokenNum >= emCommandState.tokenCount) {
          emCommandState.error = EMBER_CMD_ERR_WRONG_NUMBER_OF_ARGUMENTS;
          goto kickout;
        }
      } else {
        break;
      }
    }
  }

  // If you put '?' as the first character
  // of the argument format string, then you effectivelly
  // prevent the argument validation, and the command gets executed.
  // At that point it is down to the command to deal with whatever
  // arguments it got.
  if ( commandFinger->argumentTypes[0] == '?' ) {
    goto kickout;
  }

  // Validate the arguments.
  for (argTypeNum = 0, argNum = 0;
       tokenNum < emCommandState.tokenCount;
       tokenNum++, argNum++) {
    uint8_t type = commandFinger->argumentTypes[argTypeNum];
    uint8_t firstChar = emFirstByteOfArg(argNum);
    switch (type) {
      // Integers
      case 'u':
      case 'v':
      case 'w':
      case 's': {
        uint32_t limit = (type == 'u' ? 0xFF
                          : (type == 'v' ? 0xFFFF
                             : (type == 's' ? 0x7F : 0xFFFFFFFFUL)));
        if (emStringToUnsignedInt(argNum, true) > limit) {
          emCommandState.error = EMBER_CMD_ERR_ARGUMENT_OUT_OF_RANGE;
        }
        break;
      }

      // String
      case 'b':
        if (firstChar != '"' && firstChar != '{') {
          emCommandState.error = EMBER_CMD_ERR_ARGUMENT_SYNTAX_ERROR;
        }
        break;

      case 0:
        emCommandState.error = EMBER_CMD_ERR_WRONG_NUMBER_OF_ARGUMENTS;
        break;

      default:
        emCommandState.error = EMBER_CMD_ERR_INVALID_ARGUMENT_TYPE;
        break;
    }

    if (commandFinger->argumentTypes[argTypeNum + 1] != '*') {
      argTypeNum += 1;
    }

    if (emCommandState.error != EMBER_CMD_SUCCESS) {
      goto kickout;
    }
  }

  if (!(commandFinger->argumentTypes[argTypeNum] == 0
        || commandFinger->argumentTypes[argTypeNum + 1] == '*')) {
    emCommandState.error = EMBER_CMD_ERR_WRONG_NUMBER_OF_ARGUMENTS;
  }

  kickout:

  if (emCommandState.error == EMBER_CMD_SUCCESS) {
    (commandFinger->action)();
  }
}

int8u emberCopyStringArgument(int8s argNum,
                              int8u *destination,
                              int8u maxLength,
                              boolean leftPad)
{
  int8u padLength;
  int8u argLength;
  int8u *contents = emberStringCommandArgument(argNum, &argLength);
  if (argLength > maxLength) {
    argLength = maxLength;
  }
  padLength = leftPad ? maxLength - argLength : 0;
  MEMSET(destination, 0, padLength);
  MEMMOVE(destination + padLength, contents, argLength);
  return argLength;
}
