/*
 * File: command-interpreter2-test.c
 * Description: Unit test for the command interpreter
 * Author(s): Richard Kelsey, Matteo Paris
 *
 * Copyright 2003 by Ember Corporation. All rights reserved.                *80*
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "core/ember-stack.h"
#include "include/error.h"
#include "command-interpreter2.h"

#define PORT 17

// We run the test twice, the second time prepending 'nested' to each command.
static bool prependNested = false;

// Definition so extern in command-interpreter2.c is satisfied.
uint8_t serialPort = PORT;

//----------------------------------------------------------------
// Stubs

// The emberSerialReadByte() stub reads from a string.  When it hits the end
// there are no more characters available.

static uint8_t *currentInput = NULL;

EmberStatus emberSerialReadByte(uint8_t port, uint8_t *dataByte)
{
  assert(port == PORT);
  if (currentInput == NULL || *currentInput == 0) {
    return EMBER_SERIAL_RX_EMPTY;
  } else {
    *dataByte = *currentInput;
    currentInput += 1;

    return EMBER_SUCCESS;
  }
}

EmberStatus emberSerialWriteData(uint8_t port, uint8_t *data, uint8_t length)
{
  return EMBER_ERR_FATAL;
}

EmberStatus emberSerialPrintf(uint8_t port, PGM_P formatString, ...)
{
  return EMBER_ERR_FATAL;
}

EmberStatus emberSerialPrintfLine(uint8_t port, PGM_P formatString, ...)
{
  return EMBER_ERR_FATAL;
}

uint8_t stringToIpAddress(const uint8_t *string,
                          uint8_t stringLength,
                          uint8_t *target)
{
  return 0;
}

//----------------------------------------------------------------
// Command functions.
//
// The command functions just verify that the parsed command arguments are
// what they are supposed to be.

enum {
  INTEGERS_COMMAND,
  STRING_COMMAND,
  HEX_STRING_COMMAND,
  HEX_COMMAND,
  VAR_ARGS_COMMAND,
};

// The expected next command and its arguments.

static int nextCommand = -1;
static int arg0 = -1;
static int arg1 = -1;
static int arg2 = -1;
static int arg3 = -1;

// Argument specification is "uvws" (unsigned one byte, unsigned two byte,
// unsigned four byte, and signed one byte).

static void integersCommand(void)
{
  assert(nextCommand == INTEGERS_COMMAND);
  assert(arg0 == emberUnsignedCommandArgument(0));
  assert(arg1 == emberUnsignedCommandArgument(1));
  assert(arg2 == emberSignedCommandArgument(2));
  assert(arg3 == emberSignedCommandArgument(3));

  nextCommand = -1;
  arg0 = -1;
  arg1 = -1;
  arg2 = -1;
  arg3 = -1;
}

// The strings we expect to get as arguments.

static uint8_t *strings[] = {
  (uint8_t *)"just",
  (uint8_t *)"some strings, $%*&1:",
};

// Argument specification is "bb" (two strings).

static void stringCommand(void)
{
  uint8_t i;
  uint8_t temp[256];

  assert(nextCommand == STRING_COMMAND);
  for (i = 0; i < 2; i++) {
    uint8_t length;
    uint8_t *argument = emberStringCommandArgument(i, &length);
    memcpy(temp, argument, length);
    temp[length] = 0;
    assert(strcmp((char *)temp, (char *)strings[i]) == 0);
  }

  nextCommand = -1;
}

// Argument specification is "b"

static void hexStringCommand(void)
{
  uint8_t i;
  uint8_t length;
  uint8_t *contents = emberStringCommandArgument(0, &length);

  assert(nextCommand = HEX_STRING_COMMAND);

  for (i = 0; i <= 0x0F; i++) {
    assert(i == contents[i]);
  }

  nextCommand = -1;
}

// To test the case where there is an exact match
// and a longer inexact match.
static void hexCommand(void)
{
  assert(nextCommand == HEX_COMMAND);
  nextCommand = -1;
}

static void varArgsCommand(void)
{
  assert(nextCommand == VAR_ARGS_COMMAND);
  nextCommand = -1;
}

//----------------------------------------------------------------

static int nextErrorCode = -1;

void emberCommandErrorHandler(EmberCommandStatus errorCode,
                              EmberCommandEntry *command)
{
  if (errorCode != nextErrorCode) {
    fprintf(stderr, "Wrong error, wanted %X, got %X]\n",
            nextErrorCode,
            errorCode);
    assert(errorCode == nextErrorCode);
  } else {
    nextErrorCode = -1;         // so we can detect that this call occured
  }
}

//----------------------------------------------------------------
// 'S' to test case insensitivity

EmberCommandEntry nestedCommands[] = {
  emberCommandEntryAction("integers", integersCommand, "uvws", "integers"),
  emberCommandEntryAction("string", stringCommand, "bb", "string"),
  emberCommandEntryAction("hexString", hexStringCommand, "b", "hexstring"),
  emberCommandEntryAction("hex", hexCommand, "", "hex"),
  emberCommandEntryAction("varArgs", varArgsCommand, "u*", "varArgs"),
  emberCommandEntryTerminator()
};

EmberCommandEntry emberCommandTable[] = {
  emberCommandEntryAction("integers", integersCommand, "uvws", "integers"),
  emberCommandEntryAction("string", stringCommand, "bb", "string"),
  emberCommandEntryAction("hexString", hexStringCommand, "b", "hexstring"),
  emberCommandEntryAction("hex", hexCommand, "", "hex"),
  emberCommandEntryAction("varArgs", varArgsCommand, "u*", "varArgs"),
  emberCommandEntrySubMenu("nested", nestedCommands, "nested"),
  emberCommandEntryTerminator()
};

// We alternate between reading from a port and parsing an existing string.

static bool usePort = true;

static void reallyParse(uint8_t *string)
{
  bool prompt;
  if (usePort) {
    currentInput = string;
    prompt = emberProcessCommandInput(PORT);
    usePort = false;
  } else {
    prompt = emberProcessCommandString(string, strlen((char *)string));
    usePort = true;
  }

  bool hasEol = (strstr((char *)string, "\n") != NULL);
  assert(prompt == hasEol);
}

static void parseString(uint8_t *string)
{
  if (prependNested) {
    reallyParse((uint8_t *)"nested ");
  }
  reallyParse(string);
}

//----------------------------------------------------------------

static void run(void)
{
  //----------------
  // Check that a bogus command produces the correct error.

  nextErrorCode = EMBER_CMD_ERR_NO_SUCH_COMMAND;
  parseString((uint8_t *)"foo\n");
  assert(nextErrorCode == -1);

  nextErrorCode = EMBER_CMD_ERR_NO_SUCH_COMMAND;
  parseString((uint8_t *)"foo # bar\n");
  assert(nextErrorCode == -1);

  nextErrorCode = EMBER_CMD_ERR_NO_SUCH_COMMAND;
  parseString((uint8_t *)"f!oo\n");
  assert(nextErrorCode == -1);

  nextErrorCode = (prependNested
                   ? EMBER_CMD_ERR_NO_SUCH_COMMAND
                   : EMBER_CMD_ERR_WRONG_NUMBER_OF_ARGUMENTS);
  parseString((uint8_t *)"nested\n");
  assert(nextErrorCode == -1);

  fprintf(stderr, ".");

  //----------------
  // Integer arguments of different types.

  nextCommand = INTEGERS_COMMAND;
  arg0 = 10;
  arg1 = 11;
  arg2 = 12;
  arg3 = -13;

  parseString((uint8_t *)"iNtEgers 10 11 12 -13\n");

  fprintf(stderr, ".");

  // Abbreviated command name

  nextCommand = INTEGERS_COMMAND;
  arg0 = 1;
  arg1 = 10;
  arg2 = 0xA;
  arg3 = -0x0F;
  parseString((uint8_t *)"i 1 10 0xA -0x0F\n");
  fprintf(stderr, ".");

  // Extra characters in command name

  nextCommand = INTEGERS_COMMAND;
  arg0 = 1;
  arg1 = 10;
  arg2 = 0xA;
  arg3 = -0x0F;
  parseString((uint8_t *)"integersXYZ 1 10 0xA -0x0F\n");
  fprintf(stderr, ".");

  // Hexidecimal arguments of different types.

  nextCommand = INTEGERS_COMMAND;
  arg0 = 0xA1;
  arg1 = 0x1BC;
  arg2 = 0x3C;
  arg3 = -0x1C;

  // Throw in a '\r' character to make sure it doesn't cause trouble.
  parseString((uint8_t *)"integers 0xA1 0x1BC 0x3c -0x1c\r\n");

  fprintf(stderr, ".");

  // Mixing bases

  nextCommand = INTEGERS_COMMAND;
  arg0 = 0xA1;
  arg1 = 12;
  arg2 = 0x3C;
  arg3 = -50;

  parseString((uint8_t *)"INTEGERS 0xA1 12 0x3c -50\n");

  fprintf(stderr, ".");

  // Check that the minimum and maximum values work.

  nextCommand = INTEGERS_COMMAND;
  arg0 = 0x0;
  arg1 = 0x0;
  arg2 = 0x0;
  arg3 = -0x7F;

  parseString((uint8_t *)"integers 0 0 0 -0x7F \n");

  fprintf(stderr, ".");

  nextCommand = INTEGERS_COMMAND;
  arg0 = 0xFF;
  arg1 = 0xFFFF;
  arg2 = 0xFFFFFFFF;
  arg3 = 0x7F;

  parseString((uint8_t *)"integers 0xFF 0xFFFF 0xFFFFFFFF 0x7F \n");

  fprintf(stderr, ".");

  // There was a bug where a 0x... argument caused the first digit of the
  // following argument to be interpreted as base 16.  This makes sure that
  // we have not regressed.
  nextErrorCode = EMBER_CMD_ERR_ARGUMENT_SYNTAX_ERROR;
  parseString((uint8_t *)"integers 10 0x11 A2 13\n");
  assert(nextErrorCode == -1);

  fprintf(stderr, ".");

  nextErrorCode = EMBER_CMD_ERR_WRONG_NUMBER_OF_ARGUMENTS;
  parseString((uint8_t *)"integers 5\n");
  assert(nextErrorCode == -1);
  fprintf(stderr, ".");

  nextErrorCode = EMBER_CMD_ERR_WRONG_NUMBER_OF_ARGUMENTS;
  parseString((uint8_t *)"integers 5 4 3 2 1 10\n");
  assert(nextErrorCode == -1);
  fprintf(stderr, ".");

  //----------------
  // String arguments.

  nextCommand = STRING_COMMAND;
  parseString((uint8_t *)"string \"");
  reallyParse((uint8_t *)strings[0]);
  reallyParse((uint8_t *)"\" \"");
  reallyParse((uint8_t *)strings[1]);
  reallyParse((uint8_t *)"\"\n");
  fprintf(stderr, ".");

  parseString((uint8_t *)"string");
  {
    int j;
    reallyParse((uint8_t *)" \"");
    for (j = 0; j < 9; j += 1) {
      reallyParse((uint8_t *)"abcdefghijklmnopqrstuvwxyz");
    }
    reallyParse((uint8_t *)"\"");
  }

  nextErrorCode = EMBER_CMD_ERR_STRING_TOO_LONG;
  parseString((uint8_t *)"\n");
  assert(nextErrorCode == -1);

  //----------------
  // Hex strings

  nextCommand = HEX_STRING_COMMAND;
  parseString((uint8_t *)"hexstring { 00 010  2030405060708090A0B0C0D0E0F}\n");

  fprintf(stderr, ".");

  nextErrorCode = EMBER_CMD_ERR_ARGUMENT_SYNTAX_ERROR;
  parseString((uint8_t *)"hexstring {123}\n");  // needs even number of digits
  assert(nextErrorCode == -1);

  nextErrorCode = EMBER_CMD_ERR_ARGUMENT_SYNTAX_ERROR;
  parseString((uint8_t *)"hexstring {1A2G}\n"); // non hex digit
  assert(nextErrorCode == -1);

  // Exact match works for command that is a substring of another.
  nextCommand = HEX_COMMAND;
  parseString((uint8_t *)"hex\n");

  // Multiple inexact matches produces an error.
  nextErrorCode = EMBER_CMD_ERR_NO_SUCH_COMMAND;
  parseString((uint8_t *)"hexs\n");
  assert(nextErrorCode == -1);

  // Multiple inexact matches produces an error.
  nextErrorCode = EMBER_CMD_ERR_NO_SUCH_COMMAND;
  parseString((uint8_t *)"he\n");
  assert(nextErrorCode == -1);

  nextCommand = VAR_ARGS_COMMAND;
  parseString((uint8_t *)"varargs 0x7a\n");
  nextCommand = VAR_ARGS_COMMAND;
  parseString((uint8_t *)"varArgs 3 5\n");
  nextErrorCode = EMBER_CMD_ERR_ARGUMENT_SYNTAX_ERROR;
  parseString((uint8_t *)"varArgs 3 xxx\n");
  assert(nextErrorCode == -1);

  fprintf(stderr, ".");
}

static void newlinesTest(void)
{
  const char * const newlines[] = {
    "varargs\r",
    "varargs\n",
    "varargs\r",
    "\n\r",
    "\r",
    "\r",
    "\n\r",
    "\n\n",
    "\n",
    NULL
  };
  uint8_t i;
  for (i = 0; newlines[i] != NULL; i++) {
    nextCommand = VAR_ARGS_COMMAND;
    assert(emberProcessCommandString(((const uint8_t *)newlines[i]),
                                     strlen(newlines[i])));
  }
  for (i = 0; newlines[i] != NULL; i++) {
    uint8_t j;
    for (j = 0; newlines[i][j + 1] != '\0'; j++) {
      assert(!emberProcessCommandString(((const uint8_t *)newlines[i]) + j, 1));
    }
    nextCommand = VAR_ARGS_COMMAND;
    assert(emberProcessCommandString(((const uint8_t *)newlines[i]) + j, 1));
  }
}

int
main(void)
{
  fprintf(stderr, "[Testing command interpreter2 ");
  emberCommandReaderInit();
  run();

  // Run again, prepending 'nested ' to each command.
  prependNested = true;
  run();

  newlinesTest();

  fprintf(stderr, " done]\n");
  return 0;
}
