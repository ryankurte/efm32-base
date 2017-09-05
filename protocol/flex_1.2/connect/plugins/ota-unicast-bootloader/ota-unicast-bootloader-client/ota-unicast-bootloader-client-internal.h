// Copyright 2016 Silicon Laboratories, Inc.

#ifndef _OTA_UNICAST_BOOTLOADER_CLIENT_INTERNAL_H_
#define _OTA_UNICAST_BOOTLOADER_CLIENT_INTERNAL_H_

#include "ota-unicast-bootloader/ota-unicast-bootloader-protocol.h"
#include "ota-unicast-bootloader/ota-unicast-bootloader-common-internal.h"

//------------------------------------------------------------------------------
// Internal states

#define STATE_OTA_CLIENT_IDLE                                               0x00
#define STATE_OTA_CLIENT_IMAGE_DOWNLOAD_WAITING                             0x01

#define clientIsIdle()  (internalState == STATE_OTA_CLIENT_IDLE)
#define clientInImageDownload() \
  (internalState == STATE_OTA_CLIENT_IMAGE_DOWNLOAD_WAITING)

//------------------------------------------------------------------------------
// Download history types and definitions.

#define HISTORY_TABLE_SIZE        5

typedef struct {
  uint8_t imageTag;
  uint8_t otaProtocolStatus;
  uint8_t applicationStatus;
  EmberNodeId serverId;
  EmberNodeId alternateServerId;
} EmHistoryEntry;

//------------------------------------------------------------------------------
// Test-related macros

#if defined(EMBER_SCRIPTED_TEST)
#include "core/ember-stack.h"
#include "core/parcel.h"
#include "core/scripted-stub.h"
#define scriptCheckHistoryTableAdd(index) \
  simpleScriptCheck("history table add", "history table add", "i", (index))
#define scriptCheckHistoryTableRemove(index, oldestIndex) \
  simpleScriptCheck("history table remove", "history table remove", "ii", (index), (oldestIndex))
#else
#define scriptCheckHistoryTableAdd(index)
#define scriptCheckHistoryTableRemove(index, oldestIndex)
#endif

#endif // _OTA_UNICAST_BOOTLOADER_CLIENT_INTERNAL_H_
