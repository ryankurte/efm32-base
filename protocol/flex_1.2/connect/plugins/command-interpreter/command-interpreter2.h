/** @file command-interpreter2.h
 * @addtogroup command_interpreter
 * @brief Processes commands coming from the serial port.
 *
 * <!-- Culprit(s): Richard Kelsey, Matteo Paris -->
 *
 * <!-- Copyright 2004-2007 by Ember Corporation.  All rights reserved.  *80* -->
 */

#ifndef __COMMAND_INTERPRETER2_H__
#define __COMMAND_INTERPRETER2_H__

/** @addtogroup command_interpreter
 * Interpret serial port commands. See command-interpreter2.c for source code.
 *
 * See the following application usage example followed by a brief explanation.
 * @code
 *
 * // Usage: network form 22 0xAB12 -3 { 00 01 02 A3 A4 A5 A6 A7 }
 * void formCommand(void)
 * {
 *   uint8_t channel = emberUnsignedCommandArgument(0);
 *   uint16_t panId  = emberUnsignedCommandArgument(1);
 *   int8_t power   = emberSignedCommandArgument(2);
 *   uint8_t length;
 *   uint8_t *eui64  = emberStringCommandArgument(3, &length);
 *   ...
 *   ... call emberFormNetwork() etc
 *   ...
 * }
 *
 * // The main command table.
 * EmberCommandEntry emberCommandTable[] = {
 *   emberCommandEntrySubMenu("network",  networkCommands, "Network form/join commands"),
 *   emberCommandEntryAction("status",    statusCommand,   "Prints application status),
 *   ...
 *   emberCommandEntryTerminator()
   };
 *
 * // The table of network commands.
 * EmberCommandEntry networkCommands[] = {
 *   emberCommandEntryAction("form", formCommand, "uvsh", "Form a network"),
 *   { "join",       joinCommand, "uvsh" },
 *   ...
 *   emberCommandEntryTerminator()
   };
 *
 * void main(void)
 * {
 *    emberCommandReaderInit();
 *    while(0) {
 *      ...
 *      // Process input and print prompt if it returns true.
 *      if (emberProcessCommandInput(serialPort)) {
 *         emberSerialPrintf(1, "%p>", PROMPT);
 *      }
 *      ...
 *    }
 * }
 * @endcode
 *
 * -# Applications specify the commands that can be interpreted
 *    by defining the emberCommandTable array of type ::EmberCommandEntry.
 *    The table includes the following information for each command:
 *   -# The full command name.
 *   -# Your application's function name that implements the command.
 *   -# An ::EmberCommandEntry::argumentTypes string specifies the number and types of arguments
 *      the command accepts.  See ::argumentTypes for details.
 *   -# A description string explains the command.
 *   .
 * -# A default error handler ::emberCommandErrorHandler() is provided to
 *    deal with incorrect command input. Applications may override it.
 * -# The application calls ::emberCommandReaderInit() to initalize, and
 *    ::emberProcessCommandInput() in its main loop.
 * -# Within the application's command functions, use emberXXXCommandArgument()
 *    functions to retrieve command arguments.
 *
 * The command interpreter does extensive processing and validation of the
 * command input before calling the function that implements the command.
 * It checks that the number, type, syntax, and range of all arguments
 * are correct.  It performs any conversions necessary (for example,
 * converting integers and strings input in hexadecimal notation into
 * the corresponding bytes), so that no additional parsing is necessary
 * within command functions.  If there is an error in the command input,
 * ::emberCommandErrorHandler() is called rather than a command function.
 *
 * The command interpreter allows inexact matches of command names.  The
 * input command may be either shorter or longer than the actual command.
 * However, if more than one inexact match is found and there is no exact
 * match, an error of type EMBER_CMD_ERR_NO_SUCH_COMMAND will be generated.
 * To disable this feature, define EMBER_REQUIRE_EXACT_COMMAND_NAME in the
 * application configuration header.
 *
 *@{
 */

/** @name Command Table Settings
 *@{
 */
#ifndef EMBER_MAX_COMMAND_ARGUMENTS
/** The maximum number of arguments a command can have.  A nested command
 * counts as an argument.
 */
#define EMBER_MAX_COMMAND_ARGUMENTS 14
#endif

#ifndef EMBER_COMMAND_BUFFER_LENGTH
#define EMBER_COMMAND_BUFFER_LENGTH 125
#endif

/** The max message size for custom commands reserves three bytes for
 * the length (1 bytes) and the custom command identifier (2 bytes).
 */
#define EMBER_CUSTOM_COMMAND_BUFFER_LENGTH (EMBER_COMMAND_BUFFER_LENGTH - 3)

/** Whether or not the command entry structure will include descriptions for
 *  the commands.  This consumes additional CONST space, which is expensive
 *  on the XAP.  By default descriptions are not included.
 */
#if defined(DOXYGEN_SHOULD_SKIP_THIS)
#define EMBER_COMMAND_INTEPRETER_HAS_DESCRIPTION_FIELD
#endif

/** @} // END name group
 */

// The (+ 1) takes into account the leading command.
#define MAX_TOKEN_COUNT (EMBER_MAX_COMMAND_ARGUMENTS + 1)

typedef void (*CommandAction)(void);

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**@brief Command entry for a command table.
 */
typedef struct {
#else
typedef PGM struct {
#endif
  /** Use letters, digits, and underscores, '_', for the command name.
   * Command names are case-sensitive.
   */
  PGM_P name;

#ifdef EMBER_APPLICATION_HAS_BINARY_COMMAND_INTERPRETER
  /** A two-byte identifier for serial communication */
  uint16_t identifier;
#endif

  /** A reference to a function in the application that implements the
   *  command.
   *  If this entry refers to a nested command, then action field
   *  has to be set to NULL.
   */
  CommandAction action;

  /**
   * In case of normal (non-nested) commands, argumentTypes is a
   * string that specifies the number and types of arguments the
   *  command accepts.  The argument specifiers are:
   *  - u:   one-byte unsigned integer.
   *  - v:   two-byte unsigned integer
   *  - w:   four-byte unsigned integer
   *  - s:   one-byte signed integer
   *  - b:   string.  The argument can be entered in ascii by using
   *         quotes, for example: "foo".  Or it may be entered
   *         in hex by using curly braces, for example: { 08 A1 f2 }.
   *         There must be an even number of hex digits, and spaces
   *         are ignored.
   *  - *:   zero or more of the previous type.
   *         If used, this must be the last specifier.
   *  - ?:   Unknown number of arguments. If used this must be the only
   *         character. This means, that command interpreter will not
   *         perform any validation of arguments, and will call the
   *         action directly, trusting it that it will handle with
   *         whatever arguments are passed in.
   *  Integer arguments can be either decimal or hexadecimal.
   *  A 0x prefix indicates a hexadecimal integer.  Example: 0x3ed.
   *
   *  In case of a nested command (action is NULL), then argumentTypes will
   *  contain a pointer to the nested commands.
   */
  PGM_P argumentTypes;

  /** A description of the command.
   */

  PGM_P description;
} EmberCommandEntry;

#ifdef EMBER_APPLICATION_HAS_BINARY_COMMAND_INTERPRETER
  #define emberBinaryCommandEntryAction(identifier, command, arguments, description) \
  { "", identifier, command, arguments, description }
  #define emberBinaryCommandEntrySubMenu(identifier, nestedCommands, description) \
  { "", identifier, NULL, NULL, description, nestedCommands }
  #define emberCommandEntryAction(name, command, arguments, description) \
  { name, 0, command, arguments, description }
  #define emberCommandEntrySubMenu(name, nestedCommands, description) \
  { name, 0, NULL, (PGM_P)nestedCommands, description }
  #define emberCommandEntryTerminator() \
  { NULL, 0, NULL, NULL, NULL }

// deprecated names
  #define emberBinaryCommand        emberBinaryCommandEntryAction
  #define emberBinaryNestedCommand  emberBinaryCommandEntrySubMenu
#else
  #define emberCommandEntryAction(name, command, arguments, description) \
  { name, command, arguments, description }
  #define emberCommandEntrySubMenu(name, nestedCommands, description) \
  { name, NULL, (PGM_P)nestedCommands, description }
  #define emberCommandEntryTerminator() \
  { NULL, NULL, NULL, NULL }
#endif

// deprecated names
#define emberCommand        emberCommandEntryAction
#define emberNestedCommand  emberCommandEntrySubMenu

extern EmberCommandEntry emberCommandTable[];

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/** @brief Command error states.
 *
 * If you change this list, ensure you also change the strings that describe
 * these errors in the array emberCommandErrorNames[] in
 * command-interpreter2-error.c.
 */
enum EmberCommandStatus
#else
typedef uint8_t EmberCommandStatus;
enum
#endif
{
  EMBER_CMD_SUCCESS,
  EMBER_CMD_ERR_PORT_PROBLEM,
  EMBER_CMD_ERR_NO_SUCH_COMMAND,
  EMBER_CMD_ERR_WRONG_NUMBER_OF_ARGUMENTS,
  EMBER_CMD_ERR_ARGUMENT_OUT_OF_RANGE,
  EMBER_CMD_ERR_ARGUMENT_SYNTAX_ERROR,
  EMBER_CMD_ERR_STRING_TOO_LONG,
  EMBER_CMD_ERR_INVALID_ARGUMENT_TYPE
};

/** @name Functions to Retrieve Arguments
 * Use the following functions in your functions that process commands to
 * retrieve arguments from the command interpreter.
 * These functions pull out unsigned integers, signed integers, and strings,
 * and hex strings.  Index 0 is the first command argument.
 *@{
 */

/** Returns the number of arguments for the current command. */
uint8_t emberCommandArgumentCount(void);

/** Retrieves unsigned integer arguments. */
uint32_t emberUnsignedCommandArgument(uint8_t argNum);

/** Retrieves signed integer arguments. */
int32_t emberSignedCommandArgument(uint8_t argNum);

/** Retrieve quoted string or hex string arguments.
 * Hex strings have already been converted into binary.
 * Ascii strings have been null terminated.  The null terminator
 * is not included in the returned length argument.
 * To retrieve the name of the command itself, use an argNum of -1.
 * For example, to retrieve the first character of the command, do:
 * uint8_t firstChar = emberStringCommandArgument(-1, NULL)[0].
 * If the command is nested, an index of -2, -3, etc will work to retrieve
 * the higher level command names.
 * Note that [-1] only returns the text entered. If an abbreviated command
 * name is entered only the text entered will be returned with [-1].
 */
uint8_t *emberStringCommandArgument(int8_t argNum, uint8_t *length);

// emberStringCommandArgument(-1, NULL) returns a pointer to the string
// with the command line name, but the caller must use the length to print it;
// it's not NULL-terminated, unless it happens to be the only text entered
// on the command line.
// emberCommandName() returns a NULL-terminated command name, which is easier
// than requiring every caller to know about the command name length.
const char *emberCommandName(void);

/** A convenience macro for copying security key arguments to an
 * EmberKeyData pointer.
 */
#define emberCopyKeyArgument(index, keyDataPointer)           \
  (emberGetStringArgument((index),                            \
                          emberKeyContents((keyDataPointer)), \
                          EMBER_ENCRYPTION_KEY_SIZE,          \
                          TRUE))

/** A convenience macro for copying eui64 arguments to an EmberEUI64. */
#define emberCopyEui64Argument(index, eui64) \
  (emberGetStringArgument((index), (eui64), EUI64_SIZE, TRUE))

/** Copies the string argument to the given destination up to maxLength.
 * If the argument length is nonzero but less than maxLength
 * and leftPad is true, leading zeroes are prepended to bring the
 * total length of the target up to maxLength.  If the argument
 * is longer than the maxLength, it is truncated to maxLength.
 * Returns the minimum of the argument length and maxLength.
 * Ascii strings are null terminated, but the null terminator
 * is not included in the returned length.
 *
 * This function is commonly used for reading in hex strings
 * such as EUI64 or key data and left padding them with zeroes.
 * See ::emberGetKeyArgument and ::emberGetEui64Argument for
 * convenience macros for this purpose.
 */
uint8_t emberGetStringArgument(int8_t argNum,
                               uint8_t *destination,
                               uint8_t maxLength,
                               bool leftPad);

/** Parses and returns, via target, an IP address at the provided index
 * Returns true if an IP address was successfully parsed
 * Return false otherwise
 */
bool emberGetIpArgument(uint8_t index, uint8_t *target);

/** Parses and returns, via target, an IP prefix at the provided index
 * Returns true if an IP prefix was successfully parsed
 * Return false otherwise
 */
bool emberGetIpPrefixArgument(uint8_t index, uint8_t *target);

/** A convenience macro for copying security key arguments to an
 * EmberKeyData pointer.
 */
#define emberGetKeyArgument(index, keyDataPointer)            \
  (emberGetStringArgument((index),                            \
                          emberKeyContents((keyDataPointer)), \
                          EMBER_ENCRYPTION_KEY_SIZE,          \
                          true))

/** Copies the EUI64 argument to the given EmberEui64 destination, reversing the
 * bytes. EUI64's are stored little endian so reversing the bytes means they are
 * big endian in the input command string.
 */
#define emberGetEui64Argument(index, eui64) \
  emberGetExtendedPanIdArgument(index, (eui64)->bytes)

/** Copies the extended PAN id argument to the given destination, reversing the
 * bytes. Extended PAN ids are stored little endian so reversing the bytes
 * means they are big endian in the input command string.
 */
void emberGetExtendedPanIdArgument(int8_t index, uint8_t *extendedPanId);

/** @} // END name group
 */

void emberCommandErrorHandler(EmberCommandStatus status,
                              EmberCommandEntry *command);
void emberPrintCommandUsage(EmberCommandEntry *entry);
void emberPrintCommandUsageNotes(void);
void emberPrintCommandTable(void);
void emberCommandClearBuffer(void);

/** @brief Initialize the command interpreter.
 */
void emberCommandReaderInit(void);

/** @brief Process the given string as a command.
 */
bool emberProcessCommandString(const uint8_t *input, uint16_t sizeOrPort);

/** @brief Process input coming in on the given serial port.
 * @return true if an end of line character was read.
 * If the application uses a command line prompt,
 * this indicates it is time to print the prompt.
 * @code
 * void emberProcessCommandInput(uint8_t port);
 * @endcode
 */
#define emberProcessCommandInput(port) \
  emberProcessCommandString(NULL, port)

/** @brief Returns the value of hex digit ch (0 - 15).
 * Returns a value > 15 if ch is not a hex digit.
 */
uint8_t emberHexToInt(uint8_t ch);

/** @brief Parses textual representations of IPv6 addresses as described in
 * http://www.ietf.org/rfc/rfc4291.txt.
 */
bool emberStringToIpAddress(const uint8_t *string,
                            uint8_t stringLength,
                            uint8_t *target);

/** @brief Parses textual representations of IPv6 prefixes as described in
 * http://www.ietf.org/rfc/rfc4291.txt.
 */
bool emberStringToIpPrefix(const uint8_t *string,
                           uint8_t stringLength,
                           uint8_t *target);

/** @brief For use when declaring a separate command streams.  The fields
 * are not accessed directly by the application.
 */
typedef struct {
  // Finite-state machine's current state.
  uint8_t state;

  // The command line is stored in this buffer.
  // Spaces and trailing '"' and '}' characters are removed,
  // and hex strings are converted to bytes.
  uint8_t buffer[EMBER_COMMAND_BUFFER_LENGTH];

  // Indices of the tokens (command(s) and arguments) in the above buffer.
  // The (+ 1) lets us store the ending index.
  uint8_t tokenIndices[MAX_TOKEN_COUNT + 1];

  // The number of tokens read in, including the command(s).
  uint8_t tokenCount;

  // Used while reading in the command line.
  uint16_t index;

  // First error found in this command.
  uint8_t error;

  // Storage for reading in a hex string. A value of 0xFF means unused.
  uint8_t hexHighNibble;

  // The token number of the first true argument after possible nested commands.
  uint8_t argOffset;

  // Remember the previous character seen by emberProcessCommandString()
  // to ignore an LF following a CR.
  uint8_t previousCharacter;

  // Unlike the above values, this does not change as commands are read.
  uint8_t defaultBase;

  // Keep a pointer to the current command the user ran.
  EmberCommandEntry *currentCommand;
} EmberCommandState;

/** @brief Must be called to initialize a command state before passing it to
 *   emberRunCommandInterpreter().
 */
void emberInitializeCommandState(EmberCommandState *state);

/** @brief Type of error handlers; the command argument is currently
 *   always NULL.
 */

typedef void EmberCommandErrorHandler (EmberCommandStatus status,
                                       EmberCommandEntry *command);

/** @brief For use when additional different command streams are being used.
 */
bool emberRunCommandInterpreter(bool commandIsBinary,
                                EmberCommandState *state,
                                EmberCommandEntry *commands,
                                EmberCommandErrorHandler *errorHandler,
                                const uint8_t *input,
                                uint16_t sizeOrPort);

// This function declaration really belongs in command-interpreter2-util.h, but
// I'm putting it here in command-interpreter2.h so that znet sees it as well
// zip. (znet doesn't have command-interpreter2-util.*)
void emberCommandReaderSetDefaultBase(uint8_t base);
/** @} // END addtogroup
 */

#endif
