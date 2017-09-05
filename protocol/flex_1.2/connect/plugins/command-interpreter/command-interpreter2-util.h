#ifndef __COMMAND_INTERPRETER2_UTIL_H__
#define __COMMAND_INTERPRETER2_UTIL_H__

#if !defined APP_SERIAL
extern uint8_t serialPort;
  #define APP_SERIAL serialPort
#endif

enum {
  CMD_AWAITING_ARGUMENT,
  CMD_READING_ARGUMENT,
  CMD_READING_STRING,                  // have read opening " but not closing "
  CMD_READING_HEX_STRING,              // have read opening { but not closing }
  CMD_READING_TO_EOL                   // clean up after error
};

#if (defined MULTIPLE_COMMAND_INTERPRETERS) || (defined UNIX_HOST) || (defined UNIX_HOST_SIM)

typedef struct {
  EmberCommandState *state;
  EmberCommandEntry *commandTable;
  EmberCommandErrorHandler *errorHandler;
} CurrentInterpreter;

extern CurrentInterpreter emCurrentInterpreter;

#define emCommandState (*emCurrentInterpreter.state)
#define COMMAND_TABLE (emCurrentInterpreter.commandTable)
#define ERROR_HANDLER (emCurrentInterpreter.errorHandler)

#else

extern EmberCommandState emCommandState;
#define COMMAND_TABLE emberCommandTable
#define ERROR_HANDLER emberCommandErrorHandler

#endif

void emCommandReaderInitStateVar(EmberCommandState *state);
void emCommandReaderReinit(void);
uint8_t *emTokenPointer(int8_t tokenNum);
uint8_t emTokenLength(uint8_t num);
uint8_t emFirstByteOfArg(uint8_t argNum);
uint32_t emStringToUnsignedInt(uint8_t argNum, bool swallowLeadingSign);
bool emberGetNestedCommand(const EmberCommandEntry *entry,
                           const EmberCommandEntry **nestedCommand);
bool emberProcessBinaryCommand(const uint8_t *input, uint16_t inputSize);
bool emCompareCommandName(const uint8_t *command1,
                          const uint8_t *command2,
                          bool binaryCommand);

#ifdef RTOS
bool emberCommandInterpreterRtosInit(EmberCommandState *state,
                                     EmberCommandEntry *commandTable,
                                     EmberCommandErrorHandler *errorHandler);
void emberCommandInterpreterSetState(EmberCommandState *state,
                                     EmberCommandEntry *commandTable,
                                     EmberCommandErrorHandler *errorHandler);
#endif //RTOS

#endif // __COMMAND_INTERPRETER2_UTIL_H__
