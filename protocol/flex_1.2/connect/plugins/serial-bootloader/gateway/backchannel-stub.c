// File: backchannel-stub.c
//
// Description: Stub code for simulating an ISA backchannel for EZSP host
//   applications.
//
// Author(s): Rob Alexander <ralexander@ember.com>
//
// Copyright 2009 by Ember Corporation.  All rights reserved.               *80*
//------------------------------------------------------------------------------

#include PLATFORM_HEADER //compiler/micro specifics, types

#include "stack/include/ember-types.h"
#include "stack/include/error.h"

//------------------------------------------------------------------------------

const bool backchannelSupported = false;
bool backchannelEnable = false;
int backchannelSerialPortOffset = 0;

//------------------------------------------------------------------------------

EmberStatus backchannelStartServer(uint8_t port)
{
  return EMBER_LIBRARY_NOT_PRESENT;
}

EmberStatus backchannelStopServer(uint8_t port)
{
  return EMBER_LIBRARY_NOT_PRESENT;
}

int backchannelReceive(uint8_t port, char* data)
{
  return -1;
}

EmberStatus backchannelSend(uint8_t port, uint8_t * data, uint8_t length)
{
  return EMBER_LIBRARY_NOT_PRESENT;
}

EmberStatus backchannelGetConnection(uint8_t port,
                                     bool remapStdinStdout)
{
  return EMBER_LIBRARY_NOT_PRESENT;
}

EmberStatus backchannelCloseConnection(uint8_t port)
{
  return EMBER_LIBRARY_NOT_PRESENT;
}

EmberStatus backchannelMapStandardInputOutputToRemoteConnection(int port)
{
  return EMBER_LIBRARY_NOT_PRESENT;
}

EmberStatus backchannelServerPrintf(const char* formatString, ...)
{
  return EMBER_LIBRARY_NOT_PRESENT;
}

EmberStatus backchannelClientPrintf(uint8_t port, const char* formatString, ...)
{
  return EMBER_LIBRARY_NOT_PRESENT;
}

EmberStatus backchannelClientVprintf(uint8_t port,
                                     const char* formatString,
                                     va_list ap)
{
  return EMBER_LIBRARY_NOT_PRESENT;
}

uint8_t backchannelCheckConnection(uint8_t port, bool waitForConnection)
{
  return 0;
}
