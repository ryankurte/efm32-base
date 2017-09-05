// Copyright 2015 Silicon Laboratories, Inc.

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include "hal/hal.h"
#include "stack/include/ember.h"

#include "mailbox-server.h"

EmberEventControl emAfPluginMailboxServerEventControl;

//------------------------------------------------------------------------------
// Internal types, variables and function prototypes.

typedef struct {
  EmberNodeId source;
  EmberNodeId destination;
  EmberMessageOptions txOptions;
  uint32_t timeoutTimeMs;
  uint8_t tag;
  uint8_t localTag;
  EmberBuffer packetBuffer;
} EmAfMailboxQueueEntry;

static EmAfMailboxQueueEntry packetTable[EMBER_AF_PLUGIN_MAILBOX_SERVER_PACKET_TABLE_SIZE];

static uint8_t nextPacketToExpireIndex;
static uint8_t nextLocalTag = 0;

static bool entryInUse(uint8_t entryIndex);
static uint8_t getFreeEntryIndex(void);
static void updateNextPacketToExpire(void);
static void messageDeliveredHandler(EmberAfMailboxStatus status,
                                    uint8_t entryIndex);

// Packet table managing functions
static EmberAfMailboxStatus addMessage(EmberNodeId source,
                                       EmberNodeId destination,
                                       uint8_t *message,
                                       EmberMessageLength messageLength,
                                       uint8_t tag,
                                       bool useSecurity);
static uint8_t messageLookupByDestinationAndOptions(EmberNodeId destination,
                                                    bool secured,
                                                    bool *moreMessages);
static uint8_t messageLookupByDestinationAndLocalTag(EmberNodeId destination,
                                                     uint8_t localTag);

// Command handlers
static void incomingAddMessageCommandHandler(EmberNodeId source,
                                             EmberNodeId destination,
                                             EmberMessageOptions txOptions,
                                             uint8_t tag,
                                             uint8_t *payload,
                                             EmberMessageLength payloadLength);
static void incomingGetMessageCommandHandler(EmberNodeId source,
                                             EmberMessageOptions txOptions);
static void dataMessageCommandSentHandler(EmberStatus status,
                                          EmberNodeId destination,
                                          uint8_t localTag);

// Command send functions
static void sendAddMessageResponseOrMessageStatusCommand(bool isMessageStatus,
                                                         uint8_t tag,
                                                         EmberNodeId messageDestination,
                                                         EmberNodeId commandDestination,
                                                         EmberMessageOptions txOptions,
                                                         EmberAfMailboxStatus status);
static void sendGetMessageNullResponseCommand(EmberNodeId commandDestination,
                                              EmberMessageOptions txOptions);
static void sendDataMessageCommand(uint8_t entryIndex, bool messagePending);

//------------------------------------------------------------------------------
// Public APIs

EmberAfMailboxStatus emberAfPluginMailboxServerAddMessage(EmberNodeId destination,
                                                          uint8_t *message,
                                                          EmberMessageLength messageLength,
                                                          uint8_t tag,
                                                          bool useSecurity)
{
  return addMessage(emberGetNodeId(), // source node is the local node
                    destination,
                    message,
                    messageLength,
                    tag,
                    useSecurity);
}

//------------------------------------------------------------------------------
// Implemented plugin callbacks

void emAfPluginMailboxServerInitCallback(void)
{
  uint8_t i;

  emberEventControlSetInactive(emAfPluginMailboxServerEventControl);

  // Initializes the packet table
  for (i = 0; i < EMBER_AF_PLUGIN_MAILBOX_SERVER_PACKET_TABLE_SIZE; i++) {
    packetTable[i].packetBuffer = EMBER_NULL_BUFFER;
  }
}

void emAfPluginMailboxServerStackStatusCallback(EmberStatus status)
{
  // Clear the table if the stack goes down.
  if (status == EMBER_NETWORK_DOWN) {
    emAfPluginMailboxServerInitCallback();
  }
}

void emAfPluginMailboxServerMarkBuffersCallback(void)
{
  uint8_t i;

  for (i = 0; i < EMBER_AF_PLUGIN_MAILBOX_SERVER_PACKET_TABLE_SIZE; i++) {
    emberMarkBuffer(&packetTable[i].packetBuffer);
  }
}

void emAfPluginMailboxServerEventHandler(void)
{
  emberEventControlSetInactive(emAfPluginMailboxServerEventControl);

  messageDeliveredHandler(EMBER_MAILBOX_STATUS_MESSAGE_TIMED_OUT,
                          nextPacketToExpireIndex);

  packetTable[nextPacketToExpireIndex].packetBuffer = EMBER_NULL_BUFFER;

  updateNextPacketToExpire();
}

void emAfPluginMailboxServerIncomingMessageCallback(EmberIncomingMessage *message)
{
  if (message->endpoint == EMBER_AF_PLUGIN_MAILBOX_SERVER_MAILBOX_ENDPOINT) {
    uint8_t commandId = mailboxProtocolCommandId(message->payload);

    switch (commandId) {
      case EMBER_MAILBOX_PROTOCOL_COMMAND_ID_ADD_MESSAGE:
        // Enforce at least 1 byte of payload.
        if (message->length <= EMBER_MAILBOX_PROTOCOL_ADD_MESSAGE_HEADER_LENGTH) {
          return;
        }

        incomingAddMessageCommandHandler(message->source,
                                         emberFetchLowHighInt16u(message->payload + EMBER_MAILBOX_PROTOCOL_ADD_MESSAGE_DESTINATION_OFFSET),
                                         message->options,
                                         message->payload[EMBER_MAILBOX_PROTOCOL_ADD_MESSAGE_TAG_OFFSET],
                                         message->payload + EMBER_MAILBOX_PROTOCOL_ADD_MESSAGE_PAYLOAD_OFFSET,
                                         message->length - EMBER_MAILBOX_PROTOCOL_ADD_MESSAGE_HEADER_LENGTH);
        break;
      case EMBER_MAILBOX_PROTOCOL_COMMAND_ID_GET_MESSAGE:
        if (message->length < EMBER_MAILBOX_PROTOCOL_GET_MESSAGE_HEADER_LENGTH) {
          return;
        }

        incomingGetMessageCommandHandler(message->source, message->options);
        break;
    }
  }
}

void emAfPluginMailboxServerMessageSentCallback(EmberStatus status,
                                                EmberOutgoingMessage *message)
{
  if (message->endpoint == EMBER_AF_PLUGIN_MAILBOX_SERVER_MAILBOX_ENDPOINT) {
    uint8_t commandId = mailboxProtocolCommandId(message->payload);

    switch (commandId) {
      case EMBER_MAILBOX_PROTOCOL_COMMAND_ID_DATA_MESSAGE:
        dataMessageCommandSentHandler(status, message->destination, message->tag);
        break;
    }
  }
}

//------------------------------------------------------------------------------
// Static variables and functions

// Returns true if the entry at the passed index is in use, false otherwise.
static bool entryInUse(uint8_t entryIndex)
{
  return (packetTable[entryIndex].packetBuffer != EMBER_NULL_BUFFER);
}

// Returns 0xFF if the table is full, otherwise it returns the index of a free
// entry.
static uint8_t getFreeEntryIndex(void)
{
  uint8_t i;

  // Initializes the packet table
  for (i = 0; i < EMBER_AF_PLUGIN_MAILBOX_SERVER_PACKET_TABLE_SIZE; i++) {
    if (!entryInUse(i)) {
      return i;
    }
  }

  return 0xFF;
}

// Updates the plugin event to fire when the closest packet in time expires.
static void updateNextPacketToExpire(void)
{
  uint32_t nowMs = halCommonGetInt32uMillisecondTick();
  uint32_t nextTimeoutMs = MAX_INT32U_VALUE;
  uint8_t i;

  for (i = 0; i < EMBER_AF_PLUGIN_MAILBOX_SERVER_PACKET_TABLE_SIZE; i++) {
    if (entryInUse(i)) {
      uint32_t thisEntryTimeoutMs =
        (timeGTorEqualInt32u(nowMs, packetTable[i].timeoutTimeMs))
        ? 0
        : elapsedTimeInt32u(nowMs, packetTable[i].timeoutTimeMs);

      if (thisEntryTimeoutMs < nextTimeoutMs) {
        nextTimeoutMs = thisEntryTimeoutMs;
        nextPacketToExpireIndex = i;
      }
    }
  }

  if (nextTimeoutMs == 0) {
    emberEventControlSetActive(emAfPluginMailboxServerEventControl);
  } else if (nextTimeoutMs == MAX_INT32U_VALUE) {
    emberEventControlSetInactive(emAfPluginMailboxServerEventControl);
  } else {
    emberEventControlSetDelayMS(emAfPluginMailboxServerEventControl,
                                nextTimeoutMs);
  }
}

static EmberAfMailboxStatus addMessage(EmberNodeId source,
                                       EmberNodeId destination,
                                       uint8_t *message,
                                       EmberMessageLength messageLength,
                                       uint8_t tag,
                                       bool useSecurity)
{
  uint8_t entryIndex;

  if (message == NULL || messageLength == 0) {
    return EMBER_MAILBOX_STATUS_INVALID_CALL;
  }

  if (source == EMBER_NULL_NODE_ID || destination == EMBER_NULL_NODE_ID) {
    return EMBER_MAILBOX_STATUS_INVALID_ADDRESS;
  }

  if (messageLength
      > ((useSecurity)
         ? EMBER_MAX_SECURED_APPLICATION_PAYLOAD_LENGTH
         : EMBER_MAX_UNSECURED_APPLICATION_PAYLOAD_LENGTH)
      - EMBER_MAILBOX_PROTOCOL_DATA_MESSAGE_HEADER_LENGTH) {
    return EMBER_MAILBOX_STATUS_MESSAGE_TOO_LONG;
  }

  entryIndex = getFreeEntryIndex();

  if (entryIndex == 0xFF) {
    return EMBER_MAILBOX_STATUS_MESSAGE_TABLE_FULL;
  }

  packetTable[entryIndex].packetBuffer = emberAllocateBuffer(messageLength);
  if (packetTable[entryIndex].packetBuffer == EMBER_NULL_BUFFER) {
    return EMBER_MAILBOX_STATUS_MESSAGE_NO_BUFFERS;
  }

  MEMCOPY(emberGetBufferPointer(packetTable[entryIndex].packetBuffer),
          message,
          messageLength);

  packetTable[entryIndex].source = source;
  packetTable[entryIndex].destination = destination;
  packetTable[entryIndex].txOptions = (EMBER_OPTIONS_ACK_REQUESTED
                                       | (useSecurity
                                          ? EMBER_OPTIONS_SECURITY_ENABLED
                                          : 0));
  packetTable[entryIndex].tag = tag;
  packetTable[entryIndex].timeoutTimeMs =
    (halCommonGetInt32uMillisecondTick()
     + EMBER_AF_PLUGIN_MAILBOX_SERVER_PACKET_TIMEOUT_S * 1000);

  updateNextPacketToExpire();

  return EMBER_MAILBOX_STATUS_SUCCESS;
}

// Returns a valid entry index if a message for the destination node was found
// in the table, 0xFF otherwise. It also sets the passed moreMessages flag in
// case there are multiple packets in the table for the passed destination.
// If multiple packets for the same destination are found, the earliest entry to
// expire is returned.
static uint8_t messageLookupByDestinationAndOptions(EmberNodeId destination,
                                                    bool secured,
                                                    bool *moreMessages)
{
  uint32_t nowMs = halCommonGetInt32uMillisecondTick();
  uint32_t timeoutMs = MAX_INT32U_VALUE;
  uint8_t retIndex = 0xFF;
  uint8_t i;

  *moreMessages = false;

  for (i = 0; i < EMBER_AF_PLUGIN_MAILBOX_SERVER_PACKET_TABLE_SIZE; i++) {
    if (entryInUse(i)
        && packetTable[i].destination == destination
        && (secured
            || !(packetTable[i].txOptions & EMBER_OPTIONS_SECURITY_ENABLED))) {
      uint32_t thisEntryTimeoutMs =
        (timeGTorEqualInt32u(nowMs, packetTable[i].timeoutTimeMs))
        ? 0
        : elapsedTimeInt32u(nowMs, packetTable[i].timeoutTimeMs);

      if (retIndex == 0xFF) {
        retIndex = i;
        timeoutMs = thisEntryTimeoutMs;
      } else {
        *moreMessages = true;
        if (thisEntryTimeoutMs < timeoutMs) {
          retIndex = i;
          timeoutMs = thisEntryTimeoutMs;
        }
      }
    }
  }

  return retIndex;
}

static uint8_t messageLookupByDestinationAndLocalTag(EmberNodeId destination,
                                                     uint8_t localTag)
{
  uint8_t i;

  for (i = 0; i < EMBER_AF_PLUGIN_MAILBOX_SERVER_PACKET_TABLE_SIZE; i++) {
    if (entryInUse(i)
        && packetTable[i].destination == destination
        && packetTable[i].localTag == localTag) {
      return i;
    }
  }

  return 0xFF;
}

static void messageDeliveredHandler(EmberAfMailboxStatus status,
                                    uint8_t entryIndex)
{
  if (packetTable[entryIndex].source == emberGetNodeId()) {
    emberAfPluginMailboxServerMessageDeliveredCallback(status,
                                                       packetTable[entryIndex].destination,
                                                       packetTable[entryIndex].tag);
  } else {
    sendAddMessageResponseOrMessageStatusCommand(true,
                                                 packetTable[entryIndex].tag,
                                                 packetTable[entryIndex].destination,
                                                 packetTable[entryIndex].source,
                                                 packetTable[entryIndex].txOptions,
                                                 status);
  }
}

//------------------------------------------------------------------------------
// Command send functions

static void sendAddMessageResponseOrMessageStatusCommand(bool isMessageStatus,
                                                         uint8_t tag,
                                                         EmberNodeId messageDestination,
                                                         EmberNodeId commandDestination,
                                                         EmberMessageOptions txOptions,
                                                         EmberAfMailboxStatus status)
{
  uint8_t message[EMBER_MAILBOX_PROTOCOL_ADD_MESSAGE_RESPONSE_HEADER_LENGTH];

  message[EMBER_MAILBOX_PROTOCOL_FRAME_CONTROL_OFFSET] =
    ((isMessageStatus)
     ? EMBER_MAILBOX_PROTOCOL_COMMAND_ID_MESSAGE_STATUS
     : EMBER_MAILBOX_PROTOCOL_COMMAND_ID_ADD_MESSAGE_RESPONSE);

  message[EMBER_MAILBOX_PROTOCOL_ADD_MESSAGE_RESPONSE_TAG_OFFSET] = tag;

  emberStoreLowHighInt16u(message + EMBER_MAILBOX_PROTOCOL_ADD_MESSAGE_RESPONSE_DESTINATION_OFFSET,
                          messageDestination);

  message[EMBER_MAILBOX_PROTOCOL_ADD_MESSAGE_RESPONSE_STATUS_OFFSET] = status;

  emberMessageSend(commandDestination,
                   EMBER_AF_PLUGIN_MAILBOX_SERVER_MAILBOX_ENDPOINT,
                   0, // tag, we only use the local tag for data messages.
                   EMBER_MAILBOX_PROTOCOL_ADD_MESSAGE_RESPONSE_HEADER_LENGTH,
                   message,
                   txOptions);
}

static void sendGetMessageNullResponseCommand(EmberNodeId commandDestination,
                                              EmberMessageOptions txOptions)
{
  uint8_t message[EMBER_MAILBOX_PROTOCOL_NULL_MESSAGE_HEADER_LENGTH];

  message[EMBER_MAILBOX_PROTOCOL_FRAME_CONTROL_OFFSET] =
    EMBER_MAILBOX_PROTOCOL_COMMAND_ID_NULL_MESSAGE;

  emberMessageSend(commandDestination,
                   EMBER_AF_PLUGIN_MAILBOX_SERVER_MAILBOX_ENDPOINT,
                   0, // tag, we only use the local tag for data messages.
                   EMBER_MAILBOX_PROTOCOL_NULL_MESSAGE_HEADER_LENGTH,
                   message,
                   txOptions | EMBER_OPTIONS_ACK_REQUESTED);
}

static void sendDataMessageCommand(uint8_t entryIndex, bool messagePending)
{
  uint8_t message[EMBER_MAX_UNSECURED_APPLICATION_PAYLOAD_LENGTH];

  message[EMBER_MAILBOX_PROTOCOL_FRAME_CONTROL_OFFSET] =
    (EMBER_MAILBOX_PROTOCOL_COMMAND_ID_DATA_MESSAGE
     | ((messagePending)
        ? EMBER_MAILBOX_PROTOCOL_FRAME_CONTROL_MESSAGE_PENDING_BIT
        : 0));

  message[EMBER_MAILBOX_PROTOCOL_DATA_MESSAGE_TAG_OFFSET] =
    packetTable[entryIndex].tag;

  emberStoreLowHighInt16u(message + EMBER_MAILBOX_PROTOCOL_DATA_MESSAGE_SOURCE_OFFSET,
                          packetTable[entryIndex].source);

  MEMCOPY(message + EMBER_MAILBOX_PROTOCOL_DATA_MESSAGE_PAYLOAD_OFFSET,
          emberGetBufferPointer(packetTable[entryIndex].packetBuffer),
          emberGetBufferLength(packetTable[entryIndex].packetBuffer));

  packetTable[entryIndex].localTag = nextLocalTag++;

  emberMessageSend(packetTable[entryIndex].destination,
                   EMBER_AF_PLUGIN_MAILBOX_SERVER_MAILBOX_ENDPOINT,
                   packetTable[entryIndex].localTag,
                   (EMBER_MAILBOX_PROTOCOL_DATA_MESSAGE_HEADER_LENGTH
                    + emberGetBufferLength(packetTable[entryIndex].packetBuffer)),
                   message,
                   packetTable[entryIndex].txOptions);
}

//------------------------------------------------------------------------------
// Command Handlers

static void incomingAddMessageCommandHandler(EmberNodeId source,
                                             EmberNodeId destination,
                                             EmberMessageOptions txOptions,
                                             uint8_t tag,
                                             uint8_t *payload,
                                             EmberMessageLength payloadLength)
{
  EmberAfMailboxStatus status = addMessage(source,
                                           destination,
                                           payload,
                                           payloadLength,
                                           tag,
                                           ((txOptions & EMBER_OPTIONS_SECURITY_ENABLED) > 0));

  sendAddMessageResponseOrMessageStatusCommand(false,
                                               tag,
                                               destination,
                                               source,
                                               txOptions,
                                               status);
}

static void incomingGetMessageCommandHandler(EmberNodeId source,
                                             EmberMessageOptions txOptions)
{
  bool messagePending;
  bool getMessageCommandSecured =
    (txOptions & EMBER_OPTIONS_SECURITY_ENABLED) > 0;
  uint8_t entryIndex =
    messageLookupByDestinationAndOptions(source,
                                         getMessageCommandSecured,
                                         &messagePending);

  if (entryIndex == 0xFF) {
    sendGetMessageNullResponseCommand(source, txOptions);
  } else {
    sendDataMessageCommand(entryIndex, messagePending);
  }
}

static void dataMessageCommandSentHandler(EmberStatus status,
                                          EmberNodeId destination,
                                          uint8_t localTag)
{
  uint8_t entryIndex =
    messageLookupByDestinationAndLocalTag(destination, localTag);

  // This could be the result of multiple get_message commands received from the
  // same client.
  if (entryIndex == 0xFF) {
    return;
  }

  if (status == EMBER_SUCCESS) {
    messageDeliveredHandler(EMBER_MAILBOX_STATUS_SUCCESS, entryIndex);

    // Free the entry
    packetTable[entryIndex].packetBuffer = EMBER_NULL_BUFFER;
    updateNextPacketToExpire();
  }

  // If the message was not delivered, leave it in the queue: either the final
  // destination will send a new get message command, or it will expire.
}
