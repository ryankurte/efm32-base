// Wire replacement sample application.
//
// Copyright 2014 Silicon Laboratories, Inc.                                *80*

#if !defined(UNIX_HOST) && !defined(CORTEXM3_EMBER_MICRO)
#include "em_device.h"
#include "em_chip.h"
#include "bsp.h"
#endif

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER

#include "stack/include/ember.h"
#ifndef UNIX_HOST
#include "heartbeat/heartbeat.h"
#endif
#include "hal/hal.h"
#include "poll/poll.h"
#include "command-interpreter/command-interpreter2.h"
#include "debug-print/debug-print.h"

#ifdef EMBER_AF_PLUGIN_FREE_RTOS
#include "FreeRTOS.h"
#include "task.h"
#include EMBER_AF_API_FREE_RTOS
extern long stack_blocked_deep_sleep;
extern long stack_allowed_deep_sleep;
#endif

#define WIRE_REPLACEMENT_TX_POWER         0
#define WIRE_REPLACEMENT_PAN_ID           0xDEED
#define WIRE_REPLACEMENT_PROTOCOL_ID      0xC00E
#define WIRE_REPLACEMENT_MASTER_ID        EMBER_COORDINATOR_ADDRESS
#define WIRE_REPLACEMENT_SLAVE_ID         (EMBER_COORDINATOR_ADDRESS + 1)

#define WIRE_REPLACEMENT_PROTOCOL_ID_OFFSET  0
#define WIRE_REPLACEMENT_COMMAND_ID_OFFSET   2
#define WIRE_REPLACEMENT_DATA_OFFSET         3
#define WIRE_REPLACEMENT_MINIMUM_LENGTH      WIRE_REPLACEMENT_DATA_OFFSET
#define WIRE_REPLACEMENT_HEADER_LENGTH       WIRE_REPLACEMENT_DATA_OFFSET

#define NETWORK_UP_LED                       BOARDLED0

#define WIRE_REPLACEMENT_SECURITY_KEY    { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, \
                                           0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, \
                                           0xAA, 0xAA, 0xAA, 0xAA }

enum {
  WIRE_REPLACEMENT_COMMAND_ID_DATA = 0,
};
typedef uint8_t WireReplacementCommandId;

static int8_t txPower = WIRE_REPLACEMENT_TX_POWER;

static EmberStatus send(EmberNodeId nodeId,
                        WireReplacementCommandId commandId,
                        uint8_t *buffer,
                        uint8_t bufferLength);

static EmberMessageOptions txOptions = EMBER_OPTIONS_ACK_REQUESTED;

static EmberKeyData securityKey = WIRE_REPLACEMENT_SECURITY_KEY;

//------------------------------------------------------------------------------
// Callbacks

// This callback is called when the application starts and can be used to
// perform any additional initialization required at system startup.
void emberAfMainInitCallback(void)
{
  emberAfCorePrintln("Powered UP");
  emberAfCorePrintln("\n%p>", EMBER_AF_DEVICE_NAME);

  emberNetworkInit();
}

// This callback is called in each iteration of the main application loop and
// can be used to perform periodic functions.
void emberAfMainTickCallback(void)
{
  #ifndef UNIX_HOST
  if (emberStackIsUp()) {
    halSetLed(NETWORK_UP_LED);
  } else {
    halClearLed(NETWORK_UP_LED);
  }
  #endif
}

void emberAfIncomingMessageCallback(EmberIncomingMessage *message)
{
  if (message->length < WIRE_REPLACEMENT_MINIMUM_LENGTH
      || (emberFetchLowHighInt16u(message->payload
                                  + WIRE_REPLACEMENT_PROTOCOL_ID_OFFSET)
          != WIRE_REPLACEMENT_PROTOCOL_ID)) {
    return;
  }

  switch (message->payload[WIRE_REPLACEMENT_COMMAND_ID_OFFSET]) {
    case WIRE_REPLACEMENT_COMMAND_ID_DATA:
    {
      uint8_t i;
      emberAfCorePrint("RX: Data from 0x%2x:", message->source);
      for (i = WIRE_REPLACEMENT_DATA_OFFSET; i < message->length; i++) {
        emberAfCorePrint(" %x", message->payload[i]);
      }
      emberAfCorePrintln("");
      break;
    }
  }
}

void emberAfMessageSentCallback(EmberStatus status,
                                EmberOutgoingMessage *message)
{
  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("TX: 0x%x", status);
  }
}

void emberAfStackStatusCallback(EmberStatus status)
{
  switch (status) {
    case EMBER_NETWORK_UP:
      emberAfCorePrintln("Network up");

      // If the node joined as a sleepy slave start short polling.
      emberAfPluginPollEnableShortPolling(emberGetNodeType()
                                          == EMBER_STAR_SLEEPY_END_DEVICE);
      break;
    case EMBER_NETWORK_DOWN:
      emberAfCorePrintln("Network down");
      break;
    case EMBER_JOIN_FAILED:
      emberAfCorePrintln("Join failed");
      break;
    default:
      emberAfCorePrintln("Stack status: 0x%x", status);
      break;
  }
}

void emberAfChildJoinCallback(EmberNodeType nodeType,
                              EmberNodeId nodeId)
{
  emberAfCorePrintln("Slave 0x%2x joined as %s", nodeId,
                     (nodeType == EMBER_STAR_END_DEVICE)
                     ? "end device"
                     : "sleepy");
}

void emberFrequencyHoppingStartClientCompleteHandler(EmberStatus status)
{
  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("FH Client sync failed, status=0x%x", status);
  } else {
    emberAfCorePrintln("FH Client Sync Success");
  }
}

void emberAfEnergyScanCompleteCallback(int8_t mean,
                                       int8_t min,
                                       int8_t max,
                                       uint16_t variance)
{
  emberAfCorePrintln("Energy scan complete, mean=%d min=%d max=%d var=%d",
                     mean, min, max, variance);
}

//------------------------------------------------------------------------------
// Static functions

static EmberStatus send(EmberNodeId nodeId,
                        WireReplacementCommandId commandId,
                        uint8_t *buffer,
                        uint8_t bufferLength)
{
  uint8_t message[EMBER_MAX_UNSECURED_APPLICATION_PAYLOAD_LENGTH];
  EmberMessageLength messageLength = 0;

  if ((bufferLength + WIRE_REPLACEMENT_HEADER_LENGTH)
      > ((txOptions & EMBER_OPTIONS_SECURITY_ENABLED)
         ? EMBER_MAX_SECURED_APPLICATION_PAYLOAD_LENGTH
         : EMBER_MAX_UNSECURED_APPLICATION_PAYLOAD_LENGTH)) {
    return EMBER_MESSAGE_TOO_LONG;
  }
  emberStoreLowHighInt16u(message + messageLength, WIRE_REPLACEMENT_PROTOCOL_ID);
  messageLength += 2;
  message[messageLength++] = commandId;
  if (bufferLength != 0) {
    MEMCOPY(message + messageLength, buffer, bufferLength);
    messageLength += bufferLength;
  }

  return emberMessageSend(nodeId,
                          0, // endpoint
                          0, // messageTag
                          messageLength,
                          message,
                          txOptions);
}

//------------------------------------------------------------------------------
// CLI commands

// When we start as a master, we form a network and enable permit join.
void startMasterCommand(void)
{
  EmberStatus status;
  EmberNetworkParameters params;

  // Initialize the security key to the default key prior to forming the
  // network.
  emberSetSecurityKey(&securityKey);

  params.panId = WIRE_REPLACEMENT_PAN_ID;
  params.radioChannel = emberUnsignedCommandArgument(0);
  params.radioTxPower = txPower;

  status = emberFormNetwork(&params);

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("Form failed, 0x%x", status);
  } else {
    emberPermitJoining(0xFF);
  }
}

void startSlaveCommand(void)
{
  EmberStatus status;
  EmberNetworkParameters params;

  // Initialize the security key to the default key prior to joining the
  // network.
  emberSetSecurityKey(&securityKey);

  params.panId = WIRE_REPLACEMENT_PAN_ID;
  params.radioChannel = emberUnsignedCommandArgument(0);
  params.radioTxPower = txPower;

  status = emberJoinNetworkExtended(EMBER_STAR_END_DEVICE,
                                    WIRE_REPLACEMENT_SLAVE_ID,
                                    &params);

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("Join failed, 0x%x", status);
  }
}

void startSleepySlaveCommand(void)
{
  EmberStatus status;
  EmberNetworkParameters params;

  // Initialize the security key to the default key prior to joining the
  // network.
  emberSetSecurityKey(&securityKey);

  params.panId = WIRE_REPLACEMENT_PAN_ID;
  params.radioChannel = emberUnsignedCommandArgument(0);
  params.radioTxPower = txPower;

  status = emberJoinNetworkExtended(EMBER_STAR_SLEEPY_END_DEVICE,
                                    WIRE_REPLACEMENT_SLAVE_ID,
                                    &params);

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("Join as sleepy failed, 0x%x", status);
  }
}

void setTxOptionsCommand(void)
{
  txOptions = emberUnsignedCommandArgument(0);
  emberAfCorePrintln("TX options set: MAC acks %s, security %s, priority %s",
                     ((txOptions & EMBER_OPTIONS_ACK_REQUESTED) ? "enabled" : "disabled"),
                     ((txOptions & EMBER_OPTIONS_SECURITY_ENABLED) ? "enabled" : "disabled"),
                     ((txOptions & EMBER_OPTIONS_HIGH_PRIORITY) ? "enabled" : "disabled"));
}

void setTxPowerCommand(void)
{
  txPower = emberSignedCommandArgument(0);

  if (emberSetRadioPower(txPower) == EMBER_SUCCESS) {
    emberAfCorePrintln("TX power set: %d", (int8_t)emberGetRadioPower());
  } else {
    emberAfCorePrintln("TX power set failed");
  }
}

void setSecurityKeyCommand(void)
{
  EmberKeyData key;
  emberCopyKeyArgument(0, &key);

  if (emberSetSecurityKey(&key) == EMBER_SUCCESS) {
    emberAfCorePrint("Security key set { ");
    emberAfCorePrintBuffer(key.contents, EMBER_ENCRYPTION_KEY_SIZE, TRUE);
    emberAfCorePrintln("}");
  } else {
    emberAfCorePrintln("Security key set failed");
  }
}

void dataCommand(void)
{
  EmberStatus status;
  uint8_t length;
  uint8_t *contents = emberStringCommandArgument(0, &length);
  EmberNodeId destination = (emberGetNodeType() == EMBER_STAR_COORDINATOR)
                            ? WIRE_REPLACEMENT_SLAVE_ID
                            : WIRE_REPLACEMENT_MASTER_ID;

  status = send(destination,
                WIRE_REPLACEMENT_COMMAND_ID_DATA,
                contents,
                length);

  emberAfCorePrint("TX: Data to 0x%2x:{", destination);
  emberAfCorePrintBuffer(contents, length, TRUE);
  emberAfCorePrintln("}: status=0x%x", status);
}

void infoCommand(void)
{
  emberAfCorePrintln("%p:", EMBER_AF_DEVICE_NAME);
  emberAfCorePrintln("  network state: 0x%x", emberNetworkState());
  emberAfCorePrintln("      node type: 0x%x", emberGetNodeType());
  emberAfCorePrintln("          eui64: >%x%x%x%x%x%x%x%x",
                     emberGetEui64()[7],
                     emberGetEui64()[6],
                     emberGetEui64()[5],
                     emberGetEui64()[4],
                     emberGetEui64()[3],
                     emberGetEui64()[2],
                     emberGetEui64()[1],
                     emberGetEui64()[0]);
  emberAfCorePrintln("        node id: 0x%2x", emberGetNodeId());
  emberAfCorePrintln("         pan id: 0x%2x", emberGetPanId());
  emberAfCorePrintln("        channel: %d", (uint16_t)emberGetRadioChannel());
  emberAfCorePrintln("          power: %d", (int16_t)emberGetRadioPower());

#ifdef EMBER_AF_PLUGIN_FREE_RTOS
  emberAfCorePrintln("rtos deep block: %l", rtosGetDeepSleepBlockCount());
  emberAfCorePrintln(" stk block deep: %l", stack_blocked_deep_sleep);
  emberAfCorePrintln(" stk allow deep: %l", stack_allowed_deep_sleep);
#endif
}

void pollCommand(void)
{
  EmberStatus status = emberPollForData();

  emberAfCorePrintln("Poll status 0x%x", status);
}

void counterCommand(void)
{
  uint8_t counterType = emberUnsignedCommandArgument(0);
  uint32_t counter;
  EmberStatus status = emberGetCounter(counterType, &counter);

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("Get counter failed, status=0x%x", status);
  } else {
    emberAfCorePrintln("Counter type=0x%x: %d", counterType, counter);
  }
}

void startEnergyScanCommand(void)
{
  EmberStatus status;
  uint8_t channelToScan = emberUnsignedCommandArgument(0);
  uint8_t samples = emberUnsignedCommandArgument(1);
  status = emberStartEnergyScan(channelToScan, samples);

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("Start energy scanning failed, status=0x%x", status);
  } else {
    emberAfCorePrintln("Start energy scanning: channel %d, samples %d",
                       channelToScan, samples);
  }
}

#ifdef UNIX_HOST
void setPhyModeCommand(void)
{
  uint8_t mode = emberUnsignedCommandArgument(0);
  EmberStatus status = emberSetPhyMode(mode);

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("Phy mode change failed, status=0x%x", status);
  } else {
    emberAfCorePrintln("Phy mode changed successfully");
  }
}

void getPhyModeCommand(void)
{
  uint8_t mode = emberGetPhyMode();

  if (mode == 0) {
    emberAfCorePrintln("Phy mode: SubGHz");
  } else if (mode == 1) {
    emberAfCorePrintln("Phy mode: 2.4GHz");
  } else {
    emberAfCorePrintln("Invalid image");
  }
}
#endif

void startFrequencyHoppingServerCommand(void)
{
  EmberStatus status = emberFrequencyHoppingStartServer();

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("FH Server Failed, 0x%x", status);
  } else {
    emberAfCorePrintln("FH Server Success");
  }
}

void startFrequencyHoppingClientCommand(void)
{
  EmberStatus status;
  EmberNodeId nodeId;
  EmberPanId panId;

  nodeId = emberUnsignedCommandArgument(0);
  panId = emberUnsignedCommandArgument(1);

  status = emberFrequencyHoppingStartClient(nodeId, panId);

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("FH Client Failed, 0x%x", status);
  } else {
    emberAfCorePrintln("FH Client Success");
  }
}

void stopFrequencyHoppingCommand(void)
{
  EmberStatus status = emberFrequencyHoppingStop();

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("FH Stop Failed, 0x%x", status);
  } else {
    emberAfCorePrintln("FH Stopped");
  }
}
