// File: ncp-link.c
//
// Description: Connect the NCP to a communications layer for
// sending and receiving management and packet data.
//
// Copyright 2014 by Silicon Laboratories. All rights reserved.            *80*

#include PLATFORM_HEADER
#include "stack/include/ember.h"
#include "stack/framework/debug.h"
#include "command-interpreter/command-interpreter2.h"
#include "command-interpreter/command-interpreter2-util.h"
#include "host-ncp/uart-link-protocol.h"
#include "host-ncp/ncp-link.h"

bool ncpLinkManagementPacketHandler(ManagementType managementType,
                                    uint8_t *data,
                                    uint8_t len)
{
  // Make sure that we're only given management command packets
  // because that's all that we know how to handle.
  assert(managementType == MANAGEMENT_COMMAND);
  return emberProcessBinaryCommand(data, len);
}

void emReallySendManagementCommand(const uint8_t *command, uint16_t length)
{
  // Strip off the UART link header which should never have been added in
  // the first place...
  command += 5;
  length -= 5;

  // length is stored in the first byte
  if (!ncpLinkSendManagementCommand(MANAGEMENT_COMMAND,
                                    command,
                                    length)) {
    // Call an error handler to indicate that management data could not be sent
    ncpLinkManagementErrorHandler(command, length, false);
  }
}
