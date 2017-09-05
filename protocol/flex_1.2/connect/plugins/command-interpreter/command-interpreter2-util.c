/**
 * File: command-interpreter2-util.c
 * Description: Common functionality for ASCII and Binary command interpreter2s
 *
 * Copyright 2013 by Silicon Laboratories. All rights reserved.             *80*
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

//------------------------------------------------------------------------------
// Forward declarations.
// static void callCommandAction(void);
// static uint8_t charDowncase(uint8_t c);

// Multiple interpreters.
//
// This is done so as to minimize the flash and RAM hit when only
// using one interpreter.  Unix hosts always have space and often need
// multiple interpreters.

#if defined MULTIPLE_COMMAND_INTERPRETERS || defined UNIX_HOST || defined UNIX_HOST_SIM

EmberCommandState emCommandState0;

CurrentInterpreter emCurrentInterpreter =
{ &emCommandState0, emberCommandTable, emberCommandErrorHandler };

bool emberRunCommandInterpreter(bool commandIsBinary,
                                EmberCommandState *state,
                                EmberCommandEntry *commandTable,
                                EmberCommandErrorHandler *errorHandler,
                                const uint8_t *input,
                                uint16_t length)
{
  // save the current state
  EmberCommandState *previousState = emCurrentInterpreter.state;
  EmberCommandEntry *previousCommandTable = emCurrentInterpreter.commandTable;
  EmberCommandErrorHandler *previousErrorHandler = emCurrentInterpreter.errorHandler;

  // push the state
  emCurrentInterpreter.state = state;
  emCurrentInterpreter.commandTable = commandTable;
  emCurrentInterpreter.errorHandler = errorHandler;

  bool result = (commandIsBinary
                 ? emberProcessBinaryCommand(input, length)
                 : emberProcessCommandString(input, length));

  // pop the state
  emCurrentInterpreter.state = previousState;
  emCurrentInterpreter.commandTable = previousCommandTable;
  emCurrentInterpreter.errorHandler = previousErrorHandler;

  return result;
}

void emberInitializeCommandState(EmberCommandState *state)
{
  emCurrentInterpreter.state = state;
  emberCommandReaderInit();
  emCurrentInterpreter.state = &emCommandState0;
}

#ifdef RTOS
  #include "rtos/rtos.h"
bool emberCommandInterpreterRtosInit(EmberCommandState *state,
                                     EmberCommandEntry *commandTable,
                                     EmberCommandErrorHandler *errorHandler)
{
  if (rtosRegisterSharedGlobal((void**)&emCurrentInterpreter.state) != EMBER_SUCCESS) {
    return false;
  }
  if (rtosRegisterSharedGlobal((void**)&emCurrentInterpreter.commandTable) != EMBER_SUCCESS) {
    return false;
  }
  if (rtosRegisterSharedGlobal((void**)&emCurrentInterpreter.errorHandler) != EMBER_SUCCESS) {
    return false;
  }
  emberCommandInterpreterSetState(state, commandTable, errorHandler);

  return true;
}

void emberCommandInterpreterSetState(EmberCommandState *state,
                                     EmberCommandEntry *commandTable,
                                     EmberCommandErrorHandler *errorHandler)
{
  emCurrentInterpreter.state = state;
  emCurrentInterpreter.commandTable = commandTable;
  emCurrentInterpreter.errorHandler = errorHandler;
}
#endif //RTOS

#else

EmberCommandState emCommandState;

#endif

//----------------------------------------------------------------
// Initialize the state maachine.

// Initialize the command interpreter.
void emCommandReaderInitStateVar(EmberCommandState *state)
{
  state->state = CMD_AWAITING_ARGUMENT;
  state->index = 0;
  state->tokenIndices[0] = 0;
  state->tokenCount = 0;
  state->error = EMBER_CMD_SUCCESS;
  state->hexHighNibble = 0xFF;
  state->argOffset = 0;
  state->currentCommand = NULL;
  state->defaultBase = 10;
  state->previousCharacter = 0;
}

// Reinitialize for the next command line.
void emCommandReaderReinit(void)
{
  emCommandState.state = CMD_AWAITING_ARGUMENT;
  emCommandState.index = 0;
  emCommandState.tokenIndices[0] = 0;
  emCommandState.tokenCount = 0;
  emCommandState.error = EMBER_CMD_SUCCESS;
  emCommandState.hexHighNibble = 0xFF;
  emCommandState.argOffset = 0;
  emCommandState.currentCommand = NULL;
  // Don't set previousCharacter; its value must be preserved from one
  // command to the next.
}

void emberCommandReaderInit(void)
{
  emCommandState.defaultBase = 10;
  emCommandState.previousCharacter = 0;
  emCommandReaderReinit();
}

// We provide a way of overriding the default base for numbers on the
// command line for backwards compatibility.
// This function is intentionally NOT documented in the .h file so it
// isn't used anywhere it shouldn't be.
void emberCommandReaderSetDefaultBase(uint8_t base)
{
  emCommandState.defaultBase = base;
}

//----------------------------------------------------------------
// Retrieving arguments

uint8_t emberCommandArgumentCount(void)
{
  return (emCommandState.tokenCount - emCommandState.argOffset);
}

uint8_t *emTokenPointer(int8_t tokenNum)
{
  return emCommandState.buffer + emCommandState.tokenIndices[tokenNum];
}

uint8_t emTokenLength(uint8_t num)
{
  return (emCommandState.tokenIndices[num + 1]
          - emCommandState.tokenIndices[num]);
}

uint8_t emFirstByteOfArg(uint8_t argNum)
{
  return *(emTokenPointer(argNum + emCommandState.argOffset));
}

uint32_t emStringToUnsignedInt(uint8_t argNum, bool swallowLeadingSign)
{
  uint8_t tokenNum = argNum + emCommandState.argOffset;
  uint8_t *string = emTokenPointer(tokenNum);
  uint8_t length = emTokenLength(tokenNum);
  uint32_t result = 0;
  uint8_t base = emCommandState.defaultBase;
  uint8_t i;
  assert(argNum < emberCommandArgumentCount());
  for (i = 0; i < length; i++) {
    uint8_t next = string[i];
    if (swallowLeadingSign && i == 0 && next == '-') {
      // do nothing
    } else if ((next == 'x' || next == 'X')
               && result == 0
               && (i == 1 || i == 2)) {
      base = 16;
    } else {
      uint8_t value = emberHexToInt(next);
      if (value < base) {
        result = result * base + value;
      } else {
        emCommandState.error = EMBER_CMD_ERR_ARGUMENT_SYNTAX_ERROR;
        return 0;
      }
    }
  }
  return result;
}

#define isBinary() (emCommandState.defaultBase == 0xFF)

static uint32_t getBinaryUnsignedCommandArgument(uint8_t argNum)
{
  uint8_t size = emTokenLength(argNum);
  uint8_t *place = emTokenPointer(argNum);

  if (size == 1) {
    return *place;
  } else if (size == 2) {
    return emberFetchHighLowInt16u(place);
  } else if (size == 4) {
    return emFetchInt32u(false, place);
  } else {
    assert(false);
#ifndef EMBER_STACK_COBRA
    return 0;
#else
#pragma diag_remark=Pe111
    return 0;
#pragma diag_warning=Pe111
#endif
  }
}

static int32_t getBinarySignedCommandArgument(uint8_t argNum)
{
  uint8_t size = emTokenLength(argNum);
  uint8_t *place = emTokenPointer(argNum);
  int8_t result = *place;

  if (size == 1) {
    // first cast the unsigned to signed, then return
    return result;
  } else if (size == 2) {
    // first cast the unsigned to signed, then return
    int16_t result = emberFetchHighLowInt16u(place);
    return result;
  } else if (size == 4) {
    return emFetchInt32u(false, place);
  } else {
    assert(false);
#ifndef EMBER_STACK_COBRA
    return result;
#else
#pragma diag_remark=Pe111
    return result;
#pragma diag_warning=Pe111
#endif
  }
}

static uint8_t *getBinaryStringCommandArgument(int8_t argNum, uint8_t *length)
{
  if (length != NULL) {
    *length = emTokenLength(argNum) - 1;
  }

  // the length is stored in the first byte, skip it
  return emTokenPointer(argNum) + 1;
}

uint32_t emberUnsignedCommandArgument(uint8_t argNum)
{
  if (isBinary()) {
    return getBinaryUnsignedCommandArgument(argNum);
  } else {
    return emStringToUnsignedInt(argNum, false);
  }
}

int32_t emberSignedCommandArgument(uint8_t argNum)
{
  if (isBinary()) {
    return getBinarySignedCommandArgument(argNum);
  } else {
    bool negative = (emFirstByteOfArg(argNum) == '-');
    int32_t result = (int32_t) emStringToUnsignedInt(argNum, negative);
    return (negative ? -result : result);
  }
}

uint8_t *emberStringCommandArgument(int8_t argNum, uint8_t *length)
{
  if (isBinary()) {
    return getBinaryStringCommandArgument(argNum, length);
  } else {
    uint8_t tokenNum = argNum + emCommandState.argOffset;
    uint8_t leadingQuote = (argNum < 0 ? 0 : 1);
    uint8_t *string = emTokenPointer(tokenNum);
    uint8_t trailingNull = (string[0] == '"' ? 1 : 0);
    if (length != NULL) {
      *length = emTokenLength(tokenNum) - leadingQuote - trailingNull;
    }
    return string + leadingQuote;
  }
}

const char *emberCommandName(void)
{
  return emCommandState.currentCommand->name;
}

uint8_t emberGetStringArgument(int8_t argNum,
                               uint8_t *destination,
                               uint8_t maxLength,
                               bool leftPad)
{
  uint8_t padLength;
  uint8_t argLength;
  uint8_t *contents = emberStringCommandArgument(argNum, &argLength);
  uint8_t trailingNull = (emFirstByteOfArg(argNum) == '"' ? 1 : 0);
  // The argLength does not include the trailing null, but we want
  // to copy the null as well so we increment it within this function.
  argLength += trailingNull;

  if (argLength > maxLength) {
    argLength = maxLength;
  }

  padLength = leftPad ? maxLength - argLength : 0;
  MEMSET(destination, 0, padLength);
  MEMMOVE(destination + padLength, contents, argLength);
  return argLength -= trailingNull;
}

void emberGetExtendedPanIdArgument(int8_t index, uint8_t *extendedPanId)
{
  uint8_t bigEndianExtendedPanId[EXTENDED_PAN_ID_SIZE];
  emberGetStringArgument(index, bigEndianExtendedPanId, EXTENDED_PAN_ID_SIZE, true);
  emberReverseMemCopy(extendedPanId, bigEndianExtendedPanId, EXTENDED_PAN_ID_SIZE);
}

enum {
  EXPECTING_WORD,
  EXPECTING_PADDING,
  EXPECTING_DELIMITER
};

static bool stringToIp(const uint8_t *string,
                       uint8_t stringLength,
                       uint8_t *target,
                       bool address)
{
  uint8_t i;
  uint8_t padIndex;
  uint8_t index = 0;
  bool padded = false;
  uint16_t word = 0;
  uint8_t wordChars = 0;
  uint8_t state = (string[0] == ':' ? EXPECTING_PADDING : EXPECTING_WORD);
  uint8_t ipLength = (address ? 16 : 8);

  for (i = 0; i < stringLength; i++) {
    uint8_t ch = string[i];
    bool isColon = (ch == ':');
    uint8_t value = emberHexToInt(ch);
    bool isLast = (i + 1 == stringLength);
    bool nextIsColon = (string[i + 1] == ':');
    //fprintf(stderr,
    //        "i:%d state:%d ch:%c colon:%d nextIsColon:%d value:%d index:%d\n",
    //        i, state, ch, isColon, nextIsColon, value, index);
    if ((!isColon && value > 15)
        || index == ipLength) {
      return false;
    }

    switch (state) {
      case EXPECTING_WORD:
        if (isColon) {
          return false;
        } else {
          if (wordChars == 4) {
            return false;
          }
          word <<= 4;
          word += value;
          wordChars += 1;
        }
        if (isLast || nextIsColon) {
          emberStoreHighLowInt16u(target + index, word);
          index += 2;
          state = EXPECTING_DELIMITER;
        }
        break;

      case EXPECTING_PADDING:
      case EXPECTING_DELIMITER: {
        if (!isColon
            || isLast // Trailing colon not allowed.
            || (padded && nextIsColon) // Only one padding allowed.
            || (state == EXPECTING_PADDING && !nextIsColon)) {
          return false;
        } else if (nextIsColon) {
          i += 1;
          padded = true;
          padIndex = index;
        }
        word = 0;
        wordChars = 0;
        state = EXPECTING_WORD;
        break;
      }
    }
  }
  if (padded) {
    uint8_t padBytes = ipLength - index;
    uint8_t suffixLength = index - padIndex;
    if (padBytes == 0) {
      return false;  // Nothing to pad.
    }
    MEMMOVE(target + padIndex + padBytes,
            target + padIndex,
            suffixLength);
    MEMSET(target + padIndex, 0, padBytes);
  } else if (index != ipLength) {
    return false;
  }
  return true;
}

bool emberStringToIpAddress(const uint8_t *string,
                            uint8_t stringLength,
                            uint8_t *target)
{
  return stringToIp(string, stringLength, target, true); // address
}

bool emberStringToIpPrefix(const uint8_t *string,
                           uint8_t stringLength,
                           uint8_t *target)
{
  return stringToIp(string, stringLength, target, false); // prefix
}

static bool getIpArgument(uint8_t index, uint8_t *target, bool address)
{
  uint8_t destLength;
  const uint8_t *destText = emberStringCommandArgument(index, &destLength);

  if (!stringToIp(destText, destLength, target, address)) {
    emberSerialPrintfLine(APP_SERIAL, "Bad IP format");
    return false;
  }

  return true;
}

bool emberGetIpArgument(uint8_t index, uint8_t *target)
{
  return getIpArgument(index, target, true); // address
}

bool emberGetIpPrefixArgument(uint8_t index, uint8_t *target)
{
  return getIpArgument(index, target, false); // prefix
}

//----------------------------------------------------------------
// Command lookup and processing

// Returs true if entry is a nested command, and in this case
// it populates the nestedCommand pointer.
// Otherwise it returns false, and does nothing with nestedCommand
//
// Nested commands are implemented by setting the action
// field to NULL
bool emberGetNestedCommand(const EmberCommandEntry *entry,
                           const EmberCommandEntry ** nestedCommand)
{
  if ( entry->action == NULL ) {
    *nestedCommand = (const EmberCommandEntry*)entry->argumentTypes;
    return true;
  } else {
    // setting argumentTypes to 'n' is a deprecated way of declaring nested
    // commands, and any instances of it must be updated to support the new
    // method
    assert(entry->argumentTypes[0] != 'n');

    // anything else means it's not a nested command
    return false;
  }
}

uint8_t emberHexToInt(uint8_t ch)
{
  return ch - (ch >= 'a'
               ? 'a' - 10
               : (ch >= 'A'
                  ? 'A' - 10
                  : (ch <= '9'
                     ? '0'
                     : 0)));
}

bool emCompareCommandName(const uint8_t *command1,
                          const uint8_t *command2,
                          bool binaryCommand)
{
  return (MEMCOMPARE(command1 + (binaryCommand ? 1 : 0),
                     command2,
                     emStrlen(command2))
          == 0);
}

uint8_t emberKeyDataCommandArgument(uint8_t argNum, EmberKeyData *key)
{
  uint8_t keyLength = emberGetStringArgument(argNum, key->contents, EMBER_ENCRYPTION_KEY_SIZE, false);
  assert(keyLength == EMBER_ENCRYPTION_KEY_SIZE);
  return keyLength;
}

uint8_t emberEUI64CommandArgument(uint8_t argNum, EmberEUI64 *eui64)
{
  uint8_t keyLength = emberGetStringArgument(argNum, *eui64, EUI64_SIZE, false);
  assert(keyLength == EUI64_SIZE);
  return keyLength;
}
