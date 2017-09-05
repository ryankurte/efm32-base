// Copyright 2016 Silicon Laboratories, Inc.

#ifndef _OTA_UNICAST_BOOTLOADER_COMMON_INTERNAL_H_
#define _OTA_UNICAST_BOOTLOADER_COMMON_INTERNAL_H_

#if (defined(EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_SERVER_SECURITY_ENABLED) \
  || defined(EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_CLIENT_SECURITY_ENABLED))
#define SECURITY_ENABLED
#endif

//------------------------------------------------------------------------------
// Image related defines

#if defined(SECURITY_ENABLED)
// Segment size: depends on whether we use security or not (plugin option).
#define MAX_APPLICATION_PAYLOAD_LENGTH \
  EMBER_MAX_SECURED_APPLICATION_PAYLOAD_LENGTH
#define BROADCAST_TX_OPTIONS  (EMBER_OPTIONS_SECURITY_ENABLED)
#define UNICAST_TX_OPTIONS  (EMBER_OPTIONS_ACK_REQUESTED \
                             | EMBER_OPTIONS_SECURITY_ENABLED)
#else
#define MAX_APPLICATION_PAYLOAD_LENGTH \
  EMBER_MAX_UNSECURED_APPLICATION_PAYLOAD_LENGTH
#define BROADCAST_TX_OPTIONS  (0)
#define UNICAST_TX_OPTIONS  (EMBER_OPTIONS_ACK_REQUESTED)
#endif // SECURITY_ENABLED

#define MAX_SEGMENT_PAYLOAD_LENGTH \
  (MAX_APPLICATION_PAYLOAD_LENGTH  \
   - EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_HEADER_LENGTH)

// We define here the maximum image size we support (we use 2 bytes for the
// segment index leaving the index 0xFFFF reserved, which allows us to support
// an image of up to (65534*(107-4))=6750002 bytes in case we use unsecured
// messages, an image of up to (65534*(98-4))=6160196 bytes in case we use
// security).
#define MAXIMUM_IMAGE_SIZE (0xFFFE * MAX_SEGMENT_PAYLOAD_LENGTH)

// We break down the broadcast process in blocks, so that when we query the
// targets for missing block, they can provide a bitmask that fits in a single
// message (512 segments in a block requires a 64 bytes bitmask, which fits
// comfortably in a single message, with or without security).
// This should always be a multiple of 8.
#define MAX_SEGMENTS_IN_A_BLOCK           512

// (Max) length of a missing segments bitmask in bytes.
#define MISSING_SEGMENTS_BITMASK_LENGTH   (MAX_SEGMENTS_IN_A_BLOCK / 8)

#define INVALID_SEGMENT_INDEX             0xFFFF

#endif // _OTA_UNICAST_BOOTLOADER_COMMON_INTERNAL_H_
