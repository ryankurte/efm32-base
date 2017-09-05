// Copyright 2016 Silicon Laboratories, Inc.

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include "hal/hal.h"
#include "stack/include/ember.h"

#include "ota-unicast-bootloader-client.h"
#include "ota-unicast-bootloader-client-internal.h"

//------------------------------------------------------------------------------
// Internal variables and static functions prototypes.

EmberEventControl emAfPluginOtaUnicastBootloaderClientEventControl;

static uint8_t internalState = STATE_OTA_CLIENT_IDLE;

// The index of the segment the client expects to receive
static uint32_t expectedSegmentIndex;

static struct {
  uint32_t size;
  EmberNodeId serverId;
  uint8_t tag;
} imageInfo;

// Image download process related static functions
static void imageDownloadFinished(EmberAfOtaUnicastBootloaderStatus status);
static void handleIncomingHandshake(EmberIncomingMessage *message);
static void handleIncomingImageSegment(EmberIncomingMessage *message);

// Bootload request process related static function
static void handleIncomingBootloadRequest(EmberIncomingMessage *message);
static void sendHandshakeResponse(EmberNodeId serverId,
                                  uint8_t imageTag,
                                  uint8_t targetStatus);
static void sendImageSegmentResponse(EmberNodeId serverId,
                                     uint8_t imageTag,
                                     uint32_t segmentIndex,
                                     uint8_t targetStatus);

//------------------------------------------------------------------------------
// Public APIs

EmberAfOtaUnicastBootloaderStatus emberAfPluginOtaUnicastBootloaderClientAbortImageDownload(uint8_t imageTag,
                                                                                            uint8_t applicationErrorStatus)
{
  if (clientInImageDownload()
      && imageTag == imageInfo.tag) {
    imageDownloadFinished(EMBER_OTA_UNICAST_BOOTLOADER_STATUS_ABORTED);

    return EMBER_OTA_UNICAST_BOOTLOADER_STATUS_SUCCESS;
  }

  return EMBER_OTA_UNICAST_BOOTLOADER_STATUS_INVALID_CALL;
}

//------------------------------------------------------------------------------
// Implemented plugin callbacks

void emAfPluginOtaUnicastBootloaderClientInitCallback(void)
{
}

void emAfPluginOtaUnicastBootloaderClientIncomingMessageCallback(EmberIncomingMessage *message)
{
  if (message->endpoint != EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_CLIENT_ENDPOINT) {
    return;
  }

#if defined(EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_CLIENT_SECURITY_ENABLED)
  // If security is enabled, discard all unsecured messages.
  if (!(message->options & EMBER_OPTIONS_SECURITY_ENABLED)) {
    return;
  }
#endif // EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_CLIENT_SECURITY_ENABLED

  // Check which command ID has arrived
  switch (emOtaUnicastBootloaderProtocolCommandId(message->payload)) {
    case EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_COMMAND_ID_HANDSHAKE:
      if (message->length
          == EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_HANDSHAKE_HEADER_LENGTH) {
        handleIncomingHandshake(message);
      }
      break;

    case EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_COMMAND_ID_IMAGE_SEGMENT:
      if (message->length
          > EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_HEADER_LENGTH) {
        handleIncomingImageSegment(message);
      }
      break;
    case EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_COMMAND_ID_BOOTLOAD_REQUEST:
      if (message->length
          == EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_BOOTLOAD_REQ_HEADER_LENGTH) {
        handleIncomingBootloadRequest(message);
      }
      break;
  }
}

void emAfPluginOtaUnicastBootloaderClientEventHandler(void)
{
  emberEventControlSetInactive(emAfPluginOtaUnicastBootloaderClientEventControl);

  // We timed-out waiting for a message from the server(s): fail the ongoing
  // image download process.
  if (internalState == STATE_OTA_CLIENT_IMAGE_DOWNLOAD_WAITING) {
    imageDownloadFinished(EMBER_OTA_UNICAST_BOOTLOADER_STATUS_TIMEOUT);
  }
}

//------------------------------------------------------------------------------
// Image download process related static functions

static void imageDownloadFinished(EmberAfOtaUnicastBootloaderStatus status)
{
  internalState = STATE_OTA_CLIENT_IDLE;
  emberEventControlSetInactive(emAfPluginOtaUnicastBootloaderClientEventControl);
  emberAfPluginOtaUnicastBootloaderClientImageDownloadCompleteCallback(status,
                                                                       imageInfo.tag,
                                                                       ((status == EMBER_OTA_UNICAST_BOOTLOADER_STATUS_SUCCESS)
                                                                        ? imageInfo.size
                                                                        : 0));
}

static bool serverOk(EmberNodeId serverId)
{
  return (serverId == imageInfo.serverId);
}

static uint32_t getTotalSegmentsCount(void)
{
  uint32_t totalSegments =
    (uint32_t)(imageInfo.size / MAX_SEGMENT_PAYLOAD_LENGTH);

  assert(imageInfo.size > 0);

  if ((imageInfo.size % MAX_SEGMENT_PAYLOAD_LENGTH) > 0) {
    totalSegments++;
  }

  return totalSegments;
}

static void handleIncomingHandshake(EmberIncomingMessage *message)
{
  EmberNodeId serverId = message->source;
  uint8_t imageTag =
    message->payload[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_HANDSHAKE_TAG_OFFSET];

  // Client is idle: see if the application is interested in receiving this image.
  if (clientIsIdle()) {
    imageInfo.serverId = serverId;
    imageInfo.tag = imageTag;
    imageInfo.size = (  (message->payload[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_HANDSHAKE_LENGTH_OFFSET])
                        | (message->payload[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_HANDSHAKE_LENGTH_OFFSET + 1] << 8)
                        | (message->payload[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_HANDSHAKE_LENGTH_OFFSET + 2] << 16)
                        | (message->payload[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_HANDSHAKE_LENGTH_OFFSET + 3] << 24));

    if (!emberAfPluginOtaUnicastBootloaderClientNewIncomingImageCallback(imageInfo.serverId,
                                                                         imageInfo.tag)) {
      // Respond to the server with a REFUSED status
      sendHandshakeResponse(serverId,
                            imageTag,
                            EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_REFUSED);
      // Finally, abort
      imageDownloadFinished(EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_REFUSED);

      return;
    }
    // Initially we expect to receive segment 0.
    expectedSegmentIndex = 0;

    internalState = STATE_OTA_CLIENT_IMAGE_DOWNLOAD_WAITING;

    // And respond to the server with a ONGOING status
    sendHandshakeResponse(serverId,
                          imageTag,
                          EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_ONGOING);
  } else {
    // unexpected handshake from a different server
    if (!serverOk(serverId)) {
      // Handshake from a different server, refuse it and go on.
      sendHandshakeResponse(serverId,
                            imageTag,
                            EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_REFUSED);
    }
  }
}

static void sendHandshakeResponse(EmberNodeId serverId,
                                  uint8_t imageTag,
                                  uint8_t targetStatus)
{
  uint8_t message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_HANDSHAKE_RESP_HEADER_LENGTH];

  uint8_t clientResponse;

  switch (targetStatus) {
    // Image accepted
    case  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_ONGOING:
      clientResponse = EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_ONGOING;
      break;
    // Image refused
    case  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_REFUSED:
      clientResponse = EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_REFUSED;
      break;
    // Application aborts the procedure
    case  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_ABORTED:
      clientResponse = EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_ABORTED;
      break;
    default:
      // unknown value, aborting with FAILED
      clientResponse = EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_FAILED;
  }

  // Frame control
  message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_OFFSET] =
    (  (EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_COMMAND_ID_HANDSHAKE_RESPONSE
        << EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_COMMAND_ID_OFFSET)
       | (clientResponse
          << EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_OFFSET));

  // Image tag
  message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_HANDSHAKE_RESP_TAG_OFFSET] =
    imageTag;

  // Application status
  message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_HANDSHAKE_RESP_STATUS_OFFSET] =
    targetStatus;

  emberMessageSend(serverId,
                   EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_SERVER_ENDPOINT,
                   0, // messageTag
                   EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_HANDSHAKE_RESP_HEADER_LENGTH,
                   message,
                   UNICAST_TX_OPTIONS);
}

static void handleIncomingImageSegment(EmberIncomingMessage *message)
{
  EmberNodeId serverId = message->source;
  uint8_t imageTag =
    message->payload[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_TAG_OFFSET];
  uint32_t segmentIndex = (  (message->payload[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_INDEX_OFFSET])
                             | (message->payload[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_INDEX_OFFSET + 1] << 8)
                             | (message->payload[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_INDEX_OFFSET + 2] << 16)
                             | (message->payload[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_INDEX_OFFSET + 3] << 24));

  uint8_t segmentLength = message->length
                          - EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_HEADER_LENGTH;

  uint32_t startIndex = segmentIndex * MAX_SEGMENT_PAYLOAD_LENGTH;
  uint32_t endIndex = startIndex + segmentLength - 1;

  // Client is idle: image segments shall not be recevied without handshake
  if (clientIsIdle()) {
    return;
  }

  // Check if segment is expected and coming from the correct server and image
  if (internalState == STATE_OTA_CLIENT_IMAGE_DOWNLOAD_WAITING) {
    if ( (serverOk(serverId)) && (imageTag == imageInfo.tag)) {
      // Segment of expected image from expected server.

      // Check if segment is in sequence.
      if ( segmentIndex <= expectedSegmentIndex ) {
        // Segment OK or already received, don't abort

        if (segmentIndex == expectedSegmentIndex) {
          // Account for the last segment which may very well be a partial segment.
          if (endIndex >= imageInfo.size) {
            endIndex = imageInfo.size - 1;
          }

          // Segment is the one we expected, pass it to the application
          emberAfPluginOtaUnicastBootloaderClientIncomingImageSegmentCallback(serverId,
                                                                              startIndex,
                                                                              endIndex,
                                                                              imageTag,
                                                                              (message->payload + EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_PAYLOAD_OFFSET));

          // expecting the next segment
          expectedSegmentIndex++;
        }
        // Last segment?
        if ( segmentIndex ==  (getTotalSegmentsCount() - 1) ) {
          // Yes
          sendImageSegmentResponse(serverId,
                                   imageTag,
                                   segmentIndex,
                                   EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_COMPLETED);
          // Finish with SUCCESS status
          imageDownloadFinished(EMBER_OTA_UNICAST_BOOTLOADER_STATUS_SUCCESS);
        } else {
          // Not the last segment
          // Send ONGOING response
          sendImageSegmentResponse(serverId,
                                   imageTag,
                                   segmentIndex,
                                   EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_ONGOING);
        }

        // Refresh the download timeout.
        emberEventControlSetDelayQS(emAfPluginOtaUnicastBootloaderClientEventControl,
                                    EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_CLIENT_IMAGE_DOWNLOAD_TIMEOUT_S * 4);
      } else {
        // Segment is ahead of where we expect it to be from
        // -> might have missed some segments
        // Abort with OUT_OF_SEQ
        sendImageSegmentResponse(serverId,
                                 imageTag,
                                 segmentIndex,
                                 EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_OUT_OF_SEQ);
        // Abort with FAILED status.
        imageDownloadFinished(EMBER_OTA_UNICAST_BOOTLOADER_STATUS_FAILED);
      }
    } else {
      // Unexptected image tag or server ID, no processing
      // Reply with REFUSED
      sendImageSegmentResponse(serverId,
                               imageTag,
                               segmentIndex,
                               EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_REFUSED);
    }
  }
}

static void sendImageSegmentResponse(EmberNodeId serverId,
                                     uint8_t imageTag,
                                     uint32_t segmentIndex,
                                     uint8_t targetStatus)
{
  uint8_t message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_RESP_HEADER_LENGTH];

  uint8_t clientResponse;

  switch (targetStatus) {
    case EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_COMPLETED:
      clientResponse = EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_COMPLETED;
      break;
    case  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_ONGOING:
      clientResponse = EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_ONGOING;
      break;
    case  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_FAILED:
      clientResponse = EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_FAILED;
      break;
    case  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_REFUSED:
      clientResponse = EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_REFUSED;
      break;
    case  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_ABORTED:
      clientResponse = EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_ABORTED;
      break;
    case  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_OUT_OF_SEQ:
      clientResponse = EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_OUT_OF_SEQ;
      break;
    default:
      // unknown value, aborting
      clientResponse = EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_FAILED;
  }

  // Frame control
  message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_OFFSET] =
    (  (EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_COMMAND_ID_IMAGE_SEGMENT_RESPONSE
        << EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_COMMAND_ID_OFFSET)
       | (clientResponse
          << EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_OFFSET));

  // Image tag
  message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_RESP_TAG_OFFSET] =
    imageTag;

  // Segment Index
  message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_RESP_INDEX_OFFSET] =
    BYTE_0(segmentIndex);
  message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_RESP_INDEX_OFFSET + 1] =
    BYTE_1(segmentIndex);
  message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_RESP_INDEX_OFFSET + 2] =
    BYTE_2(segmentIndex);
  message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_RESP_INDEX_OFFSET + 3] =
    BYTE_3(segmentIndex);

  // Application status
  message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_RESP_STATUS_OFFSET] =
    targetStatus;

  emberMessageSend(serverId,
                   EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_SERVER_ENDPOINT,
                   0, // messageTag
                   EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_IMAGE_SEGMENT_RESP_HEADER_LENGTH,
                   message,
                   UNICAST_TX_OPTIONS);
}

//------------------------------------------------------------------------------
// Bootload request process related static function

static void sendBootloadResponse(EmberNodeId serverId,
                                 uint8_t imageTag,
                                 uint8_t applicationStatus,
                                 bool accepted)
{
  uint8_t message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_BOOTLOAD_RESP_HEADER_LENGTH];

  // Frame control
  message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_OFFSET] =
    ((EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_COMMAND_ID_BOOTLOAD_RESPONSE
      << EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_COMMAND_ID_OFFSET)
     | (((accepted)
         ? EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_COMPLETED
         : EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_REFUSED)
        << EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_FRAME_CONTROL_RESP_STATUS_OFFSET));
  // Image tag
  message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_BOOTLOAD_RESP_TAG_OFFSET] = imageTag;
  // Application status
  message[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_BOOTLOAD_RESP_APP_STATUS_OFFSET] =
    applicationStatus;

  // We don't bother checking the return status here. If we fail, we simply wait
  // for another request from the server.
  emberMessageSend(serverId,
                   EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_SERVER_ENDPOINT,
                   0,  // messageTag
                   EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_BOOTLOAD_RESP_HEADER_LENGTH,
                   message,
                   UNICAST_TX_OPTIONS);
}

static void handleIncomingBootloadRequest(EmberIncomingMessage *message)
{
  uint8_t applicationStatus;
  uint8_t imageTag =
    message->payload[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_BOOTLOAD_REQ_TAG_OFFSET];
  uint32_t delayMs = (message->payload[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_BOOTLOAD_REQ_DELAY_OFFSET]
                      | (message->payload[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_BOOTLOAD_REQ_DELAY_OFFSET + 1] << 8)
                      | (message->payload[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_BOOTLOAD_REQ_DELAY_OFFSET + 2] << 16)
                      | (message->payload[EMBER_OTA_UNICAST_BOOTLOADER_PROTOCOL_BOOTLOAD_REQ_DELAY_OFFSET + 3] << 24));
  bool accepted =
    emberAfPluginOtaUnicastBootloaderClientIncomingRequestBootloadCallback(message->source,
                                                                           imageTag,
                                                                           delayMs,
                                                                           &applicationStatus);

  sendBootloadResponse(message->source, imageTag, applicationStatus, accepted);
}
