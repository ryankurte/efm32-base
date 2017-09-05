// Copyright 2015 Silicon Laboratories, Inc.

#ifndef _OTA_BOOTLOADER_SERVER_INTERNAL_H_
#define _OTA_BOOTLOADER_SERVER_INTERNAL_H_

#include "ota-bootloader/ota-bootloader-protocol.h"
#include "ota-bootloader/ota-bootloader-common-internal.h"

//------------------------------------------------------------------------------
// Internal states

#define STATE_OTA_SERVER_IDLE                                               0x00
#define STATE_OTA_SERVER_SEGMENT_BROADCAST_INTERVAL                         0x01
#define STATE_OTA_SERVER_SEGMENT_BROADCAST_PENDING                          0x02
#define STATE_OTA_SERVER_MISSING_SEGMENTS_UNICAST_INTERVAL                  0x03
#define STATE_OTA_SERVER_MISSING_SEGMENTS_UNICAST_PENDING                   0x04
#define STATE_OTA_SERVER_MISSING_SEGMENTS_WAITING_RESPONSE                  0x05
#define STATE_OTA_SERVER_IMAGE_DISTRIBUTION_COMPLETED                       0x06
#define STATE_OTA_SERVER_TARGET_STATUS_REQUEST_UNICAST_INTERVAL             0x07
#define STATE_OTA_SERVER_TARGET_STATUS_REQUEST_UNICAST_PENDING              0x08
#define STATE_OTA_SERVER_TARGET_STATUS_REQUEST_WAITING_RESPONSE             0x09
#define STATE_OTA_SERVER_TARGET_STATUS_REQUEST_COMPLETED                    0x0A
#define STATE_OTA_SERVER_BOOTLOAD_REQUEST_UNICAST_INTERVAL                  0x0B
#define STATE_OTA_SERVER_BOOTLOAD_REQUEST_UNICAST_PENDING                   0x0C
#define STATE_OTA_SERVER_BOOTLOAD_REQUEST_WAITING_RESPONSE                  0x0D
#define STATE_OTA_SERVER_BOOTLOAD_REQUEST_COMPLETED                         0x0E

#define serverIsIdle()  (internalState == STATE_OTA_SERVER_IDLE)

#define serverIsCompletingProcess() \
  (processCompleteState != STATE_OTA_SERVER_IDLE)

#define serverInImageDistributionProcess()                      \
  (internalState >= STATE_OTA_SERVER_SEGMENT_BROADCAST_INTERVAL \
   && internalState <= STATE_OTA_SERVER_MISSING_SEGMENTS_WAITING_RESPONSE)

#define serverCompletingDistributionProcess() \
  (processCompleteState == STATE_OTA_SERVER_IMAGE_DISTRIBUTION_COMPLETED)

#define serverInTargetStatusRequestProcess()                                \
  (internalState >= STATE_OTA_SERVER_TARGET_STATUS_REQUEST_UNICAST_INTERVAL \
   && internalState <= STATE_OTA_SERVER_TARGET_STATUS_REQUEST_WAITING_RESPONSE)

#define serverCompletingTargetStatusRequestProcess() \
  (processCompleteState == STATE_OTA_SERVER_TARGET_STATUS_REQUEST_COMPLETED)

#define serverInBootloadRequestProcess()                               \
  (internalState >= STATE_OTA_SERVER_BOOTLOAD_REQUEST_UNICAST_INTERVAL \
   && internalState <= STATE_OTA_SERVER_BOOTLOAD_REQUEST_WAITING_RESPONSE)

#define serverCompletingBootloadRequestProcess() \
  (processCompleteState == STATE_OTA_SERVER_BOOTLOAD_REQUEST_COMPLETED)

//------------------------------------------------------------------------------
// Target list related defines

#define INVALID_TARGET_LIST_INDEX            0xFFFF

// We allocate 4 bytes for each target, 2 bytes for the short ID, 1 byte for
// storing the application target status and 1 byte for misc target information.
#define TARGET_LIST_ENTRY_LENGTH                                   4
#define TARGET_LIST_ENTRY_SHORT_ID_LENGTH                          2
#define TARGET_LIST_ENTRY_SHORT_ID_OFFSET                          0
#define TARGET_LIST_ENTRY_APP_STATUS_LENGHT                        1
#define TARGET_LIST_ENTRY_APP_STATUS_OFFSET                        2
#define TARGET_LIST_ENTRY_MISC_INFO_LENGHT                         1
#define TARGET_LIST_ENTRY_MISC_INFO_OFFSET                         3
#define TARGET_LIST_ENTRY_MISC_INFO_OTA_STATUS_MASK                0x0F
#define TARGET_LIST_ENTRY_MISC_INFO_OTA_STATUS_OFFSET              0
#define TARGET_LIST_ENTRY_MISC_INFO_UNREACHABLE_BIT                0x10

#define TARGET_LIST_ENTRY_MISC_INFO_INIT_VALUE                     \
  (EMBER_OTA_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_ONGOING \
    << TARGET_LIST_ENTRY_MISC_INFO_OTA_STATUS_OFFSET)

uint16_t emTargetLookup(EmberNodeId targetShortId);

uint8_t *emGetTargetApplicationStatus(uint16_t targetIndex);

#define emSetTargetApplicationStatus(targetIndex, appStatus) \
  (*emGetTargetApplicationStatus(targetIndex) = (appStatus))

uint8_t *emGetTargetMiscInfo(uint16_t targetIndex);

#define emTargetIsUnreachable(targetIndex) \
  ((*emGetTargetMiscInfo(targetIndex)      \
    & TARGET_LIST_ENTRY_MISC_INFO_UNREACHABLE_BIT))

#define emSetTargetUnreachable(targetIndex) \
  ((*emGetTargetMiscInfo(targetIndex)       \
      |= TARGET_LIST_ENTRY_MISC_INFO_UNREACHABLE_BIT))

#define emGetTargetOtaStatus(targetIndex)          \
  ((*emGetTargetMiscInfo(targetIndex)              \
    & TARGET_LIST_ENTRY_MISC_INFO_OTA_STATUS_MASK) \
   >> TARGET_LIST_ENTRY_MISC_INFO_OTA_STATUS_OFFSET)

#define emSetTargetOtaStatus(targetIndex, otaStatus)                    \
  do {                                                                  \
    uint8_t *info = emGetTargetMiscInfo(targetIndex);                   \
    *info &= ~TARGET_LIST_ENTRY_MISC_INFO_OTA_STATUS_MASK;              \
    *info |= ((otaStatus & TARGET_LIST_ENTRY_MISC_INFO_OTA_STATUS_MASK) \
              << TARGET_LIST_ENTRY_MISC_INFO_OTA_STATUS_OFFSET);        \
  } while (0)

#endif // _OTA_BOOTLOADER_SERVER_INTERNAL_H_
