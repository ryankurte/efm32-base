// File: gateway.h
///
// Description:  Functions for Gateway specific behavior for a host application.
//   In this case we assume our application is running on
//   a PC with Unix library support, connected to a 260 via serial uart.
//
// Author(s): Rob Alexander <ralexander@ember.com>
//
// Copyright 2008 by Ember Corporation.  All rights reserved.               *80*
//
//------------------------------------------------------------------------------

// This function's signature will vary depending on the platform.
// Platforms that support a command-line (i.e. PCs) will have the
// standard "int argc, char* argv[]" while those without (i.e. the
// embedded ones) will use "void".  This allows easy compatibility
// cross-platform.
EmberStatus gatewayInit(MAIN_FUNCTION_PARAMETERS);
EmberStatus gatewayBackchannelStart(void);
void        gatewayBackchannelStop(void);

#if defined COMMAND_INTERPRETER_SUPPORT
EmberStatus gatewayCommandInterpreterInit(PGM_P cliPrompt,
                                          EmberCommandEntry commands[]);
#else
EmberStatus gatewayCliInit(PGM_P cliPrompt,
                           cliSerialCmdEntry cliCmdList[],
                           int cliCmdListLength);
#endif
void gatewayWaitForEvents(void);
void gatewayWaitForEventsWithTimeout(uint32_t timeoutMs);

// The difference in seconds between the ZigBee Epoch: January 1st, 2000
// and the Unix Epoch: January 1st 1970.
#define UNIX_ZIGBEE_EPOCH_DELTA (uint32_t) 94668800UL
